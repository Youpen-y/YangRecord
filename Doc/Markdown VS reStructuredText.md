Markdown 与 reStructuredText 都是轻量级标记语言，注重纯文本的可读性，两者都广泛用于 API 文档。
Markdown 的语法目的：用作网页写作的格式，即支持内联 HTML 。
reStructuredText 专为编写技术文档而设计。（写书）
- reStructuredText 提供了称为 `directives` 和 `roles` 的标准扩展机制。如可以使用 math roles 写数学方程。
```bash
The area of a circle is :math:`A_\text{c} = (\pi/4) d^2`.
```
- 既能很好地使用 Javascript 库 MathJax 渲染为 HTML ，也可以使用 LaTeX 渲染为 PDF 。
- 为不同的语言结构提供了一组 roles 和 directives 。如 `:py:class` （Python 类），`:cpp:enum:`（C++ 枚举）
```