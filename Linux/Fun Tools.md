`asciinema` - 终端录制工具
```bash
$ asciinema rec demo.cast # 捕获终端的所有内容并以 asciicast 格式保存到 demo.cast 文件中
# ctrl+d 或 键入exit 退出

$ asciinema play demo.cast # 重播记录
# 可以通过 space 暂停/恢复，通过 ctrl+c 结束播放
```

---
`tte` - Terminal Text Effects，终端视觉效果引擎
可作为系统应用程序安装以在终端中产生效果，也可以作为 python 库安装以在 python 脚本/应用程序中启用效果。
```bash
$ cat your_text | tte <effect> [options] 
```

effect:
```
    beams               Create beams which travel over the canvas illuminating the characters behind them.
    binarypath          Binary representations of each character move through the terminal towards the home coordinate of the character.
    blackhole           Characters are consumed by a black hole and explode outwards.
    bouncyballs         Characters are bouncy balls falling from the top of the canvas.
    bubbles             Characters are formed into bubbles that float down and pop.
    burn                Burns vertically in the canvas.
    colorshift          Display a gradient that shifts colors across the terminal.
    crumble             Characters lose color and crumble into dust, vacuumed up, and reformed.
    decrypt             Display a movie style decryption effect.
    errorcorrect        Some characters start in the wrong position and are corrected in sequence.
    expand              Expands the text from a single point.
    fireworks           Characters launch and explode like fireworks and fall into place.
    matrix              Matrix digital rain effect.
    middleout           Text expands in a single row or column in the middle of the canvas then out.
    orbittingvolley     Four launchers orbit the canvas firing volleys of characters inward to build the input text from the center out.
    overflow            Input text overflows and scrolls the terminal in a random order until eventually appearing ordered.
    pour                Pours the characters into position from the given direction.
    print               Lines are printed one at a time following a print head. Print head performs line feed, carriage return.
    rain                Rain characters from the top of the canvas.
    randomsequence      Prints the input data in a random sequence.
    rings               Characters are dispersed and form into spinning rings.
    scattered           Text is scattered across the canvas and moves into position.
    slice               Slices the input in half and slides it into place from opposite directions.
    slide               Slide characters into view from outside the terminal.
    spotlights          Spotlights search the text area, illuminating characters, before converging in the center and expanding.
    spray               Draws the characters spawning at varying rates from a single point.
    swarm               Characters are grouped into swarms and move around the terminal before settling into position.
    synthgrid           Create a grid which fills with characters dissolving into the final text.
    unstable            Spawn characters jumbled, explode them to the edge of the canvas, then reassemble them in the correct layout.
    vhstape             Lines of characters glitch left and right and lose detail like an old VHS tape.
    waves               Waves travel across the terminal leaving behind the characters.
    wipe                Wipes the text across the terminal to reveal characters.
```

---
`btm`
bottom《 A customizable cross-platform graphical process/system monitor for the terminal. Supports Linux, macOS, and Windows.
```bash
$ btm
```

![[btm image.png]]

---
`r2`
r2 是 `radare` 的完全重写。它提供了一组库、工具和插件来简化逆向工程任务。可以编辑本地硬盘上的文件、查看内核内存以及在本地或通过远程 `gdb/windbg` 服务器调试程序。`r2` 的广泛体系结构支持允许分析、模拟、调试、修改和反汇编任何二进制文件。
`radare2` 可以直接从命令行使用，因此无需进入交互模式即可运行命令。可以在 `shell` 脚本中包含 `radare2` 时非常方便。
只需在终端中输入一行即可执行分析、提取信息或操作二进制文件。

---
`mapscii`


---
CRIU (Checkpoint and Restore in Userspace)
一个实用程序用于 checkpoint/restore Linux tasks。
```bash
$ criu dump
$ criu restore
```
附：[与其他 CR 的对比](https://criu.org/Comparison_to_other_CR_projects)