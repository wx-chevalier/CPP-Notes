# Introduction

Rvalue references are a feature of C++ that was added with the C++11 standard. What makes rvalue references a bit difficult to grasp is that when you first look at them, it is not clear what their purpose is or what problems they solve. Therefore, I will not jump right in and explain what rvalue references are. Instead, I will start with the problems that are to be solved and then show how rvalue references provide the solution. That way, the definition of rvalue references will appear plausible and natural to you.

> 右值引用是随 C++11 标准添加的 C++ 的一项功能。使右值引用有点难以理解的是，当您第一次查看它们时，并不清楚它们的目的是什么或它们解决了什么问题。因此，我不会直接跳进去解释什么是右值引用。相反，我将从要解决的问题开始，然后展示右值引用如何提供解决方案。这样，右值引用的定义对您来说将是合理和自然的。

Rvalue references solve at least two problems:

> 右值引用至少解决了两个问题：

- Implementing move semantics
- Perfect forwarding

If you are not familiar with these problems, do not worry. Both of them will be explained in detail below. We'll start with move semantics. But before we're ready to go, I need to remind you of what lvalues and rvalues are in C++. Giving a rigorous definition is surprisingly difficult, but the explanation below is good enough for the purpose at hand.

> 如果您不熟悉这些问题，请不要担心。下面将详细解释它们。我们将从移动语义开始。但在我们准备出发之前，我需要提醒你什么是左值和右值 C++。给出一个严格的定义是非常困难的，但下面的解释对于手头的目的来说已经足够了。

The original definition of lvalues and rvalues from the earliest days of C is as follows: An lvalue is an expression e that may appear on the left or on the right hand side of an assignment, whereas an rvalue is an expression that can only appear on the right hand side of an assignment. For example,

> 从 C 语言早期开始，左值和右值的原始定义如下：左值是可能出现在赋值左侧或右侧的表达式，而右值是只能出现在赋值右侧的表达式 e 。例如

```cpp
  int a = 42;
  int b = 43;

  // a and b are both l-values:
  a = b; // ok
  b = a; // ok
  a = a * b; // ok

  // a * b is an rvalue:
  int c = a * b; // ok, rvalue on right hand side of assignment
  a * b = 42; // error, rvalue on left hand side of assignment
```

In C++, this is still useful as a first, intuitive approach to lvalues and rvalues. However, C++ with its user-defined types has introduced some subtleties regarding modifiability and assignability that cause this definition to be incorrect. There is no need for us to go further into this. Here is an alternate definition which, although it can still be argued with, will put you in a position to tackle rvalue references: An lvalue is an expression that refers to a memory location and allows us to take the address of that memory location via the & operator. An rvalue is an expression that is not an lvalue. Examples are:

> 在 C++ 中，这仍然是有用的，作为左值和右值的第一个直观方法。但是，C++ 及其用户定义类型引入了一些关于可修改性和可分配性的细微之处，导致此定义不正确。我们没有必要进一步讨论这个问题。这里有一个替代定义，虽然它仍然可以争论，但将使您能够处理右值引用： 左值是一个引用内存位置的表达式，并允许我们通过 & 运算符获取该内存位置的地址。右值是不是左值的表达式。例如：

```cpp
  // lvalues:
  //
  int i = 42;
  i = 43; // ok, i is an lvalue
  int* p = &i; // ok, i is an lvalue
  int& foo();
  foo() = 42; // ok, foo() is an lvalue
  int* p1 = &foo(); // ok, foo() is an lvalue

  // rvalues:
  //
  int foobar();
  int j = 0;
  j = foobar(); // ok, foobar() is an rvalue
  int* p2 = &foobar(); // error, cannot take the address of an rvalue
  j = 42; // ok, 42 is an rvalue
```

If you are interested in a rigorous definition of rvalues and lvalues, a good place to start is Mikael Kilpeläinen's ACCU article on the subject.

> 如果您对右值和右值的严格定义感兴趣，那么 Mikael Kilpeläinen 关于该主题的 ACCU 文章是一个很好的起点。
