# 函数

函数是执行某种操作的代码块。函数可以选择性地定义使调用方可以将实参传递到函数中的输入形参。函数可以选择性地返回值作为输出。函数可用于在单个可重用块中封装常用操作（理想情况是使用可清晰地描述函数行为的名称）。以下函数从调用方接受两个整数并返回其总和；a 和 b 是 int 类型的参数。

```cpp
int sum(int a, int b)
{
    return a + b;
}
```

可以从程序中任意数量的位置调用函数。传递给函数的值是自变量，其类型必须与函数定义中的参数类型兼容。

```cpp
int main()
{
    int i = sum(10, 32);
    int j = sum(i, 66);
    cout << "The value of j is" << j << endl; // 108
}
```

函数长度没有实际限制，但良好的设计适用于执行单个明确定义的任务的函数。复杂算法应尽可能分解成易于理解的更简单函数。在类范围中定义的函数称为成员函数。在 C++ 中（与其他语言不同），函数还可以在命名空间范围中定义（包括隐式全局命名空间）。此类函数称为 自由函数 或 非成员函数;它们在标准库中广泛使用。函数可能会重载，这意味着如果函数的不同版本在形参的数量和/或类型上有所不同，它们可能会共享相同的名称。

# 函数定义

函数定义由声明和函数体组成，括在大括号中，其中包含变量声明、语句和表达式。以下示例显示了完整的函数定义：

```cpp
int foo(int i, std::string s)
    {
       int value {i};
       MyClass mc;
       if(strcmp(s, "default") != 0)
       {
            value = mc.do_something(i);
       }
       return value;
    }
```

函数体内声明的变量称为局部变量。它们会在函数退出时超出范围；因此，函数应永远不返回对局部变量的引用！

```cpp
    MyClass& boom(int i, std::string s)
    {
       int value {i};
       MyClass mc;
       mc.Initialize(i,s);
       return mc;
    }
```

## 函数声明的各个部分

最小函数 声明 由返回类型、函数名称和参数列表 (可能为空）以及向编译器提供更多指令的可选关键字组成。以下示例是函数声明：

```cpp
int sum(int a, int b);
```

函数定义包含声明以及函数体（这是大括号之间的所有代码）：

```cpp
int sum(int a, int b)
{
    return a + b;
}
```

后接分号的函数声明可以出现在程序中的多个位置处。它必须在每个翻译单元中对该函数的任何调用之前出现。根据单个定义规则 (ODR)，函数定义必须仅在程序中出现一次。

函数声明的必需部分有：

1. 返回类型，指定函数将返回的值的类型，如果不返回任何值，则为 **`void`**。在 C++11 中，**`auto`** 是有效返回类型，可指示编译器从返回语句推断类型。在 C++14 中，还允许使用 `decltype(auto)`。有关详细信息，请参阅下面的“返回类型中的类型推导”。
2. 函数名称，必须以字母或下划线开头，不能包含空格。一般情况下，标准库函数名称中的前导下划线表示私有成员函数或不适合代码使用的非成员函数。
3. 参数列表（一组用大括号限定、逗号分隔的零个或多个参数），指定类型以及可以用于在函数体内访问值的可选局部变量名。

函数声明的可选部分有：

1. **`constexpr`**，指示函数的返回值是常量值，可以在编译时进行计算。

   ```cpp
   constexpr float exp(float x, int n)
   {
       return n == 0 ? 1 :
           n % 2 == 0 ? exp(x * x, n / 2) :
           exp(x * x, (n - 1) / 2) * x;
   };
   ```

2. 其链接规范，**`extern`** 或 **`static`**。

   ```cpp
   //Declare printf with C linkage.
   extern "C" int printf( const char *fmt, ... );
   ```

3. **`inline`**，指示编译器将对函数的每个调用替换为函数代码本身。在某个函数快速执行并且在性能关键代码段中重复调用的情况下，内联可以帮助提高性能。

   ```cpp
   inline double Account::GetBalance()
   {
       return balance;
   }
   ```

4. **`noexcept`** 表达式，指定函数是否可以引发异常。在以下示例中，如果 `is_pod` 表达式的计算结果 **`true`**为 ，函数不会引发异常。

   ```cpp
   #include <type_traits>

   template <typename T>
   T copy_object(T& obj) noexcept(std::is_pod<T>) {...}
   ```

5. （仅限成员函数）cv 限定符，指定函数是 **`const`** 还是 **`volatile`**。

6. （仅限成员函数）**`virtual`**、**`override`** 或 **`final`**。**`virtual`** 指定可以在派生类中重写函数。**`override`** 表示派生类中的函数在重写虚函数。**`final`** 表示不能在任何进一步的派生类中重写函数。

7. (成员函数仅) **`static`** 应用于成员函数意味着该函数不与类的任何对象实例相关联。

8. （仅限非静态成员函数）ref 限定符，向编译器指定隐式对象参数 (`*this`) 是右值引用与左值引用时要选择的函数重载。

## const 和 constexpr 函数

可以将成员函数声明为 const ，以指定不允许该函数更改类中任何数据成员的值。通过将成员函数声明为 const，可以帮助编译器强制执行 const-correctness。如果有人错误地尝试使用声明为 const 的函数来修改对象，则会引发编译器错误。

如果函数生成的值可在编译时确定，则将函数声明为 constexpr。constexpr 函数通常比常规函数执行速度更快。

## 函数局部变量

在函数主体内声明的变量称为局部变量。非静态局部变量仅在函数体内可见，如果它们在堆栈上声明，则当函数退出时会超出范围。构造局部变量并通过值返回它时，编译器通常可以执行所谓的返回值优化以避免不必要的复制操作。如果通过引用返回局部变量，则编译器会发出警告，因为调用方为使用该引用而进行的任何尝试会在局部变量已销毁之后进行。

在 C++ 中，局部变量可以声明为静态。变量仅在函数体中可见，但是对于函数的所有实例，存在变量的单个副本。局部静态对象将在 atexit 指定的终止期间销毁。如果由于程序的控制流绕过了其声明而未构造静态对象，则不会尝试销毁该对象。

# 函数模板

函数模板类似于类模板；它基于模板自变量生成具体功能。在许多情况下，模板能够推断类型参数，因此无需显式指定它们。

```cpp
template<typename Lhs, typename Rhs>
auto Add2(const Lhs& lhs, const Rhs& rhs)
{
    return lhs + rhs;
}

auto a = Add2(3.13, 2.895); // a is a double
auto b = Add2(string{ "Hello" }, string{ " World" }); // b is a std::string
```

# 函数形参和实参

函数具有零种或多种类型的逗号分隔参数列表，其中每个参数都具有可以用于在函数体内访问它的名称。函数模板可以指定更多类型或值参数。调用方传递实参（其类型与形参列表兼容的具体值）。默认情况下，参数通过值传递给函数，这意味着函数会收到所传递的对象的副本。对于大型对象，创建副本的成本可能很高，而且并非总是必要的。若要使自变量通过引用（特别是左值引用）进行传递，请向参数添加引用限定符：

```cpp
void DoSomething(std::string& input){...}
```

当函数修改通过引用传递的参数时，它会修改原始对象，而不是本地副本。若要防止函数修改这类实参，请将形参限定为 const&：

```cpp
void DoSomething(const std::string& input){...}
```

C++ 11：若要显式处理通过右值引用或通过左值引用传递的自变量，请对参数使用双与号以指示通用引用：

```cpp
void DoSomething(const std::string&& input){...}
```

只要关键字 void 是自变量声明列表中的第一个也是唯一一个成员，那么在参数声明列表中使用单个关键字 void 声明的函数就没有自变量。列表中的其他地方的 void 类型的自变量产生错误。例如：

```cpp
// OK same as GetTickCount()
long GetTickCount( void );
```

虽然除非 void 如此处所述指定参数是非法的，但从类型 void (派生的类型（如指向 void) 的 void 指针和数组）可以出现在参数声明列表的任何位置。

## 默认自变量

函数签名中的最后一个或几个参数可能会分配有默认自变量，这意味着调用方可能会在调用函数时省略自变量（除非要指定某个其他值）。

```cpp
int DoSomething(int num,
    string str,
    Allocator& alloc = defaultAllocator)
{ ... }

// OK both parameters are at end
int DoSomethingElse(int num,
    string str = string{ "Working" },
    Allocator& alloc = defaultAllocator)
{ ... }

// C2548: 'DoMore': missing default parameter for parameter 2
int DoMore(int num = 5, // Not a trailing parameter!
    string str,
    Allocator& = defaultAllocator)
{...}
```

# 函数返回类型

函数可能不会返回另一个函数或内置数组；但是，它可以返回指向这些类型的指针，或生成函数对象的 lambda。除了这些情况，函数可以返回处于范围内的任何类型的值，或者它可以返回任何值（在这种情况下返回类型是 void）。

## 结尾返回类型

“普通”返回类型位于函数签名左侧。尾随返回类型位于签名的最右侧，前面是 -> 运算符。当返回值的类型取决于模板参数时，结尾返回类型在函数模板中尤其有用。

```cpp
template<typename Lhs, typename Rhs>
auto Add(const Lhs& lhs, const Rhs& rhs) -> decltype(lhs + rhs)
{
    return lhs + rhs;
}
```

当 与尾随返回类型结合使用时 auto ，它仅充当 decltype 表达式生成的占位符，并且本身不执行类型推导。

## 返回类型中的类型推导 (C++14)

在 C++14 中，可以使用 auto 指示编译器从函数体推断返回类型，而不必提供结尾返回类型。请注意，auto 始终推导为按值返回。使用 auto&& 可指示编译器推导引用。在此示例中，auto 会推导为 lhs 和 rhs 之和的非常量值副本。

```cpp
template<typename Lhs, typename Rhs>
auto Add2(const Lhs& lhs, const Rhs& rhs)
{
    return lhs + rhs; //returns a non-const object by value
}
```

请注意， auto 不会保留它所推断的类型的常量。对于返回值需要保留其自变量的常量性或引用性的转发函数，可以使用 decltype(auto) 关键字，该关键字使用 decltype 类型推断规则并保留所有类型信息。decltype(auto) 可以用作左侧的普通返回值，或结尾返回值。下面的示例演示的 decltype(auto) 用于采用在模板实例化之前未知的返回类型实现函数自变量的完美转发。

```cpp
template<typename F, typename Tuple = tuple<T...>, int... I>
decltype(auto) apply_(F&& f, Tuple&& args, index_sequence<I...>)
{
    return std::forward<F>(f)(std::get<I>(std::forward<Tuple>(args))...);
}

template<typename F, typename Tuple = tuple<T...>,
    typename Indices = make_index_sequence<tuple_size<Tuple>::value >>
   decltype( auto)
    apply(F&& f, Tuple&& args)
{
    return apply_(std::forward<F>(f), std::forward<Tuple>(args), Indices());
}
```

## 从函数返回多个值

可通过多种方式从函数返回多个值：

1. 将值封装在命名类或结构对象中。要求类或结构定义对调用方可见：

   ```cpp
   #include <string>
   #include <iostream>

   using namespace std;

   struct S
   {
       string name;
       int num;
   };

   S g()
   {
       string t{ "hello" };
       int u{ 42 };
       return { t, u };
   }

   int main()
   {
       S s = g();
       cout << s.name << " " << s.num << endl;
       return 0;
   }
   ```

2. 返回 std::tuple 或 std::pair 对象：

   ```cpp
   #include <tuple>
   #include <string>
   #include <iostream>

   using namespace std;

   tuple<int, string, double> f()
   {
       int i{ 108 };
       string s{ "Some text" };
       double d{ .01 };
       return { i,s,d };
   }

   int main()
   {
       auto t = f();
       cout << get<0>(t) << " " << get<1>(t) << " " << get<2>(t) << endl;

       // --or--

       int myval;
       string myname;
       double mydecimal;
       tie(myval, myname, mydecimal) = f();
       cout << myval << " " << myname << " " << mydecimal << endl;

       return 0;
   }
   ```

3. **Visual Studio 2017 版本 15.3 及更高版本**（在 [`/std:c++17`](https://learn.microsoft.com/zh-cn/cpp/build/reference/std-specify-language-standard-version?view=msvc-170) 模式及更高版本中可用）：使用结构化绑定。结构化绑定的优点是，存储返回值的变量在声明返回值的同时进行初始化，在某些情况下，这可以显著提高效率。在语句 `auto[x, y, z] = f();` 中，括号引入并初始化了整个函数块范围内的名称。

   ```cpp
   #include <tuple>
   #include <string>
   #include <iostream>

   using namespace std;

   tuple<int, string, double> f()
   {
       int i{ 108 };
       string s{ "Some text" };
       double d{ .01 };
       return { i,s,d };
   }
   struct S
   {
       string name;
       int num;
   };

   S g()
   {
       string t{ "hello" };
       int u{ 42 };
       return { t, u };
   }

   int main()
   {
       auto[x, y, z] = f(); // init from tuple
       cout << x << " " << y << " " << z << endl;

       auto[a, b] = g(); // init from POD struct
       cout << a << " " << b << endl;
       return 0;
   }
   ```

4. 除了使用返回值本身之外，还可以通过定义任意数量的参数来“返回”值以使用“按引用传递”，以便函数可以修改或初始化调用方提供的对象的值。有关详细信息，请参阅 [引用类型函数自变量](https://learn.microsoft.com/zh-cn/cpp/cpp/reference-type-function-arguments?view=msvc-170)。

# 函数指针

C++ 通过与 C 语言相同的方式支持函数指针。但是更加类型安全的替代方法通常是使用函数对象。

如果声明返回函数指针类型的函数，则建议使用 **`typedef`** 声明函数指针类型的别名。例如

```cpp
typedef int (*fp)(int);
fp myFunction(char* s); // function returning function pointer
```

如果未完成此操作，则可以通过函数指针的声明符语法推断出函数声明的正确语法，方法是将上述示例中的标识符 (`fp` 替换为函数名称和参数列表) ，如下所示：

```cpp
int (*myFunction(char* s))(int);
```

前面的声明等效于之前使用的 **`typedef`** 声明。
