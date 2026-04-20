# C/C++中 #pragma once 的使用

在 C/C++中，为了避免同一个文件被 include 多次，有两种方式：一种是#ifndef 方式，一种是#pragma once 方式(在头文件的最开始加入)。

```
#ifndef SOME_UNIQUE_NAME_HERE
#define SOME_UNIQUE_NAME_HERE
// contents of the header...
#endif // SOME_UNIQUE_NAME_HERE
```

```
#pragma once // contents of the header...
```

#ifndef 的是方式是受 C/C++语言标准支持。#ifndef 方式依赖于宏名不能冲突。它不光可以保证同一个文件不会被包含多次，也能保证内容完全相同的两个文件不会被不小心同时包含。缺点是如果不同头文件中的宏名不小心”碰撞”，可能就会导致你看到头文件明明存在，编译器却硬说找不到声明的状况。由于编译器每次都需要打开头文件才能判定是否有重复定义，因此在编译大型项目时，#ifndef 会使得编译时间相对较长，因此一些编译器逐渐开始支持#pragma once 的方式。

#pragma once 一般由编译器提供保证：同一个文件不会被包含多次。这里所说的”同一个文件”是指物理上的一个文件，而不是指内容相同的两个文件。无法对一个头文件中的一段代码作#pragma once 声明，而只能针对文件。此方式不会出现宏名碰撞引发的奇怪问题，大型项目的编译速度也因此提供了一些。缺点是如果某个头文件有多份拷贝，此方法不能保证它们不被重复包含。在 C/C++中，#pragma once 是一个非标准但是被广泛支持的方式。
