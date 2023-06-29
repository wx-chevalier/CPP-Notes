# 构造函数

若要自定义类初始化其成员的方式，或是如要在创建类的对象时调用函数，请定义构造函数。构造函数具有与类相同的名称，没有返回值。可以定义所需数量的重载构造函数，以各种方式自定义初始化。通常，构造函数具有公共可访问性，以便类定义或继承层次结构外部的代码可以创建类的对象。但也可以将构造函数声明为 protected 或 private。

构造函数可以选择采用成员初始化表达式列表。 与在构造函数主体中赋值相比，初始化类成员是更高效的方式。 以下示例演示具有三个重载构造函数的类 Box。 最后两个构造函数使用成员初始化列表：

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
- 构造函数可以初始化一个已声明为 **`const`**、**`volatile`** 或 **`const volatile`** 的对象。 该对象在构造函数完成之后成为 **`const`**。
- 若要在实现文件中定义构造函数，请为它提供限定名称，如同任何其他成员函数一样：`Box::Box(){...}`。

# 成员初始化表达式列表

构造函数可以选择具有成员初始化表达式列表，该列表会在构造函数主体运行之前初始化类成员。首选成员初始化表达式列表，而不是在构造函数主体中赋值。 成员初始化表达式列表直接初始化成员。 以下示例演示了成员初始化表达式列表，该列表由冒号后的所有 identifier(argument) 表达式组成：

```cpp
Box(int width, int length, int height)
        : m_width(width), m_length(length), m_height(height)
    {}
```

标识符必须引用类成员；它使用参数的值进行初始化。 参数可以是构造函数参数之一、函数调用或 std::initializer_list<T>。const 成员和引用类型的成员必须在成员初始化表达式列表中进行初始化。若要确保在派生构造函数运行之前完全初始化基类，请调用初始化表达式列表中的任何参数化基类构造函数。
