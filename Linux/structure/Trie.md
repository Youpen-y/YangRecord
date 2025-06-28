Trie （发音为 try）也称为 digital tree 或 prefix tree，是一种专门的搜索树数据结构，用于存储和检索字典或集合中的字符串。

示例：表示集合`{cat, cow, pig, pin}` 的 Trie
![[Trie-example.png]]
特征：
- 边使用字符标记（不变量：节点边的上标记不会相同）
- 每个节点代表一个 string 。
- 双圈节点：从根到当前节点上的字符拼接是集合中的元素（所有页节点都是双圈节点）

与二叉搜索树不同，Trie 中的节点不存储其关联的键。相反，每个节点在 Tire 中的位置决定了其关联的键，节点之间的连接由单个字符而不是而不是整个键定义。

---
Trie 对于 autocomplete（自动完成）、spell checking（拼写检查）和 IP routing（IP 路由）等任务特别有效，由于其基于前缀（prefix-based）的组织且没有哈希冲突，因此具有优于哈希表的优势。

每个子节点都与其父节点共享一个公共前缀，根节点表示空字符串。
尽管基本的 Trie 实现可能是内存密集型的，但已经开发了各种优化技术，例如 Compression 和 Bitwise representations 以提高其效率。一个著名的优化是基数树（radix tree） ，它提供更高效的基于前缀的存储。

虽然 Tries 通常存储字符串，但它们可以适应处理任何有序的元素序列，例如数字排列或图形排列（Permutations of digits or shapes）。
元素可以是：
- 数字（如 `[3, 1, 4, 1]`）
- 图形（如 [🟥, 🟨, 🟩]）
- 甚至二进制位（如 `0101...`）

一个值得注意的变体是 Bitwise trie ，使用固定长度二进制数据（如整数或内存地址）中的

与 BSTs（二叉搜索树）相比：
- 如果 Trie 包含许多短字符串，会比 BSTs 好，因为每个节点有许多子节点
- 如果 Tire 包含少量长字符串，将比 BSTs 差，因为许多节点仅有一个孩子

*Radix trees* 是对 Tries 的优化 —— 仅在需要分支时引入节点。

---
操作
Tire 操作时间复杂度为 O(w) ，其中 w 是插入字符串的长度。而与存在 trie 中的字符串的数目无关。

1. 查找
```
// 伪代码：在 Trie x 中查找字符串 key
Trie-Find(x, key)
    for 0 ≤ i < key.length do
        if x.Children[key[i]] = nil then
            return false
        end if
        x := x.Children[key[i]]
    repeat
    return x.Value
```
示例：`search("cat")`
![[Trie-search.png]]
2. 插入
```
Trie-Insert(x, key, value)
    for 0 ≤ i < key.length do
        if x.Children[key[i]] = nil then
            x.Children[key[i]] := Node()
        end if
        x := x.Children[key[i]]
    repeat
    x.Value := value
    x.Is-Terminal := True
```
示例：`insert("crow")`
![[Trie-insert.png]]

3. 删除
```
Trie-Delete(x, key)
    if x = nil then
        return nil
    end if

    if key = nil then
        // 将节点变为非双圈节点
        if x.Is-Terminal = True then
            x.Is-Terminal := False
            x.Value := nil
        end if

        // 检查是否需要清理该节点
        for 0 ≤ i < x.Children.length do
            if x.Children[i] != nil then
                return x
            end if
        repeat

        // 没有子节点且不是终止节点，删除
        return nil
    end if

    // 递归删除子节点
    x.Children[key[0]] := Trie-Delete(x.Children[key[0]], key[1:])
    return x
```
为了删除一个字符串，首先将节点变为单圈节点。示例：`delete("crow")`
![[Trie-delete.png|400]]
如果该节点是叶子节点，我们应该移除它。我们沿着树向上移动，移除所有单圈叶子节点，这样就恢复了不变式
![[Trie-delete2.png|400]]

4. 查找指定前缀的所有字符串

5. 求两个 Tries 的并（Union） 和交（Intersection）；线性时间，如果两个 Tries 不相交会更快

6. 以字母序遍历节点的所有边：以字典序产生一个字符串列表（用于排序）；找到两个单词之间的所有字符串。


示例实现：
```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define ALPHABET_SIZE 26 // 假设只支持小写字母 'a'-'z'

typedef struct TrieNode {
	bool is_terminal;  // 是否是单词结尾
	struct TrieNode* children[ALPHABET_SIZE];
} TrieNode;

// 创建节点
TrieNode* create_node() {
	TrieNode* node = (TrieNode*)malloc(sizeof(TrieNode));
	node->is_terminal = false;
	for (int i = 0; i < ALPHABET_SIZE; i++) {
		node->children[i] = NULL;
	}
	return node;
}

// 插入字符串
void insert(TrieNode* root, const char* word) {
	TrieNode* curr = root;
	for (int i = 0; word[i]; i++) {
		int idx = word[i] - 'a';
		if (!curr->children[idx])
		  curr->children[idx] = create_node();
		curr = curr->children[idx];
	}
	curr->is_terminal = true;
}

// 查找字符串
bool search(TrieNode* root, const char* word) {
	TrieNode* curr = root;
	for (int i = 0; word[i]; i++) {
		int idx = word[i] - 'a';
		if (!curr->children[idx])
		  return false;
		curr = curr->children[idx];
	}
	return curr->is_terminal;
}

// 删除字符串（递归）
bool delete_helper(TrieNode* node, const char* word, int depth) {
	if (!node) return false;

    if (word[depth] == '\0') {
	    if (!node->is_terminal)
	       return false;   // 不存在该词

        node->is_terminal = false;

        // 如果没有任何子节点了，说明可以删除该节点
        for (int i = 0; i < ALPHABET_SIZE; i++)
            if (node->children[i])
                return false; // 不能删除，仍有子树

        return true; // 可以删除
	}

    int idx = word[depth] - 'a';
    if (delete_helper(node->children[idx], word, depth + 1)) {
	    // 子节点可以删除
	    free(node->children[idx]);
	    node->children[idx] = NULL;

        // 当前节点是否也可以删除？
        if (!node->is_terminal) {
	        for (int i = 0; i < ALPHABET_SIZE; i++)
	           if (node->children[i])
	               return false;
	        return true;
        }
    }

    return false;
}

void delete_word(TrieNode* root, const char* word) {
	delete_helper(root, word, 0);
}
```






