Stackening
==========

What is Stac..., oh forget the name, and why it's called so?
------------------------------------------------------------

Stackening 程序设计语言是这个编程语言泛滥的时代中又一个**编译型**程序设计语言.

下一个版本的目标之一是换一个短一点名字, 这样在写 README 时就不必为这么长的单词补齐下面的等号了. 因此现在就不给 ASCII Art Logo 了.

It aims at what?
----------------

简单.
从以下方面: 设计, 编码, 配置.

* 设计: 整体架构文档见 https://github.com/neuront/stackening/wiki/ModulesAndArchitecture
* 编码: 模块组织紧凑; 函数短小, 参数个数少; 类成员/接口少; 类继承层次少; 能多态就不分支/选择; 容错而不使用异常
* 配置: 不依赖配置复杂的库

What is needed and how to build it?
-----------------------------------

### 编译源代码需要

* GCC 4.5.x (C++ 2011 标准中的 Lambda, `std::unique_ptr`, Move Semantic 支持) (注: 使用 GCC 4.7.0 experimental 可能导致 g++ 崩溃)
* gawk 3.1.x (AWK)
* flex 2.5.x (词法分析)
* bison 2.4.x (语法分析)
* GMP 5.x (高精度算术库, 用于字面常量折叠优化)

以上配置如需改变, 请修改源码目录中的 misc/mf-template.mk, parser/lex-script, parser/syn-script

在 Stackening 源代码目录下执行 `make` 来编译.

### 编译单元测试需要
* GoogleTest 1.5.x
* POSIX Thread 支持 (GoogleTest 依赖)

不进行单元测试没关系, 并且当下单元测试覆盖率并不高

在 Stackening 源代码目录下执行 `make runtest` 进行测试.

### 编译后端代码
Stackening 没有独立的后端, 它将生成 C++ 代码, 然后使用 g++ 编译这些代码生成可执行程序, 所以需要 GCC 4.3+ 进行后端编译, 此处的配置在源码目录的 stk.sh 中

Compared with other languages, what's the differences?
------------------------------------------------------

Stackening 是编译型语言, 其语法形式是抄袭完全动态的 Python 语言的, 此外还抄袭一些函数式程序设计语言

### 缩进

* Stackening 要求的缩进比 Python 更加强, 缩进必须是 4 空格的整数倍, 而不可以是制表符, 或者 1-3 个空格
* 缩进不可以参差不齐, 子句必须统一缩进
* 缩进还要紧邻, 如函数体的缩进只可以比函数头的缩进多一级

### 类型推导

* 变量定义无需指定类型 (Stackening 将从初值进行推导)
* 函数定义无须指定参数与返回值类型 (Stackening 将从实参和 `return` 语句推导)

### 不可修改
* 变量定义后不可以改变值或类型
* `=` 符号的作用是比较两侧操作数相等, 而不是赋值

### 嵌套函数定义
Stackening 允许函数嵌套定义, 并且内层定义的函数可以直接引用外层函数中定义的局部变量.

### `ifnot` 分支
对于条件成立时什么也不用做而条件不成立的时候需要忙活的分支, 如果不希望在条件前面打上碍眼的感叹号, 可以尝试这种分支语句.

Use it to compile
-----------------

编译完成后会生成 stk-core.out 文件, 它从标准输入读取 Stackening 源代码, 并输出后端代码 ''片段'' 定向到标准输出.

源码目录下有脚本 stk.sh 来更方便地编译 Stackening 源代码, 使用方法:

`$ sh stk.sh 源代码文件 目标可执行文件`

samples 目录下有很多样例可以参考. 其中 samples/errors 目录下的样例包含一些典型的错误, 而 samples/warnings 目录下的样例包含典型的警告.

很抱歉, 语言规范还没有.

The End Of Stackening
---------------------
最后一次合并到主干的代码包括下列特性

简单的**闭包**函数被引入了. 由于 Stackening 支持多个函数重载同名但参数个数不相同, 如果闭包引用的函数有重载, 那么引用函数必须在函数名之后加上 `@参数个数`. (闭包的实现非常暴力) 你可以在 samples/pair.stkn, samples/vector-multi.stkn 中看到简单的闭包使用样例.

这次更新将是 Stackening 的终结, 下一个版本项目名字将改变.

What will be added / changed next, except the replace of the ball-ache name?
----------------------------------------------------------------------------
主要

设计: 修正现在 `instance` 和 `proto` 就 `proto::function` 而互相依赖的问题;

测试: 完善测试案例;

可选

设计: 符号表作为单独的库模块实现;

Contributing
------------

1. Fork it.
2. Create a branch (`git checkout -b stackening_my_own`)
3. Commit your changes (`git commit -a -m "I have add some kick-ass features, or I have fixed some ball-ache bugs."`)
4. Push to the branch (`git push origin stackening_my_own`)
5. Create an *Issue* with a link to your branch
6. Enjoy tsundere by Kagami Sama and wait