# 移动语义

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
