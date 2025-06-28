`WebP` （Web Picture）是 Google 开发的一种图像文件格式，提供有损和无损压缩，旨在为 Web 生成更小图像，以替代 JPEG、PNG 和 GIF 文件格式。

特点：

| 特性         | 说明                                                    |
| ---------- | ----------------------------------------------------- |
| **高压缩率**   | 相比 JPEG 和 PNG，WebP 在质量相近的情况下文件更小。                     |
| **支持有损压缩** | 类似于 JPEG，通过去除不必要的数据减少文件大小。                            |
| **支持无损压缩** | 类似于 PNG，保留完整图像数据但仍有较高压缩率。                             |
| **支持透明度**  | 无损模式下支持 alpha 通道（即透明像素），而 JPEG 不支持。                   |
| **支持动画**   | 可以包含多帧图像，类似 GIF，体积更小且质量更好。                            |
| **浏览器兼容性** | 大部分现代浏览器已支持，包括 Chrome、Firefox、Edge、Opera、Safari（新版本）。 |

处理工具 —— `webp`
```bash
# 包信息
sudo apt show webp
Package: webp
Version: 1.3.2-0.4build3
Priority: extra
Section: universe/graphics
Source: libwebp
Origin: Ubuntu
Maintainer: Ubuntu Developers <ubuntu-devel-discuss@lists.ubuntu.com>
Original-Maintainer: Jeff Breidenbach <jab@debian.org>
Bugs: https://bugs.launchpad.net/ubuntu/+filebug
Installed-Size: 375 kB
Depends: libc6 (>= 2.34), libgif7 (>= 5.1), libglut3.12 (>= 3.4.0), libglx0, libjpeg8 (>= 8c), libopengl0, libpng16-16t64 (>= 1.6.2), libsharpyuv0 (>= 1.3.2), libtiff6 (>= 4.0.3), libwebp7 (>= 1.3.2), libwebpdemux2 (>= 1.3.2), libwebpmux3 (>= 1.3.2)
Homepage: https://developers.google.com/speed/webp/
Task: ubuntustudio-publishing, edubuntu-desktop-gnome, edubuntu-desktop-gnome-raspi
Download-Size: 103 kB
APT-Manual-Installed: yes
APT-Sources: http://mirrors.tuna.tsinghua.edu.cn/ubuntu noble/universe amd64 Packages
Description: Lossy compression of digital photographic images
 Image Compression format, based on the VP8 codec.
 WebP uses the modern VP8 compression format to deliver efficient
 compression of images for the web. More than 30% extra gain over
 optimized JPEG, for same quality, is not unusual.

# 安装 webp 包
sudo apt install webp
```

格式转换
`cwebp` —— 压缩图像文件为 `WebP` 文件
语法
```bash
cwebp [options] input_file -o output_file.webp
```
`options`：
- `-lossless`：无损压缩
- `-q float`：指定 RGB 通道压缩因子（compression factor），范围为 0 到 100 ，默认为 75 。在有损压缩（默认）中，小的因子产生的文件小但质量低，100 代表最优质量。在无损压缩中（指定 `-lossless` 选项）， 小因子压缩更快，但文件更大。
- `-resize 800 600`：把图像调整为 800 像素宽，600 像素高。
- `-sns 80`：设置空间降噪强度为 80，较高的值会产生更平滑的图像，但可能牺牲一些细节。

---
`dwebp` —— 从 `WebP` 文件解压为图像文件
语法
```bash
dwebp [options] input_file.webp
```
---
示例：
- 转换为 `webp`
```bash
# 将 JPEG 转为 WebP（有损压缩）
cwebp input.jpg -q 80 -o output.webp

# 将 PNG 转为 WebP（无损）
cwebp -lossless input.png -o output.webp
```
- 从 `webp` 转出
```bash
dwebp input.webp -o output.png
```