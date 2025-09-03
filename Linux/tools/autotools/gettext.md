- Internationalization （国际化）= I18n
改变程序以支持多种语言和文化习惯。
1. 字符处理（`unicode`）
2. 区域设置感知（日期格式、货币、数字、时区等）
3. Localizability
	1. 隔离可本地化的项目（消息、图片等）
	2. 实施本地化上述项目所需的基础设施

- Localization （本地化）= L10n
提供国际化的软件包，以支持个人的母语和文化习惯。
1. 将可本地化的项目（消息、图片等）翻译为另一种语言

Gettext = 用于翻译程序输出消息的完整工具集

---
原程序：
```c
#include <config.h>
#include <stdio.h>

void say_hello()
{
	puts("Hello World!");
	puts("This is " PACKAGE_STRING ".");
}
```


```c
#include <config.h>
#include <stdio.h>
#include "gettext.h"
#define _(string) gettext (string)

void say_hello()
{
	puts(_("Hello World!"));
	printf(_("This is %s.\n"), PACKAGE_STRING);
}
```
必须翻译的消息使用 `_(...)` 标记。

| 步骤          | 描述                            |
| ----------- | ----------------------------- |
| 1️⃣ 标记字符串   | 用 `_()`、`gettext()` 包裹要翻译的字符串 |
| 2️⃣ 提取字符串   | 使用 `xgettext` 生成 `.pot` 文件    |
| 3️⃣ 翻译      | 翻译者创建 `.po` 文件，翻译 `msgstr`    |
| 4️⃣ 编译翻译文件  | 用 `msgfmt` 生成 `.mo` 文件        |
| 5️⃣ 程序运行时加载 | 程序根据系统语言加载相应 `.mo`，显示翻译内容     |
`gettext` 在当前区域的目录查找英文消息的翻译。

---
如何给程序添加国际化（`i18n`） 支持的步骤？
1. 从没有国际化的简单程序开始（以 `hello` 为例）
2. 在 `configure.ac` 文件中添加如下宏，这将告诉 `autoconf` 和 `automake`：项目使用 GNU Gettext ，需要生成支持国际化的相关代码。
```m4
AM_GNU_GETTEXT([external]) # GNU libc 或 external Gettext 库将被使用（如果发现）
AM_GNU_GETTEXT_VERSION([0.21])   # 或其他版本
```

3. 运行 `gettextize` 以提供基础设施
```bash
# 在项目根目录下运行
gettextize --copy --no-changelog
```

4. 完善 `gettextize` 留下的配置文件
- 完善 `po/Makevars.template` 文件并重命名为 `po/Makevars`
- 在 `POTFILES.in` 中列出可能包含可翻译字符串的源文件。如：
```po/POTFILES.in
src/main.c
```

5. 更新 `src/Makefile.am` ，让程序 `hello` 链接必要的库
```src/Makefile.am
AM_CPPFLAGS = -DLOCALEDIR=\"$(localedir)\"
bin_PROGRAMS = hello
hello_SOURCES = main.c say.c say.h gettext.h
hello_LDADD = $(LIBINTL)
```
`$(LIBINTL)` 列出了任何国际化程序都应链接到的库。（也可以使用 `LDADD` 不要前面的 `hello_`）
包含 `gettext.h` （将随后使用），以便其被分发。
`$(LOCALEDIR)` 是消息目录安装的位置，这是在初始化期间需要的。

6. 更新代码
```src/main.c
#include <locale.h>
#include "say.h"

int main()
{
    setlocale(LC_ALL, "");  // 按照环境中指定的方式初始化区域设置（如，用户在环境中设置 LANG=fr_FR 以获取法语消息）
    bindtextdomain(PACKAGE, LOCALEDIR);
    textdomain(PACKAGE); // 告诉 Gettext ，哪里可以找到此程序的消息目录，（同一包中的所有程序通常共享相同的消息目录
    say_hello();
    return 0;
}
```

```src/say.c
#include <config.h>
#include <stdio.h>
#include "gettext.h"
#define _(string) gettext(string)

void say_hello()
{
	puts(_("Hello World!"));
	printf(_("This is %s.\n"), PACKAGE_STRING);
}
```
- GNU `gettext` 所支持的国际化系统叫做 NLS ，可以被禁用
	- 显示使用 `./configure --disable-nls`
	- 如果你的系统上没有安装 `gettext` 库或开发头文件，`autoconf` 也会自动禁用 NLS 。
如果 NLS 被禁用（显式或隐式），`gettext.h` 中会用如下方式定义这些函数：
```c
#define gettext(Msgid) (Msgid)
#define textdomain(Domain) ((char *)(Domain))
#define bindtextdomain(Domain, Dir) ((char *)(Dir))
```
这些定义是“no-op”—— 不会做任何事，直接返回原始字符串。

GNU `gettext` 国际化系统中 `po/` 目录与 `.pot` 模板
`po/` 是 `gettext` 项目中标准的目录，用来存放翻译相关的文件（message catalogs）。
这个目录通常包含以下类型的文件：
- `*.pot`：模板（Template）文件
- `*.po`：各个语言的翻译文件（Portable Object）
- `*.mo`：编译后的二进制翻译文件（Machine Object）
```
源码：printf(_("Hello")) 
        ↓
xgettext 提取
        ↓
.po 模板 → amhello.pot
        ↓
翻译人员根据它生成：
        fr.po, zh_CN.po ...
```

如何为项目添加新的语言翻译？
1.  使用 `msginit` 从 `.pot` 模板文件（如 `amhello.pot`）初始化新语言翻译文件 `po/LL.po` 或 `po/LL_CC.po` 。该 `.po` 文件将包含待翻译的字符串及其原始文本，供翻译者填写翻译。
> `LL` 是语言代码，`CC` 是国家代码
```bash
cd po
msginit --input=amhello.pot --locale=zh_CN.UTF-8
```
2.  完善 `po/LL.po`（或 `po/LL_CC.po`），以法语为例
```po/fr.po
#: src/say.c:9 
msgid "Hello World!" 
msgstr "Bonjour Monde !"  

#: src/say.c:10 
#, c-format 
msgid "This is %s.\n" 
msgstr "Ceci est %s.\n"
```
同时使用 `msginit` 定制一些配置。
3. 新翻译 `po/fr.po` 的验证
```bash
msgfmt --statistics --check po/fr.po
```

4. 在 `LINGUAS` 中注册语言
```bash
echo fr >> LINGUAS
```

5. 配置构建时执行 `make update-po`
```bash
$ ./configure --prefix ~/test
$ make
$ cd po
$ make update-po
$ cd ..
$ make install
$ LANG=fr_FR ~/test/bin/hello
Bonjour Monde !
Ceci est amhello 3.0.
```

---
为了确保翻译完整、质量高，在发布前应冻结字符串内容：
1. 发布前约两周：
	-  执行：（生成最新 `.pot` 模板，发送给翻译者）
```bash
make update-po
```

2. 之后不要更改源代码中的字符串，以免打乱翻译工作。
3. 收集并合并翻译者返回的 `.po` 文件后再发布版本。