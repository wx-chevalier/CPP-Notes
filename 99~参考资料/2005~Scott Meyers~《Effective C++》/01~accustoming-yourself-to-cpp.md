原书的第一部分，主要是对 C++语言的介绍以及优化 `#define` 的方法等比较基础的知识点。

<!-- more -->

## item1 视 C++为一个语言集合

不同于 C，C++已经不再是一个带有单一语言规则的计算机语言。今天的 C++是一个多重泛型编程语言，它同时支持*面向过程*、_面向对象_、_函数式编程_、_泛型编程_、_元编程_ 。因此，我们需要**将 C+++视为一个由相关语言组成的集合，而不是单一语言**。它包含了四个语言模块：

- **C**：C++是以 C 语言为基础的，C++的语法规则、预处理器、内置数据类型、数组、指针等都是来自于 C，许多 C++的语法就是较高级的 C。
- **Object-Oriented C++**：也就是很多时候提到的*C With Classes* ，其中包含了 C++面向对象的机制：类，以及封装、继承、多态、虚函数等类的支持机制。
- **Template C++**：泛型编程模块。
- **STL**：STL 是一个封装了容器、迭代器、算法以及函数对象的 template 库。

**为什么要区分 C++中的语言模块**
为了提高 C++的效率，使用不同的语言模块需要遵循不同的编程策略，例如：

- 在 C 语言中，一般使用值传递（pass-by-value）；
- 在 OOC 和模板 C++中，使用常量引用（pass-by-reference-const）传递更加高效；
- 对于 STL，因为迭代器是基于指针构造而成，直接使用值传递即可。

## item2 用 const,enum,inline 代替#define

换言之，以**编译器替换预处理器**。预处理宏会在编译器之前被简单的替换成代码，经常会发生意料之外的错误，这种错误往往难以跟踪到。因此，除了一些特殊的指令外，如`#include`，应该尽量避免使用预处理宏。解决之道是对`#define`进行替换。

### 2.1 const 替换#define 定义的常量

- **数值常量**：对于数值类型的常量，可以通过定义一个全局常量来对`#define`进行替换，如`const double A=3.141592654;`替换`#define A 3.141592654`。
- **常量指针**：由于常量定义通常被放在头文件中以便被不同的源码包含，因此需要把指针以及指针指向的数据都定义位`const`，如`const char* const author="Scotter Meyers";`。
- **类的常量**：对于类中的常量成员，为了保证其能被类的对象访问到，又不会生成多个拷贝，需要将其声明为静态常量，即

```
class Player{
private:
    static const int numTurns=5;//静态成员变量，属于类不会在实例化过程中产生拷贝；const成员，不能进行赋值操作
}
```

### 2.2 enum 使用技巧

当我们要在一个类中声明一个常量，这个常量不允许在在声明时进行初始化，而接下来某个语句明确要用到这个变量，比如说静态数组的声明，如：

```c++
class Player{
private:
    static const int numTurns;//一些编译器不允许static成员在声明时赋值，需要在类外进行定义
    int scores[numTurns]; // 编译无法通过，在编译过程中必须要指定数组大小
}
```

面对上面个的问题，我们可以选择使用 enum 来解决，即：

```
class Player{
private:
    enum {NumTurns=5};    // 因为枚举类型的数值可以充当int使用，
    int scores[NumTurns]; // 所以，编译通过。
}
```

### 2.3 inline 函数

对于一些简单又需要反复调用的程序语句，将其封装成函数是非常不划算的，因为调用函数的开销甚至超过了函数内代码运行的开销。`#define`实现类似于函数的宏定义，好处是可以减少简单函数调用造成额外开销，但是其代码非常不雅观，而且会导致一些未知的错误。C++提供了**内联函数**来帮助我们避免简单函数调用带来的不必要开销，也可以避免宏替换带来的不可预料的错误。

**Note：总结**

- 对于单纯的常量，最好用`const`对象或`enum`对象替换`#define`
- 对于形似函数的宏，最好用`inline`函数替换`#define`

## item3 多用 const

const 允许指定一个语义约束，告诉编译器和程序员某个值应该保持不变，而编译器会强制实施这个约束。

### 3.1 const 与指针

- **常量指针**：指向常量的指针，const 在`*`左边
- **指针常量**：指针类型的常量，const 在`*`右边

```
const char* p; // 数据是常量
char const* p; // 数据是常量
char* const p; // 指针是常量
const char* const p; // 指针和数据都是常量
```

### 3.2 const 与 STL 迭代器

STL 迭代器相当于类型 T 的指针，即`T*`。因此，如果想定义一个迭代器指向一个常数，需要使用 const_iterator。

```
std::vector<int> vec;
std::vector<int>::const_iterator const_itr = vec.begin(); // const_itr类似于const T*，指向一个常量
++const_itr;                                              // 迭代器可变
*const_itr = 10;                                          // 错误，*const_itr不可变

const std::vector<int>::iterator itr = vec.begin();       // iter类似于T* const
++itr;                                                    // 错误，指针是一个常量
*itr = 10;                                                // 改变指向的值
```

### 3.3 const 与函数

const 最有效的用法是在函数声明时的应用。在一个函数声明式内，const 可以和函数返回值、参数以及函数自身产生关联。例如，我们可以让函数返回一个常量值，可以降低因用户错误而造成的意外。

```
class Rational{...};
Rational operator*(const Rational& lhs, const Rational& rhs){...};
```

在某处使用乘法操作符时，误把比较操作符`===`写成了赋值操作符`=`，如

```
Rational a,b,c;
if((a*b)=c){...}     // 编译器不会报错，很难追踪错误
```

解决办法是**将操作符定义为返回 const**，这样对其赋值将会是非法操作。

```
const Rational operator*(const Rational& lhs, const Rational& rhs){...};
```

### 3.4 const 与类的成员函数

用 const 关键字修饰类的成员函数主要有两个作用：

- 一是可以直观的告诉用户函数是否会改变成员变量；
- 二是用 const 修饰的对象只能调用 const 修饰的成员函数。

### 3.5 数据常量性和逻辑常量性

C++标准对成员函数常量性的规定是**数据常量性**，即不允许成员变量被修改。C++编译器对此的检测标准是**检查该成员函数中有没有给成员变量进行赋值操作**。

```
class CTextBlock
{
    public:
        char& operator[](std::size_t pos) const
        {
            return pText[pos];
        }
    private:
        char* pText;
}
```

只有指针属于对象，指针所指向的数据不属于对象，const 修饰的`operator[]`中并没有赋值操作符，编译器会通过编译，但是其存在着潜在风险。如：

```
const CTextBlock ctb("Hello"); // 声明一个常量对象
char* pc = &ctb[0];            // 调用operator[]取得一个指针，指向ctb的数据
*pc = "J";                     // 数据被修改为"Jello"
```

数据常量性还存在着另一个局限性，如：

```
class CTextBlock
{
    public:
        std::size_t length() const
        {
            if(!lengthIsValid)
            {
                textLength = std::strlen(pText);  // 有赋值操作，编译会发生错误，但事实上这种改变是允许且必要的
                lengthIsValid = true;
            }
            return textLength;
        }
    private:
        char* pText;
        std::size_t textLength;
        bool lengthIsValid;
}
```

解决办法是**逻辑常量性**，即使用`mutable`关键字来修饰成员变量，允许数据被修改，但是这些修改不反映到类外。

```cpp
class CTextBlock
{
    public:
        std::size_t length() const
        {
            if(!lengthIsValid)
            {
                textLength = std::strlen(pText);  // 编译可以通过
                lengthIsValid = true;
            }
            return textLength;
        }
    private:
        char* pText;
        mutable std::size_t textLength;
        mutable bool lengthIsValid;
}
```

### 3.6 在 const 和 non-const 成员函数中避免重复

在 C+++中，两个函数如果只是常量性不同，可以被重载。但是这样就存在着两个几乎完全重复的函数，虽然可以另外写一个私有函数进行调用，但是还是存在一些重复的代码，如函数调用和 return 等语句。我们真正应该实现的是**一次实现，两次调用**，即其中一个调用另一个，需要用到**常量性转换**。

```
class TextBlock
{
   public:
        const char& operator[](std::size_t pos) const
        {
            // ....
            return pText[ pos ];
        }
        char& operator[](std::size_t pos)
        {
            return
                const_cast<char&>(                         // 将[]的const移除，转换成char&
                    static_cast<const TextBlock&>(*this)   // 为*this加上const，从而可以调用const操作
                        [ position ]
                );
        }
    private:
        char* pText;
}
```

**Note：总结**

- 变量，指针，迭代器以及函数都可以通过 const 的修饰来实现只读的目的；
- 编译器强制使用的是数据常量性，但是编写程序的时候应该采用逻辑常量性，对需要修改的成员变量加上`mutalbe`关键字修饰；
- const 和 non-const 成员函数有着大量重复的实现，可以使用 non-const 函数来调用 const 函数来避免重复。

## item4 确保对象在使用前已被初始化

C++不能保证每个对象在定义时都被自动初始化，最佳的办法就是永远在使用对象之前先将其初始化。

### 4.1 内置数据类型的初始化

C++的内置数据类型继承自 C，不能保证变量在定义式自动初始化。使用未初始化的数据可能会导致程序运行错误，因此需要手动进行初始化。

### 4.2 类的初始化

- 对于用户自定义的类，需要构造函数来完成类的初始化，需要保证构造函数将对象的每一个成员初始化；
- C++规定，对象的成员变量的初始化动作发生在进入构造函数体之前。因此，在构造函数体内对成员变量进行的是赋值操作，其先调用默认构造函数对成员函数进行初始化，然后对它们赋予新值；
- 较优的办法是使用**初始化列表**来对成员变量进行初始化，从而不再调用默认构造函数，直接进行赋值操作；
- C++有着固定的初始化顺序，基类先于派生类被初始化，成员变量是按照其声明的顺序进行初始化的

- 类的初始化：构造函数与初始化列表
- 引用和常量的初始化
- 初始化顺序：继承关系中，先父类后子类；同一个类中，只与成员变量的声明顺序有关
- 非局部静态对象的初始化

### 4.3 non-local static 对象的初始化

在不同的源码文件中，分别包含了至少一个 non-local static 对象，当这些对象发生互动时，他们的初始化顺序是不确定的，直接使用这些对象会给程序的运行带来风险。

**Note：non-local static 对象**
所谓的 static 对象，其生命周期从被构造出来到程序结束，包括全局对象、定义在 namespace 作用域内的对象、在类或者函数内被声明为 static 的对象。其中，在函数内的 static 对象称为 local static 对象，其他的 static 对象我们就称为 nono-local static 对象。
静态对象不是基于堆或者栈的，初始化的静态对象在内存的 Data 段，未初始化的位于 BSS 段。

```cpp
/* demo1.cpp */
class FileSyste
{
public:
    // ...
    std::size_t numDisks() const;
};
extern FileSystem fs;// 在全局范围声明一个对象fs，供其他单元调用

/* demo2.cpp */
class Directory
{
public:
    Directory(param)
    {
        std::size_t disks = fs.numDisks();  // 调用了fs对象
    }
}
```

在上述代码中，如果我们要创建一个`Directory`对象，构造函数就会调用`fs`对象。但是两个对象是在不同的源文件不同的时间建立起来的，无法保证 fs 已经被初始化。解决办法就是：**将 non-local static 对象转换成 local static 对象**，即将每一个 non-local static 对象放到一个函数里面去，这是单例模式的常用的手法。其依据是：在 C++中，函数的 local static 对象会在该函数第一次被调用时进行初始化，因此我们只需要让函数返回一个指向 local static 对象的指针或引用，就可以得到一个在使用时保证被初始化的对象。

```cpp
/* demo1.cpp */
class FileSyste
{
public:
    // ...
    std::size_t numDisks() const;
}
FileSystem& fs()
{
    static FileSystem fs; // 声明了一个局部静态变量
    return fs;
}

/* demo2.cpp */
class Directory
{
public:
    Directory(param)
    {
        std::size_t disks = fs().numDisks();  // 调用了fs对象
    }
}
Directory& dir()
{
    static Directory dir;
    return dir;
}
```

**Note：总结**

- 对于内置的数据类型，要进行手动初始化；
- 构造函数对类进行初始化最好使用*初始化列表* 来替换*在构造函数中使用赋值操作* 。构造函数按照变量声明的顺序进行初始化；
- 对于静态对象，用局部静态对象来替换全局静态对象来保证使用前确定被初始化。
