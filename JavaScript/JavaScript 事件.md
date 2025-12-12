HTML 事件是发生在 HTML 元素上的事情。当在 HTML 页面中使用 JavaScript 时，JavaScript 可以触发这些事件。

HTML 事件可以是浏览器行为，也可以是用户行为。如：HTML 页面完成加载，HTML input 字段改变时，HTML 按钮被点击。下面是常见的 HTML 事件的列表。

| 事件            | 描述                    |
| ------------- | --------------------- |
| `onchange`    | HTML 元素改变             |
| `onclick`     | 用户点击 HTML 元素          |
| `onmouseover` | 鼠标指针移动到指定的元素上时发生      |
| `onmouseout`  | 用户从一个 HTML 元素上移开鼠标时发生 |
| `onkeydown`   | 用户按下键盘按键              |
| `onload`      | 浏览器已完成页面的加载           |
在事件触发时，可以执行 JavaScript 代码。
```html
<some-HTML-element some-event='JavaScript code'> // 或者是双引号
```