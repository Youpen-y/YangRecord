Locale environment variables （区域环境变量）告诉 OS 如何显示或输出某些类型的文本。

```bash
$ locale
LANG=en_US.UTF-8
LANGUAGE=en_US:en
LC_CTYPE="en_US.UTF-8"
LC_NUMERIC=en_US.UTF-8
LC_TIME=en_US.UTF-8
LC_COLLATE="en_US.UTF-8"
LC_MONETARY=en_US.UTF-8
LC_MESSAGES="en_US.UTF-8"
LC_PAPER=en_US.UTF-8
LC_NAME=en_US.UTF-8
LC_ADDRESS=en_US.UTF-8
LC_TELEPHONE=en_US.UTF-8
LC_MEASUREMENT=en_US.UTF-8
LC_IDENTIFICATION=en_US.UTF-8
LC_ALL=
```

1. `LANG` - 用于处理 Linux 系统的语言。当使用 LANG 变量指定语言时，系统会使用该变量指定的语言打印消息。
```bash
# 安装中文语言包
# sudo apt install language-pack-zh-hans

# 临时修改
export LANG=zh_CN.utf8

# 永久修改（单用户）
echo "export LANG=zh_CN.utf8" >> ~/.bashrc
source ~/.bashrc

# 永久修改（所有用户）
sudo vim /etc/default/locale
LANG=zh_CN.utf8
```

2. `LC_xxx` - 
- `LC_ALL`：优先于所有其他 `LC_*` 变量和 `LANG` 。如果设置了 `LC_ALL`，则其值将用于所有语言环境类别。
- `LC_CTYPE`：定义字符分类和大小写转换规则。包括哪些字符被视为字母、数字或标点符号，以及如何转换大小写字符。它还影响字符编码
- `LC_NUMERIC`：控制非货币数值的格式，小数分隔符、千位分隔符和数字分组（以 `.` 或 `,` 分隔）
- `LC_TIME`：指定日期和时间的格式。包括日、月、年的顺序、星期和月份的名称以及 12 小时制或 24 小时制。
- `LC_COLLATE`：确定用于比较和排序字符串的排序顺序。
- `LC_MONETARY`：定义货币值的格式。包括货币符号、其位置、小数点分隔符以及千位分隔符。
- `LC_MESSAGES`：控制应用程序和 `shell` 中信息性消息和诊断消息的语言和格式。
- `LC_PAPER`：指定默认纸张尺寸。
- `LC_NAME`：定义名称的格式。
- `LC_ADDRESS`：定义地址的格式。
- `LC_TELEPHONE`：定义电话号码的格式。
- `LC_MEASUREMENT`：指定测量单位（如公制或英制）。
- `LC_IDENTIFICATION`：不像其他的那么常用或定义明确，但与识别信息相关。

2. `LANGUAGE` - GNU `gettext` 特有的变量，一个用于国际化软件消息的系统。与其他变量不同，`LANGUAGE` 可以是一个以冒号分隔的区域设置列表。程序将按顺序尝试在列出的区域设置中查找翻译，直到找到消息目录。如果定义了 `LANGUAGE` ，则在消息翻译方面，它优先于 `LC_ALL` 、`LC_MESSAGES` 和 `LANG` 。

优先级：
`LC_ALL` > `LC_*` > `LANG` > Default Locale

区域设置名称格式：
```bash
language[_territory][.encoding][@modifier]
```
- `language`：两个字母的 ISO 639-1 语言代码（如 `en`, `fr`, `zh`）
- `_territory`：ISO 3166-1 国家/地区代码（`_US` 代表美国，`_CA` 代表加拿大）
- `.encoding`：可选的字符编码（`.UTF-8`）。
- `@modifier`：特定变体的可选修饰符（例如，`@euro` 表示使用欧元货币的地区）


示例：
查看所有用户的当前语言环境
```bash
cat /etc/locale.conf

# localectl status
# sudo localectl set-locale LANG=en_IN.UTF-8
```


显示 `Linux` 或类`Unix`系统上所有可用的语言环境
```bash
locale -a
```
也可以通过 `less /usr/share/i18n/SUPPORTED` 查看所有支持的语言环境的列表。