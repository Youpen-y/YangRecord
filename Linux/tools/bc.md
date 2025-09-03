`bc` （Basic Calculator）—— 任意精度计算语言

特点：
- 支持任意精度的运算
- 支持浮点数（整数除法不会默认输出小数）
- 支持变量、循环、条件语句
- 可在脚本中嵌入使用
- 可启用标准数学库（`-l`）

使用：
1. 进入交互模式
```bash
$ bc
bc 1.07.1
Copyright 1991-1994, 1997, 1998, 2000, 2004, 2006, 2008, 2012-2017 Free Software Foundation, Inc.
This is free software with ABSOLUTELY NO WARRANTY.
For details type `warranty'. 
3 + 10
13
10 / 4
2
^C
(interrupt) use quit to exit.
quit
```

2. 直接在命令行中计算
```bash
echo "3 + 5" | bc
echo "10/4"  | bc
```

3. 浮点运算
```bash
echo "scale=2; 10 / 4" | bc -l  # 使用 scale=... 控制精度
```