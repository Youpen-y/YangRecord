1. `Grip` - GitHub Readme Instant Preview
在本地以 GitHub 风格预览 Markdown 文件。
安装：
```bash
pip install grip
```

使用：
渲染仓库的 readme
```bash
$ cd repo
$ grip
* Running on http://localhost:6419/
```

```bash
$ grip CHANGES.md [0.0.0.0][[:][80]] 
```


2. 不常见但有用标签
- `<details>...</details>` 和 `<summary>...</summary>`
```markdown
<details>
 <summary> This is a summary </summary>
Hello World!
</details>
```

<details>
 <summary> This is a summary </summary>
 Hello World!
</details>

3. 引用式链接（reference-style link）语法：正文更简洁，所有链接地址集中在文末，以便统一管理
```markdown
This is a [Mathematical equations][math] 链接

[math]: https://example.com
```
This is a [Mathematical equations][math] 链接

[math]: https://example.com

4. 脚注（footnote）语法
```markdown
This is a sentence with footnote[^1]

[^1]: some content
```
This is a sentence with footnote[^1]

[^1]: some content


5. 图片导入
 `img alt` 是在图片不能正常显示时出现的文本提示；`img title` 是在鼠标移动到图片上时的文本提示。
- 方式1
```markdown
![img alt](../image/example.jpg "img title")
```
- 方式2（html 方式，可调整宽度和高度）
```
<img src="example.jpg" alt="img alt" title="img title" width="500" height="600" style="...">
```
`style="..."` 用于调整图片格式的 `css` 代码。

如何让图片居中？
0. 使用 `<center>` （其中 `style` 指定图片随页面缩放）
```html
<center>
    <img src="image.jpg" alt="img alt" style="width: 100%;" />
</center>
```

0. 使用 `text-align: center` （适用于行内元素，`img` 默认是行内元素）
```html
<div style="text-align: center;">
    <img src="image.jpg" alt="img alt" />
</div>
```
2. 使用 Flexbox （现代网页常用）
```html
<div style="display: flex; justify-content: center;">
    <img src="image.jpg" alt="img alt" />
</div>
```

如何让文字环绕图片？
1. 使用 HTML `float: right`
```
<img src="" style="float: right; margin: 10px;">    
```