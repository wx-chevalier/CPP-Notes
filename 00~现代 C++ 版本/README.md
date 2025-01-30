# C++ 版本变迁

在过去的几十年，计算机技术以令人惊讶的速度发展着，当前，笔 记本电脑的计算速度和存储信息的能力超过了 20 世纪 60 年代的大型机。很多程序员可能还记得，将数叠穿孔卡片提交给充斥整个房间的大型计 算机系统的时代，而这种系统只有 100KB 的内存，比当今智能手机的内 存都少得多。计算机语言也得到了发展，尽管变化可能不是天翻地覆 的，但也是非常重要的。体积更大、功能更强的计算机引出了更大、更 复杂的程序，而这些程序在程序管理和维护方面带来了新的问题。

在 20 世纪 70 年代，C 和 Pascal 这样的语言引领人们进入了结构化编 程时代，这种机制把秩序和规程带进了迫切需要这种性质的领域中。除 了提供结构化编程工具外，C 还能生成简洁、快速运行的程序，并提供 了处理硬件问题的能力，如管理通信端口和磁盘驱动器。这些因素使 C 语言成为 20 世纪 80 年代占统治地位的编程语言。同时，20 世纪 80 年代，人们也见证了一种新编程模式的成长：面向对象编程（OOP）。SmallTalk 和 C++语言具备这种功能。下面更深入地介绍 C 和 OOP。

# 特性概览

## C++ 11

- [C++11 Language Features](#c11-language-features)

  - [Move semantics](#move-semantics)
  - [Rvalue references](#rvalue-references)
  - [Forwarding references](#forwarding-references)
  - [Variadic templates](#variadic-templates)
  - [Initializer lists](#initializer-lists)
  - [Static assertions](#static-assertions)
  - [auto](#auto)
  - [Lambda expressions](#lambda-expressions)
  - [decltype](#decltype)
  - [Type aliases](#type-aliases)
  - [nullptr](#nullptr)
  - [Strongly-typed enums](#strongly-typed-enums)
  - [Attributes](#attributes)
  - [constexpr](#constexpr)
  - [Delegating constructors](#delegating-constructors)
  - [User-defined literals](#user-defined-literals)
  - [Explicit virtual overrides](#explicit-virtual-overrides)
  - [Final specifier](#final-specifier)
  - [Default functions](#default-functions)
  - [Deleted functions](#deleted-functions)
  - [Range-based for loops](#range-based-for-loops)
  - [Special member functions for move semantics](#special-member-functions-for-move-semantics)
  - [Converting constructors](#converting-constructors)
  - [Explicit conversion functions](#explicit-conversion-functions)
  - [Inline namespaces](#inline-namespaces)
  - [Non-static data member initializers](#non-static-data-member-initializers)
  - [Right angle brackets](#right-angle-brackets)
  - [Ref-qualified member functions](#ref-qualified-member-functions)
  - [Trailing return types](#trailing-return-types)
  - [Noexcept specifier](#noexcept-specifier)
  - [char32_t and char16_t](#char32_t-and-char16_t)
  - [Raw string literals](#raw-string-literals)

- [C++11 Library Features](#c11-library-features)
  - [std::move](#stdmove)
  - [std::forward](#stdforward)
  - [std::thread](#stdthread)
  - [std::to_string](#stdto_string)
  - [Type traits](#type-traits)
  - [Smart pointers](#smart-pointers)
  - [std::chrono](#stdchrono)
  - [Tuples](#tuples)
  - [std::tie](#stdtie)
  - [std::array](#stdarray)
  - [Unordered containers](#unordered-containers)
  - [std::make_shared](#stdmake_shared)
  - [std::ref](#stdref)
  - [Memory model](#memory-model)
  - [std::async](#stdasync)
  - [std::begin/end](#stdbeginend)

## C++ 14

- [binary literals](#binary-literals)
- [generic lambda expressions](#generic-lambda-expressions)
- [lambda capture initializers](#lambda-capture-initializers)
- [return type deduction](#return-type-deduction)
- [decltype(auto)](#decltypeauto)
- [relaxing constraints on constexpr functions](#relaxing-constraints-on-constexpr-functions)
- [variable templates](#variable-templates)
- [\[\[deprecated\]\] attribute](#deprecated-attribute)

## C++ 17

- [template argument deduction for class templates](#template-argument-deduction-for-class-templates)
- [declaring non-type template parameters with auto](#declaring-non-type-template-parameters-with-auto)
- [folding expressions](#folding-expressions)
- [new rules for auto deduction from braced-init-list](#new-rules-for-auto-deduction-from-braced-init-list)
- [constexpr lambda](#constexpr-lambda)
- [lambda capture this by value](#lambda-capture-this-by-value)
- [inline variables](#inline-variables)
- [nested namespaces](#nested-namespaces)
- [structured bindings](#structured-bindings)
- [selection statements with initializer](#selection-statements-with-initializer)
- [constexpr if](#constexpr-if)
- [utf-8 character literals](#utf-8-character-literals)
- [direct-list-initialization of enums](#direct-list-initialization-of-enums)
- [\[\[fallthrough\]\], \[\[nodiscard\]\], \[\[maybe_unused\]\] attributes](#fallthrough-nodiscard-maybe_unused-attributes)
- [\_\_has_include](#__has_include)
- [class template argument deduction](#class-template-argument-deduction)

## C++ 20

- Language Features

  - [Coroutines](#coroutines)
  - [Concepts](#concepts)
  - [Designated initializers](#designated-initializers)
  - [Template syntax for lambdas](#template-syntax-for-lambdas)
  - [Range-based for loop with initializer](#range-based-for-loop-with-initializer)
  - [\[\[likely\]\] and \[\[unlikely\]\] attributes](#likely-and-unlikely-attributes)
  - [Deprecate implicit capture of this](#deprecate-implicit-capture-of-this)
  - [Class types in non-type template parameters](#class-types-in-non-type-template-parameters)
  - [constexpr virtual functions](#constexpr-virtual-functions)
  - [explicit(bool)](#explicitbool)
  - [Immediate functions](#immediate-functions)
  - [using enum](#using-enum)
  - [Lambda capture of parameter pack](#lambda-capture-of-parameter-pack)
  - [char8_t](#char8_t)
  - [constinit](#constinit)

- [C++20 Library Features](#c20-library-features)
  - [Concepts library](#concepts-library)
  - [Synchronized buffered outputstream](#synchronized-buffered-outputstream)
  - [std::span](#stdspan)
  - [Bit operations](#bit-operations)
  - [Math constants](#math-constants)
  - [std::is_constant_evaluated](#stdis_constant_evaluated)
  - [std::make_shared supports arrays](#stdmake_shared-supports-arrays)
  - [starts_with and ends_with on strings](#starts_with-and-ends_with-on-strings)
  - [Check if associative container has element](#check-if-associative-container-has-element)
  - [std::bit_cast](#stdbit_cast)
  - [std::midpoint](#stdmidpoint)
  - [std::to_array](#stdto_array)

# Links

- https://github.com/AnthonyCalandra/modern-cpp-features
- [Effective-Modern-Cpp](https://github.com/downdemo/Effective-Modern-Cpp): Modern C++ 一般指 C++11 及其之后的标准，已在工业界被广泛应用。C++ 初学者适合从 C++ Primer 开始学习 Modern C++ 的基本语法，通过 Effective C++ 掌握 C++98 的最佳实践，通过 Effective STL 掌握 STL 的正确使用，通过 Effective Modern C++ 掌握 C++11/14 的最佳实践，至此即可避开语言缺陷，得心应手地发挥 C++ 的长处。此为个人笔记，还将补充 C++17 相关特性。
