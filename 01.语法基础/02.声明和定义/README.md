# 声明与定义

C++ 程序由各种实体组成，例如变量、函数、类型和命名空间。必须先声明其中的每个实体才能使用它们。声明指定实体的唯一名称，以及有关其类型和其他特征的信息。在 C++ 中，声明名称的位置就是它对编译器可见的位置。无法引用稍后在编译单元中某个位置声明的函数或类。变量应尽可能在靠近其使用位置之前的位置声明。

```cpp
#include <string>

int f(int i); // forward declaration

int main()
{
    const double pi = 3.14; //OK
    int i = f(2); //OK. f is forward-declared
    C obj; // error! C not yet declared.
    std::string str; // OK std::string is declared in <string> header
    j = 0; // error! No type specified.
    auto k = 0; // OK. type inferred as int by compiler.
}

int f(int i)
{
    return i + 42;
}

namespace N {
   class C{/*...*/};
}
```

- 在第 5 行，声明了 `main` 函数。 在第 7 行，声明并初始化了名为 `pi` 的 **`const`** 变量。 在第 8 行，使用函数 `f` 生成的值声明并初始化了整数 `i`。 由于第 3 行中的前向声明，名称 `f` 将对编译器可见。

- 在第 9 行，声明了类型为 `C` 的名为 `obj` 的变量。 但是，此声明会引发错误，因为 `C` 只会在程序中的后期阶段声明，而不会前向声明。 若要修复该错误，可将 `C` 的整个定义移到 `main` 之前，或者为其添加前向声明。 此行为不同于 C# 等其他语言。 在这些语言中，函数和类可以在源文件中的声明位置之前使用。

- 在第 10 行，声明了类型为 `std::string` 的名为 `str` 的变量。 名称 `std::string` 可见，因为它是在 `string` 头文件中引入的，该文件已合并到第 1 行的源文件中。 `std` 是声明 `string` 类的命名空间。

- 在第 11 行，由于名称 `j` 尚未声明，因此会引发错误。 与 JavaScript 等其他语言不同，声明必须提供类型。 在第 12 行，使用了 **`auto`** 关键字，告知编译器根据用于初始化 `k` 的值来推理其类型。 在本例中，编译器选择 **`int`** 作为类型。

# 声明范围

声明引入的名称在出现声明的作用域内有效。 在以上示例中，在 main 函数内部声明的变量是局部变量。 可以在 main 外部的全局作用域内声明名为 i 的另一个变量，它将是一个单独的实体。 但是，这种名称重复可能导致编程器混淆和错误，应该避免。 在第 21 行，在命名空间 N 的作用域内声明了类 C。 使用命名空间有助于避免名称冲突。 大多数 C++ 标准库名称都在 std 命名空间中声明。

## 定义

必须定义并声明某些实体，包括函数、类、枚举和常量变量。 稍后在程序中使用实体时，定义将为编译器提供生成计算机代码所需的所有信息。 在以上示例中，第 3 行包含函数 f 的声明，但该函数的定义在第 15 到 18 行中提供。 在第 21 行，声明和定义了类 C（不过，定义该类没有任何意义）。 必须在声明常量变量的同一语句中定义该变量，换言之，为其赋值。 内置类型（例如 int）的声明将自动成为定义，因为编译器知道要为其分配多少空间。

以下示例演示的声明也是定义：

```cpp
// Declare and define int variables i and j.
int i;
int j = 10;

// Declare enumeration suits.
enum suits { Spades = 1, Clubs, Hearts, Diamonds };

// Declare class CheckBox.
class CheckBox : public Control
{
public:
    Boolean IsChecked();
    virtual int     ChangeState() = 0;
};
```

下面这些声明不是定义：

```cpp
extern int i;
char *strchr( const char *Str, const char Target );
```
