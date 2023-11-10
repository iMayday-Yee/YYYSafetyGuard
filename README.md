# YYYSafetyGuard
## 基于 API HOOK 的软件行为分析系统
一共是三个模块：
* `FinalMain`：主程序（用于选择要HOOK的软件和要注入的dll文件，并且展示输出内容，用`QT`写的UI界面）
* `TestConsole`：注射器程序（向目标程序注入dll）
* `InjectDll`：具有API HOOK功能的dll文件

同时，还包括一个用来测试的目标程序：
* `TestApp`：测试程序（可以调用各种 Windows API，用`MFC`写的UI界面）
