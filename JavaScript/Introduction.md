> Created to "make web pages alive".

该语言的程序称为脚本，可直接写在网页的 HTML 中，并在页面加载时自动运行。脚本以纯文本形式提供和执行，无需特殊准备或编译即可运行。

JavaScript 名称中的 Java 纯粹是营销决策“蹭热度”。

如今，JavaScript 不仅可以在浏览器中运行，还可以在服务器上执行，或者是任何包含 JavaScript engine（JS 引擎）程序的设备上执行。
不同的引擎有不同的代号：
- `V8` - Chrome, Opera 和 Edge 中
- `SpiderMonkey` - Firefox 中
-  其他如 IE 的 `Chakra`，Safari 的 `JavaScriptCore, Nitro, SquirrelFish`等

引擎很复杂，但基础很简单
1. 引擎（如果是浏览器则嵌入）读取（“解析”）脚本
2. 然后将脚本转换为（“编译为”）机器代码
3. 机器代码可以快速运行

JavaScript 被称为“安全语言”，是因为它刻意屏蔽了对底层硬件和内存的直接访问，让开发者专注于应用逻辑，而不用担心底层错误或安全漏洞。这种设计来自它的历史使命 —— 为浏览器网页服务，而浏览器环境并不需要（也不能允许）脚本直接操作硬件。

JavaScript 本身是一门语言，它的核心语法和特性（比如函数、对象、原型、闭包）是固定的。但它能做的“事情”（比如操作文件、发起网络请求、操纵网页）却取决于它运行的宿主环境（host environment）。
常见的宿主环境：
- 浏览器
- `Node.js`（后端运行环境）
- `Deno`，`Bun` 等其他 JS 运行时

JavaScript 的功能很大程度上取决于它运行的环境。
在 `Node.js` 环境下，JavaScript 拥有访问操作系统底层资源的能力。
- 文件系统（`fs`）：读写文件、创建目录、删除文件
- 网络（`http, net`）：发起和监听网络请求，构建服务器
- 进程（`process, child_process`）：与操作系统交互、执行命令

浏览器内的 JavaScript 有强大的 Web API：
- DOM 操作：增删改查 HTML 元素，改变样式
- 事件处理：响应用户操作（点击、键盘输入、拖拽等）
- 网络请求（fetch，XMLHttpRequest）：与服务器交互，加载或提交数据
- 浏览器功能：操作本地存储（localStorage，sessionStorage）、使用地理定位 API、访问摄像头/麦克风（需权限）
浏览器侧的 JavaScript 主要是“用户交互 + 页面控制 + 与后端通信”
当然，浏览器内的 JavaScript 功能也有限制，主要是为了防止邪恶网页访问私人信息或损害用户数据。如：
1. 网页上的 JavaScript 不得读取/写入硬盘上的任意文件、复制或执行程序。无法直接访问操作系统功能。
2. 现代浏览器允许它处理文件，但访问权限是有限的，仅在用户执行某些操作时才提供，如将该文件“拖放”到浏览器窗口或通过`<input>` 标签选择它。
3. 有多种方法可以与摄像头/麦克风和其他设备进行交互，但它们需要用户的明确许可。因此，启用 JavaScript 的页面可能不会偷偷地启用网络摄像头、观察周围环境并将信息发送给 NSA（美国国家安全局）。
4. 不同的选项卡（tabs）/窗口（windows）通常彼此不了解，即每个 tab/window 都是一个独立的 JavaScript 执行环境，默认情况下，A 网页上的脚本，无法直接访问 B 网页上的脚本或数据。不过，在某些时候，A 和 B 可以在一定程度上交互，比如 B 窗口是页面 A 使用 `window.open()` 打开的，这是 A 窗口会得到 B 窗口的引用（通过 `window` 对象），不过这种交互也不是完全开放的，需要遵守**同源策略**。
> 同源策略（Same-Origin Policy）：协议、域名、端口都相同才算是同源，不同源的窗口/页面，即使有 `window.open` 或 `iframe` 关系，也不能随便互相访问 DOM 或数据。

What makes JavaScript unique?
- 与 HTML/CSS 完全集成
- 简单的事情简单来做
- 大多数浏览器都支持并默认启用

许多语言被“转译”为 JavaScript 并提供某些功能。如 `CoffeeScript, TypeScript, Flow, Dart, Brython, Kotlin`.

---
Developer console（开发者控制台）
在出错的页面，按下 `F12`（Mac: `Cmd+Opt+J`）打开控制台。控制台有类似终端的界面，以蓝色 `>` 符号开始，标记了一个 "command line" ，我们可以在其中键入 JavaScript 命令，按下 `Enter` 以运行。如果要插入多行，请按 `Shift+Enter`。

---
JavaScript 程序可以使用 `<script>` 标记插入到 HTML 文档的任何地方。
早期 `<script>` 标签有一些现在很少使用的属性，如 `type` 和 `language`. 但可能仍出现在旧代码中。
```html
<!DOCTYPE HTML>
<html>
<body>
    <p>Before the script...</p>
    
    <script>
        alert( 'Hello, world!' );
    </script>
    
    <p>...After the script.</p>
    
</body>
</html>
```
外部脚本：如果有很多 JavaScript 代码，可以将其放入一个单独的文件里。
```html
<script src="/path/to/script.js"></script>
<script src="/js/script1.js"></script>
```
这里，`/path/to/script.js` 是从站点根到脚本的绝对路径（当然也可提供相对路径），附加多个脚本需使用多个标记。 
或者通过完整的 URL：
```html
<script src="https://cdnjs.cloudflare.com/ajax/libs/lodash.js/4.17.11/lodash.js"></script>
```
注意：
> 如果设置 `src` ，则忽略脚本标签中的内容。即，单个 `<script>` 标签不能同时包含 `src` 属性和代码。
```html
<script src="file.js">
    alert(1); // Note: 该内容将被忽略
</script>
```

---
现代化模式开关：`"use strict";`
如何启用？
- 写在整个脚本开头 —> 整个文件
```js
"use strict";
// 以下所有代码都遵循严格模式
```
- 写在函数开头 —> 只对该函数生效
```js
function test(){
    "use strict";
    // 严格模式生效
}
```
现代工具（如 `Babel`，`ESLint`，模块系统）默认都会启用严格模式，所以写 `ES6+` 模块时，严格模式自动生效，不用手动写 `"use strict";`