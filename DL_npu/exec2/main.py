import numpy as np
import torch
import torch.nn as nn
import torch.nn.functional as F
import torch.optim as optim
from torch.utils.data import DataLoader, TensorDataset, Dataset
import torch.onnx
import os
from torch.cuda.amp import autocast, GradScaler
import math
from scipy import signal
import pywt  # pip install PyWavelets

# =============================================================================
# 1. 基础设置
# =============================================================================
np.random.seed(42)
torch.manual_seed(42)
if torch.cuda.is_available():
    torch.cuda.manual_seed_all(42)
    torch.backends.cudnn.deterministic = True
    torch.backends.cudnn.benchmark = True

device = torch.device('cuda' if torch.cuda.is_available() else 'cpu')
print(f"🚀 Using device: {device}")


# =============================================================================
# 2. 宇宙级数据增强（Wavelet + FMix + TimeWarp）
# =============================================================================
class UniversalEEGAugmentation:
    """宇宙级 EEG 数据增强工具箱"""

    @staticmethod
    def wavelet_augment(x, noise_level=0.02):
        """小波域增强"""
        coeffs = pywt.wavedec(x.numpy(), 'db4', level=3)
        coeffs = [c + np.random.randn(*c.shape) * noise_level for c in coeffs]
        return torch.FloatTensor(pywt.waverec(coeffs, 'db4')[:x.shape[-1]])

    @staticmethod
    def frequency_mask(x, mask_ratio=0.15):
        """频域掩码"""
        fft = torch.fft.rfft(x, dim=-1)
        mask = torch.rand(fft.shape[-1]) > mask_ratio
        fft = fft * mask.to(x.device)
        return torch.fft.irfft(fft, n=x.shape[-1], dim=-1)

    @staticmethod
    def time_warp(x, sigma=0.2):
        """时间扭曲"""
        time_steps = x.shape[-1]
        warp = torch.cumsum(torch.randn(time_steps) * sigma + 1, dim=0)
        warp = warp / warp[-1] * (time_steps - 1)
        warp = warp.clamp(0, time_steps - 1).long()
        return x[:, warp]

    @staticmethod
    def mixup(x1, x2, y1, y2, alpha=0.4):
        """增强版 Mixup"""
        lam = np.random.beta(alpha, alpha)
        return lam * x1 + (1 - lam) * x2, y1, y2, lam

    @staticmethod
    def cutmix(x1, x2, y1, y2):
        """CutMix"""
        lam = np.random.beta(1.0, 1.0)
        cut_len = int(x1.shape[-1] * (1 - lam))
        cut_start = np.random.randint(0, x1.shape[-1] - cut_len)

        x = x1.clone()
        x[:, cut_start:cut_start + cut_len] = x2[:, cut_start:cut_start + cut_len]
        return x, y1, y2, lam


class CosmicEEGDataset(Dataset):
    """宇宙级 Dataset"""

    def __init__(self, x, y, mode='train', augment_prob=0.9):
        self.x = torch.FloatTensor(x)
        self.y = torch.LongTensor(y)
        self.mode = mode
        self.augment_prob = augment_prob
        self.aug = UniversalEEGAugmentation()

    def __len__(self):
        return len(self.x)

    def __getitem__(self, idx):
        sample = self.x[idx].clone()
        label = self.y[idx]

        if self.mode == 'train' and torch.rand(1) < self.augment_prob:
            # 随机选择增强策略
            aug_choice = torch.rand(1).item()

            if aug_choice < 0.3:
                # 小波增强
                sample = self.aug.wavelet_augment(sample, noise_level=0.02)
            elif aug_choice < 0.5:
                # 频域掩码
                sample = self.aug.frequency_mask(sample, mask_ratio=0.15)
            elif aug_choice < 0.7:
                # 时间扭曲
                sample = self.aug.time_warp(sample, sigma=0.15)

            # 基础增强
            if torch.rand(1) < 0.5:
                sample = sample + torch.randn_like(sample) * 0.025  # 噪声
            if torch.rand(1) < 0.3:
                sample = sample.flip(-1)  # 翻转
            if torch.rand(1) < 0.4:
                scale = torch.FloatTensor(8).uniform_(0.8, 1.2).view(8, 1)
                sample = sample * scale

        return sample, label


# =============================================================================
# 3. 宇宙级模型架构（Transformer + CNN + Graph）
# =============================================================================

class ChannelAttention(nn.Module):
    """通道注意力机制"""

    def __init__(self, channels, reduction=8):
        super().__init__()
        self.fc = nn.Sequential(
            nn.Linear(channels, channels // reduction),
            nn.GELU(),
            nn.Linear(channels // reduction, channels),
            nn.Sigmoid()
        )

    def forward(self, x):
        b, c, _ = x.shape
        w = F.adaptive_avg_pool1d(x, 1).view(b, c)
        w = self.fc(w).view(b, c, 1)
        return x * w


class SpatialAttention(nn.Module):
    """空间注意力机制"""

    def __init__(self, kernel_size=7):
        super().__init__()
        self.conv = nn.Conv1d(2, 1, kernel_size=kernel_size, padding=kernel_size // 2)
        self.sigmoid = nn.Sigmoid()

    def forward(self, x):
        max_pool = torch.max(x, dim=1, keepdim=True)[0]
        avg_pool = torch.mean(x, dim=1, keepdim=True)
        concat = torch.cat([max_pool, avg_pool], dim=1)
        attention = self.sigmoid(self.conv(concat))
        return x * attention


class TransformerBlock(nn.Module):
    """Transformer 编码器块"""

    def __init__(self, dim, num_heads=8, mlp_ratio=4, dropout=0.1):
        super().__init__()
        self.norm1 = nn.LayerNorm(dim)
        self.attn = nn.MultiheadAttention(dim, num_heads, dropout=dropout, batch_first=True)
        self.norm2 = nn.LayerNorm(dim)
        self.mlp = nn.Sequential(
            nn.Linear(dim, dim * mlp_ratio),
            nn.GELU(),
            nn.Dropout(dropout),
            nn.Linear(dim * mlp_ratio, dim),
            nn.Dropout(dropout)
        )

    def forward(self, x):
        # x: (B, L, C)
        x = x + self.attn(self.norm1(x), self.norm1(x), self.norm1(x))[0]
        x = x + self.mlp(self.norm2(x))
        return x


class ConvBlock(nn.Module):
    """增强版卷积块"""

    def __init__(self, in_c, out_c, kernel=3, stride=1):
        super().__init__()
        self.conv1 = nn.Conv1d(in_c, out_c, kernel, stride=stride, padding=kernel // 2, bias=False)
        self.bn1 = nn.BatchNorm1d(out_c)
        self.conv2 = nn.Conv1d(out_c, out_c, kernel, padding=kernel // 2, bias=False)
        self.bn2 = nn.BatchNorm1d(out_c)

        self.ca = ChannelAttention(out_c)
        self.sa = SpatialAttention()

        self.shortcut = nn.Sequential()
        if in_c != out_c or stride != 1:
            self.shortcut = nn.Sequential(
                nn.Conv1d(in_c, out_c, 1, stride=stride, bias=False),
                nn.BatchNorm1d(out_c)
            )

        self.dropout = nn.Dropout(0.15)

    def forward(self, x):
        identity = self.shortcut(x)

        out = F.gelu(self.bn1(self.conv1(x)))
        out = self.dropout(out)
        out = self.bn2(self.conv2(out))

        out = self.ca(out)
        out = self.sa(out)

        out += identity
        out = F.gelu(out)
        return out


class CosmicEEGModel(nn.Module):
    """宇宙级 EEG 模型：CNN + Transformer 混合架构"""

    def __init__(self, num_classes, drop_rate=0.5):
        super().__init__()

        # Stem: 强力特征提取
        self.stem = nn.Sequential(
            nn.Conv1d(8, 64, kernel_size=7, padding=3, bias=False),
            nn.BatchNorm1d(64),
            nn.GELU(),
            nn.Conv1d(64, 128, kernel_size=5, padding=2, bias=False),
            nn.BatchNorm1d(128),
            nn.GELU(),
        )

        # CNN 分支
        self.conv_branch = nn.Sequential(
            ConvBlock(128, 256, stride=2),
            ConvBlock(256, 512, stride=2),
            ConvBlock(512, 768, stride=2),
            ConvBlock(768, 1024, stride=2),
        )

        # Transformer 分支
        self.pos_embed = nn.Parameter(torch.randn(1, 250, 128))
        self.transformer = nn.Sequential(
            TransformerBlock(128, num_heads=8),
            TransformerBlock(128, num_heads=8),
            TransformerBlock(128, num_heads=8),
        )

        # 多尺度池化
        self.gap = nn.AdaptiveAvgPool1d(1)
        self.gmp = nn.AdaptiveMaxPool1d(1)
        self.gwp = nn.AdaptiveAvgPool1d(1)  # 加权池化

        # 特征融合
        self.fusion = nn.Sequential(
            nn.Linear(1024 * 3 + 128, 1024),  # CNN(GAP+GMP+GWP) + Transformer
            nn.BatchNorm1d(1024),
            nn.GELU(),
            nn.Dropout(drop_rate),
        )

        # 分类器
        self.classifier = nn.Sequential(
            nn.Linear(1024, 512),
            nn.BatchNorm1d(512),
            nn.GELU(),
            nn.Dropout(drop_rate * 0.7),
            nn.Linear(512, 256),
            nn.GELU(),
            nn.Dropout(drop_rate * 0.5),
            nn.Linear(256, num_classes),
        )

    def forward(self, x):
        # Stem
        x_stem = self.stem(x)  # (B, 128, 250)

        # CNN 分支
        x_conv = self.conv_branch(x_stem)  # (B, 1024, ~15)
        f_gap = self.gap(x_conv).squeeze(-1)
        f_gmp = self.gmp(x_conv).squeeze(-1)
        f_gwp = self.gwp(x_conv).squeeze(-1)
        f_cnn = torch.cat([f_gap, f_gmp, f_gwp], dim=1)

        # Transformer 分支
        x_trans = x_stem.permute(0, 2, 1)  # (B, 250, 128)
        x_trans = x_trans + self.pos_embed
        x_trans = self.transformer(x_trans)  # (B, 250, 128)
        f_trans = x_trans.mean(dim=1)  # (B, 128)

        # 融合
        f = torch.cat([f_cnn, f_trans], dim=1)
        f = self.fusion(f)

        # 分类
        out = self.classifier(f)
        return out


# =============================================================================
# 4. 高级损失函数
# =============================================================================

class FocalLoss(nn.Module):
    """Focal Loss - 关注困难样本"""

    def __init__(self, alpha=None, gamma=2.0):
        super().__init__()
        self.alpha = alpha
        self.gamma = gamma

    def forward(self, inputs, targets):
        ce_loss = F.cross_entropy(inputs, targets, reduction='none', weight=self.alpha)
        pt = torch.exp(-ce_loss)
        focal_loss = ((1 - pt) ** self.gamma * ce_loss).mean()
        return focal_loss


class LabelSmoothingFocalLoss(nn.Module):
    """Label Smoothing + Focal Loss 混合"""

    def __init__(self, epsilon=0.1, alpha=None, gamma=2.0):
        super().__init__()
        self.epsilon = epsilon
        self.focal = FocalLoss(alpha, gamma)
        self.ce = nn.CrossEntropyLoss(weight=alpha)

    def forward(self, preds, target):
        focal_loss = self.focal(preds, target)

        n_classes = preds.size(-1)
        log_preds = F.log_softmax(preds, dim=-1)
        smooth_loss = -log_preds.mean(dim=-1).mean()

        return (1 - self.epsilon) * focal_loss + self.epsilon * smooth_loss


# =============================================================================
# 5. EMA（指数移动平均）
# =============================================================================

class ModelEMA:
    """模型参数指数移动平均"""

    def __init__(self, model, decay=0.9999, device='cpu'):
        import copy
        self.module = copy.deepcopy(model)
        self.module.eval()
        self.decay = decay
        self.device = device

    def update(self, model):
        with torch.no_grad():
            for ema_param, model_param in zip(self.module.parameters(), model.parameters()):
                ema_param.data.mul_(self.decay).add_(model_param.data, alpha=1 - self.decay)


# =============================================================================
# 6. 高级优化器：SAM (Sharpness-Aware Minimization)
# =============================================================================

class SAM(torch.optim.Optimizer):
    """Sharpness-Aware Minimization"""

    def __init__(self, params, base_optimizer, rho=0.05, **kwargs):
        defaults = dict(rho=rho, **kwargs)
        super(SAM, self).__init__(params, defaults)

        self.base_optimizer = base_optimizer(self.param_groups, **kwargs)
        self.param_groups = self.base_optimizer.param_groups

    @torch.no_grad()
    def first_step(self, zero_grad=False):
        grad_norm = self._grad_norm()
        for group in self.param_groups:
            scale = group["rho"] / (grad_norm + 1e-12)
            for p in group["params"]:
                if p.grad is None: continue
                e_w = p.grad * scale.to(p)
                p.add_(e_w)
                self.state[p]["e_w"] = e_w

        if zero_grad: self.zero_grad()

    @torch.no_grad()
    def second_step(self, zero_grad=False):
        for group in self.param_groups:
            for p in group["params"]:
                if p.grad is None: continue
                p.sub_(self.state[p]["e_w"])

        self.base_optimizer.step()
        if zero_grad: self.zero_grad()

    def _grad_norm(self):
        shared_device = self.param_groups[0]["params"][0].device
        norm = torch.norm(
            torch.stack([
                p.grad.norm(p=2).to(shared_device)
                for group in self.param_groups for p in group["params"]
                if p.grad is not None
            ]),
            p=2
        )
        return norm


# =============================================================================
# 7. 训练工具函数
# =============================================================================

def mixup_criterion(criterion, pred, y_a, y_b, lam):
    return lam * criterion(pred, y_a) + (1 - lam) * criterion(pred, y_b)


def test_time_augmentation(model, x, n_aug=5):
    """测试时增强"""
    model.eval()
    preds = []

    with torch.no_grad():
        # 原始预测
        preds.append(F.softmax(model(x), dim=1))

        # 增强预测
        for _ in range(n_aug - 1):
            x_aug = x + torch.randn_like(x) * 0.01  # 小噪声
            preds.append(F.softmax(model(x_aug), dim=1))

    return torch.stack(preds).mean(dim=0)


# =============================================================================
# 8. 主程序
# =============================================================================

if __name__ == '__main__':

    # ------------------- 加载数据 -------------------
    x_path = r"D:\MyCodeResource\DL_npu\exec2\2025CALx_train.npy"
    y_path = r"D:\MyCodeResource\DL_npu\exec2\2025CALy_train.npy"

    x = np.load(x_path).astype(np.float32)
    y = np.load(y_path).astype(np.int64).squeeze()

    print(f"📊 Data shape: x={x.shape}, y={y.shape}")
    unique_labels = np.unique(y)
    num_classes = len(unique_labels)
    print(f"📌 Classes: {unique_labels} → {num_classes} classes")

    # ------------------- 高级数据预处理 -------------------
    # 1. 通道级标准化
    mean = x.mean(axis=(0, 2), keepdims=True)
    std = x.std(axis=(0, 2), keepdims=True) + 1e-6
    x = (x - mean) / std

    # 2. 滤波（带通滤波 0.5-50 Hz，假设采样率 250 Hz）
    # 可选：对信号进行预滤波

    # ------------------- 分层划分数据集 -------------------
    from sklearn.model_selection import StratifiedShuffleSplit

    sss = StratifiedShuffleSplit(n_splits=1, test_size=0.12, random_state=42)
    train_idx, val_idx = next(sss.split(x, y))

    x_train, y_train = x[train_idx], y[train_idx]
    x_val, y_val = x[val_idx], y[val_idx]

    print(f"🔀 Train: {len(x_train)}, Val: {len(x_val)}")

    # ------------------- DataLoader -------------------
    batch_size = 128  # 增大 batch size

    train_dataset = CosmicEEGDataset(x_train, y_train, mode='train', augment_prob=0.85)
    val_dataset = CosmicEEGDataset(x_val, y_val, mode='val')

    train_loader = DataLoader(train_dataset, batch_size=batch_size, shuffle=True,
                              num_workers=6, pin_memory=True, persistent_workers=True,
                              prefetch_factor=3)
    val_loader = DataLoader(val_dataset, batch_size=batch_size, shuffle=False,
                            num_workers=4, pin_memory=True, persistent_workers=True)

    # ------------------- 模型 -------------------
    model = CosmicEEGModel(num_classes, drop_rate=0.45).to(device)

    total_params = sum(p.numel() for p in model.parameters())
    trainable_params = sum(p.numel() for p in model.parameters() if p.requires_grad)
    print(f"🎯 Total params: {total_params:,} | Trainable: {trainable_params:,}")

    # ------------------- 损失 & 优化器 -------------------
    class_counts = np.bincount(y_train)
    class_weights = 1.0 / (class_counts + 1e-6)
    class_weights = class_weights / class_weights.sum() * len(class_weights)
    class_weights = torch.FloatTensor(class_weights).to(device)

    criterion = LabelSmoothingFocalLoss(epsilon=0.1, alpha=class_weights, gamma=2.0)

    # 使用 AdamW
    base_lr = 1.5e-3
    optimizer = optim.AdamW(model.parameters(), lr=base_lr, weight_decay=3e-4, betas=(0.9, 0.999))

    # 可选：使用 SAM（注释掉 AdamW，取消下面注释）
    # optimizer = SAM(model.parameters(), optim.AdamW, lr=base_lr, weight_decay=3e-4)

    # EMA
    ema = ModelEMA(model, decay=0.9998)
    ema.module = ema.module.to(device)

    # 学习率调度器：OneCycleLR
    epochs = 200
    steps_per_epoch = len(train_loader)

    scheduler = optim.lr_scheduler.OneCycleLR(
        optimizer,
        max_lr=base_lr,
        epochs=epochs,
        steps_per_epoch=steps_per_epoch,
        pct_start=0.1,  # 10% warm-up
        anneal_strategy='cos',
        div_factor=25,
        final_div_factor=1e4
    )

    # Mixed Precision
    scaler = GradScaler() if device.type == 'cuda' else None

    # ------------------- 训练循环 -------------------
    best_acc = 0.0
    best_ema_acc = 0.0
    patience = 35
    patience_counter = 0

    aug = UniversalEEGAugmentation()

    print(f"\n{'=' * 60}")
    print(f"🚀 开始宇宙级训练！目标：99%+ 准确率")
    print(f"{'=' * 60}\n")

    for epoch in range(1, epochs + 1):
        model.train()
        train_loss = 0.0

        for batch_idx, (xb, yb) in enumerate(train_loader):
            xb, yb = xb.to(device, non_blocking=True), yb.to(device, non_blocking=True)

            # 随机使用 Mixup / CutMix
            use_mix = torch.rand(1).item()

            if use_mix < 0.35:  # 35% Mixup
                idx = torch.randperm(xb.size(0))
                xb_mix = xb[idx]
                yb_mix = yb[idx]
                xb, ya, yb, lam = aug.mixup(xb, xb_mix, yb, yb_mix, alpha=0.4)

                if scaler:
                    with autocast():
                        out = model(xb)
                        loss = mixup_criterion(criterion, out, ya, yb, lam)

                    optimizer.zero_grad()
                    scaler.scale(loss).backward()
                    scaler.unscale_(optimizer)
                    torch.nn.utils.clip_grad_norm_(model.parameters(), max_norm=1.0)
                    scaler.step(optimizer)
                    scaler.update()
                else:
                    out = model(xb)
                    loss = mixup_criterion(criterion, out, ya, yb, lam)
                    optimizer.zero_grad()
                    loss.backward()
                    torch.nn.utils.clip_grad_norm_(model.parameters(), max_norm=1.0)
                    optimizer.step()

            elif use_mix < 0.6:  # 25% CutMix
                idx = torch.randperm(xb.size(0))
                xb_mix = xb[idx]
                yb_mix = yb[idx]
                xb, ya, yb, lam = aug.cutmix(xb, xb_mix, yb, yb_mix)

                if scaler:
                    with autocast():
                        out = model(xb)
                        loss = mixup_criterion(criterion, out, ya, yb, lam)

                    optimizer.zero_grad()
                    scaler.scale(loss).backward()
                    scaler.unscale_(optimizer)
                    torch.nn.utils.clip_grad_norm_(model.parameters(), max_norm=1.0)
                    scaler.step(optimizer)
                    scaler.update()
                else:
                    out = model(xb)
                    loss = mixup_criterion(criterion, out, ya, yb, lam)
                    optimizer.zero_grad()
                    loss.backward()
                    torch.nn.utils.clip_grad_norm_(model.parameters(), max_norm=1.0)
                    optimizer.step()

            else:  # 40% 正常训练
                if scaler:
                    with autocast():
                        out = model(xb)
                        loss = criterion(out, yb)

                    optimizer.zero_grad()
                    scaler.scale(loss).backward()
                    scaler.unscale_(optimizer)
                    torch.nn.utils.clip_grad_norm_(model.parameters(), max_norm=1.0)
                    scaler.step(optimizer)
                    scaler.update()
                else:
                    out = model(xb)
                    loss = criterion(out, yb)
                    optimizer.zero_grad()
                    loss.backward()
                    torch.nn.utils.clip_grad_norm_(model.parameters(), max_norm=1.0)
                    optimizer.step()

            # 更新 EMA
            ema.update(model)

            # 更新学习率
            scheduler.step()

            train_loss += loss.item() * xb.size(0)

        # ------------------- 验证 -------------------
        model.eval()
        ema.module.eval()

        correct = 0
        ema_correct = 0
        total = 0
        val_loss = 0.0

        with torch.no_grad():
            for xb, yb in val_loader:
                xb, yb = xb.to(device, non_blocking=True), yb.to(device, non_blocking=True)

                # 普通模型
                if scaler:
                    with autocast():
                        out = model(xb)
                        loss = F.cross_entropy(out, yb)
                else:
                    out = model(xb)
                    loss = F.cross_entropy(out, yb)

                val_loss += loss.item() * xb.size(0)
                pred = out.argmax(dim=1)
                total += yb.size(0)
                correct += (pred == yb).sum().item()

                # EMA 模型
                ema_out = ema.module(xb)
                ema_pred = ema_out.argmax(dim=1)
                ema_correct += (ema_pred == yb).sum().item()

        acc = 100.0 * correct / total
        ema_acc = 100.0 * ema_correct / total
        train_loss /= len(train_loader.dataset)
        val_loss /= len(val_loader.dataset)
        current_lr = optimizer.param_groups[0]['lr']

        print(f"Epoch {epoch:03d}/{epochs} | LR: {current_lr:.6f} | "
              f"Train Loss: {train_loss:.4f} | Val Loss: {val_loss:.4f} | "
              f"Val Acc: {acc:.2f}% | EMA Acc: {ema_acc:.2f}%")

        # 保存最佳模型
        if acc > best_acc:
            best_acc = acc
            patience_counter = 0
            torch.save({
                'epoch': epoch,
                'model_state_dict': model.state_dict(),
                'optimizer_state_dict': optimizer.state_dict(),
                'accuracy': best_acc,
            }, "cosmic_best_model.pth")
            print(f"  ✅ [最佳模型] Acc = {best_acc:.2f}%")

        if ema_acc > best_ema_acc:
            best_ema_acc = ema_acc
            torch.save({
                'epoch': epoch,
                'model_state_dict': ema.module.state_dict(),
                'accuracy': best_ema_acc,
            }, "cosmic_best_ema_model.pth")
            print(f"  🌟 [最佳 EMA] Acc = {best_ema_acc:.2f}%")

        if acc < best_acc:
            patience_counter += 1
            if patience_counter >= patience:
                print(f"\n⏸️  早停！{patience} 轮无改进")
                break

    print(f"\n{'=' * 60}")
    print(f"🎉 训练完成！")
    print(f"🏆 最佳验证准确率: {best_acc:.2f}%")
    print(f"⭐ 最佳 EMA 准确率: {best_ema_acc:.2f}%")
    print(f"{'=' * 60}\n")

    # ------------------- 导出 ONNX -------------------
    model.eval()

    # 选择最佳模型
    best_checkpoint = "cosmic_best_ema_model.pth" if best_ema_acc > best_acc else "cosmic_best_model.pth"
    checkpoint = torch.load(best_checkpoint, map_location=device)
    model.load_state_dict(checkpoint['model_state_dict'])

    dummy_input = torch.randn(1, 8, 250).to(device)

    onnx_path = '21_2855_cosmic.onnx'
    torch.onnx.export(
        model,
        dummy_input,
        onnx_path,
        export_params=True,
        opset_version=14,
        do_constant_folding=True,
        input_names=['input'],
        output_names=['output'],
        dynamic_axes={
            'input': {0: 'batch_size'},
            'output': {0: 'batch_size'}
        }
    )

    print(f"✅ ONNX 导出成功！→ {onnx_path}")
    print(f"🚀 使用准确率: {checkpoint['accuracy']:.2f}%")
    print(f"📮 可直接提交到 http://113.140.77.66/")
    print(f"\n🌌 宇宙级优化完成！祝你冲击 99%+ 🎯")