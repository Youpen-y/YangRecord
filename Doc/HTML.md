`<img>` 标签：用于在网页中插入图片。
特点：
- 自闭合标签，没有像 `<div>...</div>` 的结束标签。
属性：
- `src`：source，指定图片文件的路径或 URL 。
- `alt`：alternative text，替代文本，在图片无法显示时，浏览器将显示 `alt` 的文字。另外，对于无障碍，屏幕阅读器会读出 `alt` 的内容。
- `width, height`：（可选）控制显示大小。
示例：
```html
<img src="dog.jpg" alt="一只小狗" width="300" height="200">
```

---
`<a>` 标签：超链接（anchor），可以跳转到网页、文件、邮箱等
语法：
```html
<a href="target_url"> link_text_or_image_or_others </a>
```
示例：
```html
<!-- 跳转到其他网站 -->
<a href="https://www.google.com">Google</a>

<!-- 跳转到当前页面的某个位置 -->
<a href="#section1">跳到第1部分</a>

<!-- 发送邮件 -->
<a href="mailto:someone@example.com">发邮件给我</a>

<!-- 新标签页打开 -->
<a href="https://example.com" target="_blank">新窗口打开 Example</a>
```
----
`<br/>` 标签：换行（break）
作用：强制换行，相当于回车。
特点：自闭合标签
如果写的是 `HTML5`，可以使用 `<br>`；如果是 `XHTML` ，使用 `<br/>`

---
`<figure>` 标签：包裹一段独立内容（图片、图表、代码等），配合 `<figcaption>` 提供说明。
1. 插图
```html
<figure>
  <img src="diagram.png" alt="系统架构图">
  <figcaption>系统架构图，展示了各个模块的关系</figcaption>
</figure>
```
2. 代码片段
```html
<figure>
<code>print(hello)</code>
<figcaption>代码</figcaption>
</figure>
```
