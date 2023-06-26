# C++ 数组

数组是相同类型的对象序列，它们占据一块连续的内存区。 传统的 C 样式数组是许多 bug 的根源，但至今仍很常用，尤其是在较旧的代码库中。 在现代 C++ 中，我们强烈建议使用 [`std::vector`](https://learn.microsoft.com/zh-cn/cpp/standard-library/vector-class?view=msvc-170) 或 [`std::array`](https://learn.microsoft.com/zh-cn/cpp/standard-library/array-class-stl?view=msvc-170) 而不是本部分所述的 C 样式数组。 这两种标准库类型都将其元素存储为连续的内存块。 但是，它们提供更高的类型安全性，并支持保证指向序列中有效位置的迭代器。 有关详细信息，请参阅[容器](https://learn.microsoft.com/zh-cn/cpp/standard-library/stl-containers?view=msvc-170)。

## 堆栈声明

在 C++ 数组声明中，数组大小在变量名称之后指定，而不是像在其他某些语言中那样的在类型名称之后指定。 以下示例声明了要在堆栈上分配的 1000 个双精度值的数组。 元素数量必须以整数文本或常量表达式的形式提供。 这是因为，编译器必须知道要分配多少堆栈空间；它不能使用在运行时计算的值。 为数组中的每个元素分配默认值 0。 如果你不指定默认值，则每个元素最初包含恰好位于该内存位置的任意随机值。

```cpp
    constexpr size_t size = 1000;

    // Declare an array of doubles to be allocated on the stack
    double numbers[size] {0};

    // Assign a new value to the first element
    numbers[0] = 1;

    // Assign a value to each subsequent element
    // (numbers[1] is the second element in the array.)
    for (size_t i = 1; i < size; i++)
    {
        numbers[i] = numbers[i-1] * 1.1;
    }

    // Access each element
    for (size_t i = 0; i < size; i++)
    {
        std::cout << numbers[i] << " ";
    }
```

数组中的第一个元素是第 0 个元素。 最后一个元素是 (n-1) 元素，其中 n 是数组可以包含的元素数量。 声明中的元素数量必须是整数类型，且必须大于 0。 你需要负责确保程序永远不会将大于 `(size - 1)` 的值传递给下标运算符。

仅当数组是 **`struct`** 或 **`union`** 中的最后一个字段并启用了 Microsoft 扩展（未设置 **`/Za`** 或 **`/permissive-`**）时，零大小的数组才是合法的。

分配和访问基于堆栈的数组的速度比基于堆的数组更快。 但是，堆栈空间是有限的。 数组元素的数量不能太大，以免占用过多的堆栈内存。 多大的数量算作“太大”取决于程序。 可以使用分析工具来确定数组是否太大。

## 堆声明

你可能需要一个太大的、以致无法在堆栈上分配的数组，或者需要一个其大小在编译时未知的数组。 可以使用 [`new[\]`](https://learn.microsoft.com/zh-cn/cpp/cpp/new-operator-cpp?view=msvc-170) 表达式在堆上分配此数组。 运算符返回指向第一个元素的指针。 下标运算符处理指针变量的方式与处理基于堆栈的数组的方式相同。 还可以使用[指针算术](https://learn.microsoft.com/zh-cn/cpp/c-language/pointer-arithmetic?view=msvc-170)将指针移到数组中的任意元素。 你需要负责确保：

- 始终保留原始指针地址的副本，以便可以在不再需要数组时删除内存。
- 不会将指针地址递增或递减至超过数组边界。

以下示例演示在运行时如何在堆上定义一个数组。 其中演示了如何使用下标运算符和指针算术来访问数组元素：

```cpp
void do_something(size_t size)
{
    // Declare an array of doubles to be allocated on the heap
    double* numbers = new double[size]{ 0 };

    // Assign a new value to the first element
    numbers[0] = 1;

    // Assign a value to each subsequent element
    // (numbers[1] is the second element in the array.)
    for (size_t i = 1; i < size; i++)
    {
        numbers[i] = numbers[i - 1] * 1.1;
    }

    // Access each element with subscript operator
    for (size_t i = 0; i < size; i++)
    {
        std::cout << numbers[i] << " ";
    }

    // Access each element with pointer arithmetic
    // Use a copy of the pointer for iterating
    double* p = numbers;

    for (size_t i = 0; i < size; i++)
    {
        // Dereference the pointer, then increment it
        std::cout << *p++ << " ";
    }

    // Alternate method:
    // Reset p to numbers[0]:
    p = numbers;

    // Use address of pointer to compute bounds.
    // The compiler computes size as the number
    // of elements * (bytes per element).
    while (p < (numbers + size))
    {
        // Dereference the pointer, then increment it
        std::cout << *p++ << " ";
    }

    delete[] numbers; // don't forget to do this!

}
int main()
{
    do_something(108);
}
```

## 初始化数组

可以在循环中、以每次一个元素的方式或者在单个语句中初始化数组。 以下两个数组的内容是相同的：

```cpp
    int a[10];
    for (int i = 0; i < 10; ++i)
    {
        a[i] = i + 1;
    }

    int b[10]{ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
```

## 将数组传递给函数

将数组传递给函数时，该数组将作为指向第一个元素的指针传递，无论它是基于堆栈的数组还是基于堆的数组。 指针不包含其他大小或类型信息。 此行为称为指针衰减。 将数组传递给函数时，始终必须在单独的参数中指定元素数量。 此行为还意味着将数组传递给函数时不会复制数组元素。 若要防止函数修改元素，请将参数指定为指向 **`const`** 元素的指针。

以下示例演示了一个接受数组和长度的函数。 指针指向原始数组而不是副本。 由于参数不是 **`const`**，因此该函数可以修改数组元素。

```cpp
void process(double *p, const size_t len)
{
    std::cout << "process:\n";
    for (size_t i = 0; i < len; ++i)
    {
        // do something with p[i]
    }
}
```

将数组参数 `p` 声明并定义为 **`const`**，使其在函数块中为只读：

```cpp
void process(const double *p, const size_t len);
```

也可以用这些方式声明相同的函数，而无需改变行为。 数组仍作为指向第一个元素的指针传递：

```cpp
// Unsized array
void process(const double p[], const size_t len);

// Fixed-size array. Length must still be specified explicitly.
void process(const double p[1000], const size_t len);
```

## 多维数组

从其他数组构造的数组是多维数组。 通过按顺序放置多个括起来的常数表达式来指定这些多维数组。 例如，考虑此声明：

```cpp
int i2[5][7];
```

它指定类型为 **`int`** 的数组，从概念上以五行七列的二维矩阵排列，如下图所示：

![多维数组的概念布局。](https://learn.microsoft.com/zh-cn/cpp/cpp/media/vc38rc1.gif?view=msvc-170)

图像是一个网格，7 个单元格宽，5 个单元格高。 每个单元格都包含单元格的索引。 第一个单元格索引标记为 0，0。 该行中的下一个单元格为 0，1，依此依此表示该行中最后一个单元格为 0，6。 下一行以索引 1，0 开头。 之后的单元格的索引为 1，1。 该行中的最后一个单元格为 1，6。 此模式将重复，直到最后一行（以索引 4，0 开头）。 最后一行中的最后一个单元格的索引为 4，6。 ：：：image-end

可以声明具有初始化表达式列表的多维数组（如[初始化表达式](https://learn.microsoft.com/zh-cn/cpp/cpp/initializers?view=msvc-170)中所述）。 在这些声明中，可以省略指定第一维的边界的常数表达式。 例如：

```cpp
// arrays2.cpp
// compile with: /c
const int cMarkets = 4;
// Declare a float that represents the transportation costs.
double TransportCosts[][cMarkets] = {
   { 32.19, 47.29, 31.99, 19.11 },
   { 11.29, 22.49, 33.47, 17.29 },
   { 41.97, 22.09,  9.76, 22.55 }
};
```

前面的声明定义四列三行的数组。 行表示工厂，列表示从工厂装运到的市场。 值是从工厂运输到市场的成本。 忽略数组的第一个维度，但编译器会通过检查该初始值设定项来填充它。

对 n 维数组类型使用间接寻址运算符 (\*) 将生成 n-1 维数组。 如果 n 为 1，则将生成标量（或数组元素）。

C++ 数组按行优先顺序存储。 行优先顺序意味着最后一个下标变化最快。

## 示例

还可以在函数声明中省略多维数组第一个维的边界规范，如下所示：

```cpp
// multidimensional_arrays.cpp
// compile with: /EHsc
// arguments: 3
#include <limits>   // Includes DBL_MAX
#include <iostream>

const int cMkts = 4, cFacts = 2;

// Declare a float that represents the transportation costs
double TransportCosts[][cMkts] = {
   { 32.19, 47.29, 31.99, 19.11 },
   { 11.29, 22.49, 33.47, 17.29 },
   { 41.97, 22.09,  9.76, 22.55 }
};

// Calculate size of unspecified dimension
const int cFactories = sizeof TransportCosts /
                  sizeof( double[cMkts] );

double FindMinToMkt( int Mkt, double myTransportCosts[][cMkts], int mycFacts);

using namespace std;

int main( int argc, char *argv[] ) {
   double MinCost;

   if (argv[1] == 0) {
      cout << "You must specify the number of markets." << endl;
      exit(0);
   }
   MinCost = FindMinToMkt( *argv[1] - '0', TransportCosts, cFacts);
   cout << "The minimum cost to Market " << argv[1] << " is: "
       << MinCost << "\n";
}

double FindMinToMkt(int Mkt, double myTransportCosts[][cMkts], int mycFacts) {
   double MinCost = DBL_MAX;

   for( size_t i = 0; i < cFacts; ++i )
      MinCost = (MinCost < TransportCosts[i][Mkt]) ?
         MinCost : TransportCosts[i][Mkt];

   return MinCost;
}
```

Output 复制

```output
The minimum cost to Market 3 is: 17.29
```

编写函数 `FindMinToMkt`，以便添加不需要更改任何代码而仅需重新编译的新工厂。

## 初始化数组

具有类构造函数的对象数组由构造函数初始化。 如果初始化表达式列表中的项少于数组中的元素，则默认的构造函数将用于剩余元素。 如果没有为类定义默认构造函数，初始化表达式列表必须完整，即数组中的每个元素都必须有一个初始化表达式。

考虑定义了两个构造函数的`Point` 类：

```cpp
// initializing_arrays1.cpp
class Point
{
public:
   Point()   // Default constructor.
   {
   }
   Point( int, int )   // Construct from two ints
   {
   }
};

// An array of Point objects can be declared as follows:
Point aPoint[3] = {
   Point( 3, 3 )     // Use int, int constructor.
};

int main()
{
}
```

`aPoint` 的第一个元素是使用构造函数 `Point( int, int )` 构造的；剩余的两个元素是使用默认构造函数构造的。

静态成员数组（是否为 **`const`**）可在其定义中进行初始化（类声明的外部）。 例如：

```cpp
// initializing_arrays2.cpp
class WindowColors
{
public:
    static const char *rgszWindowPartList[7];
};

const char *WindowColors::rgszWindowPartList[7] = {
    "Active Title Bar", "Inactive Title Bar", "Title Bar Text",
    "Menu Bar", "Menu Bar Text", "Window Background", "Frame"   };
int main()
{
}
```

## 访问数组元素

您可以使用数组下标运算符 (`[ ]`) 访问数组的各个元素。 如果使用不带下标的一维数组的名称，它将评估为指向数组第一个元素的指针。

```cpp
// using_arrays.cpp
int main() {
   char chArray[10];
   char *pch = chArray;   // Evaluates to a pointer to the first element.
   char   ch = chArray[0];   // Evaluates to the value of the first element.
   ch = chArray[3];   // Evaluates to the value of the fourth element.
}
```

使用多维数组时，可以在表达式中使用各种组合。

```cpp
// using_arrays_2.cpp
// compile with: /EHsc /W1
#include <iostream>
using namespace std;
int main() {
   double multi[4][4][3];   // Declare the array.
   double (*p2multi)[3];
   double (*p1multi);

   cout << multi[3][2][2] << "\n";   // C4700 Use three subscripts.
   p2multi = multi[3];               // Make p2multi point to
                                     // fourth "plane" of multi.
   p1multi = multi[3][2];            // Make p1multi point to
                                     // fourth plane, third row
                                     // of multi.
}
```

在前面的代码中，`multi` 是 **`double`** 类型的三维数组。 `p2multi` 指针指向大小为三的 **`double`** 类型的数组。 在此示例中，该数组与一个、两个和三个下标一起使用。 尽管更为常见的是指定所有下标（如在 `cout` 语句中），但有时选择数组元素的特定子集会很有用，如 `cout` 后面的语句中所示。

## 重载下标运算符

与其他运算符相似，下标运算符 (`[]`) 也可由用户重新定义。 如果没有重载下标运算符，下标运算符的默认行为是使用以下方法组合数组名称和下标：

```
*((array_name) + (subscript))
```

像涉及指针类型的所有加法中一样，缩放将自动执行以调整类型的大小。 结果值不是来自 `array_name` 的 n 个字节，而是数组的第 n 个元素。 有关此转换的详细信息，请参阅[加法运算符](https://learn.microsoft.com/zh-cn/cpp/cpp/additive-operators-plus-and?view=msvc-170)。

同样，对于多维数组，将使用以下方法获取地址：

```
((array_name) + (subscript1 * max2 * max3 * ... * maxn) + (subscript2 * max3 * ... * maxn) + ... + subscriptn))
```

## 表达式中的数组

当数组类型的标识符出现在 **`sizeof`**、address-of (`&`) 或引用的初始化以外的表达式中时，该标识符将转换为指向第一个数组元素的指针。 例如：

```cpp
char szError1[] = "Error: Disk drive not ready.";
char *psz = szError1;
```

指针 `psz` 指向数组 `szError1` 的第一个元素。 与指针不同，数组不是可修改的左值。 因此，以下赋值是非法的：

```cpp
szError1 = psz;
```
