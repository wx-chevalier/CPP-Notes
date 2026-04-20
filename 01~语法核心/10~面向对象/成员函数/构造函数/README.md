# C++ 构造函数详解

## 基础概念

构造函数是一种特殊的成员函数，用于初始化类的对象。它具有以下特点：

- 函数名与类名相同
- 没有返回值
- 可以重载
- 通常声明为 public
- 可以声明为 `inline`、`explicit`、`friend` 或 `constexpr`

## 构造函数的基本语法

```cpp
class Box {
public:
    // 1. 默认构造函数
    Box() {}

    // 2. 单参数构造函数（使用explicit防止隐式转换）
    explicit Box(int i) : m_width(i), m_length(i), m_height(i) {}

    // 3. 多参数构造函数
    Box(int width, int length, int height)
        : m_width(width), m_length(length), m_height(height)
    {}

private:
    // 成员变量初始化
    int m_width{ 0 };
    int m_length{ 0 };
    int m_height{ 0 };
};
```

## 构造函数的使用方式

```cpp
int main() {
    // 1. 默认构造
    Box b;

    // 2. 统一初始化语法（推荐）
    Box b2 {5};              // 调用单参数构造函数
    Box b3 {5, 8, 12};      // 调用三参数构造函数

    // 3. 传统函数调用语法
    Box b4(2, 4, 6);        // 调用三参数构造函数
}
```

## 高级特性

### 1. 成员初始化列表

- 在构造函数主体执行前初始化成员
- 比在构造函数体内赋值更高效
- const 成员和引用类型必须使用初始化列表

```cpp
Box(int width, int length, int height)
    : m_width(width), m_length(length), m_height(height)
{}
```

### 2. constexpr 构造函数

要求：

- 满足 constexpr 函数的所有条件
- 类没有虚拟基类
- 所有参数都是文本类型
- 所有非静态数据成员都必须初始化
- 类的所有基类和非静态成员都有 constexpr 构造函数

### 3. 初始化列表构造函数

支持使用 `std::initializer_list<T>` 进行构造：

```cpp
Box(initializer_list<string> list, int w = 0, int h = 0, int l = 0)
    : m_contents(list), m_width(w), m_height(h), m_length(l)
{}

// 使用示例
Box b{ "apples", "oranges", "pears" };
```

### 4. 复合类的构造函数

当类包含其他类的对象时：

- 先调用成员对象的构造函数
- 再调用当前类的构造函数
- 必须在初始化列表中调用没有默认构造函数的成员类构造函数

```cpp
class StorageBox : public Box {
public:
    StorageBox(int width, int length, int height, Label label)
        : Box(width, length, height),  // 先调用基类构造函数
          m_label(label)               // 再初始化成员对象
    {}
private:
    Label m_label;
};
```

## 最佳实践

1. 对单参数构造函数使用 `explicit` 关键字，防止意外的隐式类型转换
2. 优先使用成员初始化列表而不是构造函数体内的赋值
3. 优先使用统一初始化语法 `{}`
4. 确保所有成员变量都被正确初始化
