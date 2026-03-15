# auto

auto 关键字指示编译器使用已声明变量的初始化表达式或 lambda 表达式参数来推导其类型。在大多情况下，建议使用 **`auto`** 关键字（除非确实需要转换），因为此关键字具有以下好处：

- **鲁棒性：** 如果表达式的类型已更改（包括更改函数返回类型时），则它只会正常工作。
- **性能：** 可以保证没有转换。
- **可用性：**不必担心类型名称拼写困难和拼写有误。
- **效率：**代码会变得更高效。

可能不需要使用 **`auto`** 的转换情况：

- 需要特定类型，其他任何操作都不会执行。
- 在表达式模板帮助程序类型中，例如 `(valarray+valarray)`。

若要使用 **`auto`** 关键字，请使用它而不是类型来声明变量，并指定初始化表达式。此外，还可通过使用说明符和声明符（如 **`const`**、**`volatile`**）、指针 (**`\*`**)、引用 (**`&`**) 以及右值引用 (**`&&`**) 来修改 **`auto`** 关键字。编译器计算初始化表达式，然后使用该信息来推断变量类型。

**`auto`** 初始化表达式可以采用多种形式：

- 通用初始化语法，例如 `auto a { 42 };`。
- 赋值语法，例如 `auto b = 0;`。
- 通用赋值语法，它结合了上述两种形式，例如 `auto c = { 3.14159 };`。
- 直接初始化或构造函数样式的语法，例如 `auto d( 1.41421f );`。

当 **`auto`** 用于在基于范围的 **`for`** 语句中声明循环参数时，它使用不同的初始化语法，例如`for (auto& i : iterable) do_action(i);`。有关详细信息，请参阅基于范围的 `for` 语句 (C++)。**`auto`** 关键字 (keyword) 是类型的占位符，但它本身不是类型。因此，**`auto`**关键字 (keyword) 不能用于强制转换或运算符，例如 `sizeof`。

# 使用括号初始值设定项 (C++14) 的类型推导

下面的代码示例演示如何使用大括号初始化 auto 变量。请注意 B 和 C 与 A 与 E 之间的差异。

```cpp
#include <initializer_list>

int main()
{
    // std::initializer_list<int>
    auto A = { 1, 2 };

    // std::initializer_list<int>
    auto B = { 3 };

    // int
    auto C{ 4 };

    // C3535: cannot deduce type for 'auto' from initializer list'
    auto D = { 5, 6.7 };

    // C3518 in a direct-list-initialization context the type for 'auto'
    // can only be deduced from a single initializer expression
    auto E{ 8, 9 };

    return 0;
}
```

| 错误号                                                                                                                                                                                                                                 | 说明                                                                                                                                                                                                                                                       |
| :------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- | :--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| [C3530](https://learn.microsoft.com/zh-cn/cpp/error-messages/compiler-errors-2/compiler-error-c3530?view=msvc-170)                                                                                                                     | **`auto`** 关键字 (keyword) 不能与任何其他类型说明符组合使用。                                                                                                                                                                                             |
| [C3531](https://learn.microsoft.com/zh-cn/cpp/error-messages/compiler-errors-2/compiler-error-c3531?view=msvc-170)                                                                                                                     | 使用 **`auto`** 关键字声明的符号必须具有初始值设定项。                                                                                                                                                                                                     |
| [C3532](https://learn.microsoft.com/zh-cn/cpp/error-messages/compiler-errors-2/compiler-error-c3532?view=msvc-170)                                                                                                                     | 你错误地使用了 **`auto`** 关键字来声明类型。例如，声明了方法返回类型或数组。                                                                                                                                                                               |
| [C3533](https://learn.microsoft.com/zh-cn/cpp/error-messages/compiler-errors-2/compiler-error-c3533?view=msvc-170)、[C3539](https://learn.microsoft.com/zh-cn/cpp/error-messages/compiler-errors-2/compiler-error-c3539?view=msvc-170) | 不能使用 **`auto`** 关键字 (keyword) 声明参数或模板参数。                                                                                                                                                                                                  |
| [C3535](https://learn.microsoft.com/zh-cn/cpp/error-messages/compiler-errors-2/compiler-error-c3535?view=msvc-170)                                                                                                                     | 不能使用 **`auto`** 关键字 (keyword) 声明方法或模板参数。                                                                                                                                                                                                  |
| [C3536](https://learn.microsoft.com/zh-cn/cpp/error-messages/compiler-errors-2/compiler-error-c3536?view=msvc-170)                                                                                                                     | 在初始化符号之前，不能使用它。实际上，这意味着变量不能用于初始化自身。                                                                                                                                                                                     |
| [C3537](https://learn.microsoft.com/zh-cn/cpp/error-messages/compiler-errors-2/compiler-error-c3537?view=msvc-170)                                                                                                                     | 不能强制转换为使用 **`auto`** 关键字 (keyword) 声明的类型。                                                                                                                                                                                                |
| [C3538](https://learn.microsoft.com/zh-cn/cpp/error-messages/compiler-errors-2/compiler-error-c3538?view=msvc-170)                                                                                                                     | 使用 **`auto`** 关键字声明的声明符列表中的所有符号必须解析为相同的类型。有关详细信息，请参阅[声明和定义](https://learn.microsoft.com/zh-cn/cpp/cpp/declarations-and-definitions-cpp?view=msvc-170)。                                                       |
| [C3540](https://learn.microsoft.com/zh-cn/cpp/error-messages/compiler-errors-2/compiler-error-c3540?view=msvc-170)、[C3541](https://learn.microsoft.com/zh-cn/cpp/error-messages/compiler-errors-2/compiler-error-c3541?view=msvc-170) | [sizeof](https://learn.microsoft.com/zh-cn/cpp/cpp/sizeof-operator?view=msvc-170) 和 [typeid](https://learn.microsoft.com/zh-cn/cpp/extensions/typeid-cpp-component-extensions?view=msvc-170) 运算符不能应用于使用 关键字 (keyword) 声明的**`auto`**符号。 |

# 示例

这些代码片段阐释了可使用 **`auto`** 关键字的一些方法。

下面的声明等效。在第一个语句中，将变量 `j` 声明为类型 **`int`**。在第二个语句中，将变量 `k` 推导为类型 **`int`**，因为初始化表达式 (0) 是整数。

```cpp
int j = 0;  // Variable j is explicitly type int.
auto k = 0; // Variable k is implicitly type int because 0 is an integer.
```

以下声明等效，但第二个声明比第一个更简单。使用 **`auto`** 关键字的最令人信服的一个原因是简单。

```cpp
map<int,list<string>>::iterator i = m.begin();
auto i = m.begin();
```

当 **`for`** 和范围 **`for`** 循环启动时，下列代码片段将声明变量 `iter` 和 `elem` 的类型。

```cpp
// cl /EHsc /nologo /W4
#include <deque>
using namespace std;

int main()
{
    deque<double> dqDoubleData(10, 0.1);

    for (auto iter = dqDoubleData.begin(); iter != dqDoubleData.end(); ++iter)
    { /* ... */ }

    // prefer range-for loops with the following information in mind
    // (this applies to any range-for with auto, not just deque)

    for (auto elem : dqDoubleData) // COPIES elements, not much better than the previous examples
    { /* ... */ }

    for (auto& elem : dqDoubleData) // observes and/or modifies elements IN-PLACE
    { /* ... */ }

    for (const auto& elem : dqDoubleData) // observes elements IN-PLACE
    { /* ... */ }
}
```

下面的代码片段使用 **`new`** 运算符和指针声明来声明指针。

```cpp
double x = 12.34;
auto *y = new auto(x), **z = new auto(&x);
```

下一个代码片段在每个声明语句中声明多个符号。请注意，每个语句中的所有符号将解析为同一类型。

```cpp
auto x = 1, *y = &x, **z = &y; // Resolves to int.
auto a(2.01), *b (&a);         // Resolves to double.
auto c = 'a', *d(&c);          // Resolves to char.
auto m = 1, &n = m;            // Resolves to int.
```

此代码片段使用条件运算符 (`?:`) 将变量 `x` 声明为值为 200 的整数：

```cpp
int v1 = 100, v2 = 200;
auto x = v1 > v2 ? v1 : v2;
```

下面的代码片段将变量 `x` 初始化为类型 **`int`**，将变量 `y` 初始化对类型 **`const int`** 的引用，将变量 `fp` 初始化为指向返回类型 **`int`** 的函数的指针。

```cpp
int f(int x) { return x; }
int main()
{
    auto x = f(0);
    const auto& y = f(1);
    int (*p)(int x);
    p = f;
    auto fp = p;
    //...
}
```
