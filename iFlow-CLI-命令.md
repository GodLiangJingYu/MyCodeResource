# iFlow CLI 命令参考

## 命令

| 命令 | 说明 |
|------|------|
| `/about` | 显示版本信息 |
| `/language` | 更改 iFlow CLI 语言 |
| &nbsp;&nbsp;`zh-CN` | 简体中文 |
| &nbsp;&nbsp;`en-US` | English |
| `/agents` | 代理交互命令 |
| &nbsp;&nbsp;`list` | 列出可用的代理 |
| &nbsp;&nbsp;`refresh` | 从源文件刷新代理 |
| &nbsp;&nbsp;`online` | 浏览并从在线仓库安装代理 |
| &nbsp;&nbsp;`install` | 使用引导设置安装新代理 |
| `/2025` | 查看 2025 年度总结 |
| `/auth` | 更改认证方式 |
| `/bug` | 提交错误报告 |
| `/chat` | 管理对话历史 |
| &nbsp;&nbsp;`list` | 列出已保存的对话检查点 |
| &nbsp;&nbsp;`save` | 将当前对话保存为检查点。用法：`/chat save <标签>` |
| &nbsp;&nbsp;`resume` | 从检查点恢复对话。用法：`/chat resume <标签>` |
| &nbsp;&nbsp;`delete` | 删除对话检查点。用法：`/chat delete <标签>` |
| `/clear` | 清除屏幕和对话历史 |
| `/cleanup-checkpoint` | 清理所有检查点历史，释放磁盘空间 |
| `/cleanup-history` | 清理当前项目的对话历史，释放磁盘空间 |
| `/commands` | 管理市场命令：列出本地命令、浏览在线命令、获取详细信息、添加/移除 CLI 命令（项目/全局范围） |
| &nbsp;&nbsp;`list` | 列出项目和全局范围内本地安装的命令 |
| &nbsp;&nbsp;`online` | 在交互式对话框中浏览在线市场的可用命令 |
| &nbsp;&nbsp;`get` | 通过 ID 获取特定命令的详情 |
| &nbsp;&nbsp;`add` | 通过 ID 添加特定命令到本地 CLI（使用 `--scope global` 进行系统范围安装） |
| &nbsp;&nbsp;`remove` | 移除本地安装的命令（使用 `--scope global` 从全局移除） |
| `/compress` | 通过摘要替换来压缩上下文（别名：`/compact`、`/summarize`） |
| `/copy` | 将最后的结果或代码片段复制到剪贴板 |
| `/demo` | 用于研究和头脑风暴工作流的交互式任务 |
| `/docs` | 在浏览器中打开完整的 iFlow CLI 文档 |
| `/directory` | 管理工作空间目录 |
| &nbsp;&nbsp;`add` | 向工作区添加目录（绝对路径）；如需添加多个目录，请使用逗号分隔 |
| &nbsp;&nbsp;`show` | 显示工作区中的所有目录 |
| `/editor` | 设置外部编辑器偏好 |
| `/export` | 导出对话历史 |
| &nbsp;&nbsp;`clipboard` | 将对话复制到系统剪贴板 |
| &nbsp;&nbsp;`file` | 将对话保存到当前目录的文件中 |
| `/extensions` | 列出激活的扩展 |
| `/help` | iFlow CLI 帮助信息 |
| `/ide` | 管理 IDE 连接 |
| `/init` | 分析项目并创建或更新定制的 IFLOW.md 文件 |
| `/log` | 显示当前会话日志存储位置 |
| `/mcp` | 列出已配置的 MCP 服务器和工具，浏览在线仓库，或使用支持 OAuth 的服务器进行身份验证 |
| &nbsp;&nbsp;`list` | 已配置 MCP 服务器和工具的交互式列表 |
| &nbsp;&nbsp;`auth` | 与支持 OAuth 的 MCP 服务器进行身份验证 |
| &nbsp;&nbsp;`online` | 浏览并安装在线仓库中的 MCP 服务器 |
| &nbsp;&nbsp;`refresh` | 刷新 MCP 服务器和工具列表，并重新加载设置文件 |
| `/memory` | 记忆交互命令 |
| &nbsp;&nbsp;`show` | 显示当前内存内容 |
| &nbsp;&nbsp;`add` | 向内存添加内容 |
| &nbsp;&nbsp;`refresh` | 从源刷新内存 |
| &nbsp;&nbsp;`list` | 列出所有内存文件 |
| `/model` | 切换模型 |
| `/output-style` | 更改您的输出样式偏好（使用 `--scope global` 进行全局设置） |
| `/output-style:new` | 使用 `/output-style:new <描述>` 创建自定义输出样式 |
| `/qa` | 基于知识库检索的智能问答 |
| `/quit` | 退出 CLI |
| `/statusline` | Set up iFlow status line UI |
| `/restore` | 恢复工具调用。这将重置对话和文件历史记录到建议工具调用时的状态 |
| `/resume` | 从历史记录中恢复之前的会话历史 |
| `/skills` | 管理技能 |
| &nbsp;&nbsp;`list` | 已配置技能的交互式列表 |
| &nbsp;&nbsp;`refresh` | 刷新技能列表 |
| &nbsp;&nbsp;`online` | 浏览在线技能市场 |
| `/stats` | 检查会话统计。用法：`/stats [model\|tools]` |
| &nbsp;&nbsp;`model` | 显示模型使用统计 |
| &nbsp;&nbsp;`tools` | 显示工具使用统计 |
| `/theme` | 更改主题 |
| `/terminal-setup` | 安装 Shift+Enter 快捷键以支持在输入框中换行 |
| `/tools` | 列出可用的 iFlow CLI 工具 |
| `/update` | 更新版本 |
| `/vim` | 开启/关闭 vim 模式 |
| `/setup-github` | 设置 GitHub Actions |
| `!` | shell 命令 |

## 键盘快捷键

| 快捷键 | 功能 |
|--------|------|
| `Alt+Left/Right` | 在输入中跳转单词 |
| `Ctrl+C` | 退出应用程序 |
| `Ctrl+G` | 切换帮助对话框 |
| `Ctrl+Enter` | 新行 |
| `Ctrl+L` | 清除屏幕 |
| `Ctrl+X` | 在外部编辑器中打开输入 |
| `Ctrl+Y` | 切换 YOLO 模式 |
| `Ctrl+O` | 切换调试控制台显示 |
| `Ctrl+V` | 图片粘贴 |
| `Enter` | 发送消息 |
| `Esc` | 取消操作 |
| `Shift+Tab` / `Alt+M` | 切换模式 |
| `Up/Down` | 循环浏览您的提示历史 |

