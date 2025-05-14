`PowerShell` 执行策略（Execution Policy）是一个安全功能，用于控制哪些 PowerShell 脚本可以在系统上运行。它可以帮助防止恶意脚本的执行，从而保护系统安全。
在 Windows 计算机上，您可以为本地计算机、当前用户或特定会话设置执行策略。
##### 执行策略的类型
- `Restricted`
	- 最严格的策略。它完全禁止运行任何脚本，包括 `.ps1` 文件和 `.psm1` 模块文件
- `AllSigned`
	- 只有经过数字签名的脚本才能运行。签名必须由受信任的证书颁发机构（CA）颁发。
- `RemoteSigned`
	- 允许运行本地创建的脚本（即未签名的脚本），但远程下载的脚本必须经过数字签名。
- `Unrestricted`
	- 允许运行所有脚本，无论是否签名。这是最宽松的策略。
- `Bypass`
	- 允许运行所有脚本，但不会加载配置文件（如 `Profile.ps1`）。这是一种折中的策略，既允许运行脚本，又不会加载可能包含恶意代码的配置文件。
- `Default`
	- 设置默认执行策略
##### 默认执行策略
默认情况下，Windows 系统的 PowerShell 执行策略是 `Restricted`。这意味着系统默认不允许运行任何脚本，以防止恶意脚本的执行。

##### 查看当前的执行策略
```powershell
Get-ExecutionPolicy
```

##### 更改执行策略
```powershell
Set-ExecutionPolicy -Scope Process -ExecutionPolicy Bypass
```
- `-Scope Process`：仅对当前 `PowerShell` 会话生效，不会影响系统全局设置。
- `-ExecutionPolicy Bypass`：允许运行脚本，但不会加载配置文件。
