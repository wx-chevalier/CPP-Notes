# decltype

decltype 类型说明符生成指定表达式的类型。decltype 类型说明符与 auto 关键字一起，主要对编写模板库的开发人员有用。使用 auto 和 decltype 声明函数模板，其返回类型取决于其模板参数的类型。或者，使用 auto 和 decltype 声明一个函数模板，该模板包装对另一个函数的调用，然后返回包装函数的返回类型。

下面的代码示例演示了 **`decltype`** 类型标识符的一些用途。首先，假定已编码以下语句。

```cpp
int var;
const int&& fx();
struct A { double x; }
const A* a = new A();
```

接下来，检查由下表中四个 **`decltype`** 语句返回的类型。

| 语句                | 类型            | 说明                                                                                                                               |
| :------------------ | :-------------- | :--------------------------------------------------------------------------------------------------------------------------------- |
| `decltype(fx());`   | `const int&&`   | 对 **`const int`** 的 [rvalue 引用](https://learn.microsoft.com/zh-cn/cpp/cpp/rvalue-reference-declarator-amp-amp?view=msvc-170)。 |
| `decltype(var);`    | **`int`**       | 变量 `var` 的类型。                                                                                                                |
| `decltype(a->x);`   | **`double`**    | 成员访问的类型。                                                                                                                   |
| `decltype((a->x));` | `const double&` | 内部括号导致语句作为表达式而不是成员访问计算。由于 `a` 声明为 **`const`** 指针，因此类型是对 **`const double`** 的引用。           |

# decltype 和 auto

在 C++14 中，可以使用 decltype(auto) 不带尾随返回类型的函数模板来声明其返回类型取决于其模板参数的类型。在 C++11 中，可以使用 decltype 尾随返回类型的类型说明符以及 auto 关键字 (keyword) 来声明函数模板，该模板的返回类型取决于其模板参数的类型。例如，请考虑以下代码示例，其中函数模板的返回类型取决于模板参数的类型。在代码示例中 UNKNOWN ，占位符指示无法指定返回类型。

```cpp
template<typename T, typename U>
UNKNOWN func(T&& t, U&& u){ return t + u; };
```

类型说明符的 decltype 引入使开发人员能够获取函数模板返回的表达式的类型。请使用替代函数声明语法（稍后会展示）、auto 关键字和 decltype 类型说明符来声明后指定返回类型。后期指定的返回类型是在编译声明时确定的，而不是在编码时确定的。

以下原型阐述一个替代函数声明的语法。const 和 volatile 限定符以及 throw 异常规范是可选的。占 function_body 位符表示指定函数的用途的复合语句。作为最佳编码做法，expression 语句中的 decltype 占位符应与 中的 语句指定的 return 表达式（如果有） function_body 匹配。

```cpp
//C++11
template<typename T, typename U>
auto myFunc(T&& t, U&& u) -> decltype (forward<T>(t) + forward<U>(u))
        { return forward<T>(t) + forward<U>(u); };

//C++14
template<typename T, typename U>
decltype(auto) myFunc(T&& t, U&& u)
        { return forward<T>(t) + forward<U>(u); };
```
