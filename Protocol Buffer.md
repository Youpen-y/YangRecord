Protocol Buffers 是一种语言无关、平台无关的可扩展机制，用于**序列化结构化数据**。
> 将内存中的对象转换成紧凑的二进制数据，从二进制数据还原对象。

#### 旨在解决的问题？
Protocol Buffers 为类型化（`typed`）的结构化（`structured data`）数据包提供了一种序列化格式，高效处理“小到中等”体积的数据包（常见 1-10MB）。
适用于临时网络传输。如 API 请求、RPC 调用、即时通信消息；或者永久保存的数据，如数据库存档、日志文件、备份。
- `typed`：指每个字段都有明确类型（`string`，`int32`，`bool`等），在 `.proto` 中声明，并在编译时检查。
- `structured data`：数据有层次结构（嵌套对象、数组、枚举等），不是平铺的键值对。
- 另一个特点：允许在不使现有数据失效或不要求更新代码的情况下扩展新的信息。
示例：
```proto
message Person {
	string name = 1;
	int32 age = 2;
	string email = 3; // 新增
}
```
在新增了 `email` 字段后
- 旧版本的程序在读取新数据时，会忽略它不认识的字段（如 `email`）；
- 新版本的程序在读取旧数据时，`email` 字段会是默认值（如空字符串）

类似 JSON ，但更小、更快，而且能生成本地语言绑定（native language bindings）。
> 这里本地语言绑定的含义是指 `protobuf` 会为你用的编程语言自动生成可以直接调用的类、方法、结构体等代码，而不用像 JSON 一样手写解析或序列化逻辑。
> 这里引入 JSON 的另一个不足：需要手写代码去解析 JSON 字符串（或者调用各种库），对象结构和字段名只是字符串约定，没有编译时类型检查。这就需要开发者自己保证字段名和类型一致，否则直到运行时才报错。

#### Protobuf 的做法
![[Protocol Buffer 流程.png]]
1. 由 `.proto` 文件定义数据结构，如：
```proto
syntax = "proto3";

message Person {
	string name = 1;
	int32 age = 2;
}
```
2. 运行 `protoc` 编译器，指定目标语言，生成操作相应的 Protocol Buffer 的类（类包含每个字段的简单访问器，以及将整个结构序列化和解析为原始字节的相应方法）
```bash
protoc --cpp_out=. person.proto
$ ls
person.pb.cc  person.pb.h  person.proto
# --------------------------------
protoc --python_out=. person.proto
$ ls
person_pb2.py  person.proto
# --------------------------------
protoc --java_out=. person.proto
$ ls
PersonOuterClass.java  person.proto
# --------------------------------
protoc --go_out=. person.proto

```
这些自动生成的代码就是 `protobuf` 做的“绑定”（bindings）—— 将 `.proto` 中的定义直接“绑定”到目标编程语言的数据类型和方法。

> Protocol buffer = 定义语言（`.proto`文件中创建）+ `proto` 编译器生成的与数据交互的代码 + 特定于语言的运行时库 + 写入文件的数据的序列化格式 + 写入文件（或通过网络连接）的数据的序列化格式 + 序列化的数据

支持多种语言：直接支持 C++, C#, Java, Python, Ruby等；单独仓库：Dart 和 Go
#### 不适合情景
1. 协议缓冲区倾向于假定“整个消息可以一次性加载到内存中，通常不会超过几个 MB，否则请考虑其他解决方案”；在处理大型数据时，处理过程中可能会出现同一份数据在内存中存在多份副本（原始数据文件、网络缓冲区 TCP buffer/gRPC buffer、Protobuf 反序列化时构造的对象等），这些副本的存在是短暂的，但会让内存瞬间激增。
> 简而言之：Protobuf 并不是为处理超大单条消息而优化的；大于几 MB 的数据要考虑流式/分片；否则，序列化/反序列化过程可能在内存中产生多份数据副本，导致峰值内存使用量远超数据本身大小。

2. 当 Protocol buffers 被序列化时，相同的数据可以有多种不同的二进制序列化形式。在不完全解析它们的情况下，不能比较两个消息是否相等。

3. 消息并没有被压缩。虽然消息（Protobuf 输出的二进制文件）可以像任何其他文件一样被 zip 或 gzip 压缩，但像 JPEG 和 PNG 使用的专用压缩算法对于适当类型的数据会产生更小的文件。
> `Protobuf` 的二进制格式虽然紧凑（比 JSON 小很多），但并非压缩算法，只是高效的序列化格式。
> 需要更小的文件，可以额外用 ZIP， GZIP 压缩。

4. Protocol buffer 消息在涉及大型、多维浮点数组的许多科学和工程应用中，在大小和速度上都不是最有效的。对于这些应用，`FITS` 和类似格式具有更低的额外开销。

5. Protocol buffer 对于科学计算中的非面向对象语言（如 `Fortran` 和 `IDL`）支持不佳。

6. Protocol buffer 的序列化结构里没有字段名、没有类型信息、没有完整结构说明，里面存的只是字段编号（tag）、字段值、以及根据类型编码后的原始字节。但它有完整的 `schema`（结构描述），而且可以通过反射（`reflection`）机制读取和使用。
> “反射 schema”：`.proto` 文件在编译时可以生成一个 `.desc`（descriptor）文件。程序可以在运行时加载该描述文件，之后通过 API 访问各个字段。

7. Protocol buffer 不是任何组织的正式标准。不适合用于需要基于标准建立的合法或其他要求的环境。

---
多个版本：`2023`
##### 标量值类型
- `double`
- `float`
- `int32`, `int64`, `uint32`, `uint64`, `sint32`, `sint64`
- `fixed32`, `fixed64`, `sfixed32`, `sfixed64`
- `bool`
- `string`
- `bytes`
- 枚举类型
```proto
enum Corpus {
CORPUS_UNSPECIFIED = 0;
CORPUS_UNIVERSAL = 1;
CORPUS_WEB = 2;
CORPUS_IMAGES = 3;
CORPUS_LOCAL = 4;
CORPUS_NEWS = 5;
CORPUS_PRODUCTS = 6;
CORPUS_VIDEO = 7;
}

message SearchRequest {
  string query = 1;
  int32 page_number = 2;
  int32 results_per_page = 3;
  Corpus corpus = 4;
}
```
`SearchRequest.corpus` 字段的默认值是 `CORPUS_UNSPECIFIED`，枚举的第一个值。
...

#### 示例
1. 定义 `.proto` 文件
```proto
syntax = "proto3";

package example;

message Person {
	string name = 1;
	int32 age = 2;
	repeated string emails = 3;
}
```

2. `protoc` 编译器生成 C++ 代码
```bash
protoc --cpp_out=. person.proto
```

3. 编写 C++ 程序
```cpp
#include <iostream>
#include <fstream>
#include "person.pb.h"

int main() {
    // 初始化 protobuf 库（可选，但推荐）
    GOOGLE_PROTOBUF_VERIFY_VERSION;

    // 创建一个 Person 对象
    example::Person person;
    person.set_name("Alice");
    person.set_age(30);
    person.add_emails("alice@example.com");
    person.add_emails("alice@work.com");

    // 序列化到二进制文件
    std::ofstream output("person.bin", std::ios::out | std::ios::binary);
    if (!person.SerializeToOstream(&output)) {
        std::cerr << "Failed to write person." << std::endl;
        return -1;
    }
    output.close();

    // 从二进制文件反序列化
    example::Person person2;
    std::ifstream input("person.bin", std::ios::in | std::ios::binary);
    if (!person2.ParseFromIstream(&input)) {
        std::cerr << "Failed to parse person." << std::endl;
        return -1;
    }
    input.close();

    // 输出内容
    std::cout << "Name: " << person2.name() << std::endl;
    std::cout << "Age: " << person2.age() << std::endl;
    std::cout << "Emails: ";
    for (const auto& email : person2.emails()) {
        std::cout << email << " ";
    }
    std::cout << std::endl;

    // 清理 protobuf 库
    google::protobuf::ShutdownProtobufLibrary();

    return 0;
}
```

4. 编译与运行
```bash
g++ main.cpp person.pb.cc -o main `pkg-config --cflags --libs protobuf`
./main
```