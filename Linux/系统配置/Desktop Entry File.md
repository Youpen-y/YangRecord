Desktop Entry File 是创建和排序图形用户界面（GUI）快捷方式的标准方法。`GNOME` , `KDE` 和 `Xfce` 都遵循 `XDG Desktop Entry Specification` （事实上的标准）。

`.desktop` 文件是 `Linux` 桌面环境用来定义应用程序快捷方式和启动器的配置文件。它提供程序的元数据，如名称、图标、执行命令和其他属性。该文件通常用于在基于 Linux 的系统中的应用程序菜单、桌面启动器或面板中创建快捷方式。
> 为了正确运行，`.desktop` 文件必须是可执行的。

Entry File 可以描述三种元素类型：
- 应用程序的快捷方式（`.desktop` 扩展名）
- 资源链接（`.desktop` 扩展名）
- 子菜单（`.directory` 扩展名）
这些元素充当快速可视的访问点，每一项通常都表现为桌面上或系统菜单上的 GUI 条目。该入口所在位置取决于 Desktop Entry File 所在位置。

与普通文件一样，用户可以在创建 `.desktop` 文件的位置执行它们。然而它们的功能之一在于与 GUI 的连接。

目录入口文件（`directory entry file`）最好放置在桌面上或 `$XDG_DATA_DIRS/applications` 中。如果未设置 `XDG_DATA_DIRS` 环境变量，则默认值为：
- `/usr/share` （global）
- `~/.local/share` （user-specific）
在桌面上放置一个 `.desktop` 文件，将创建一个快捷方式；在 `$XDG_DATA_DIRS/applications` 中放置将为该快捷方式添加一个 GUI 菜单图标。
使用 `.directory` 创建系统 GUI 子菜单。

这些文件由三类行组成
- 注释 （以`#`开头）
- 组标题
- 键值对

示例：（注意：`.desktop` 不支持行内注释，仅理解使用）
```desktop
[Desktop Entry]		# Group header line(needed)
Name=				# 应用程序名称
Comment=			# 应用程序简述
Exec=				# 定义启动应用程序时要执行的命令
Icon=				# 与应用程序关联的图标文件的路径
Type=				# Entry 类型（Application/Link/Directory）
Terminal=			# 应用程序是否应在终端窗口中运行
Categories=			# 应用程序应在菜单中显示的类别或组
StartupNotify=		# 桌面环境是否应显示应用程序启动通知
NoDisplay=			# 应用程序是否应从菜单中隐藏
Actions=			# 可以在应用程序上执行的其他操作，如打开特定文件
```
由于应用程序可以接受参数，因此 `Exec` 中可使用
- `%f`：单个文件名
- `%u`：单个 URL
- `%k`：替换为当前 `.desktop` 文件的完整路径（`URI` 格式或本地文件系统路径）

`Categories` 决定应用在应用菜单中分类和显示位置。[分类规范参考](https://specifications.freedesktop.org/menu-spec/latest/category-registry.html)

| 类别名           | 说明              | 子类                                                              |
| ------------- | --------------- | --------------------------------------------------------------- |
| `AudioVideo`  | 音频和视频类应用（播放器等）  | `Audio`, `Video`, `Player`, `Recorder`                          |
| `Development` | 开发工具（IDE、编辑器）   | `IDE`, `Debugger`, `GUIDesigner`                                |
| `Education`   | 教育类软件           |                                                                 |
| `Game`        | 游戏              |                                                                 |
| `Graphics`    | 图形工具（如 GIMP、绘图） | `2DGraphics`, `3DGraphics`, `Viewer`                            |
| `Network`     | 网络工具（浏览器、聊天）    | `InstantMessaging`                                              |
| `Office`      | 办公软件（文档、表格等）    | `Calendar`, `ContactManagement`, `Spreadsheet`, `WordProcessor` |
| `Settings`    | 系统设置            |                                                                 |
| `System`      | 系统工具（监控器、终端等）   |                                                                 |
| `Utility`     | 实用工具（计算器、文本编辑器） |                                                                 |
| `Audio`       | 音频相关，如播放器、录音机   |                                                                 |
| `Video`       | 视频相关，如播放器、编辑器   |                                                                 |
| `Security`    | 安全工具、防火墙等       |                                                                 |

示例：
```
[Desktop Entry] 
Name=MyTextEditor 
Comment=A simple text editor 
Exec=mytexteditor %F 
Icon=/path/to/icon.png 
Terminal=false 
Type=Application 
Categories=TextEditor;Utility; 
StartupNotify=true 
NoDisplay=false 
Actions=OpenNewWindow;OpenExistingFile; 

# 附加操作1
[Desktop Action OpenNewWindow] 			
Name=Open New Window 
Exec=mytexteditor 

# 附加操作2
[Desktop Action OpenExistingFile] 		
Name=Open Existing File 
Exec=mytexteditor %U
```


更新 `desktop entries` 数据库，通常，`desktop entry` 更改一般会由桌面环境自动获取。如果情况并非如此，可强制更新 `~/.local/share/applications` 中定义的桌面条目。
```bash
update-desktop-database ~/.local/share/applications
```