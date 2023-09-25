# C++ 类型系统

类型的概念在 C++ 中很重要。每个变量、函数自变量和函数返回值必须具有一个类型以进行编译。此外，编译器 (包括文本值) 的所有表达式在计算前隐式指定类型。类型的一些示例包括内置类型，例如 int 用于存储整数值、double 存储浮点值或标准库类型（例如用于存储文本的类 std::basic_string ）。可以通过定义 class 或 struct 创建自己的类型。类型指定为变量 (或表达式结果) 分配的内存量。类型还指定了可以存储的值的类型、编译器如何解释这些值中的位模式，以及可对其执行的操作。本文包含对 C++ 类型系统的主要功能的非正式概述。

- 标量类型：保存已定义范围的单个值的类型。标量包括算术类型 (整型或浮点值) 、枚举类型成员、指针类型、指针到成员类型和 std::nullptr_t。基本类型通常是标量类型。
- 复合类型：不是标量类型的类型。复合类型包括数组类型、函数类型、类 (或结构) 类型、联合类型、枚举、引用和指向非静态类成员的指针。
- 变量：数据数量的符号名称。该名称可用于访问其所引用的整个代码范围内的数据。在 C++ 中，变量通常用于引用标量数据类型的实例，而其他类型的实例通常称为 对象。
- 对象：为简单起见和一致性，本文使用术语 object 来引用类或结构的任何实例。在一般意义上使用它时，它包括所有类型，甚至标量变量。
- POD 类型（纯旧数据）：C++ 中的此类非正式数据类型类别是指作为标量（参见基础类型部分）的类型或 POD 类。POD 类没有非 POD 的静态数据成员，也没有用户定义的构造函数、用户定义的析构函数或用户定义的赋值运算符。此外，POD 类无虚函数、基类、私有的或受保护的非静态数据成员。POD 类型通常用于外部数据交换，例如与用 C 语言编写的模块（仅具有 POD 类型）进行的数据交换。

# 指定变量和函数类型

C++ 既是强类型语言，也是静态类型化语言；每个对象都有一个类型，并且该类型永远不会更改。在代码中声明变量时，你必须显式指定其类型或使用 auto 关键字指示编译器通过初始值设定项推断类型。在代码中声明函数时，必须指定其返回值的类型以及每个参数的类型。如果函数未返回任何值，请使用返回值类型 void 。使用函数模板时例外，函数模板允许任意类型的参数。

首次声明变量后，无法在以后的某个时间更改其类型。但是，可以将变量的值或函数的返回值复制到另一个不同类型的变量中。此类操作称作“类型转换”，这些操作有时很必要，但也是造成数据丢失或不正确的潜在原因。

声明 POD 类型的变量时，强烈建议 对其进行初始化 ，这意味着为它提供初始值。在初始化某个变量之前，该变量会有一个“垃圾”值，该值包含之前正好位于该内存位置的位数。这是要记住的 C++ 的一个重要方面，特别是如果你来自另一种语言，为你处理初始化。声明非 POD 类类型的变量时，构造函数将处理初始化。

```c++
int result = 0;              // Declare and initialize an integer.
double coefficient = 10.8;   // Declare and initialize a floating
                             // point value.
auto name = "Lady G.";       // Declare a variable and let compiler
                             // deduce the type.
auto address;                // error. Compiler cannot deduce a type
                             // without an intializing value.
age = 12;                    // error. Variable declaration must
                             // specify a type or use auto!
result = "Kenny G.";         // error. Can't assign text to an int.
string result = "zero";      // error. Can't redefine a variable with
                             // new type.
int maxValue;                // Not recommended! maxValue contains
                             // garbage bits until it is initialized.
```
