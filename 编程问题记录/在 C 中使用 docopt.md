如何使用 `docopt` 向 C 项目中添加选项解析？
### 步骤1
根据 `docopt` 标准编写一个名为 USAGE 的文件，示例如下：
```
jiajia - a software distributed shared memory library

Usage:
	jiajia --version
	jiajia --help
Options:
	-a --alloc		Alloc memory
	-v --version 	Display version
	-h --help		Prints a short usage summary
```

### 步骤2
获取 `docopt2ragel`
```bash
pip install docopt2ragel
```

安装 `Ragel`
```bash
sudo apt install ragel
```

将 USAGE 文件编译到 `Ragel` 状态机中
```bash
docopt2ragel USAGE > usage.rl
```

将 `Ragel` 状态机编译为 C 源文件
```bash
ragel usage.rl
```

将 C 源文件包含到项目中
```c
#include "usage.c"

int main(int argc, char **argv) 
{
	int e, i;
	options_t opts;
	e = parse_options(argc, argv, &opts);

	if(-1 == e) {
		exit(-1);
	} else if (opts.help) {
		show_usage();
		exit(0);
	} else if (opts.version) {
		fprintf(stdout, "%s\n", VERSION);
		exit(0);
	}
	
}
```

### 发生了什么？
`docopt2ragel` 使用 `Docopt` 解析 `Docopt` usage 字符串。其输出是一个 `Ragel` 状态机，定义了我们的参数解析器的语法。`Ragel` 使用这个状态机生成我们需要的 C 代码。






