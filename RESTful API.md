[REST 论文](https://ics.uci.edu/~fielding/pubs/dissertation/rest_arch_style.htm) —— [Roy Thomas Fielding](http://www.ics.uci.edu/~fielding/)
符合 REST （REpresentational State Transfer）架构风格设计原则的应用程序编程接口。有时称为 REST API, RESTful Web API，是Web 应用程序与服务器通信的通用 API 标准。

RESTful API将资源组织成一组唯一的 URIs 。依赖 HTTP 请求与资源交互的架构风格。

六项 REST 设计原则（架构约束）
- 统一接口。无论请求来自何处，对同一资源的所有 API 请求都应该看起来相同。REST API 应确保同一条数据（例如用户的姓名或电子邮件地址）只属于一个统一资源标识符 (URI)。资源不应过大，但应包含客户端可能需要的所有信息。
- 客户端-服务器解耦。在 REST API 设计中，客户端和服务器应用程序必须完全独立。客户端应用程序唯一需要知道的信息是所请求资源的 URI；它不能以任何其他方式与服务器应用程序交互。同样，服务器应用程序除了通过 HTTP 向客户端应用程序传递请求的数据之外，不应修改客户端应用程序。
- 无状态。REST API 是无状态的，这意味着每个请求都需要包含处理该请求所需的所有信息。换句话说，REST API 不需要任何服务器端会话。服务器应用程序不允许存储与客户端请求相关的任何数据。
- 可缓存。如果可能，资源应该在客户端或服务器端可缓存。服务器的响应需要包含已交付的资源是否允许被缓存。目标是提高客户端的性能，同时提高服务器端的可扩展性。
- 分层系统架构。在 REST API 中，调用和响应会经过不同的层。根据经验，不要假设客户端和服务器应用程序直接相互连接。通信环路中可能存在多个不同的中介。REST API 的设计需要确保客户端和服务器都无法分辨自己是在与最终应用程序还是中介进行通信。
- 按需编码（可选）。REST API 通常发送静态资源，但在某些情况下，响应也可能包含可执行代码（例如 Java 小程序）。在这些情况下，代码应该仅按需运行。


```
HTTP Methods        CRUD
POST        --->    CREATE
GET         --->    READ
PUT/PATCH   --->    UPDATE
DELETE      --->    DELETE
=> CRUD model
```
示例：



其他API
`SOAP`，`GraphQL`，`gRPC`


API 特征
- API 类型
	- REST
	- SOAP
	- GraphQL
	- gRPC
- 处理方法
	- Real-Time
	- Batch Processing
- 数据格式
	- JSON
	- XML
	- Plain Text
- 授权方法
	- API Keys
	- OAuth Tokens
	- JWT

