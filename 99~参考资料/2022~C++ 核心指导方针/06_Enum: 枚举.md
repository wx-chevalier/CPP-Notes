# <a name="S-enum"></a>Enum: 枚举

枚举用于定义整数值的集合，并用于为这种值集定义类型。有两种类型的枚举，
“普通”的 `enum` 和 `class enum`。

枚举规则概览：

- [Enum.1: 优先采用枚举而不是宏](#Renum-macro)
- [Enum.2: 采用枚举来表示相关的具名常量的集合](#Renum-set)
- [Enum.3: 优先采用 `enum class` 而不是“普通”`enum`](#Renum-class)
- [Enum.4: 针对安全和简单的用法来为枚举定义操作](#Renum-oper)
- [Enum.5: 请勿为枚举符采用 `ALL_CAPS` 命名方式](#Renum-caps)
- [Enum.6: 避免使用无名枚举](#Renum-unnamed)
- [Enum.7: 仅在必要时才为枚举指定其底层类型](#Renum-underlying)
- [Enum.8: 仅在必要时才指定枚举符的值](#Renum-value)

### <a name="Renum-macro"></a>Enum.1: 优先采用 `enum` 而不是宏

##### 理由

宏不遵守作用域和类型规则。而且，宏的名字在预处理中就被移除，因而通常不会出现在如调试器这样的工具中。

##### 示例

首先是一些不好的老代码：

    // webcolors.h (第三方头文件)
    #define RED   0xFF0000
    #define GREEN 0x00FF00
    #define BLUE  0x0000FF

    // productinfo.h
    // 以下则基于颜色定义了产品的子类型
    #define RED    0
    #define PURPLE 1
    #define BLUE   2

    int webby = BLUE;   // webby == 2; 可能不是我们所想要的

代之以 `enum`：

    enum class Web_color { red = 0xFF0000, green = 0x00FF00, blue = 0x0000FF };
    enum class Product_info { red = 0, purple = 1, blue = 2 };

    int webby = blue;   // 错误: 应当明确
    Web_color webby = Web_color::blue;

我们用 `enum class` 来避免名字冲突。

##### 强制实施

标记定义整数值的宏。

### <a name="Renum-set"></a>Enum.2: 采用枚举来表示相关的具名常量的集合

##### 理由

枚举展示其枚举符之间是有关联的，且可以用作具名类型。

##### 示例

    enum class Web_color { red = 0xFF0000, green = 0x00FF00, blue = 0x0000FF };

##### 注解

对枚举的 `switch` 是很常见的，编译器可以对不平常的 `case` 标签进行警告。例如：

    enum class Product_info { red = 0, purple = 1, blue = 2 };

    void print(Product_info inf)
    {
        switch (inf) {
        case Product_info::red: cout << "red"; break;
        case Product_info::purple: cout << "purple"; break;
        }
    }

这种漏掉一个的 `switch` 语句通常是添加枚举符并缺少测试的结果。

##### 强制实施

- 当 `switch` 语句的 `case` 标签并未覆盖枚举的全部枚举符时，对其进行标记。
- 当 `switch` 语句的 `case` 覆盖了枚举的几个枚举符，但没有 `default` 时，对其进行标记。

### <a name="Renum-class"></a>Enum.3: 优先采用 `class enum` 而不是“普通”`enum`

##### 理由

最小化意外情况：传统的 `enum` 太容易转换为 `int` 了。

##### 示例

    void Print_color(int color);

    enum Web_color { red = 0xFF0000, green = 0x00FF00, blue = 0x0000FF };
    enum Product_info { red = 0, purple = 1, blue = 2 };

    Web_color webby = Web_color::blue;

    // 显然至少有一个调用是有问题的。
    Print_color(webby);
    Print_color(Product_info::blue);

代之以 `enum class`：

    void Print_color(int color);

    enum class Web_color { red = 0xFF0000, green = 0x00FF00, blue = 0x0000FF };
    enum class Product_info { red = 0, purple = 1, blue = 2 };

    Web_color webby = Web_color::blue;
    Print_color(webby);  // 错误: 无法转换 Web_color 为 int。
    Print_color(Product_info::red);  // 错误: 无法转换 Product_info 为 int。

##### 强制实施

【简单】 对所有非 `class enum` 定义进行警告。

### <a name="Renum-oper"></a>Enum.4: 针对安全和简单的用法来为枚举定义操作

##### 理由

便于使用并避免犯错。

##### 示例

    enum Day { mon, tue, wed, thu, fri, sat, sun };

    Day& operator++(Day& d)
    {
        return d = (d == Day::sun) ? Day::mon : static_cast<Day>(static_cast<int>(d)+1);
    }

    Day today = Day::sat;
    Day tomorrow = ++today;

这里使用 `static_cast` 有点不好，但

    Day& operator++(Day& d)
    {
        return d = (d == Day::sun) ? Day::mon : Day(++d);    // 错误
    }

是无限递归，而且不用强制转换而使用一个针对所有情况的 `switch` 太冗长了。

##### 强制实施

对重复出现的强制转换回枚举的表达式进行标记。

### <a name="Renum-caps"></a>Enum.5: 请勿为枚举符采用 `ALL_CAPS` 命名方式

##### 理由

避免和宏之间发生冲突

##### 示例，不好

     // webcolors.h （第三方头文件）
    #define RED   0xFF0000
    #define GREEN 0x00FF00
    #define BLUE  0x0000FF

    // productinfo.h
    // 以下则基于颜色定义了产品的子类型

    enum class Product_info { RED, PURPLE, BLUE };   // 语法错误

##### 强制实施

标记 ALL_CAPS 风格的枚举符。

### <a name="Renum-unnamed"></a>Enum.6: 避免使用无名枚举

##### 理由

如果无法对枚举命名的话，它的值之间就是没有关联的。

##### 示例，不好

    enum { red = 0xFF0000, scale = 4, is_signed = 1 };

这种代码在出现指定整数常量的其他方便方式之前并不少见。

##### 替代方案

代之以使用 `constexpr` 值。例如：

    constexpr int red = 0xFF0000;
    constexpr short scale = 4;
    constexpr bool is_signed = true;

##### 强制实施

对无名枚举进行标记。

### <a name="Renum-underlying"></a>Enum.7: 仅在必要时才为枚举指定其底层类型

##### 理由

缺省情况的读写都是最简单的。
`int` 是缺省的整数类型。
`int` 是和 C 的 `enum` 相兼容的。

##### 示例

    enum class Direction : char { n, s, e, w,
                                  ne, nw, se, sw };  // 底层类型可以节省空间

    enum class Web_color : int32_t { red   = 0xFF0000,
                                     green = 0x00FF00,
                                     blue  = 0x0000FF };  // 底层类型是多余的

##### 注解

对`enum`或`enum class`前向声明时有必要指定底层类型：

    enum Flags : char;

    void f(Flags);

    // ....

    enum Flags : char { /* ... */ };

或者用以确保该类型的值具有指定的位精度：

    enum Bitboard : uint64_t { /* ... */ };

##### 强制实施

????

### <a name="Renum-value"></a>Enum.8: 仅在必要时才指定枚举符的值

##### 理由

这是最简单的。
避免了枚举符值发生重复。
缺省情况会提供一组连续的值，并有利于 `switch` 语句的实现。

##### 示例

    enum class Col1 { red, yellow, blue };
    enum class Col2 { red = 1, yellow = 2, blue = 2 }; // 打错字
    enum class Month { jan = 1, feb, mar, apr, may, jun,
                       jul, august, sep, oct, nov, dec }; // 传统是从 1 开始
    enum class Base_flag { dec = 1, oct = dec << 1, hex = dec << 2 }; // 位的集合

为了和传统的值相匹配（比如 `Month`），以及当连续的值不合要求
（比如像 `Base_flag` 一样分配不同的位），是需要指定值的。

##### 强制实施

- 标记重复的枚举值
- 对明确指定的全部连续的枚举符的值进行标记。
