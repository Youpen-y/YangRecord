`git format-patch` 用于生成补丁文件（patch files）的命令，它将提交（commits）转换成标准的邮件格式（mbox 格式），可以通过邮件发送给他人，便于代码审阅、应用（`git am`）等场景。

基本语法：
```bash
git format-patch [OPTION] <commit-range>
```
将指定范围内的提交生成一组 `.patch` 文件，每个提交生成一个对应的补丁文件。

指定所操作的提交的两种方式
- `git format-patch <since>` 等价于 `git format-patch <since>..HEAD`
其中，`<since>` 可为某次提交的哈希值。
假设有这样的提交历史：`A -- B -- C -- D(main, HEAD)`，如果执行 `git format-patch B`，那么会先后生成 `C` 和 `D` 提交的补丁。
```bash
0001-Patch-for-C.patch
0002-Patch-for-D.patch
# 默认文件名包含序号和提交信息，添加 --numbered-files 选项可以只用序号
# 指定 --stdout 选项，可以直接将补丁内容打印到标准输出，而不是写入文件
```

- 通用 `<revision range>` 表达式：`git format-patch <revision range>`
示例：
1. `git format-patch B..D`：生成 `C` 和 `D` 的补丁；
2. `git format-patch A^..D`：生成从 `A` 到 `D` 的所有补丁（即 `A`，`B`，`C`，`D`）

`git format-patch` 产生的补丁默认在当前目录，但可以通过 `-o <dir>` 指定。
默认情况下，单个补丁的主题（subject）以 `[PATCH]`开头；输出多个补丁时，主题以`[PATCH n/m]`为开头。
>[!NOTE]
>`--thread` 是一个较为常用的选项，通过添加邮件头（`Message-ID`，`In-Reply-To`，`Reference`），用于在实现同一功能的多个补丁之间保持关联。
>`git format-patch --thread` 会在生成的补丁邮件中，特别是从第二个补丁开始的邮件中，自动添加两个标准的邮件头：
>- `In-Reply-To`（回复到）：该邮件头的值通常是其回复的那封邮件的 `Message-ID`（第一个补丁的） 。它告诉邮件客户端，当前这封邮件是对指定 `Message-ID` 邮件的回复。
>- `Reference`（引用）：该邮件头包含了与当前对话相关的所有邮件的 `Message-ID` 列表，通常是从对话开始的第一封邮件到当前邮件的所有 `Message-ID` 。它帮助邮件客户端构建完整的邮件线程上下文。
>Why This Matters?
>在邮件列表驱动的开发工作流中（如 Linux 内核开发），开发者通常会一次性发送一系列相关的补丁（如，一个大功能的实现可分解为多个小的、原子性的提交，每个提交生成一个补丁）。如果没有 `--thread` 选项，这些补丁会被发送为一系列独立的邮件。这将让邮件列表变得混乱，接收者难以理解补丁之间的逻辑关系。
>示例：假设生成了 3 个补丁：`0001-feat-part1.patch`，`0002-feat-part2.patch`，`0003-feat-part3.patch`。
>```
># ---------- 0001 patch ----------- #
>From 4dfa4a114bd4e491245b35c02d2f70688dd262df Mon Sep 17 00:00:00 2001
>Message-ID: <4dfa4a114bd4e491245b35c02d2f70688dd262df.1751635880.git.yongy2022@outlook.com
>From: Youpen-y <yongy2022@outlook.com>
>Date: Wed, 14 May 2025 13:42:40 +0800
>Subject: [PATCH 1/3] Add .gitconfig and .gitmessage.txt
># ---------- 0002 patch ----------- #
>From 57c16a840d081c0fd65f186902d7cd458490e8e8 Mon Sep 17 00:00:00 2001
>Message-ID: <57c16a840d081c0fd65f186902d7cd458490e8e8.1751635880.git.yongy2022@outlook.com
>In-Reply-To: <4dfa4a114bd4e491245b35c02d2f70688dd262df.1751635880.git.yongy2022@outlook.com>
>References: <4dfa4a114bd4e491245b35c02d2f70688dd262df.1751635880.git.yongy2022@outlook.com
>From: Youpen-y <yongy2022@outlook.com>
>Date: Wed, 14 May 2025 13:48:39 +0800
>Subject: [PATCH 2/3] Minor change
># ---------- 0003 patch ----------- #
>From 6cdb00ef11488e433841a6c783c8d54c0fd692f6 Mon Sep 17 00:00:00 2001
>Message-ID: <6cdb00ef11488e433841a6c783c8d54c0fd692f6.1751635880.git.yongy2022@outlook.com>
>In-Reply-To: <4dfa4a114bd4e491245b35c02d2f70688dd262df.1751635880.git.yongy2022@outlook.com>
>References: <4dfa4a114bd4e491245b35c02d2f70688dd262df.1751635880.git.yongy2022@outlook.com>
>From: Youpen-y <yongy2022@outlook.com>
>Date: Wed, 14 May 2025 14:48:02 +0800
Subject: [PATCH 3/3] Tidy up comments
>```
>请注意，`git send-email` 的默认设置是对邮件本身进行线程处理。如果你想让 `git format-patch` 负责线程，需要确保 `git send-email` 的线程被禁用（或通过设置 `--no-thread` 选项或确保 `~/.gitconfig` 中没有 `sendemail.thread = true` 的设置）。

场景用途：
- 提交代码给社区（如 Linux Kernel、coreutils）
- 离线发送补丁进行代码审查
- 跨仓库传输提交
- 与 `git am` 搭配使用，实现补丁应用

常见用法：
0. 生成某个提交本身的补丁
```bash
git format-patch -1 COMMIT_HASH_VALUE
```

0. 生成最近一次提交的补丁
```bash
git format-patch -1
```

2. 生成最后三次提交的补丁
```bash
git format-patch -3
```

3. 从某个提交开始生成补丁（不包含该提交）
```bash
git format-patch abc123
```

4. 指定提交区间（生成的是 `(start, end]`，不包含 `abc123`）
```bash
git format-patch abc123..HEAD
```
或包含起始提交
```bash
git format-patch abc123^..HEAD
```

5. 生成从起始到某个提交的所有补丁
```bash
git format-patch --root COMMIT_HASH_VALUE
```

6. 添加邮件信息（用于发送）
```bash
git format-patch --cover-letter -o patches/ origin/main
```


生成的补丁文件格式示例：
```bash
0001-commit_message_description.patch
```
内容包含：
- `From:` 作者信息
- `Subject:` 提交标题
- `Date:` 提交时间
- `diff --git` 内容
- `Signed-off-by:` （如果使用了 `--signoff, -s` 选项）

常用选项详解：

| 选项                         | 说明                                      |
| -------------------------- | --------------------------------------- |
| `-<n>`                     | 从最上面的 `<n>` 个提交中准备补丁，`<n>` 为正整数         |
| `-o <dir>`                 | 指定输出目录，而不是当前目录                          |
| `--stdout`                 | 将所有补丁输出到标准输出                            |
| `--cover-letter`           | 创建一个用于邮件封面的 `0000-cover-letter.patch`   |
| `--subject-prefix="PATCH"` | 修改邮件主题前缀（默认是 "PATCH"）                   |
| `-s/--signature=`          | 在提交信息中加入 `Signed-off-by` 的尾注，使用自己的提交者身份 |
| `--thread`                 | 设置邮件的引用关系，形成邮件讨论串结构。                    |
| `--no-stat`                | 不输出 diffstat                            |
| `--attach`                 | 用于将补丁作为附件（用于 `git send-email`）          |
| `-n/--numbered`            | 以 `[PATCH n/m]` 的格式输出名称，即使只有一个补丁        |
| `-N/--no-numbered`         | 以 `[PATCH]` 格式输出名称                      |
| `--start-number <n>`       | 指定从 `<n>` 开始给补丁编号，默认从 1 开始              |

配合 `git am` 使用
```bash
git am 0001-commit_message_description.patch
```
应用 `patch` 文件中的更改和提交信息。



