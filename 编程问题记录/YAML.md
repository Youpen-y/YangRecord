`YAML` 是流行的用于编写配置文件的语言之一。YAML 文件以 `.yaml` 和 `.yml` 为文件扩展名。
YAML 代表 YAML Ain't Markup Language ，和 XML、JSON一样，是一种人类可读的数据序列化语言。

序列化（serialization）是一个过程，其中具有不同数据结构并以不同技术集编写的一个应用程序或服务可以使用标准格式将数据传输到另一个应用程序。

YAML 是一种广泛使用的格式，用于为不同的 DevOps 工具、程序和应用程序编写配置文件，因为它具有人类可读且直观的语法。

YAML 是 JSON 的官方严格超集，可以做 JSON 可做的一切，甚至更多。有效的 YAML 文件可以包含 JSON ，并且 JSON 可以转换为 YAML 。
与 XML 和 JSON 相比，YAML 具有最人类可读、直观且紧凑的语法来定义配置。YAML使用缩进来定义文件中的结构。如
```yaml
Employees:
- name: John Doe
  department: Engineering
  country: USA
- name: YYP
  department: IT support
  country: China
```

---
YAML 中多文档支持——使用 `---` 分隔每个文档
新文件使用 `---` 表示新 YAML 文档的开始，还可以使用 `...` 来标记文档的结尾
```yaml
---
Employees:
- name: John Doe
  department: Engineering
  country: USA
- name: Kate Kateson
  department: IT support
  country: United Kingdom
---
Fruit:
 - Oranges
 - Pears
 - Apples
```

注释：使用 `#` 开头，或位于值后。
```yaml
---
# This is a full line comment
foo: bar # this is a comment, too
```

显式指定数据类型，使用 `!!` 符号和值之前的数据类型名称
```yaml
# parse this value as a string
date: !!str 2022-11-11

## parse this value as a float (it will be 1.0 instead of 1)
fave_number: !!float 1
```
---
语法简介
1. 标量：字符串（strings）、数字（numbers）、布尔值（booleans）、和空值（nulls）
```yaml
---
# integer
19

# float
8.7

# exponential
4.5e+13

# octal
0o23

# hexadecimal
0xFF

# infinity
.inf

# negative infinity
-.inf

# Not a number
.NAN
```
在 YAML 中，以下字符串表示等价
```yaml
A string in YAML!
`A string in YAML!`
"A string in YMAL!" # 可转义
```
如果要编写跨多行的字符串且要保留换行符，使用管道符号 `|`
```yaml
---
message: |
 I am good
 and lines
 and more lines
```
解释为
```yaml
---
message: I am good
 and lines
 and more lines
```
使用 `|+` 表示逐字保留每一行的换行符，同时在末尾保留换行。
而 `|-` 表示逐字保留每行的换行符，但去掉末尾的换行符。


如果希望解析器将跨多行字符串解析为单行字符串，可以使用 `>`
```yaml
---
message: >
 I am good
 and lines
 and more lines
```
解释为（注意，末尾无换行符）
```yaml
---
message: "I am good and lines and more lines"
```
如果使用 `>+` ，含义是行与行之间会折叠成单行，同时保留字符串末尾的换行符。
即替换为
```yaml
---
message: "I am good and lines and more lines\n"
```


`true/True/On/Yes` 和 `false/False/Off/No` 是 YAML 中的关键字。Null 值使用关键字 `null` 或 `~` 来表示。

YAML 中的集合
- 序列（列表/数组）
编写序列使用 `-` 后跟空格，（列表中每项放在单独的行上，可创建嵌套序类）
```yaml
---
items:
	- 1
	- 2
	- 3
	- 4
	- 5
```
等价于
```yaml
---
items: [1, 2, 3, 4, 5]
```

- 映射（字典/哈希）
```yaml
key:value
```
- 带序列的映射
```yaml
frontend_languages:
 - HTML
 - CSS
 - JavaScript
  - React
  - Angular
  - Vue
```


