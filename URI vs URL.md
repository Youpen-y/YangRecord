#### 区别
|概念|解释|
|---|---|
|**URI**（Uniform Resource Identifier）统一资源标识符|一个**用于标识某个资源**的字符串，可以是 URL，也可以是 URN（名字）。更广泛、更抽象。|
|**URL**（Uniform Resource Locator）统一资源定位符|URI 的一个子集，用于**定位资源的位置**（通常是网络地址）。它告诉你“在哪”和“怎么访问”。|

#### 示例：
URL（同时是 URI）：
```
https://example.com/index.html
ftp://ftp.example.com/file.txt
```
- 使用了什么协议（`https`，`ftp`）、在哪里找（`example.com`）、以及要访问什么（`index.html`）

URI（不一定是 URL）：
```css
urn:isbn:978-3-16-148410-0
```
- 这是一个**URN（统一资源名称）**，标识的是一本书，但**不能通过它直接访问资源**。
- 这是 URI 的另一种形式，不具备“定位”能力。

#### 关系
所有的 URL 是 URI；但并非所有的 URI 是URL。

