# YYYSafetyGuard
## 基于 API HOOK 的软件行为分析系统

### 功能介绍
是一个基于 API HOOK 的软件行为分析系统，对 Windows 很多常见的 API 进行了 hook，从而监视目标软件的行为，并进行分析。

其中，对于威胁度较小的行为，进行记录。

对于威胁度较大的行为则拒绝执行并转而执行修改之后的行为，并发出警告。

软件在整个测试过程中的行为都会被记录在日志中，在测试结束时会统计各种 API 调用的次数。

### 文件结构
一共是三个模块：
* `FinalMain`：主程序（用于选择要HOOK的软件和要注入的dll文件，并且展示输出内容，用`QT`写的UI界面）
* `TestConsole`：注射器程序（向目标程序注入dll）
* `InjectDll`：具有API HOOK功能的dll文件

同时，还包括一个用来测试的目标程序：
* `TestApp`：测试程序（可以调用各种 Windows API，用`MFC`写的UI界面）

### 使用方法
1. 用`Visual Studio`分别打开`TestConsole`, `InjectDll` 并编译
2. 用`QT`打开`FinalMain`并编译
3. 用 __安装了MFC模块__ 的`Visual Studio`打开`TestApp`并编译
4. 运行`FinalMain`和`TestApp`
5. 在`FinalMain`中选择目标程序、注射器程序和dll文件，即可开始对目标软件行为进行监视和分析
