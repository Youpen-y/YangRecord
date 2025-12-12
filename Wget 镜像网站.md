```bash
wget --mirror --convert-links --adjust-extension --page-requisites --no-parent https://example.com/
```

- `--mirror`：递归下载整个站点
- `--page-requisites`：下载图片、CSS、JS 等离线浏览需要的所有资源
- `--convert-links`：把所有链接转为本地链接
- `--adjust-extension`：自动为 HTML 文件添加扩展名
- `--no-parent`：不会跑到网站上层目录
下载完成后，在下载目录里找 `index.html` ，双击即可完全离线浏览。