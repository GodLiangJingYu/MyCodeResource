# 快速启动 nanobot 虚拟环境
Write-Host "正在激活 nanobot 虚拟环境..." -ForegroundColor Green

cd d:\MyCodeResource\bianyiyuanli\exec\exec1\nanobot-main
.\venv\Scripts\Activate.ps1

Write-Host ""
Write-Host "========================================" -ForegroundColor Cyan
Write-Host "  nanobot 虚拟环境已激活！" -ForegroundColor Green
Write-Host "========================================" -ForegroundColor Cyan
Write-Host ""
Write-Host "现在你可以运行以下命令：" -ForegroundColor Yellow
Write-Host "  - nanobot agent          : 启动 nanobot 对话"
Write-Host "  - nanobot status         : 查看状态"
Write-Host "  - cd ..\自动化工具       : 进入自动化工具目录"
Write-Host ""
Write-Host "输入 'deactivate' 可退出虚拟环境" -ForegroundColor Gray
Write-Host ""
