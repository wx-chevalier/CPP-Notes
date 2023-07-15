# 构造函数

若要自定义类初始化其成员的方式，或是如要在创建类的对象时调用函数，请定义构造函数。构造函数具有与类相同的名称，没有返回值。可以定义所需数量的重载构造函数，以各种方式自定义初始化。通常，构造函数具有公共可访问性，以便类定义或继承层次结构外部的代码可以创建类的对象。但也可以将构造函数声明为 protected 或 private。

构造函数可以选择采用成员初始化表达式列表。与在构造函数主体中赋值相比，初始化类成员是更高效的方式。以下示例演示具有三个重载构造函数的类 Box。最后两个构造函数使用成员初始化列表：

```cpp
class Box {
public:
    // Default constructor
    Box() {}

    // Initialize a Box with equal dimensions (i.e. a cube)
    explicit Box(int i) : m_width(i), m_length(i), m_height(i) // member init list
    {}

    // Initialize a Box with custom dimensions
    Box(int width, int length, int height)
        : m_width(width), m_length(length), m_height(height)
    {}

    int Volume() { return m_width * m_length * m_height; }

private:
    // Will have value of 0 when default constructor is called.
    // If we didn't zero-init here, default constructor would
    // leave them uninitialized with garbage values.
    int m_width{ 0 };
    int m_length{ 0 };
    int m_height{ 0 };
};
```

声明类的实例时，编译器会基于重载决策选择要调用的构造函数：

```cpp
int main()
{
    Box b; // Calls Box()

    // Using uniform initialization (preferred):
    Box b2 {5}; // Calls Box(int)
    Box b3 {5, 8, 12}; // Calls Box(int, int, int)

    // Using function-style notation:
    Box b4(2, 4, 6); // Calls Box(int, int, int)
}
```

- 构造函数可以声明为 **`inline`**、`explicit`、`friend` 或 `constexpr`。
- 构造函数可以初始化一个已声明为 **`const`**、**`volatile`** 或 **`const volatile`** 的对象。该对象在构造函数完成之后成为 **`const`**。
- 若要在实现文件中定义构造函数，请为它提供限定名称，如同任何其他成员函数一样：`Box::Box(){...}`。

# 成员初始化表达式列表

构造函数可以选择具有成员初始化表达式列表，该列表会在构造函数主体运行之前初始化类成员。首选成员初始化表达式列表，而不是在构造函数主体中赋值。成员初始化表达式列表直接初始化成员。以下示例演示了成员初始化表达式列表，该列表由冒号后的所有 identifier(argument) 表达式组成：

```cpp
Box(int width, int length, int height)
        : m_width(width), m_length(length), m_height(height)
    {}
```

标识符必须引用类成员；它使用参数的值进行初始化。参数可以是构造函数参数之一、函数调用或 std::initializer_list<T>。const 成员和引用类型的成员必须在成员初始化表达式列表中进行初始化。若要确保在派生构造函数运行之前完全初始化基类，请调用初始化表达式列表中的任何参数化基类构造函数。

# constexpr 构造函数

构造函数在以下情况下可能会声明为 [constexpr](https://learn.microsoft.com/zh-cn/cpp/cpp/constexpr-cpp?view=msvc-170)

- 声明为默认，或是在一般情况下满足 [constexpr 函数](https://learn.microsoft.com/zh-cn/cpp/cpp/constexpr-cpp?view=msvc-170#constexpr_functions) 的所有条件；
- 类没有虚拟基类；
- 每个参数都是[文本类型](https://learn.microsoft.com/zh-cn/cpp/cpp/trivial-standard-layout-and-pod-types?view=msvc-170#literal_types)；
- 主体不是函数 try 块；
- 所有非静态数据成员和基类子对象都会进行初始化；
- 如果类是 (a) 具有变体成员的联合，或是 (b) 具有匿名联合，则只有一个联合成员会进行初始化；
- 类类型的每个非静态数据成员以及所有基类子对象都具有 constexpr 构造函数

# 初始化表达式列表构造函数

如果某个构造函数采用 std::initializer_list<T> 作为其参数，并且任何其他参数都具有默认自变量，则当类通过直接初始化来实例化时，会在重载决策中选择该构造函数。可以使用 initializer_list 初始化可接受它的任何成员。例如，假设前面演示的 Box 类具有 std::vector<string> 成员 m_contents。可以提供如下所示的构造函数：

```cpp
Box(initializer_list<string> list, int w = 0, int h = 0, int l = 0)
        : m_contents(list), m_width(w), m_height(h), m_length(l)
{}
```

随后创建如下所示的 Box 对象：

```cpp
Box b{ "apples", "oranges", "pears" }; // or ...
    Box b2(initializer_list<string> { "bread", "cheese", "wine" }, 2, 4, 6);
```

# 显式构造函数

如果类具有带一个参数的构造函数，或是如果除了一个参数之外的所有参数都具有默认值，则参数类型可以隐式转换为类类型。例如，如果 Box 类具有一个类似于下面这样的构造函数：

```cpp
Box(int size): m_width(size), m_length(size), m_height(size){}
```

可以初始化 Box，如下所示：

```cpp
Box b = 42;
```

或将一个 int 传递给采用 Box 的函数：

```cpp
class ShippingOrder
{
public:
    ShippingOrder(Box b, double postage) : m_box(b), m_postage(postage){}

private:
    Box m_box;
    double m_postage;
}
//elsewhere...
    ShippingOrder so(42, 10.8);
```

这类转换可能在某些情况下很有用，但更常见的是，它们可能会导致代码中发生细微但严重的错误。作为一般规则，应对构造函数（和用户定义的运算符）使用 explicit 关键字以防止出现这种隐式类型转换：

```cpp
explicit Box(int size): m_width(size), m_length(size), m_height(size){}
```

构造函数是显式函数时，此行会导致编译器错误：ShippingOrder so(42, 10.8);。

# 构造函数和复合类

包含类类型成员的类称为“复合类”。创建复合类的类类型成员时，调用类自己的构造函数之前，先调用构造函数。当包含的类没有默认构造函数是，必须使用复合类构造函数中的初始化列表。在之前的 StorageBox 示例中，如果将 m_label 成员变量的类型更改为新的 Label 类，则必须调用基类构造函数，并且将 m_label 变量（位于 StorageBox 构造函数中）初始化：

```cpp
class Label {
public:
    Label(const string& name, const string& address) { m_name = name; m_address = address; }
    string m_name;
    string m_address;
};

class StorageBox : public Box {
public:
    StorageBox(int width, int length, int height, Label label)
        : Box(width, length, height), m_label(label){}
private:
    Label m_label;
};

int main(){
// passing a named Label
    Label label1{ "some_name", "some_address" };
    StorageBox sb1(1, 2, 3, label1);

    // passing a temporary label
    StorageBox sb2(3, 4, 5, Label{ "another name", "another address" });

    // passing a temporary label as an initializer list
    StorageBox sb3(1, 2, 3, {"myname", "myaddress"});
}
```
