# 欢迎回到 C++ - 现代 C++

自创建以来，C++ 即已成为世界上最常用的编程语言之一。 正确编写的 C++ 程序快速、高效。 相对于其他语言，该语言更加灵活：它可以在最高的抽象级别上运行，还可以在硅级低级别上运行。 C++ 提供高度优化的标准库。 它支持访问低级别硬件功能，从而最大限度地提高速度并最大程度地降低内存需求。 C++ 几乎可以创建任何类型的程序：游戏、设备驱动程序、HPC、云、桌面、嵌入式和移动应用等。 甚至用于其他编程语言的库和编译器也使用 C++ 编写。

C++ 的原始要求之一是与 C 语言向后兼容。 因此，C++ 始终允许 C 样式编程，其中包含原始指针、数组、以 null 结尾的字符串和其他功能。 它们可以实现良好的性能，但也可能会引发 bug 并增加复杂性。 C++ 的演变注重可显著降低 C 样式惯例使用需求的功能。 如果需要，你仍可以使用旧的 C 编程设施。 但是，在新式 C++ 代码中，对上述设施的需求会越来越少。 现代 C++ 代码更加简单、安全、美观，而且速度仍像以往一样快速。

下面几个部分概述了现代 C++ 的主要功能。 此处列出的功能在 C++11 及更高版本中可用，除非另有说明。 在 Microsoft C++ 编译器中，可以设置 [`/std`](https://learn.microsoft.com/zh-cn/cpp/build/reference/std-specify-language-standard-version?view=msvc-170) 编译器选项，指定要用于项目的标准版本。

## 资源和智能指针

C 样式编程的一个主要 bug 类型是内存泄漏。 泄漏通常是由未能为使用 **`new`** 分配的内存调用 **`delete`** 导致的。 现代 C++ 强调“资源获取即初始化”(RAII) 原则。 其理念很简单。 资源（堆内存、文件句柄、套接字等）应由对象“拥有”。 该对象在其构造函数中创建或接收新分配的资源，并在其析构函数中将此资源删除。 RAII 原则可确保当所属对象超出范围时，所有资源都能正确返回到操作系统。

为了支持对 RAII 原则的简单采用，C++ 标准库提供了三种智能指针类型：[`std::unique_ptr`](https://learn.microsoft.com/zh-cn/cpp/standard-library/unique-ptr-class?view=msvc-170)、[`std::shared_ptr`](https://learn.microsoft.com/zh-cn/cpp/standard-library/shared-ptr-class?view=msvc-170) 和 [`std::weak_ptr`](https://learn.microsoft.com/zh-cn/cpp/standard-library/weak-ptr-class?view=msvc-170)。 智能指针可处理对其拥有的内存的分配和删除。 下面的示例演示了一个类，其中包含一个数组成员，该成员是在调用 `make_unique()` 时在堆上分配的。 对 **`new`** 和 **`delete`** 的调用将由 `unique_ptr` 类封装。 当 `widget` 对象超出范围时，将调用 unique_ptr 析构函数，此函数将释放为数组分配的内存。

C++复制

```cpp
#include <memory>
class widget
{
private:
    std::unique_ptr<int[]> data;
public:
    widget(const int size) { data = std::make_unique<int[]>(size); }
    void do_something() {}
};

void functionUsingWidget() {
    widget w(1000000);  // lifetime automatically tied to enclosing scope
                        // constructs w, including the w.data gadget member
    // ...
    w.do_something();
    // ...
} // automatic destruction and deallocation for w and w.data
```

请尽可能地使用智能指针管理堆内存。 如果必须显式使用 **`new`** 和 **`delete`** 运算符，请遵循 RAII 原则。 有关详细信息，请参阅[对象生存期和资源管理 (RAII)](https://learn.microsoft.com/zh-cn/cpp/cpp/object-lifetime-and-resource-management-modern-cpp?view=msvc-170)。

## `std::string` 和 `std::string_view`

C 样式字符串是 bug 的另一个主要来源。 通过使用 [`std::string` 和 `std::wstring`](https://learn.microsoft.com/zh-cn/cpp/standard-library/basic-string-class?view=msvc-170)，几乎可以消除与 C 样式字符串关联的所有错误。 还可以利用成员函数的优势进行搜索、追加和在前面追加等操作。 两者都对速度进行了高度优化。 将字符串传递到仅需要只读访问权限的函数时，在 C++17 中，可以使用 [`std::string_view`](https://learn.microsoft.com/zh-cn/cpp/standard-library/basic-string-view-class?view=msvc-170)，以便提高性能。

## `std::vector` 和其他标准库容器

标准库容器都遵循 RAII 原则。 它们为安全遍历元素提供迭代器。 此外，它们对性能进行了高度优化，并且已充分测试正确性。 通过使用这些容器，可以消除自定义数据结构中可能引入的 bug 或低效问题。 使用 [`vector`](https://learn.microsoft.com/zh-cn/cpp/standard-library/vector-class?view=msvc-170) 替代原始数组，来作为 C++ 中的序列容器。

C++复制

```cpp
vector<string> apples;
apples.push_back("Granny Smith");
```

使用 [`map`](https://learn.microsoft.com/zh-cn/cpp/standard-library/map-class?view=msvc-170)（而不是 `unordered_map`），作为默认关联容器。 对于退化和多案例，使用 [`set`](https://learn.microsoft.com/zh-cn/cpp/standard-library/set-class?view=msvc-170)、[`multimap`](https://learn.microsoft.com/zh-cn/cpp/standard-library/multimap-class?view=msvc-170) 和 [`multiset`](https://learn.microsoft.com/zh-cn/cpp/standard-library/multiset-class?view=msvc-170)。

C++复制

```cpp
map<string, string> apple_color;
// ...
apple_color["Granny Smith"] = "Green";
```

需要进行性能优化时，请考虑以下用法：

- 例如，当嵌入非常重要时，将 [`array`](https://learn.microsoft.com/zh-cn/cpp/standard-library/array-class-stl?view=msvc-170) 类型作为类成员。
- 使用无序的关联容器，例如 [`unordered_map`](https://learn.microsoft.com/zh-cn/cpp/standard-library/unordered-map-class?view=msvc-170)。 它们的每个元素的开销较低，并且具有固定时间查找功能，但正确高效地使用它们的难度更高。
- 使用排序的 `vector`。 有关详细信息，请参阅[算法](https://learn.microsoft.com/zh-cn/cpp/standard-library/algorithms?view=msvc-170)。

请勿使用 C 样式数组。 对于需要直接访问数据的旧 API，请改用 `f(vec.data(), vec.size());` 等访问器方法。 有关容器的详细信息，请参阅 [C++ 标准库容器](https://learn.microsoft.com/zh-cn/cpp/standard-library/stl-containers?view=msvc-170)。

## 标准库算法

在假设需要为程序编写自定义算法之前，请先查看 C++ 标准库[算法](https://learn.microsoft.com/zh-cn/cpp/standard-library/algorithm?view=msvc-170)。 标准库包含许多常见操作（如搜索、排序、筛选和随机化）的算法分类，这些分类在不断增长。 数学库的内容很广泛。 在 C++17 及更高版本中，提供了许多算法的并行版本。

以下是一些重要示例：

- `for_each`，默认遍历算法（以及基于范围的 `for` 循环）。
- `transform`，用于对容器元素进行非就地修改
- `find_if`，默认搜索算法。
- `sort`、`lower_bound` 和其他默认的排序和搜索算法。

若要编写比较运算符，请使用严格的 **`<`**，并尽可能使用命名 lambda。

C++复制

```cpp
auto comp = [](const widget& w1, const widget& w2)
     { return w1.weight() < w2.weight(); }

sort( v.begin(), v.end(), comp );

auto i = lower_bound( v.begin(), v.end(), widget{0}, comp );
```

## 用 `auto` 替代显式类型名称

C++11 引入了 [`auto`](https://learn.microsoft.com/zh-cn/cpp/cpp/auto-cpp?view=msvc-170) 关键字，以便可将其用于变量、函数和模板声明中。 **`auto`** 会指示编译器推导对象的类型，这样你就无需显式键入类型。 当推导出的类型是嵌套模板时，**`auto`** 尤其有用：

C++复制

```cpp
map<int,list<string>>::iterator i = m.begin(); // C-style
auto i = m.begin(); // modern C++
```

## 基于范围的 `for` 循环

对数组和容器的 C 样式迭代容易引发索引错误，而且键入过程单调乏味。 若要消除这些错误，并提高代码的可读性，可使用基于范围的 **`for`** 循环，此循环包含标准库容器和原始数组。 有关详细信息，请参阅[基于范围的 `for` 语句](https://learn.microsoft.com/zh-cn/cpp/cpp/range-based-for-statement-cpp?view=msvc-170)。

C++复制

```cpp
#include <iostream>
#include <vector>

int main()
{
    std::vector<int> v {1,2,3};

    // C-style
    for(int i = 0; i < v.size(); ++i)
    {
        std::cout << v[i];
    }

    // Modern C++:
    for(auto& num : v)
    {
        std::cout << num;
    }
}
```

## 用 `constexpr` 表达式替代宏

C 和 C++ 中的宏是指编译之前由预处理器处理的标记。 在编译文件之前，宏标记的每个实例都将替换为其定义的值或表达式。 C 样式编程通常使用宏来定义编译时常量值。 但宏容易出错且难以调试。 在现代 C++ 中，应优先使用 [`constexpr`](https://learn.microsoft.com/zh-cn/cpp/cpp/constexpr-cpp?view=msvc-170) 变量定义编译时常量：

C++复制

```cpp
#define SIZE 10 // C-style
constexpr int size = 10; // modern C++
```

## 统一初始化

在现代 C++ 中，可以使用任何类型的括号初始化。 在初始化数组、矢量或其他容器时，这种初始化形式会非常方便。 在下面的示例中，使用三个 `S` 实例初始化 `v2`。 `v3` 将使用三个 `S` 实例进行初始化，这些实例使用括号初始化自身。 编译器基于 `v3` 声明的类型推断每个元素的类型。

C++复制

```cpp
#include <vector>

struct S
{
    std::string name;
    float num;
    S(std::string s, float f) : name(s), num(f) {}
};

int main()
{
    // C-style initialization
    std::vector<S> v;
    S s1("Norah", 2.7);
    S s2("Frank", 3.5);
    S s3("Jeri", 85.9);

    v.push_back(s1);
    v.push_back(s2);
    v.push_back(s3);

    // Modern C++:
    std::vector<S> v2 {s1, s2, s3};

    // or...
    std::vector<S> v3{ {"Norah", 2.7}, {"Frank", 3.5}, {"Jeri", 85.9} };

}
```

若要了解详细信息，请参阅[括号初始化](https://learn.microsoft.com/zh-cn/cpp/cpp/initializing-classes-and-structs-without-constructors-cpp?view=msvc-170)。

## 移动语义

现代 C++ 提供了移动语义，此功能可以避免进行不必要的内存复制。 在此语言的早期版本中，在某些情况下无法避免复制。 移动操作会将资源的所有权从一个对象转移到下一个对象，而不必再进行复制。 一些类拥有堆内存、文件句柄等资源。 实现资源所属的类时，可以定义此类的移动构造函数和移动赋值运算符。 在解析重载期间，如果不需要进行复制，编译器会选择这些特殊成员。 如果定义了移动构造函数，则标准库容器类型会在对象中调用此函数。 有关详细信息，请参阅[移动构造函数和移动赋值运算符 (C++)](https://learn.microsoft.com/zh-cn/cpp/cpp/move-constructors-and-move-assignment-operators-cpp?view=msvc-170)。

## Lambda 表达式

在 C 样式编程中，可以通过使用函数指针将函数传递到另一个函数。 函数指针不便于维护和理解。 它们引用的函数可能是在源代码的其他位置中定义的，而不是从调用它的位置定义的。 此外，它们不是类型安全的。 现代 C++ 提供了函数对象和重写 [`operator()`](https://learn.microsoft.com/zh-cn/cpp/cpp/function-call-operator-parens?view=msvc-170) 运算符的类，可以像调用函数一样调用它们。 创建函数对象的最简便方法是使用内联 [lambda 表达式](https://learn.microsoft.com/zh-cn/cpp/cpp/lambda-expressions-in-cpp?view=msvc-170)。 下面的示例演示如何使用 lambda 表达式传递函数对象，然后由 `find_if` 函数在矢量的每个元素中调用此函数对象：

C++复制

```cpp
    std::vector<int> v {1,2,3,4,5};
    int x = 2;
    int y = 4;
    auto result = find_if(begin(v), end(v), [=](int i) { return i > x && i < y; });
```

可以将 lambda 表达式 `[=](int i) { return i > x && i < y; }` 理解为“采用类型 **`int`** 的单个参数并返回一个布尔值来表示此参数是否大于 `x` 并且小于 `y` 的函数”。请注意，可在 lambda 中使用来自周围上下文的 `x` 和 `y` 变量。 `[=]` 会指定通过值捕获这些变量；换言之，对于这些值，lambda 表达式具有自己的值副本。

## 异常

与错误代码相比，新式 C++ 更注重异常，将其作为报告和处理错误条件的最佳方法。 有关详细信息，请参阅[现代 C++ 处理异常和错误的最佳做法](https://learn.microsoft.com/zh-cn/cpp/cpp/errors-and-exception-handling-modern-cpp?view=msvc-170)。

## `std::atomic`

对线程间通信机制使用 C++ 标准库 [`std::atomic`](https://learn.microsoft.com/zh-cn/cpp/standard-library/atomic-structure?view=msvc-170) 结构和相关类型。

## `std::variant` (C++17)

C 样式编程通常通过并集使不同类型的成员可以占用同一个内存位置，从而节省内存。 但是，并集不是类型安全的，并且容易导致编程错误。 C++17 引入了更加安全可靠的 [`std::variant`](https://learn.microsoft.com/zh-cn/cpp/standard-library/variant-class?view=msvc-170) 类，来作为并集的替代项。 可以使用 [`std::visit`](https://learn.microsoft.com/zh-cn/cpp/standard-library/variant-functions?view=msvc-170#visit) 函数以类型安全的方式访问 `variant` 类型的成员。
