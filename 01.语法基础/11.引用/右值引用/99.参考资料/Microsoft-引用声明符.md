# 引用声明符 &&

利用右值引用，您可以将左值与右值区分开。lvalue 引用和 rvalue 引用在语法和语义上相似，但它们遵循的规则稍有不同。有关 lvalue 和 rvalue 的详细信息，请参阅 [Lvalue 和 Rvalue](https://learn.microsoft.com/zh-cn/cpp/cpp/lvalues-and-rvalues-visual-cpp?view=msvc-170)。有关详细信息，请参阅[lvalue 引用声明符：&](https://learn.microsoft.com/zh-cn/cpp/cpp/lvalue-reference-declarator-amp?view=msvc-170)。

下面的章节介绍了 rvalue 引用如何支持“移动语义”和“完美转移”的实现。

## 移动语义

Rvalue 引用支持“移动语义”的实现，这可以显著提高应用程序的性能。利用移动语义，你可以编写将资源（如动态分配的内存）从一个对象转移到另一个对象的代码。移动语义很有效，因为它允许从临时对象（无法在程序中的其他位置引用）转移资源。

若要实现移动语义，通常可以向类提供“移动构造函数”，或者提供移动赋值运算符 (**`operator=`**)。其源是右值的复制和赋值操作随后会自动利用移动语义。与默认复制构造函数不同，编译器不提供默认移动构造函数。有关如何编写和使用移动构造函数详细信息，请参阅[移动构造函数和移动赋值运算符](https://learn.microsoft.com/zh-cn/cpp/cpp/move-constructors-and-move-assignment-operators-cpp?view=msvc-170)。

您还可以重载普通函数和运算符以利用移动语义。Visual Studio 2010 将移动语义引入到 C++ 标准库。例如，`string` 类实现了使用移动语义的操作。请考虑以下串联几个字符串并输出结果的示例：

```cpp
// string_concatenation.cpp
// compile with: /EHsc
#include <iostream>
#include <string>
using namespace std;

int main()
{
   string s = string("h") + "e" + "ll" + "o";
   cout << s << endl;
}
```

在 Visual Studio 2010 之前，每次对 **`operator+`** 的调用都会分配和返回新的临时 `string` 对象（rvalue）。**`operator+`** 不能将一个字符串附加到另一个字符串，因为它不知道源字符串是 lvalue 还是 rvalue。如果源字符串都是左值，则可能在程序中的其他位置引用它们，因此不得修改。可以使用右值引用进行修改 **`operator+`** 以采用右值，而右值引用不能在程序中的其他位置引用。通过此更改，**`operator+`** 现在可以将一个字符串附加到另一个字符串。这可以显著减少 `string` 类必须执行的动态内存分配的数量。有关 类的详细信息 `string` ，请参阅 [`basic_string` 类](https://learn.microsoft.com/zh-cn/cpp/standard-library/basic-string-class?view=msvc-170)。

当编译器不能使用返回值优化 (RVO) 或命名返回值优化 (NRVO) 时，移动语义也很有用。在这些情况下，如果类型定义了移动构造函数，则编译器将调用该函数。

若要更好地了解移动语义，请考虑将元素插入 `vector` 对象的示例。如果超出对象的容量 `vector` ，该 `vector` 对象必须为其元素重新分配足够的内存，然后将每个元素复制到另一个内存位置，以便为插入的元素腾出空间。当插入操作复制元素时，它首先创建一个新元素。然后它调用复制构造函数将数据从上一个元素复制到新元素。最后，它会销毁上一个元素。利用移动语义，可以直接移动对象而不必执行成本高昂的内存分配和复制操作。

若要在 `vector` 示例中利用移动语义，则可以编写将数据从一个对象移动到另一个对象的移动构造函数。

有关在 Visual Studio 2010 中引入移动语义到 C++ 标准库的详细信息，请参阅 [C++ 标准库](https://learn.microsoft.com/zh-cn/cpp/standard-library/cpp-standard-library-reference?view=msvc-170)。

## 完美转发

完美转发可减少对重载函数的需求，并有助于避免转发问题。当编写将引用作为其参数的泛型函数时，会引发“转移问题”。如果它将这些参数传递 (或 _转移_) 到另一个函数，例如，如果它采用类型 `const T&` 的参数，则被调用的函数无法修改该参数的值。如果泛型函数采用类型 `T&` 的参数，则无法使用 rvalue（如临时对象或整数文本）来调用该函数。

通常，若要解决此问题，则必须提供为其每个参数采用 `T&` 和 `const T&` 的重载版本的泛型函数。因此，重载函数的数量将基于参数的数量呈指数方式增加。rvalue 引用允许编写一个接受任意参数的函数版本。然后，该函数可以将它们转移到另一个函数，就像直接调用了另一个函数一样。

请考虑以下声明了四个类型 ` W``X `、`Y` 和 `Z` 的示例。每个类型的构造函数采用 **`const`** 和非 **`const`** lvalue 引用的不同组合作为其参数。

```cpp
struct W
{
   W(int&, int&) {}
};

struct X
{
   X(const int&, int&) {}
};

struct Y
{
   Y(int&, const int&) {}
};

struct Z
{
   Z(const int&, const int&) {}
};
```

假定您要编写生成对象的泛型函数。以下示例演示了一种编写此函数的方式：

```cpp
template <typename T, typename A1, typename A2>
T* factory(A1& a1, A2& a2)
{
   return new T(a1, a2);
}
```

以下示例演示了对 `factory` 函数的有效调用：

```cpp
int a = 4, b = 5;
W* pw = factory<W>(a, b);
```

但是，以下示例不包含对 `factory` 函数的有效调用。这是因为 `factory` 采用可修改的左值引用作为其参数，但它是使用右值调用的：

```cpp
Z* pz = factory<Z>(2, 2);
```

通常，若要解决此问题，您必须为 `factory` 和 `A&` 的参数的每个组合创建一个重载版本的 `const A&` 函数。利用右值引用，您可以编写一个版本的 `factory` 函数，如以下示例所示：

```cpp
template <typename T, typename A1, typename A2>
T* factory(A1&& a1, A2&& a2)
{
   return new T(std::forward<A1>(a1), std::forward<A2>(a2));
}
```

此示例使用右值引用作为 `factory` 函数的参数。函数的 [`std::forward`](https://learn.microsoft.com/zh-cn/cpp/standard-library/utility-functions?view=msvc-170#forward) 用途是将工厂函数的参数转发到模板类的构造函数。

以下示例演示了使用修改后的 `main` 函数创建 `factory`、`W`、`X` 和 `Y` 类的实例的 `Z` 函数。修改后的 `factory` 函数会将其参数（左值和右值）转发给适当的类构造函数。

```cpp
int main()
{
   int a = 4, b = 5;
   W* pw = factory<W>(a, b);
   X* px = factory<X>(2, b);
   Y* py = factory<Y>(a, 2);
   Z* pz = factory<Z>(2, 2);

   delete pw;
   delete px;
   delete py;
   delete pz;
}
```

## 右值引用的属性

**您可以重载采用左值引用和右值引用的函数。**

通过重载函数来采用 **`const`** lvalue 引用或 rvalue 引用，可以编写代码来区分不可更改的对象（lvalue）和可修改的临时值（rvalue）。可以将对象传递给采用 rvalue 引用的函数，除非该对象标记为 **`const`**。以下示例演示了函数 `f`，该函数将被重载以采用左值引用和右值引用。`main` 函数同时使用左值和右值来调用 `f`。

```cpp
// reference-overload.cpp
// Compile with: /EHsc
#include <iostream>
using namespace std;

// A class that contains a memory resource.
class MemoryBlock
{
   // TODO: Add resources for the class here.
};

void f(const MemoryBlock&)
{
   cout << "In f(const MemoryBlock&). This version can't modify the parameter." << endl;
}

void f(MemoryBlock&&)
{
   cout << "In f(MemoryBlock&&). This version can modify the parameter." << endl;
}

int main()
{
   MemoryBlock block;
   f(block);
   f(MemoryBlock());
}
```

该示例产生下面的输出：

Output 复制

```output
In f(const MemoryBlock&). This version can't modify the parameter.
In f(MemoryBlock&&). This version can modify the parameter.
```

在此示例中，对 `f` 的第一个调用将局部变量（左值）作为其自变量传递。对 `f` 的第二个调用将临时对象作为其自变量传递。由于无法在程序中的其他位置引用临时对象，因此调用会绑定到采用 rvalue 引用的 `f` 重载版本，该版本可以随意修改对象。

**编译器将已命名的右值引用视为左值，而将未命名的右值引用视为右值。**

采用 rvalue 引用作为参数的函数将参数视为函数正文中的 lvalue。编译器将命名的 rvalue 引用视为 lvalue。这是因为命名对象可由程序的多个部分引用。允许程序的多个部分从该对象修改或删除资源是危险的。例如，如果程序的多个部分尝试从同一对象转移资源，则只有第一个部分能成功转移。

以下示例演示了函数 `g`，该函数将被重载以采用左值引用和右值引用。函数 `f` 采用右值引用作为其参数（已命名的右值引用），并返回右值引用（未命名的右值引用）。在从 `g` 到 `f` 的调用中，重载决策选择采用左值引用的 `g` 版本，因为 `f` 的主体将其参数视为左值。在从 `g` 到 `main` 的调用中，重载决策选择采用右值引用的 `g` 版本，因为 `f` 返回右值引用。

```cpp
// named-reference.cpp
// Compile with: /EHsc
#include <iostream>
using namespace std;

// A class that contains a memory resource.
class MemoryBlock
{
   // TODO: Add resources for the class here.
};

void g(const MemoryBlock&)
{
   cout << "In g(const MemoryBlock&)." << endl;
}

void g(MemoryBlock&&)
{
   cout << "In g(MemoryBlock&&)." << endl;
}

MemoryBlock&& f(MemoryBlock&& block)
{
   g(block);
   return move(block);
}

int main()
{
   g(f(MemoryBlock()));
}
```

该示例产生下面的输出：

```cpp
In g(const MemoryBlock&).
In g(MemoryBlock&&).
```

在此示例中，`main` 函数将 rvalue 传递给 `f`。`f` 的主体将其命名参数视为左值。从 `f` 到 `g` 的调用会将参数绑定到左值引用（第一个重载版本的 `g`）。

- **可以将 lvalue 强制转换为 rvalue 引用。**

C++ 标准库 [`std::move`](https://learn.microsoft.com/zh-cn/cpp/standard-library/utility-functions?view=msvc-170#move) 函数可以将某个对象转换为对该对象的 rvalue 引用。也可以使用 **`static_cast`** 关键字将 lvalue 强制转换为 rvalue 引用，如以下示例所示：

```cpp
// cast-reference.cpp
// Compile with: /EHsc
#include <iostream>
using namespace std;

// A class that contains a memory resource.
class MemoryBlock
{
   // TODO: Add resources for the class here.
};

void g(const MemoryBlock&)
{
   cout << "In g(const MemoryBlock&)." << endl;
}

void g(MemoryBlock&&)
{
   cout << "In g(MemoryBlock&&)." << endl;
}

int main()
{
   MemoryBlock block;
   g(block);
   g(static_cast<MemoryBlock&&>(block));
}
```

该示例产生下面的输出：

```cpp
In g(const MemoryBlock&).
In g(MemoryBlock&&).
```

**函数模板会推导出其模板自变量类型，然后使用引用折叠规则。**

将其参数传递（或“转发”）给另一个函数的函数模板是一种常见模式。了解模板类型推导如何对采用 rvalue 引用的函数模板起作用，这很重要。

如果函数参数是右值，则编译器将参数推导为右值引用。例如，假设将对 类型的 `X` 对象的右值引用传递给采用 类型 `T&&` 作为其参数的函数模板。模板参数推导推断 `T` 为 `X`，因此该参数具有类型 `X&&`。如果函数参数是 lvalue 或 **`const`** lvalue，则编译器将其类型推导为该类型的 lvalue 引用或 **`const`** lvalue 引用。

以下示例声明了一个结构模板，然后针对不同引用类型对其进行了专用化。`print_type_and_value` 函数采用右值引用作为其参数，并将它转发给适当专用版本的 `S::print` 方法。`main` 函数演示了调用 `S::print` 方法的各种方式。

```cpp
// template-type-deduction.cpp
// Compile with: /EHsc
#include <iostream>
#include <string>
using namespace std;

template<typename T> struct S;

// The following structures specialize S by
// lvalue reference (T&), const lvalue reference (const T&),
// rvalue reference (T&&), and const rvalue reference (const T&&).
// Each structure provides a print method that prints the type of
// the structure and its parameter.

template<typename T> struct S<T&> {
   static void print(T& t)
   {
      cout << "print<T&>: " << t << endl;
   }
};

template<typename T> struct S<const T&> {
   static void print(const T& t)
   {
      cout << "print<const T&>: " << t << endl;
   }
};

template<typename T> struct S<T&&> {
   static void print(T&& t)
   {
      cout << "print<T&&>: " << t << endl;
   }
};

template<typename T> struct S<const T&&> {
   static void print(const T&& t)
   {
      cout << "print<const T&&>: " << t << endl;
   }
};

// This function forwards its parameter to a specialized
// version of the S type.
template <typename T> void print_type_and_value(T&& t)
{
   S<T&&>::print(std::forward<T>(t));
}

// This function returns the constant string "fourth".
const string fourth() { return string("fourth"); }

int main()
{
   // The following call resolves to:
   // print_type_and_value<string&>(string& && t)
   // Which collapses to:
   // print_type_and_value<string&>(string& t)
   string s1("first");
   print_type_and_value(s1);

   // The following call resolves to:
   // print_type_and_value<const string&>(const string& && t)
   // Which collapses to:
   // print_type_and_value<const string&>(const string& t)
   const string s2("second");
   print_type_and_value(s2);

   // The following call resolves to:
   // print_type_and_value<string&&>(string&& t)
   print_type_and_value(string("third"));

   // The following call resolves to:
   // print_type_and_value<const string&&>(const string&& t)
   print_type_and_value(fourth());
}
```

该示例产生下面的输出：

```cpp
print<T&>: first
print<const T&>: second
print<T&&>: third
print<const T&&>: fourth
```

为了解析每个对 `print_type_and_value` 函数的调用，编译器首先会执行模板参数推导。然后，编译器再用推导出的模板参数替换参数类型时应用引用折叠规则。例如，将局部变量 `s1` 传递给 `print_type_and_value` 函数将导致编译器生成以下函数签名：

```cpp
print_type_and_value<string&>(string& && t)
```

编译器使用引用折叠规则将签名缩短：

```cpp
print_type_and_value<string&>(string& t)
```

此版本的 `print_type_and_value` 函数随后将其参数转发到正确的专用版本的 `S::print` 方法。

下表汇总了模板自变量类型推导的引用折叠规则：

| 展开类型 | 折叠类型 |
| :------- | :------- |
| `T& &`   | `T&`     |
| `T& &&`  | `T&`     |
| `T&& &`  | `T&`     |
| `T&& &&` | `T&&`    |

模板自变量推导是实现完美转发的重要因素。[完美转发](https://learn.microsoft.com/zh-cn/cpp/cpp/rvalue-reference-declarator-amp-amp?view=msvc-170#perfect-forwarding)部分更详细地描述了完美转发。
