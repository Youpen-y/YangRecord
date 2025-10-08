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