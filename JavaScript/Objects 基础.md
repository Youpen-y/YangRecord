基础介绍
===
对象是一种 **键值对集合** （key-value pairs），用于存储和组织数据。
- 键（key）：字符串或 `Symbol`
- 值（value）：可以是任意类型（基本类型或对象）
示例：
```js
let user = {
	name: "Alice",
	age: 25,
	isAdmin: true
};
```

如何创建对象？
===
1. 对象字面量（object literal）
```js
let obj = { key: "value" };
```

2. 构造函数
```js
let obj = new Object();
obj.key = "value";
```

3. 工厂函数/自定义构造函数
```js
function User(name) {
	this.name = name;
}
let u = new User("Alice");
```

4. 类（ES6）
```js
class User {
	constructor(name) {
		this.name = name;
	}
}
let u = new User("Alice");
```

访问对象属性
===
- 点语法
```js
console.log(user.name);
```
- 方括号语法（支持动态 `key`）
```js
let key = "age";
console.log(user[key]);
console.log(user["key with space"]);
```
创建对象时，可以在对象字面量中使用方括号 —— 计算属性。
```js
let fruit = prompt("Which fruit to buy?", "apple");

let bag = {
	[fruit]: 5, // 该属性名称从变量 `fruit` 中得到
};
```
还可以在方括号内使用更复杂的表达式：
```js
let fruit = 'apple';
let bag = {
	[fruit + 'Computers']: 5 //bag.appleComputers = 5
};
```

常见操作
===
- 新增/修改属性
```js
user.city = "London";
```
- 删除属性
```js
delete user.isAdmin
```
- 判断属性是否存在
```js
console.log("name" in user);
```
- 遍历对象
```js
let user = {
	name: "John",
	age: 30,
	isAdmin: true
};

for (let key in user) {
	// keys
	alert( key ); // name, age, isAdmin
	// values for the keys
	alert( user[key] ); // John, 30, true
}
```

对象的特殊排序方式：整数属性将被排序，而其他属性按创建顺序出现。
```
let codes = {
    "49": "Germany",
    "41": "France",
    "2": "USA",
    "1": "China"
};

for (let code in codes) {
	alert(code); // 1, 2, 41, 49
}
```
>[!NOTE]
>什么是整数属性？
>“整数属性”指可以在不更改的情况下与整数之间转换的字符串。
>示例："49" 是一个整数属性名，但 "+49"，"1.2" 不是。

对象引用和复制
===
基类型的总是作为一个整体被复制。
```js
let message = "Hello!";
let phrase = message;
```
将有两个自变量，每个变量都存储字符串 `"Hello!"`
![[Regular var copy.png]]
不过，对于对象，分配给对象的变量存储的不是对象本身，而是存储其“内存中地址”—— 换句话说，存储对它的“reference”。
示例：
```js
let user = { name: "John" };
let admin = user; // copy the reference
alert ( a == b ); // true, 两个变量引用同一个对象
alert ( a === b ); // true
```
![object copy](https://javascript.info/article/object-copy/variable-copy-reference.svg)
只有两个对象是相同的对象时，它们才相等。
```js
let a = {};
let b = {};

alert( a == b ); // false
```
>![NOTE]
>Const objects can be modified. 
>将对象存储为引用的一个重要副作用是可以修改声明为 `const` 的对象。
>```js
>const user = {
>       name: "John"
    };
> user.name = "Pete";
> alert(user.name); // Pete
>```

克隆：创建一个独立的新对象
1. 创建一个空对象，然后遍历原对象属性并复制
2. 使用 `Object.assign` 方法（浅拷贝：只复制对象的第一层属性，如果属性值是对象，则依然是引用）
```js
Object.assign(dest, ...sources)
```
- 第一个参数 `dest` 是目标对象
- 其他参数是源对象的列表。
该函数将所有源对象的属性复制到目标 `dest` 中，然后将其作为结果返回。
```js
let user = { name: "John" };
let permission1 = { canView: true };
let permission2 = { canEdit: true };

// 将 permission1 和 permission2 的所有属性拷贝到 user
Object.assign(user, permission1, permission2);

// now user = { name: "John", canView: true, canEdit: true }
```
3. 展开运算符 `...`
```js
let user = { name: "Alice", info: { age: 25 } };
let clone = { ...user };

console.log(clone); // { name: "Alice", info: { age: 25 } }
```
4. 深拷贝（复制整个对象及其嵌套对象）
	1. `structuredClone(object)`
```js
let user = {
	name: "John",
	sizes: {
		height: 182,
		width: 50
	}
};
let clone = structuredClone(user);

alert( user.sizes === clone.sizes ); // false, different objects
```