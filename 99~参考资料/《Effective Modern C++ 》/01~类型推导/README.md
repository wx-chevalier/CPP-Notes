# 第 1 章 类型推导

**CHAPTER 1 Deducing Types**

C++98 有一套类型推导的规则：用于函数模板的规则。C++11 修改了其中的一些规则并增加了两套规则，一套用于`auto`，一套用于`decltype`。C++14 扩展了`auto`和`decltype`可能使用的范围。类型推导的广泛应用，让你从拼写那些或明显或冗杂的类型名的暴行中脱离出来。它让 C++程序更具适应性，因为在源代码某处修改类型会通过类型推导自动传播到其它地方。但是类型推导也会让代码更复杂，因为由编译器进行的类型推导并不总是如我们期望的那样进行。

如果对于类型推导操作没有一个扎实的理解，要想写出有现代感的 C++程序是不可能的。类型推导随处可见：在函数模板调用中，在大多数`auto`出现的地方，在`decltype`表达式出现的地方，以及 C++14 中令人费解的应用`decltype(auto)`的地方。

这一章是每个 C++程序员都应该掌握的知识。它解释了模板类型推导是如何工作的，`auto`是如何依赖类型推导的，以及`decltype`是如何按照它自己那套独特的规则工作的。它甚至解释了你该如何强制编译器使类型推导的结果可视，这能让你确认编译器的类型推导是否按照你期望的那样进行。
