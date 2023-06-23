---
title: B.4 - C++20
alias: B.4 - C++20
origin: /b-4-introduction-to-c20/
origin_title: "B.4 — Introduction to C++20"
time: 2022-3-28
type: translation
---

??? note "Key Takeaway"

## C++20 是什么？

2020 年 2 月，[ISO (国际标准化组织)](https://www.iso.org/home.html)  通过了 C++的新版本——C++20。C++20 是自 C++11 以来，包含最多变化的版本。

## C++20 的改进

这里列出了 C++ 20 增加的主要改进。请注意，这个列表不是全面的，而是旨在突出一些感兴趣的关键改进点。

- 通过`auto`参数支持[[abbreviated function templates|缩写函数模板]]([[8-15-Function-templates-with-multiple-template-types|8.15 - 具有多种类型的函数模板]])
- `Chrono` 扩展用于支持日历和时区(_暂无教程_)
- Concepts，用于对模板参数进行限制 (_暂无教程_)
- Constexpr virtual functions, `unions`, `try`, `catch`, `dynamic_cast`, and `typeid` (_暂无教程_)
- `Constinit` 关键字，用于断言某个变量被静态初始化(_暂无教程_)
- 协程 (_暂无教程_)
- [[designated-initializers|指定初始化]]([[10-6-struct-aggregate-initialization|10.6 - 结构体的聚合初始化]])
- 使用`consteval` 创建即时函数 ([[6-14-Constexpr-and-consteval-functions|6.14 - Constexpr 和 consteval 函数]])
- 模块，作为 `#include` 的替代品(_暂无教程_)
- Ranges (_暂无教程_)
- `std::erase` (_暂无教程_)
- `std::make_shared` for arrays (_暂无教程_)
- `std::map::contains()` (_暂无教程_)
- `std::span` (_暂无教程_)
- 字符串格式化库(_暂无教程_，参见  [https://en.cppreference.com/w/cpp/utility/format](https://en.cppreference.com/w/cpp/utility/format))
- 字符串字面量作为模板参数 (_暂无教程_)
- 使用飞船运算符`<=>`进行三路比较(_暂无教程_)
- Using scoped enums (_暂无教程_)
- Views (_暂无教程_)
