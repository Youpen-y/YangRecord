#### Latex 区域（Environments）
##### 导言区（Preamble）
作用：设置文档的基本信息，包括文档类型、宏包加载、自定义命令等。
位置：
- 从 `\documentclass{}` 开始
- 到 `\begin{document}` 之前
作用范围：整个文档
注意：不能包含正文内容（如  `\section{}` 之类的命令）
示例
```latex
\documentclass{article}   % 指定文档类型
\usepackage{graphicx}     % 加载宏包
\usepackage{enumitem}     % 自定义列表格式
\title{LaTex 诸区介绍}     % 定义标题
\author{YYP}              % 定义作者
\date{\today}             % 定义日期
```

##### 正文区（Document Body）
作用：文档的主要内容，如


#### 如何引用代码？

- 正文中提及接口：1. `\texttt{jia\_init}`; 2. `\verb|代码|` 或 `\verb+代码+`
- 简单嵌入多行
```latex
\begin{verbatim}
#include <stdio.h>
int main() {

}
\end{verbatim}
```

- 嵌入代码：使用 `lstlisting` 宏包
```latex
\usepackage{listings}   % 代码高亮
\usepackage{xcolor}     % 颜色支持

% 定义 C 语言代码样式 
\lstdefinestyle{CStyle}{ 
	language=C, 
	basicstyle=\ttfamily\footnotesize, 
	keywordstyle=\bfseries\color{blue}, 
	identifierstyle=\color{black}, 
	commentstyle=\color{gray}, 
	stringstyle=\color{red}, 
	numbers=none, % 不带前面的数字
	frame=single, 
	breaklines=true, 
} 
% 定义 Python 语言代码样式 
\lstdefinestyle{PythonStyle}{ 
	language=Python, 
	basicstyle=\ttfamily\small, 
	keywordstyle=\bfseries\color{purple}, 
	identifierstyle=\color{black}, 
	commentstyle=\color{teal}, 
	stringstyle=\color{orange}, 
	numbers=left, 
	numberstyle=\tiny, 
	frame=shadowbox, 
	backgroundcolor=\color{gray!10}, 
	breaklines=true, 
}

% 以上为配置操作

% 引用
\begin{lstlisting}[style=CStyle]
#include <stdio.h>
int main() {

	return 0;
}
\end{lstlisting}

\begin{lstlisting}[style=PythonStyle]
def func():
	print("testing");
\end{lstlisting}
```

#### 使用表格

```latex
\begin{table}
    \footnotesize% fontsize
    \setlength{\tabcolsep}{4pt}% column separation
    \renewcommand{\arraystretch}{1.5}% row space 
    \centering
    \begin{tabular}{lcc}
        \hline
        %\multicolumn{num_of_cols_to_merge}{alignment}{contents} \\
        %\cline{i-j}% partial hline from column i to column j
        操作系统 & LaTeX编译系统 & LaTeX文本编辑器\\
        \hline
        Windows & \href{https://www.tug.org/texlive/acquire-netinstall.html}{TexLive Full} 或 \href{https://miktex.org/download}{MiKTex} & \href{http://www.xm1math.net/texmaker/}{Texmaker}\\
        Linux & \href{https://www.tug.org/texlive/acquire-netinstall.html}{TexLive Full} & \href{http://www.xm1math.net/texmaker/}{Texmaker} 或 Vim\\
        MacOS & \href{https://www.tug.org/mactex/}{MacTex Full} & \href{http://www.xm1math.net/texmaker/}{Texmaker} 或 Texshop\\
        Overleaf & XeLaTeX+TexLive2021 & Overleaf \\
        \hline
    \end{tabular}
    \bicaption{\enspace 支持的LaTeX编译系统和编辑器}{\enspace Supported LaTeX compiler and editor}% caption
    \label{tab:compiler}
\end{table}

```
- `\begin{table}[!htbp]` 和 `\end{table}`
**`table`** 环境用于创建浮动表格。这意味着表格的位置并不是固定的，LaTeX 会根据文档的排版自动决定表格的位置。
**`[!htbp]`**：这是一个位置选项，表示表格尽量出现在以下位置：
	- **`h`**：表格放在当前位置（here），如果可能。
	- **`t`**：放在页面的顶部（top）。
	- **`b`**：放在页面的底部（bottom）。
	- **`p`**：将表格放在专门的浮动页（浮动表格页）中。
	- **`!`**：增加 LaTeX 对位置选择的优先级，使它更倾向于根据这些参数来决定位置。

- **`\bicaption{}`** 是一个用于给表格或图形添加双语标题的命令，适用于多语言文档，这里提供中英文标题。
	- **`\enspace`**：插入一个空格，通常用于在标题两侧添加空格，以使标题的显示更加美观。
- `\label{tab:sample}` 给表格或公式等创建一个标签。在引用文献、图表或公式时，使用 `\ref{}` 或 `\eqref{}` 等命令可以调用该标签，从而自动生成表格编号。
- **`\centering`** 是一个简单的命令，用于将表格（或图形）居中显示。
- **`\footnotesize`** 用于设置字体大小，这里将字体设置为 `footnotesize`，比正常文本稍小。这常用于需要节省空间的表格、脚注等部分。
- **`\setlength{\tabcolsep}{4}`** 用于设置表格列之间的间距（列分隔符的宽度）。默认情况下，列间距为 6pt，这里将其调整为 4pt，使列之间的空隙更小。
- **`\renewcommand{\arraystretch}{1.2}`** 用于调整表格行之间的间距。默认值是 1，这里的 1.2 表示行间距比默认值大 20%。也就是每行的高度比通常的高度要高 1.2 倍。
- `\begin{tabular}{lcccccccc}` 和 `\end{tabular}` 
	- **`tabular`** 环境用于创建实际的表格。
	- **`{lcccccccc}`**：定义了表格的列格式：
		- **`l`** 表示该列的内容左对齐。
		- **`c`** 表示该列的内容居中对齐。
		- 这里有 9 个列（1 个左对齐列和 8 个居中对齐列）。
- **`\hline`** 在表格中绘制水平线，通常用来分隔表格的不同部分。例如，在表头和表格数据之间使用 `\hline` 来画一条横线。
- **`\multicolumn{n}{alignment}{内容}`** 用于让单元格跨越多个列。这里，`8` 表示该单元格跨越 8 列，`c` 表示该内容居中对齐，后面的 `{跨多列的标题}` 是该单元格显示的内容。
- `&` 用于分隔表格列， `\\` 用于换行

#### 加粗文本
```latex
\textbf{文本}
```
$\textbf{文本}$
#### 引用标签
```latex
\ref{tab:compiler}   # 引用标签 tab:compiler 

\ref{sec:system}   # 引用标签 sec:system
```

#### 方程
##### 带编号的方程
```latex
\begin{equation}
E = mc^2
\end{equation}
```
$$
\begin{equation}
E = mc^2
\end{equation}
$$
##### 不编号方程
```latex
\begin{equation*}
a^2 + b^2 = c^2
\end{equation*}

或
\[

\]
```
$$
\begin{equation*}
a^2 + b^2 = c^2
\end{equation*}
$$

##### 对齐方程各部分

$$
\begin{equation}
    \begin{split}
        \text{x} & = \text{a + b} \\
		\text{y} & = \text{c + d} \\
    \end{split}
\end{equation}
$$

##### 引用方程
```latex
~\eqref{eq:ns}
```
- `~` ：不可断开的空格，确保方程引用和前后的文本在排版时不会换行
- `\eqref{eq:ns}`：引用标签为 `eq:ns` 的方程，并自动加上括号，如 $~\eqref{eq:ein}$

```latex
\ref{eq:einstein}
% 当然，~\ref{} 也确保不会断开空格
```

#### 分段函数
```latex
f(x) = 
\begin{cases}
	x^2 & \text{if } x \geq 0, \\
	-x  & \text{if } x < 0.
\end{cases}
```
$$
f(x) = 
\begin{cases}
	x^2 & \text{if } x \geq 0, \\
	-x  & \text{if } x < 0.
\end{cases}
$$

#### 为文本添加超链接
```latex
\href{https:github.com/Youpen-y/jiajia.git}{JIAJIA}
```
示例：$\href{https:github.com/Youpen-y/jiajia.git}{JIAJIA}$

#### 列表
##### 无序列表
```latex
\begin{itemize}
	\item A
	\item B
	\item C
\end{itemize}
```
效果：
- A
- B
- C
##### 有序列表
```latex
\begin{enumerate}
% 如果使用 \begin{enumerate}[label=\arabic*.] % 使编号变成 1. 2. 3.
% 如果想完全去掉点，可使用 \begin{enumerate}[label=\arabic*] % 编号编程 1 2 3
	\item A
	\item B
	\item C
\end{enumerate}
```
效果：
（1） A
（2） B
（3） C
##### 自定义列表
```latex
% 需要 enumitem 宏包来自定义列表样式
\usepackage{enumitem}

\begin{enumerate}[label=\Roman*.] % 使用大写罗马数字
	\item 第一项
	\item 第二项
	\item 第三项
\end{enumerate}

% 调整缩进
\begin{itemize}[leftmargin=2cm]
	\item 这是一项
	\item 又一项
\end{itemize}
```
效果：
I. 第一项
II. 第二项
III. 第三项

##### 嵌套列表
```latex
\begin{enumerate}
	\item 第一项
	\begin{itemize}
		\item 子项 1
		\item 子项 2
	\end{itemize}
	\item 第二项
\end{enumerate}
```

#### 插入图片
##### 单图插入
```latex
\begin{figure}[!htbp]
    \centering
    \includegraphics[width=0.40\textwidth]{Img/sample.png}
    \bicaption{\enspace 样图}{\enspace Sample Figure}
    \fignote{对图片的注释}
    \label{fig:c06h06}
\end{figure}
```
如果插图的空白区域过大，自动裁剪
```latex
\begin{figure}[!htbp]
    \centering
	% 裁剪选项参数顺序：左 下 右 上
    \includegraphics[trim = 60 mm 80mm 60mm 60mm, clip, width=0.40\textwidth]{Img/sample.png}
    \bicaption{\enspace 样图}{\enspace Sample Figure}
    \fignote{对图片的注释}
    \label{fig:c06h06}
\end{figure}
```
##### 多图插入
```latex
\begin{figure}[!htbp]
	\centering
	\begin{subfigure}[b]{0.35\textwidth}
		\includegraphics[width=\textwidth]{Img/sample1.png}
		\caption{}
		\label{fig:sample1}
	\end{subfigure}
	~ % 添加期望的间隔
	\begin{subfigure}[b]{0.35\textwidth}
		\includegraphics[width=\textwidth]{Img/sample2.png}
		\caption{}
		\label{fig:sample2}
	\end{subfigure}
	\\ % 换行
    \begin{subfigure}[b]{0.35\textwidth}
      \includegraphics[width=\textwidth]{Img/sample3.png}
      \caption{}
      \label{fig:sample3}
    \end{subfigure}%
    ~% add desired spacing
    \begin{subfigure}[b]{0.35\textwidth}
      \includegraphics[width=\textwidth]{Img/sample4.png}
      \caption{}
      \label{fig:sample4}
    \end{subfigure}

    \bicaption{\enspace 多子图测试}{\enspace A test for multi-subfig}
    \label{fig:oaspl}
\end{figure}
```
#### 数学公式
```latex
\begin{axiom}
   这是一个公理。 
\end{axiom}

\begin{theorem}
   这是一个定理。 
\end{theorem}

\begin{lemma}
   这是一个引理。 
\end{lemma}

\begin{corollary}
   这是一个推论。 
\end{corollary}

\begin{assertion}
   这是一个断言。 
\end{assertion}

\begin{proposition}
   这是一个命题。 
\end{proposition}

\begin{definition}
    这是一个定义。
\end{definition}

\begin{example}
    这是一个例子。
\end{example}

\begin{remark}
    这是一个注。
\end{remark}
```

#### 字体切换
```latex
\textrm{宋体} 
\bfseries{粗宋体} 或者 \textbf{粗宋体}
\textsf{黑体} 或 {\sffamily 黑体}
\textsf{\bfseries 粗黑体} 或 {\bfseries\sffamily 粗黑体}
\texttt{仿宋} 或 {\ttfamily 仿宋}
\texttt{\bfseries 粗仿宋} 或 {\bfseries\ttfamily 粗仿宋}
\textit{楷体} 或 {\itshape 楷体}
\textit{\bfseries 粗楷体} 或 {\bfseries\itshape 粗楷体}
```

$$
\displaylines{
\textrm{宋体} \\

\bfseries{粗宋体} 或者 \textbf{粗宋体} \\

\textsf{黑体} 或 {\sffamily 黑体} \\

\textsf{\bfseries 粗黑体} 或 {\bfseries\sffamily 粗黑体} \\

\texttt{仿宋} 或 {\ttfamily 仿宋} \\

\texttt{\bfseries 粗仿宋} 或 {\bfseries\ttfamily 粗仿宋} \\
\textit{楷体} 或 {\itshape 楷体} \\
\textit{\bfseries 粗楷体} 或 {\bfseries\itshape 粗楷体} \\
}

$$

#### 文档样式
配置文件 ：`artratex.sty` ，搜索
- 正文行距 `\linespread{1.25}` 
- 参考文献行距 `\setlength{\bibsep}{0.0ex}`
- 目录显示级数 `\setcounter{tocdepth}{2}`
- 文档超链接颜色 `\hypersetup`


#### 参考文献
```latex
\documentclass{article}
\usepackage{cite}  % 导入引用功能

\begin{document}

\nocite{*} % 引用所有条目，将 references.bib 文件中的所有条目列入参考文献列表

\bibliographystyle{plain}  % 选择参考文献样式
\bibliography{references}  % 指定.bib文件

\end{document}
```

##### 引用样式
可参考：[文献引用]([文献样式 · mohuangrui/ucasthesis Wiki](https://github.com/mohuangrui/ucasthesis/wiki/%E6%96%87%E7%8C%AE%E6%A0%B7%E5%BC%8F#%E5%BC%95%E7%94%A8%E6%A0%B7%E5%BC%8F))
- 著者-出版年
```latex
\usepackage[authoryear,list]{Style/artratex}% 文本: Jones (1995); 括号: (Jones, 1995)
```
- 顺序编码制
```latex
\usepackage[numbers,list]{Style/artratex}% 文本: Jones [1]; 括号: [1]
```
- 上标顺序编码制
```latex
\usepackage[super,list]{Style/artratex}% 文本: Jones 上标[1]; 括号: 上标[1]
```

##### 引用参考文献
```
\citep{article1}  
\citep{article1, article2}  % 多文献索引

```

#### 双面打印
`\cleardoublepage` - 该命令通常用于在双面打印时插入一个清空的页面，并确保下一页从右边的偶数页（在双面打印中，通常是右侧页）开始。如果你的文档是双面打印格式（例如 `twoside` 选项），`\cleardoublepage` 会在需要时插入一个空白页。
`\let\cleardoublepage\relax` - 将 `\cleardoublepage` 命令的功能重定义为空操作（`\relax`）。也就是说，当 LaTeX 遇到 `\cleardoublepage` 时，它不再插入清空的页面，也不会强制分页。通常这样做是为了禁用双面打印模式中的清空页面行为，特别是在需要单面打印的情况下。

#### 制图制表
[制图](https://github.com/mohuangrui/ucasthesis/wiki)
[制表](https://en.wikibooks.org/wiki/LaTeX/Tables)