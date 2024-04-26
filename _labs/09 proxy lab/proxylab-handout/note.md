- argc[0] 本程序名
- fprintf(stderr, "usage: %s <port>\n", argv[0]); 输出到stderr流
- strcasecmp 不区分大小写的strcmp
- [c语言多文件编译](https://wangdoc.com/clang/multifile)
  - 将两个文件一起编译
    $ gcc -o foo foo.c bar.c

    # 更省事的写法
    $ gcc -o foo *.c
  - 在一个文件内引用一个函数需要在此文件内声明 =》 专门一个头文件(.h)用于声明。没有放在尖括号里面，表示它是用户提供的
  - make 是一个命令行工具，使用时会自动在当前目录下搜索配置文件 makefile（也可以写成 Makefile）。该文件定义了所有的编译规则，每个编译规则对应一个编译产物。为了得到这个编译产物，它需要知道两件事。
    - 依赖项（生成该编译产物，需要用到哪些文件）
    - 生成命令（生成该编译产物的命令）



