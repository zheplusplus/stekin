Stekin
======
                                                                     __  __
                                                                     \ \/ /\
              ________________________________________________________\  /_/__
             / _______________________________________________________    ___/\
            / /\_____________________________________________________/ /\ \__\/
           / / /    __________   _______   __   ___    __    __     / / /\_\
          ( ( /    /___  ____/\ / _____/\ / /\,' _/\  / /\  /  |\  / / /  \_\
           \ \\    \__/ /\___\// /\____\// /,' ,'\_/ / / / /   || / / /
            \ \\     / / /    / /_/_    / /' ,' ,'  / / / / /| ||/ / /
             ) )\   / / /    / ____/\  /   ,' ,'   / / / / / | |/ / /
            / / /  / / /    / /\___\/ / /\ \,'    / / / / / /| ' / /
     ______/ / /  / / /    / /_/___  / / /\ \__  / / / / / / |  / /
    /_______/ /  /_/ /    /_______/\/_/ /  \__/\/_/ / /_/ /  |_/ /
    \_______\/   \_\/     \_______\/\_\/    \_\/\_\/  \__/   \__/

What is Stekin?
---------------

Stekin 程序设计语言是这个编程语言泛滥的时代中又一个**编译型**程序设计语言.

It aims at what?
----------------

简单.
从以下方面: 设计, 编码, 配置.

* 设计: 整体架构文档见 https://github.com/neuront/stekin/wiki/ModulesAndArchitecture
* 编码: 模块组织紧凑; 函数短小, 参数个数少; 类成员/接口少; 类继承层次少; 能多态就不分支/选择; 容错而不使用异常
* 配置: 不依赖配置复杂的库

What is needed and how to build it?
-----------------------------------

### 编译源代码需要

* GCC 4.6.x (C++ 2011 标准中的 Lambda, `std::unique_ptr`, Move Semantic, nullptr 支持)
* gawk 3.1.x (AWK)
* flex 2.5.x (词法分析)
* bison 2.4.x (语法分析)
* GMP 5.x (高精度算术库, 用于字面常量折叠优化)

以上配置如需改变, 请修改源码目录中的 misc/mf-template.mk, parser/lex-script, parser/syn-script

在 Stekin 源代码目录下执行 `make` 来编译.

### 编译和运行单元测试需要
* GoogleTest 1.5.x
* POSIX Thread 支持 (GoogleTest 依赖)
* valgrind (内存错误检测)

不进行单元测试没关系, 并且当下单元测试覆盖率并不高

在 Stekin 源代码目录下执行 `make runtest` 进行测试.

### 编译后端代码

编译完成后会生成 stk-core.out 文件, 它从标准输入读取 Stekin 源代码, 并输出后端代码 ''片段'' 定向到标准输出.

Stekin 没有独立的后端, 它将生成 C++ 代码, 然后使用 g++ 编译这些代码生成可执行程序, 所以需要 GCC 4.4+ 进行后端编译, 此处的配置在源码目录的 stkn.sh 中

使用 `bash stkn.sh 输入文件名 目标文件名` 可以快捷完成编译, 如

`bash stkn.sh samples/list-pipe.stkn ./a.out && ./a.out`

可以编译 `samples/list-pipe.stkn` 生成 `./a.out` 并执行它.

Stekin the language itself
--------------------------

请参阅 samples/README.md

Contributing
------------

0. Take a glimpse at coding style conventions: https://github.com/neuront/stekin/wiki/StekinCppCodingStyle
1. Fork it.
2. Create a branch (`git checkout -b stekin_my_own`)
3. Commit your changes (`git commit -a -m "I have add some kick-ass features, or I have fixed some ball-ache bugs."`)
4. Push to the branch (`git push origin stekin_my_own`)
5. Create an *Issue* with a link to your branch
6. Enjoy some music by Mr. Kenji Kawai and wait
