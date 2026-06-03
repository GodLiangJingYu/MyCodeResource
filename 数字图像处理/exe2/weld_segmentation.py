"""
焊缝图像分割与边缘提取 - 交互式调参系统
============================================
使用传统图像处理方法（非深度学习）实现焊缝区域的精准分割与边缘提取。
提供基于tkinter的交互式调参界面，支持实时参数调节。

实验目的：对焊缝图像进行分割处理，完成焊缝区域的精准分割与边缘提取。
实验环境：Python 3.x + OpenCV + NumPy + Matplotlib + Tkinter (Anaconda环境)
"""

import os
import glob
import cv2
import numpy as np
import tkinter as tk
from tkinter import ttk, filedialog, messagebox
import matplotlib

matplotlib.use("TkAgg")
from matplotlib.backends.backend_tkagg import FigureCanvasTkAgg
from matplotlib.figure import Figure


# ======================== 中文路径兼容读写 ========================

def imread_cn(path, flags=cv2.IMREAD_COLOR):
    """支持中文路径的图像读取"""
    data = np.fromfile(path, dtype=np.uint8)
    return cv2.imdecode(data, flags)


def imwrite_cn(path, img):
    """支持中文路径的图像写入"""
    ext = os.path.splitext(path)[1]
    success, buf = cv2.imencode(ext, img)
    if success:
        buf.tofile(path)
    return success


# ======================== 图像处理核心函数 ========================

def preprocess(image, blur_ksize=5, blur_type="gaussian"):
    """预处理：去噪平滑"""
    k = blur_ksize if blur_ksize % 2 == 1 else blur_ksize + 1
    if blur_type == "gaussian":
        return cv2.GaussianBlur(image, (k, k), 0)
    elif blur_type == "median":
        return cv2.medianBlur(image, k)
    elif blur_type == "bilateral":
        return cv2.bilateralFilter(image, k, 75, 75)
    return image


def illumination_correction(image, clip_limit=2.0, tile_grid=8):
    """光照校正：CLAHE + 形态学背景扣除（处理不均匀光照）

    Args:
        image: 灰度图 (uint8)
        clip_limit: CLAHE对比度限制 (越大对比度增强越强)
        tile_grid: CLAHE分块大小 (越大局部效果越弱)
    Returns:
        光照校正后的灰度图
    """
    # 1. CLAHE 增强局部对比度
    grid = max(2, tile_grid)
    clahe = cv2.createCLAHE(clipLimit=clip_limit,
                            tileGridSize=(grid, grid))
    enhanced = clahe.apply(image)

    # 2. 形态学背景扣除（大核开运算估计背景）
    bg_ksize = max(31, image.shape[0] // 8)
    if bg_ksize % 2 == 0:
        bg_ksize += 1
    kernel = cv2.getStructuringElement(cv2.MORPH_ELLIPSE,
                                       (bg_ksize, bg_ksize))
    background = cv2.morphologyEx(enhanced, cv2.MORPH_OPEN, kernel)

    # 3. 减去背景，消除光照梯度
    corrected = cv2.subtract(enhanced, background)

    return corrected


def normalize_division(image, blur_ksize=51):
    """除法归一化：消除乘性光照变化

    原理: result = image / blurred(image) * 255
    将图像除以自身模糊版本，去除低频光照梯度。

    Args:
        image: 灰度图 (uint8)
        blur_ksize: 模糊核大小 (越大消除越大尺度的光照变化)
    Returns:
        归一化后的灰度图
    """
    k = blur_ksize if blur_ksize % 2 == 1 else blur_ksize + 1
    blurred_bg = cv2.GaussianBlur(image.astype(np.float64), (k, k), 0)
    # 避免除以零
    blurred_bg = np.maximum(blurred_bg, 1.0)
    normalized = image.astype(np.float64) / blurred_bg * 255.0
    return np.uint8(np.clip(normalized, 0, 255))


def segment(image, method="otsu", thresh_val=127, max_val=255,
            adaptive_block=11, adaptive_c=2):
    """焊缝区域分割：阈值分割"""
    if method == "otsu":
        _, binary = cv2.threshold(image, 0, max_val,
                                  cv2.THRESH_BINARY + cv2.THRESH_OTSU)
        return binary
    elif method == "triangle":
        _, binary = cv2.threshold(image, 0, max_val,
                                  cv2.THRESH_BINARY + cv2.THRESH_TRIANGLE)
        return binary
    elif method == "manual" or method == "binary":
        _, binary = cv2.threshold(image, thresh_val, max_val,
                                  cv2.THRESH_BINARY)
        return binary
    elif method == "binary_inv":
        _, binary = cv2.threshold(image, thresh_val, max_val,
                                  cv2.THRESH_BINARY_INV)
        return binary
    elif method == "adaptive_mean":
        b = adaptive_block if adaptive_block % 2 == 1 else adaptive_block + 1
        return cv2.adaptiveThreshold(image, max_val,
                                     cv2.ADAPTIVE_THRESH_MEAN_C,
                                     cv2.THRESH_BINARY, b, adaptive_c)
    elif method == "adaptive_gaussian":
        b = adaptive_block if adaptive_block % 2 == 1 else adaptive_block + 1
        return cv2.adaptiveThreshold(image, max_val,
                                     cv2.ADAPTIVE_THRESH_GAUSSIAN_C,
                                     cv2.THRESH_BINARY, b, adaptive_c)
    return image


def morphological_process(binary, op_type="close", ksize=5, iterations=1):
    """形态学处理：去除噪点、填补空洞"""
    k = ksize if ksize % 2 == 1 else ksize + 1
    kernel = cv2.getStructuringElement(cv2.MORPH_RECT, (k, k))
    ops = {
        "open": cv2.MORPH_OPEN,
        "close": cv2.MORPH_CLOSE,
        "dilate": cv2.MORPH_DILATE,
        "erode": cv2.MORPH_ERODE,
    }
    return cv2.morphologyEx(binary, ops.get(op_type, cv2.MORPH_CLOSE),
                            kernel, iterations=iterations)


def detect_edges(image, method="canny", low_thresh=50, high_thresh=150,
                 sobel_ksize=3):
    """边缘检测"""
    if method == "canny":
        return cv2.Canny(image, low_thresh, high_thresh)
    elif method == "sobel":
        k = sobel_ksize if sobel_ksize % 2 == 1 else sobel_ksize + 1
        k = max(1, min(k, 7))
        gx = cv2.Sobel(image, cv2.CV_64F, 1, 0, ksize=k)
        gy = cv2.Sobel(image, cv2.CV_64F, 0, 1, ksize=k)
        mag = np.sqrt(gx ** 2 + gy ** 2)
        if mag.max() > 0:
            mag = mag / mag.max() * 255
        return np.uint8(np.clip(mag, 0, 255))
    elif method == "laplacian":
        k = sobel_ksize if sobel_ksize % 2 == 1 else sobel_ksize + 1
        k = max(1, min(k, 7))
        lap = cv2.Laplacian(image, cv2.CV_64F, ksize=k)
        return np.uint8(np.clip(np.abs(lap), 0, 255))
    elif method == "scharr":
        gx = cv2.Scharr(image, cv2.CV_64F, 1, 0)
        gy = cv2.Scharr(image, cv2.CV_64F, 0, 1)
        mag = np.sqrt(gx ** 2 + gy ** 2)
        if mag.max() > 0:
            mag = mag / mag.max() * 255
        return np.uint8(np.clip(mag, 0, 255))
    return image


def draw_contours_on_original(original, binary, color=(0, 0, 255),
                              thickness=2, min_area=100):
    """在原图上绘制焊缝轮廓（支持面积过滤小噪点）"""
    if len(original.shape) == 2:
        display = cv2.cvtColor(original, cv2.COLOR_GRAY2BGR)
    else:
        display = original.copy()
    contours, _ = cv2.findContours(binary, cv2.RETR_EXTERNAL,
                                   cv2.CHAIN_APPROX_SIMPLE)
    if contours:
        # 面积过滤：只保留面积 >= min_area 的轮廓
        filtered = [c for c in contours if cv2.contourArea(c) >= min_area]
        if filtered:
            cv2.drawContours(display, filtered, -1, color, thickness)
    return display


# ======================== 交互式调参界面 ========================

class WeldSegmentationApp:
    """焊缝分割交互式调参界面"""

    def __init__(self, root):
        self.root = root
        self.root.title("焊缝图像分割与边缘提取 - 交互式调参系统")
        self.root.geometry("1400x900")

        # 图像数据
        self.original = None
        self.gray = None
        self.image_paths = []
        self.current_idx = 0
        self.current_path = None
        self._debounce_id = None

        # 自动查找当前目录下的图片
        self._find_images()

        self._build_ui()

        if self.image_paths:
            self._load_image(self.image_paths[0])

    def _find_images(self):
        """自动查找目录下的焊缝图像"""
        exts = ("*.png", "*.jpg", "*.jpeg", "*.bmp", "*.tif", "*.tiff")
        imgs = []
        for ext in exts:
            imgs.extend(glob.glob(os.path.join(os.path.dirname(
                os.path.abspath(__file__)), ext)))
        # 排除结果图
        self.image_paths = sorted([
            p for p in imgs if "result" not in os.path.basename(p).lower()
        ])

    def _build_ui(self):
        """构建界面布局"""
        # ---- 顶部：图像选择 ----
        top = ttk.Frame(self.root)
        top.pack(fill=tk.X, padx=5, pady=3)

        ttk.Label(top, text="焊缝图像:").pack(side=tk.LEFT)
        self.img_combo = ttk.Combobox(top, state="readonly", width=40)
        self.img_combo["values"] = [os.path.basename(p) for p in
                                    self.image_paths]
        if self.image_paths:
            self.img_combo.current(0)
        self.img_combo.pack(side=tk.LEFT, padx=5)
        self.img_combo.bind("<<ComboboxSelected>>", self._on_image_change)

        ttk.Button(top, text="加载图像...", command=self._browse).pack(
            side=tk.LEFT, padx=5)
        ttk.Button(top, text="保存结果", command=self._save_results).pack(
            side=tk.RIGHT, padx=5)
        ttk.Button(top, text="重置参数", command=self._reset_params).pack(
            side=tk.RIGHT, padx=5)

        # ---- 中部：左侧参数面板 + 右侧图像显示 ----
        mid = ttk.Frame(self.root)
        mid.pack(fill=tk.BOTH, expand=True, padx=5, pady=3)

        # 左侧参数面板（带滚动条）
        left_frame = ttk.Frame(mid, width=320)
        left_frame.pack(side=tk.LEFT, fill=tk.Y)
        left_frame.pack_propagate(False)

        canvas = tk.Canvas(left_frame, highlightthickness=0)
        scrollbar = ttk.Scrollbar(left_frame, orient=tk.VERTICAL,
                                  command=canvas.yview)
        self.param_frame = ttk.Frame(canvas)

        self.param_frame.bind(
            "<Configure>",
            lambda e: canvas.configure(scrollregion=canvas.bbox("all")),
        )
        canvas.create_window((0, 0), window=self.param_frame, anchor=tk.NW)
        canvas.configure(yscrollcommand=scrollbar.set)

        canvas.pack(side=tk.LEFT, fill=tk.BOTH, expand=True)
        scrollbar.pack(side=tk.RIGHT, fill=tk.Y)

        # 鼠标滚轮支持
        def _on_mousewheel(event):
            canvas.yview_scroll(int(-1 * (event.delta / 120)), "units")

        canvas.bind_all("<MouseWheel>", _on_mousewheel)

        self._build_param_controls()

        # 右侧图像显示区域（4个子图：原图、分割、形态学、边缘）
        right = ttk.Frame(mid)
        right.pack(side=tk.LEFT, fill=tk.BOTH, expand=True)

        self.fig = Figure(figsize=(12, 7), dpi=100)
        self.fig.subplots_adjust(hspace=0.35, wspace=0.3)
        self.ax_orig = self.fig.add_subplot(2, 3, 1)
        self.ax_seg = self.fig.add_subplot(2, 3, 2)
        self.ax_morph = self.fig.add_subplot(2, 3, 4)
        self.ax_edge = self.fig.add_subplot(2, 3, 5)
        self.ax_hist = self.fig.add_subplot(2, 3, 3)

        for ax, title in [
            (self.ax_orig, "原图 (灰度)"),
            (self.ax_seg, "阈值分割"),
            (self.ax_morph, "形态学处理"),
            (self.ax_edge, "边缘检测"),
            (self.ax_hist, "灰度直方图"),
        ]:
            ax.set_title(title, fontsize=10)
            ax.axis("off")

        self.canvas_fig = FigureCanvasTkAgg(self.fig, master=right)
        self.canvas_fig.get_tk_widget().pack(fill=tk.BOTH, expand=True)

        # 底部状态栏
        self.status_var = tk.StringVar(value="就绪 - 请加载焊缝图像")
        ttk.Label(self.root, textvariable=self.status_var,
                  relief=tk.SUNKEN).pack(fill=tk.X, padx=5, pady=2)

    def _build_param_controls(self):
        """构建参数调节控件"""
        pf = self.param_frame
        self.params = {}

        def add_slider(parent, label, key, from_, to_, default, step=1):
            ttk.Label(parent, text=label, font=("", 9, "bold")).pack(
                anchor=tk.W, padx=5, pady=(8, 0))
            var = tk.DoubleVar(value=default)
            self.params[key] = var
            frame = ttk.Frame(parent)
            frame.pack(fill=tk.X, padx=5)
            scale = ttk.Scale(frame, from_=from_, to=to_, variable=var,
                              command=lambda v: self._on_param_change())
            scale.pack(side=tk.LEFT, fill=tk.X, expand=True)
            val_label = ttk.Label(frame, text=f"{default}", width=6)
            val_label.pack(side=tk.RIGHT)

            def update_label(*_):
                v = var.get()
                val_label.configure(
                    text=f"{v:.1f}" if step < 1 else f"{int(v)}")

            var.trace_add("write", update_label)

        def add_combo(parent, label, key, options, default_idx=0):
            ttk.Label(parent, text=label, font=("", 9, "bold")).pack(
                anchor=tk.W, padx=5, pady=(8, 0))
            var = tk.StringVar(value=options[default_idx])
            self.params[key] = var
            combo = ttk.Combobox(parent, textvariable=var, values=options,
                                 state="readonly")
            combo.pack(fill=tk.X, padx=5)
            combo.bind("<<ComboboxSelected>>",
                       lambda e: self._on_param_change())

        # ---- 预处理 ----
        ttk.Separator(pf, orient=tk.HORIZONTAL).pack(fill=tk.X, pady=5)
        ttk.Label(pf, text="【预处理参数】", font=("", 10, "bold")).pack(
            anchor=tk.W, padx=5)

        add_combo(pf, "预处理方式", "preprocess_type",
                  ["none", "blur_only", "clahe", "clahe+blur",
                   "normalize"], 0)
        add_combo(pf, "滤波方式", "blur_type",
                  ["gaussian", "median", "bilateral"], 0)
        add_slider(pf, "滤波核大小", "blur_ksize", 1, 31, 5, 2)
        add_slider(pf, "CLAHE对比度限制", "clahe_clip", 0.5, 10.0, 2.0,
                   0.5)
        add_slider(pf, "CLAHE分块大小", "clahe_grid", 2, 16, 8, 1)
        add_slider(pf, "背景估计核大小", "norm_ksize", 21, 151, 51, 2)

        # ---- 阈值分割 ----
        ttk.Separator(pf, orient=tk.HORIZONTAL).pack(fill=tk.X, pady=5)
        ttk.Label(pf, text="【阈值分割参数】", font=("", 10, "bold")).pack(
            anchor=tk.W, padx=5)

        add_combo(pf, "分割方法", "seg_method",
                  ["otsu", "manual", "binary", "binary_inv",
                   "adaptive_mean", "adaptive_gaussian"], 0)
        add_slider(pf, "手动阈值 (binary)", "thresh_val", 0, 255, 127)
        add_slider(pf, "最大值", "thresh_max", 0, 255, 255)
        add_slider(pf, "自适应块大小", "adaptive_block", 3, 51, 11, 2)
        add_slider(pf, "自适应常数C", "adaptive_c", -10, 20, 2)

        # ---- 形态学 ----
        ttk.Separator(pf, orient=tk.HORIZONTAL).pack(fill=tk.X, pady=5)
        ttk.Label(pf, text="【形态学处理参数】", font=("", 10, "bold")).pack(
            anchor=tk.W, padx=5)

        add_combo(pf, "形态学操作", "morph_op",
                  ["close", "open", "dilate", "erode"], 0)
        add_slider(pf, "核大小", "morph_ksize", 1, 31, 5, 2)
        add_slider(pf, "迭代次数", "morph_iter", 1, 10, 1)

        # ---- 边缘检测 ----
        ttk.Separator(pf, orient=tk.HORIZONTAL).pack(fill=tk.X, pady=5)
        ttk.Label(pf, text="【边缘检测参数】", font=("", 10, "bold")).pack(
            anchor=tk.W, padx=5)

        add_combo(pf, "边缘检测方法", "edge_method",
                  ["canny", "sobel", "laplacian", "scharr"], 0)
        add_slider(pf, "Canny低阈值", "canny_low", 0, 255, 50)
        add_slider(pf, "Canny高阈值", "canny_high", 0, 255, 150)
        add_slider(pf, "Sobel/Lap核大小", "edge_ksize", 1, 7, 3, 2)

        # ---- 叠加显示 ----
        ttk.Separator(pf, orient=tk.HORIZONTAL).pack(fill=tk.X, pady=5)
        ttk.Label(pf, text="【叠加显示】", font=("", 10, "bold")).pack(
            anchor=tk.W, padx=5)

        add_slider(pf, "轮廓线粗细", "contour_thick", 1, 5, 2)
        self.params["show_overlay"] = tk.BooleanVar(value=False)
        ttk.Checkbutton(pf, text="在原图上叠加轮廓",
                        variable=self.params["show_overlay"],
                        command=self._on_param_change).pack(anchor=tk.W,
                                                            padx=5, pady=5)

    def _load_image(self, path):
        """加载图像"""
        self.original = imread_cn(path)
        if self.original is None:
            messagebox.showerror("错误", f"无法加载图像: {path}")
            return
        self.gray = cv2.cvtColor(self.original, cv2.COLOR_BGR2GRAY)
        self.current_path = path
        self.status_var.set(f"已加载: {os.path.basename(path)}  "
                            f"尺寸: {self.original.shape[1]}x"
                            f"{self.original.shape[0]}")
        self._update_display()

    def _browse(self):
        """浏览选择图像"""
        path = filedialog.askopenfilename(
            title="选择焊缝图像",
            filetypes=[("图像文件",
                        "*.png *.jpg *.jpeg *.bmp *.tif *.tiff"),
                       ("所有文件", "*.*")])
        if path:
            name = os.path.basename(path)
            if name not in self.img_combo["values"]:
                self.image_paths.append(path)
                self.img_combo["values"] = [
                    os.path.basename(p) for p in self.image_paths
                ]
            self.img_combo.set(name)
            self._load_image(path)

    def _on_image_change(self, event=None):
        """切换图像"""
        idx = self.img_combo.current()
        if 0 <= idx < len(self.image_paths):
            self._load_image(self.image_paths[idx])

    def _on_param_change(self, *_):
        """参数变化时实时更新（带150ms防抖）"""
        if self.gray is not None:
            if self._debounce_id is not None:
                self.root.after_cancel(self._debounce_id)
            self._debounce_id = self.root.after(150, self._update_display)

    def _reset_params(self):
        """重置所有参数为默认值"""
        defaults = {
            "preprocess_type": "blur_only",
            "blur_type": "gaussian", "blur_ksize": 5,
            "clahe_clip": 2.0, "clahe_grid": 8, "norm_ksize": 51,
            "seg_method": "otsu", "thresh_val": 127, "thresh_max": 255,
            "adaptive_block": 11, "adaptive_c": 2,
            "morph_op": "close", "morph_ksize": 5, "morph_iter": 1,
            "edge_method": "canny", "canny_low": 50, "canny_high": 150,
            "edge_ksize": 3, "contour_thick": 2,
        }
        for key, val in defaults.items():
            if key in self.params:
                self.params[key].set(val)
        if "show_overlay" in self.params:
            self.params["show_overlay"].set(False)
        self._update_display()

    def _get_val(self, key):
        """获取参数值"""
        v = self.params[key]
        if isinstance(v, tk.DoubleVar):
            return int(v.get())
        return v.get()

    def _update_display(self):
        """处理流程并刷新显示"""
        if self.gray is None:
            return

        # 1. 预处理（根据选择的预处理方式）
        pp_type = self._get_val("preprocess_type")
        if pp_type == "none":
            blurred = self.gray.copy()
        elif pp_type == "blur_only":
            blurred = preprocess(
                self.gray,
                self._get_val("blur_ksize"),
                self._get_val("blur_type"),
            )
        elif pp_type == "clahe":
            blurred = illumination_correction(
                self.gray,
                clip_limit=self.params["clahe_clip"].get(),
                tile_grid=self._get_val("clahe_grid"),
            )
        elif pp_type == "clahe+blur":
            corrected = illumination_correction(
                self.gray,
                clip_limit=self.params["clahe_clip"].get(),
                tile_grid=self._get_val("clahe_grid"),
            )
            blurred = preprocess(
                corrected,
                self._get_val("blur_ksize"),
                self._get_val("blur_type"),
            )
        elif pp_type == "normalize":
            blurred = normalize_division(
                self.gray,
                blur_ksize=self._get_val("norm_ksize"),
            )
        else:
            blurred = preprocess(
                self.gray,
                self._get_val("blur_ksize"),
                self._get_val("blur_type"),
            )

        # 2. 阈值分割
        binary = segment(
            blurred,
            method=self._get_val("seg_method"),
            thresh_val=self._get_val("thresh_val"),
            max_val=self._get_val("thresh_max"),
            adaptive_block=self._get_val("adaptive_block"),
            adaptive_c=self._get_val("adaptive_c"),
        )

        # 3. 形态学处理
        morphed = morphological_process(
            binary,
            op_type=self._get_val("morph_op"),
            ksize=self._get_val("morph_ksize"),
            iterations=self._get_val("morph_iter"),
        )

        # 4. 边缘检测
        edges = detect_edges(
            morphed,
            method=self._get_val("edge_method"),
            low_thresh=self._get_val("canny_low"),
            high_thresh=self._get_val("canny_high"),
            sobel_ksize=self._get_val("edge_ksize"),
        )

        # 5. 绘图
        for ax in [self.ax_orig, self.ax_seg, self.ax_morph, self.ax_edge,
                   self.ax_hist]:
            ax.clear()
            ax.axis("off")

        self.ax_orig.imshow(self.gray, cmap="gray")
        self.ax_orig.set_title("原图 (灰度)", fontsize=10)

        self.ax_seg.imshow(binary, cmap="gray")
        self.ax_seg.set_title("阈值分割", fontsize=10)

        self.ax_morph.imshow(morphed, cmap="gray")
        self.ax_morph.set_title("形态学处理", fontsize=10)

        self.ax_edge.imshow(edges, cmap="gray")
        self.ax_edge.set_title(
            f"边缘检测 ({self._get_val('edge_method')})", fontsize=10)

        # 灰度直方图（辅助阈值选取）
        self.ax_hist.hist(blurred.ravel(), bins=256, range=(0, 256),
                          color="steelblue", alpha=0.8, linewidth=0.5)
        self.ax_hist.set_title("灰度直方图", fontsize=10)
        self.ax_hist.set_xlabel("灰度值", fontsize=8)
        self.ax_hist.set_ylabel("像素数", fontsize=8)
        self.ax_hist.tick_params(labelsize=7)
        # 标注当前阈值线
        seg_m = self._get_val("seg_method")
        if seg_m in ("manual", "binary", "binary_inv"):
            t = self._get_val("thresh_val")
            self.ax_hist.axvline(x=t, color="red", linestyle="--",
                                linewidth=1.2, label=f"阈值={t}")
            self.ax_hist.legend(fontsize=7)
        elif seg_m == "otsu":
            _, ot = cv2.threshold(blurred, 0, 255,
                                  cv2.THRESH_BINARY + cv2.THRESH_OTSU)
            # recalculate Otsu threshold value
            otsu_t, _ = cv2.threshold(blurred, 0, 255,
                                      cv2.THRESH_BINARY + cv2.THRESH_OTSU)
            # otsu_t is already the threshold from cv2.threshold
            # but we only get binary result; recalculate:
            hist = cv2.calcHist([blurred], [0], None, [256], [0, 256])
            total = blurred.shape[0] * blurred.shape[1]
            sum_total = np.dot(np.arange(256), hist.ravel())
            sum_bg, w_bg = 0.0, 0.0
            max_var, best_t = 0, 0
            for t_i in range(256):
                w_bg += hist[t_i][0]
                if w_bg == 0:
                    continue
                w_fg = total - w_bg
                if w_fg == 0:
                    break
                sum_bg += t_i * hist[t_i][0]
                mean_bg = sum_bg / w_bg
                mean_fg = (sum_total - sum_bg) / w_fg
                var = w_bg * w_fg * (mean_bg - mean_fg) ** 2
                if var > max_var:
                    max_var = var
                    best_t = t_i
            self.ax_hist.axvline(x=best_t, color="red", linestyle="--",
                                linewidth=1.2, label=f"Otsu阈值≈{best_t}")
            self.ax_hist.legend(fontsize=7)

        # 叠加轮廓显示
        if self._get_val("show_overlay"):
            overlay = draw_contours_on_original(
                self.original, morphed,
                thickness=self._get_val("contour_thick"),
            )
            self.ax_orig.imshow(cv2.cvtColor(overlay, cv2.COLOR_BGR2RGB))
            self.ax_orig.set_title("原图 + 轮廓叠加", fontsize=10)

        self.canvas_fig.draw()

        # 更新状态
        weld_pixels = np.count_nonzero(binary)
        total = binary.shape[0] * binary.shape[1]
        ratio = weld_pixels / total * 100
        self.status_var.set(
            f"焊缝像素占比: {ratio:.1f}%  |  "
            f"分割: {self._get_val('seg_method')}  |  "
            f"边缘: {self._get_val('edge_method')}"
        )

    def _save_results(self):
        """保存所有结果图像"""
        if self.gray is None:
            messagebox.showwarning("提示", "请先加载图像")
            return

        save_dir = filedialog.askdirectory(title="选择保存目录")
        if not save_dir:
            return

        # 重新处理一次
        blurred = preprocess(self.gray, self._get_val("blur_ksize"),
                             self._get_val("blur_type"))
        binary = segment(blurred, self._get_val("seg_method"),
                         self._get_val("thresh_val"),
                         self._get_val("thresh_max"),
                         self._get_val("adaptive_block"),
                         self._get_val("adaptive_c"))
        morphed = morphological_process(binary, self._get_val("morph_op"),
                                        self._get_val("morph_ksize"),
                                        self._get_val("morph_iter"))
        edges = detect_edges(morphed, self._get_val("edge_method"),
                             self._get_val("canny_low"),
                             self._get_val("canny_high"),
                             self._get_val("edge_ksize"))
        overlay = draw_contours_on_original(
            self.original, morphed,
            thickness=self._get_val("contour_thick"),
        )

        base = os.path.splitext(os.path.basename(self.current_path))[0]

        imwrite_cn(os.path.join(save_dir, f"{base}_binary.png"), binary)
        imwrite_cn(os.path.join(save_dir, f"{base}_morphed.png"), morphed)
        imwrite_cn(os.path.join(save_dir, f"{base}_edges.png"), edges)
        imwrite_cn(os.path.join(save_dir, f"{base}_overlay.png"), overlay)

        # 拼接对比图（原图 | 分割 | 边缘 | 轮廓叠加）
        h, w = self.gray.shape
        comparison = np.zeros((h, w * 4, 3), dtype=np.uint8)
        comparison[:, :w] = cv2.cvtColor(self.gray, cv2.COLOR_GRAY2BGR)
        comparison[:, w:2 * w] = cv2.cvtColor(binary, cv2.COLOR_GRAY2BGR)
        comparison[:, 2 * w:3 * w] = cv2.cvtColor(edges, cv2.COLOR_GRAY2BGR)
        comparison[:, 3 * w:] = overlay
        imwrite_cn(os.path.join(save_dir, f"{base}_comparison.png"),
                   comparison)

        messagebox.showinfo("保存成功", f"结果已保存到:\n{save_dir}")
        self.status_var.set(f"结果已保存至: {save_dir}")


# ======================== 主入口 ========================

def main():
    """启动交互式调参界面"""
    root = tk.Tk()
    app = WeldSegmentationApp(root)
    root.mainloop()


if __name__ == "__main__":
    main()
