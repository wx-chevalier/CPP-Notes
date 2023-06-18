# <a name="main"></a>C++ 核心指导方针

2022/10/6

编辑：

- [Bjarne Stroustrup](http://www.stroustrup.com)
- [Herb Sutter](http://herbsutter.com/)

翻译

- 李一楠 (li_yinan AT 163 DOT com)

本文档是处于持续改进之中的在线文档。
本文档作为开源项目，发布版本为 0.8。
复制，使用，修改，以及创建本项目的衍生物，受到一份 MIT 风格的版权授权。
向本项目作出贡献需要同意一份贡献者授权。详情参见附属的 [LICENSE](LICENSE) 文件。
我们将本项目开放给“友好用户”进行使用，复制，修改，以及生产衍生物，并希望能够获得建设性的资源投入。

十分欢迎大家提出意见和改进建议。
随着我们的知识增长，随着语言和可用的程序库的改进，我们计划对这份文档不断进行修改和扩充。
当提出您的意见时，请关注[导言](#S-introduction)部分，其中概述了我们的目标和所采用的一般方法。
贡献者的列表请参见[这里](#SS-ack)。

已知问题：

- 仍未对规则集合的完整性、一致性和可强制实施性加以全面的检查。
- 三问号 (???) 用于标记已知的信息缺失。
- 需要更新参考部分；许多前 C++11 的源代码都过于老旧。
- [To-do: 未分类的规则原型](#S-unclassified) 是一份基本上保持最新状态的 to-do 列表。

您可以[阅读本指南的范围和结构的说明](#S-abstract)，或者直接跳转到：

- [In: 导言](#S-introduction)
- [P: 理念](#S-philosophy)
- [I: 接口](#S-interfaces)
- [F: 函数](#S-functions)
- [C: 类和类层次](#S-class)
- [Enum: 枚举](#S-enum)
- [R: 资源管理](#S-resource)
- [ES: 表达式和语句](#S-expr)
- [Per: 性能](#S-performance)
- [CP: 并发与并行](#S-concurrency)
- [E: 错误处理](#S-errors)
- [Con: 常量和不可变性](#S-const)
- [T: 模板和泛型编程](#S-templates)
- [CPL: C 风格的编程](#S-cpl)
- [SF: 源文件](#S-source)
- [SL: 标准库](#sl-the-standard-library)

配套章节：

- [A: 架构相关理念](#S-A)
- [NR: 伪规则和错误的看法](#S-not)
- [RF: 参考资料](#S-references)
- [PRO: 剖面配置](#S-profile)
- [GSL: 指导方针支持库](#gsl-guidelines-support-library)
- [NL: 命名和代码布局建议](#S-naming)
- [FAQ: 常见问题的解答](#S-faq)
- [附录 A: 程序库](#S-libraries)
- [附录 B: 代码的现代化转换](#S-modernizing)
- [附录 C: 相关讨论](#S-discussion)
- [附录 D: 支持工具](#S-tools)
- [词汇表](#S-glossary)
- [To-do: 未分类的规则原型](#S-unclassified)

您可以查看有关某个具体的语言特性的一些规则：

- 赋值：
  [正规类型](#Rc-regular) --
  [优先采用初始化](#Rc-initialize) --
  [复制](#Rc-copy-semantic) --
  [移动](#Rc-move-semantic) --
  [以及其他操作](#Rc-matched) --
  [缺省操作](#Rc-eqdefault)
- `class`：
  [数据](#Rc-org) --
  [不变式](#Rc-struct) --
  [成员](#Rc-member) --
  [辅助函数](#Rc-helper) --
  [具体类型](#SS-concrete) --
  [构造函数，=，和析构函数](#S-ctor) --
  [类层次](#SS-hier) --
  [运算符](#SS-overload)
- `concept`：
  [规则](#SS-concepts) --
  [泛型编程中](#Rt-raise) --
  [模板实参](#Rt-concepts) --
  [语义](#Rt-low)
- 构造函数：
  [不变式](#Rc-struct) --
  [建立不变式](#Rc-ctor) --
  [`throw`](#Rc-throw) --
  [缺省操作](#Rc-default0) --
  [不需要](#Rc-default) --
  [`explicit`](#Rc-explicit) --
  [委派](#Rc-delegating) --
  [`virtual`](#Rc-ctor-virtual)
- 派生 `class`：
  [何时使用](#Rh-domain) --
  [作为接口](#Rh-abstract) --
  [析构函数](#Rh-dtor) --
  [复制](#Rh-copy) --
  [取值和设值](#Rh-get) --
  [多继承](#Rh-mi-interface) --
  [重载](#Rh-using) --
  [分片](#Rc-copy-virtual) --
  [`dynamic_cast`](#Rh-dynamic_cast)
- 析构函数：
  [以及构造函数](#Rc-matched) --
  [何时需要？](#Rc-dtor) --
  [不可失败](#Rc-dtor-fail)
- 异常：
  [错误](#S-errors) --
  [`throw`](#Re-throw) --
  [仅用于错误](#Re-errors) --
  [`noexcept`](#Re-noexcept) --
  [最少化 `try`](#Re-catch) --
  [无异常如何？](#Re-no-throw-codes)
- `for`：
  [范围式 `for` 和 `for`](#Res-for-range) --
  [`for` 和 `while`](#Res-for-while) --
  [`for`-初始化式](#Res-for-init) --
  [空循环体](#Res-empty) --
  [循环变量](#Res-loop-counter) --
  [循环变量的类型 ???](#Res-???)
- 函数：
  [命名](#Rf-package) --
  [单操作](#Rf-logical) --
  [不能抛出异常](#Rf-noexcept) --
  [实参](#Rf-smart) --
  [实参传递](#Rf-conventional) --
  [多返回值](#Rf-out-multi) --
  [指针](#Rf-return-ptr) --
  [lambda](#Rf-capture-vs-overload)
- `inline`:
  [小型函数](#Rf-inline) --
  [头文件中](#Rs-inline)
- 初始化：
  [总是](#Res-always) --
  [优先采用 `{}`](#Res-list) --
  [lambda](#Res-lambda-init) --
  [类内初始化式](#Rc-in-class-initializer) --
  [类成员](#Rc-initialize) --
  [工厂函数](#Rc-factory)
- lambda 表达式：
  [何时使用](#SS-lambdas)
- 运算符：
  [约定](#Ro-conventional) --
  [避免转换运算符](#Ro-conversion) --
  [与 lambda](#Ro-lambda)
- `public`, `private`, 和 `protected`：
  [信息隐藏](#Rc-private) --
  [一致性](#Rh-public) --
  [`protected`](#Rh-protected)
- `static_assert`：
  [编译时检查](#Rp-compile-time) --
  [和概念](#Rt-check-class)
- `struct`：
  [用于组织数据](#Rc-org) --
  [没有不变式时使用](#Rc-struct) --
  [不能有私有成员](#Rc-class)
- `template`：
  [抽象](#Rt-raise) --
  [容器](#Rt-cont) --
  [概念](#Rt-concepts)
- `unsigned`：
  [和 `signed`](#Res-mix) --
  [位操作](#Res-unsigned)
- `virtual`：
  [接口](#Ri-abstract) --
  [非 `virtual`](#Rc-concrete) --
  [析构函数](#Rc-dtor-virtual) --
  [不能失败](#Rc-dtor-fail)

您可以查看用于表达这些规则的一些设计概念：

- 断言：???
- 错误：???
- 异常：异常保证 (???)
- 故障：???
- 不变式：???
- 泄漏：???
- 程序库：???
- 前条件：???
- 后条件：???
- 资源：???

# <a name="S-abstract"></a>概要

本文档是一组有关如何更好使用 C++ 的指导方针的集合。
本文档的目标是帮助人们更有效地使用现代 C++。
所谓“现代 C++”的含义是指有效使用 ISO C++ 标准（目前是 C++20，但几乎所有的推荐也适用于 C++17，C++14 和 C++11）。
换句话说，如果你从现在开始算起，五年后你的代码看起来是怎么样的？十年呢？

这些指导方针所关注的是一些相对高层次的问题，比如接口，资源管理，内存管理，以及并发等等。
这样的规则会对应用的架构，以及程序库的设计都造成影响。
如果遵循这些规则，代码将会是静态类型安全的，没有资源泄露，并且能够捕捉到比当今的代码通常所能捕捉到的多得多的编程逻辑错误。
还能更快速地运行——你不必牺牲程序的正确性。

我们对于如命名约定和缩进风格一类的低层次的问题不那么关注。
当然，对程序员有帮助的任何话题都是可接受的。

我们最初的规则集合强调的是（各种形式的）安全性以及简单性。
它们也许有些过于严格了。
我们预期将会引入更多的例外情况， 以便使它们更好地适应现实世界的需要。
我们也需要更多的规则。

您可能会发现，有的规则与您的预期相反，甚至是与您的经验相违背。
其实如果我们没建议您在任何方面改变您的编码风格，那其实就是我们的失败！
请您尝试验证或者证伪这些规则吧！
尤其是，我们十分期望让一些规则能够建立在真实的测量数据上，或者是一些更好的例子之上。

您可能会觉得一些规则很显然，甚至没有什么价值。
但请记住，指导方针的目的之一就在于帮助那些经验不足的，或来自其他背景或使用其他语言的人，能够迅速行动起来。

这里的许多规则有意设计成可以由分析工具提供支持的。
违反规则的代码会打上标记，以引用（或者链接）到相关的规则。
您在开始编码前并不需要记住所有这些规则。
一种看待这些指导方针的方式，是一份恰好可以让人类读懂的针对这些工具的规范文件。

这些规则都是为了逐步引入一个代码库而设计的。
我们计划建立这样的工具，并希望其他人也能提供它们。

十分欢迎大家提出意见和改进建议。
随着我们的知识增长，随着语言和可用的程序库的改进，我们计划对这份文档不断进行修改和扩充。

# <a name="S-expr"></a>ES: 表达式和语句

表达式和语句是用以表达动作和计算的最底层也是最直接的方式。局部作用域中的声明也是语句。

有关命名、注释和缩进的规则，参见 [NL: 命名与代码布局](#S-naming)。

一般规则：

- [ES.1: 优先采用标准库而不是其他的库或者“手工自制代码”](#Res-lib)
- [ES.2: 优先采用适当的抽象而不是直接使用语言功能特性](#Res-abstr)
- [ES.3: 避免重复（DRY），避免冗余代码](#Res-DRY)

声明的规则：

- [ES.5: 保持作用域尽量小](#Res-scope)
- [ES.6: 在 for 语句的初始化式和条件中声明名字以限制其作用域](#Res-cond)
- [ES.7: 保持常用的和局部的名字尽量简短，而让非常用的和非局部的名字较长](#Res-name-length)
- [ES.8: 避免使用看起来相似的名字](#Res-name-similar)
- [ES.9: 避免 `ALL_CAPS` 式的名字](#Res-not-CAPS)
- [ES.10: 每条声明中（仅）声明一个名字](#Res-name-one)
- [ES.11: 使用 `auto` 来避免类型名字的多余重复](#Res-auto)
- [ES.12: 不要在嵌套作用域中重用名字](#Res-reuse)
- [ES.20: 坚持为对象进行初始化](#Res-always)
- [ES.21: 不要在确实需要使用变量（或常量）之前就引入它](#Res-introduce)
- [ES.22: 要等到获得了用以初始化变量的值之后才声明变量](#Res-init)
- [ES.23: 优先使用 `{}` 初始化式语法](#Res-list)
- [ES.24: 用 `unique_ptr<T>` 来保存指针](#Res-unique)
- [ES.25: 应当将对象声明为 `const` 或 `constexpr`，除非后面需要修改其值](#Res-const)
- [ES.26: 不要用一个变量来达成两个不相关的目的](#Res-recycle)
- [ES.27: 使用 `std::array` 或 `stack_array` 作为栈上的数组](#Res-stack)
- [ES.28: 为复杂的初始化（尤其是 `const` 变量）使用 lambda](#Res-lambda-init)
- [ES.30: 不要用宏来操纵程序文本](#Res-macros)
- [ES.31: 不要用宏来作为常量或“函数”](#Res-macros2)
- [ES.32: 对所有的宏名采用 `ALL_CAPS` 命名方式](#Res-ALL_CAPS)
- [ES.33: 如果必须使用宏的话，请为之提供唯一的名字](#Res-MACROS)
- [ES.34: 不要定义（C 风格的）变参函数](#Res-ellipses)

表达式的规则：

- [ES.40: 避免复杂的表达式](#Res-complicated)
- [ES.41: 对运算符优先级不保准时应使用括号](#Res-parens)
- [ES.42: 保持单纯直接的指针使用方式](#Res-ptr)
- [ES.43: 避免带有未定义的求值顺序的表达式](#Res-order)
- [ES.44: 不要对函数参数求值顺序有依赖](#Res-order-fct)
- [ES.45: 避免“魔法常量”，采用符号化常量](#Res-magic)
- [ES.46: 避免窄化转换](#Res-narrowing)
- [ES.47: 使用 `nullptr` 而不是 `0` 或 `NULL`](#Res-nullptr)
- [ES.48: 避免强制转换](#Res-casts)
- [ES.49: 当必须使用强制转换时，使用具名的强制转换](#Res-casts-named)
- [ES.50: 不要强制掉 `const`](#Res-casts-const)
- [ES.55: 避免发生对范围检查的需要](#Res-range-checking)
- [ES.56: 仅在确实需要明确移动某个对象到别的作用域时才使用 `std::move()`](#Res-move)
- [ES.60: 避免在资源管理函数之外使用 `new` 和 `delete`](#Res-new)
- [ES.61: 用 `delete[]` 删除数组，用 `delete` 删除非数组对象](#Res-del)
- [ES.62: 不要在不同的数组之间进行指针比较](#Res-arr2)
- [ES.63: 不要产生切片](#Res-slice)
- [ES.64: 使用 `T{e}` 写法来进行构造](#Res-construct)
- [ES.65: 不要解引用无效指针](#Res-deref)

语句的规则：

- [ES.70: 面临选择时，优先采用 `switch` 语句而不是 `if` 语句](#Res-switch-if)
- [ES.71: 面临选择时，优先采用范围式 `for` 语句而不是普通 `for` 语句](#Res-for-range)
- [ES.72: 当存在显然的循环变量时，优先采用 `for` 语句而不是 `while` 语句](#Res-for-while)
- [ES.73: 当没有显然的循环变量时，优先采用 `while` 语句而不是 `for` 语句](#Res-while-for)
- [ES.74: 优先在 `for` 语句的初始化部分中声明循环变量](#Res-for-init)
- [ES.75: 避免使用 `do` 语句](#Res-do)
- [ES.76: 避免 `goto`](#Res-goto)
- [ES.77: 尽量减少循环中使用的 `break` 和 `continue`](#Res-continue)
- [ES.78: 不要依靠 `switch` 语句中的隐含直落行为](#Res-break)
- [ES.79: `default`（仅）用于处理一般情况](#Res-default)
- [ES.84: 不要试图声明没有名字的局部变量](#Res-noname)
- [ES.85: 让空语句显著可见](#Res-empty)
- [ES.86: 避免在原生的 `for` 循环中修改循环控制变量](#Res-loop-counter)
- [ES.87: 请勿在条件上添加多余的 `==` 或 `!=`](#Res-if)

算术规则：

- [ES.100: 不要进行有符号和无符号混合运算](#Res-mix)
- [ES.101: 使用无符号类型进行位操作](#Res-unsigned)
- [ES.102: 使用有符号类型进行算术运算](#Res-signed)
- [ES.103: 避免上溢出](#Res-overflow)
- [ES.104: 避免下溢出](#Res-underflow)
- [ES.105: 避免除整数零](#Res-zero)
- [ES.106: 不要试图用 `unsigned` 来防止负数值](#Res-nonnegative)
- [ES.107: 不要对下标使用 `unsigned`，优先使用 `gsl::index`](#Res-subscripts)

### <a name="Res-lib"></a>ES.1: 优先采用标准库而不是其他的库或者“手工自制代码”

##### 理由

使用程序库的代码要远比直接使用语言功能特性的代码易于编写，更为简短，更倾向于更高的抽象层次，而且程序库代码假定已经经过测试。
ISO C++ 标准库是最广为了解而且经过最好测试的程序库之一。
它是任何 C++ 实现的一部分。

##### 示例

    auto sum = accumulate(begin(a), end(a), 0.0);   // 好

`accumulate` 的范围版本就更好了：

    auto sum = accumulate(v, 0.0); // 更好

但请不要手工编写众所周知的算法：

    int max = v.size();   // 不好：啰嗦，目的不清晰
    double sum = 0.0;
    for (int i = 0; i < max; ++i)
        sum = sum + v[i];

##### 例外

标准库的很大部分都依赖于动态分配（自由存储）。这些部分，尤其是容器但并不包括算法，并不适用于某些硬实时和嵌入式的应用的。在这些情况下，请考虑提供或使用类似的设施，比如说某个标准库风格的采用池分配器的容器实现。

##### 强制实施

并不容易。??? 寻找混乱的循环，嵌套的循环，长函数，函数调用的缺失，缺乏使用内建类型。圈复杂度？

### <a name="Res-abstr"></a>ES.2: 优先采用适当的抽象而不是直接使用语言功能特性

##### 理由

“适当的抽象”（比如程序库或者类），更加贴近应用的概念而不是语言概念，将带来更简洁的代码，而且更容易进行测试。

##### 示例

    vector<string> read1(istream& is)   // 好
    {
        vector<string> res;
        for (string s; is >> s;)
            res.push_back(s);
        return res;
    }

与之近乎等价的更传统且更低层的代码，更长、更混乱、更难编写正确，而且很可能更慢：

    char** read2(istream& is, int maxelem, int maxstring, int* nread)   // 不好：啰嗦而且不完整
    {
        auto res = new char*[maxelem];
        int elemcount = 0;
        while (is && elemcount < maxelem) {
            auto s = new char[maxstring];
            is.read(s, maxstring);
            res[elemcount++] = s;
        }
        *nread = elemcount;
        return res;
    }

一旦添加了溢出检查和错误处理，代码就变得相当混乱了，而且还有个要记得 `delete` 其所返回的指针以及数组所包含的 C 风格字符串的问题。

##### 强制实施

并不容易。??? 寻找混乱的循环，嵌套的循环，长函数，函数调用的缺失，缺乏使用内建类型。圈复杂度？

### <a name="Res-DRY"></a>ES.3: 避免重复（DRY），避免冗余代码

重复或者冗余的代码会干扰编码意图，导致对逻辑的理解变得困难，并使维护变得困难，以及一些其他问题。它经常出现于拷贝粘贴式编程中。

只要合适就使用标准算法，而不是编写自己的实现。

**另请参见**: [SL.1](#Rsl-lib), [ES.11](#Res-auto)

##### 示例

    void func(bool flag)    // 不好，重复代码
    {
        if (flag) {
            x();
            y();
        }
        else {
            x();
            z();
        }
    }

    void func(bool flag)    // 更好，没有重复代码
    {
        x();

        if (flag)
            y();
        else
            z();
    }

##### 强制实施

- 采用静态分析器。它至少会找出一些重复的代码构造。
- 代码评审

## ES.dcl: 声明

声明也是语句。一条声明向一个作用域中引入一个名字，并可能导致对一个具名对象进行构造。

### <a name="Res-scope"></a>ES.5: 保持作用域尽量小

##### 理由

可读性。最小化资源持有率。避免值的意外误用。

**其他形式**: 不要把名字在不必要的大作用域中进行声明。

##### 示例

    void use()
    {
        int i;    // 不好: 循环之后并不需要访问 i
        for (i = 0; i < 20; ++i) { /* ... */ }
        // 此处不存在对 i 的有意使用
        for (int i = 0; i < 20; ++i) { /* ... */ }  // 好: i 局部于 for 循环

        if (auto pc = dynamic_cast<Circle*>(ps)) {  // 好: pc 局部于 if 语句
            // ... 处理 Circle ...
        }
        else {
            // ... 处理错误 ...
        }
    }

##### 示例，不好

    void use(const string& name)
    {
        string fn = name + ".txt";
        ifstream is {fn};
        Record r;
        is >> r;
        // ... 200 行代码，都不存在使用 fn 或 is 的意图 ...
    }

大多数测量都会称这个函数太长了，但其关键点是 `fn` 所使用的资源和 `is` 所持有的文件句柄
所持有的时间比其所需长太多了，而且可能在函数后面意外地出现对 `is` 和 `fn` 的使用。
这种情况下，把读取操作重构出来可能是一个好主意：

    Record load_record(const string& name)
    {
        string fn = name + ".txt";
        ifstream is {fn};
        Record r;
        is >> r;
        return r;
    }

    void use(const string& name)
    {
        Record r = load_record(name);
        // ... 200 行代码 ...
    }

##### 强制实施

- 对声明于循环之外，且在循环之后不再使用的循环变量进行标记。
- 当诸如文件句柄和锁这样的昂贵资源超过 N 行未被使用时进行标记（对某个适当的 N）。

### <a name="Res-cond"></a>ES.6: 在 for 语句的初始化式和条件中声明名字以限制其作用域

##### 理由

可读性。
将循环变量的可见性限制到循环范围内。
避免在循环之后将循环变量用于其他目的。
最小化资源持有率。

##### 理由

    void use()
    {
        for (string s; cin >> s;)
            v.push_back(s);

        for (int i = 0; i < 20; ++i) {   // 好: i 局部于 for 循环
            // ...
        }

        if (auto pc = dynamic_cast<Circle*>(ps)) {   // 好: pc 局部于 if 语句
            // ... 处理 Circle ...
        }
        else {
            // ... 处理错误 ...
        }
    }

##### 示例，请勿如此

    int j;                            // 不好：j 在循环之外可见
    for (j = 0; j < 100; ++j) {
        // ...
    }
    // j 在此处仍可见但并不需要

**另请参见**: [不要用一个变量来达成两个不相关的目的](#Res-recycle)

##### 强制实施

- 若在 `for` 语句中修改的变量是在循环之外声明的，但并未在循环之外使用，则给出警告。
- 【困难】 对声明与循环之前，且在循环之后用于某个无关用途的循环变量进行标记。

**讨论**：限制循环变量的作用域到循环体还相当有助于代码优化。认识到引入的变量仅在循环体中可以访问，
允许进行诸如外提（hoisting），强度消减，循环不变式代码外提等等优化。

##### C++17 和 C++20 示例

注：C++17 和 C++20 还增加了 `if`、`switch` 和范围式 `for` 的初始化式语句。以下代码要求支持 C++17 和 C++20。

    map<int, string> mymap;

    if (auto result = mymap.insert(value); result.second) {
        // 本代码块中，插入成功且 result 有效
        use(result.first);  // ok
        // ...
    } // result 在此处销毁

##### C++17 和 C++20 强制实施（当使用 C++17 或 C++20 编译器时）

- 选择/循环变量，若其在选择或循环体之前声明而在其之后不再使用，则对其进行标记
- 【困难】 选择/循环变量，若其在选择或循环体之前声明而在其之后用于某个无关目的，则对其进行标记

### <a name="Res-name-length"></a>ES.7: 保持常用的和局部的名字尽量简短，而让非常用的和非局部的名字较长

##### 理由

可读性。减低在无关的非局部名字之间发生冲突的机会。

##### 示例

符合管理的简短的局部名字能够增强可读性：

    template<typename T>    // 好
    void print(ostream& os, const vector<T>& v)
    {
        for (gsl::index i = 0; i < v.size(); ++i)
            os << v[i] << '\n';
    }

索引根据惯例称为 `i`，而这个泛型函数中不存在有关这个 vector 的含义的提示，因此和其他名字一样， `v` 也没问题。与之相比，

    template<typename Element_type>   // 不好: 啰嗦，难于阅读
    void print(ostream& target_stream, const vector<Element_type>& current_vector)
    {
        for (gsl::index current_element_index = 0;
             current_element_index < current_vector.size();
             ++current_element_index
        )
        target_stream << current_vector[current_element_index] << '\n';
    }

当然，这是一个讽刺，但我们还见过更糟糕的。

##### 示例

不合惯例而简短的非局部名字则会搞乱代码：

    void use1(const string& s)
    {
        // ...
        tt(s);   // 不好: tt() 是什么？
        // ...
    }

更好的做法是，为非局部实体提供可读的名字：

    void use1(const string& s)
    {
        // ...
        trim_tail(s);   // 好多了
        // ...
    }

这样的话，有可能读者指导 `trim_tail` 是什么意思，而且读者可以找一下它并回忆起来。

##### 示例，不好

大型函数的参数的名字实际上可当作是非局部的，因此应当有意义：

    void complicated_algorithm(vector<Record>& vr, const vector<int>& vi, map<string, int>& out)
    // 根据 vi 中的索引，从 vr 中读取事件（并标记所用的 Record），
    // 向 out 中放置（名字，索引）对
    {
        // ... 500 行的代码，使用 vr，vi，和 out ...
    }

我们建议保持函数短小，但这条规则并不受到普遍坚持，而命名应当能反映这一点。

##### 强制实施

检查局部和非局部的名字的长度。同时考虑函数的长度。

### <a name="Res-name-similar"></a>ES.8: 避免使用看起来相似的名字

##### 理由

代码清晰性和可读性。太过相似的名字会拖慢理解过程并增加出错的可能性。

##### 示例，不好

    if (readable(i1 + l1 + ol + o1 + o0 + ol + o1 + I0 + l0)) surprise();

##### 示例，不好

不要在同一个作用域中用和类型相同的名字声明一个非类型实体。这样做将消除为区分它们所需的关键字 `struct` 或 `enum` 等。这同样消除了一种错误来源，因为 `struct X` 在对 `X` 的查找失败时会隐含地声明新的 `X`。

    struct foo { int n; };
    struct foo foo();       // 不好, foo 在作用域中已经是一个类型了
    struct foo x = foo();   // 需要进行区分

##### 例外

很古老的头文件中可能会用在相同作用域中用同一个名字同时声明非类型实体和类型。

##### 强制实施

- 用一个已知的易混淆字母和数字组合的列表来对名字进行检查。
- 当变量、函数或枚举符的声明隐藏了在相同作用域中所声明的类或枚举时，给出警告。

### <a name="Res-not-CAPS"></a>ES.9: 避免 `ALL_CAPS` 式的名字

##### 理由

这样的名字通常是用于宏的。因此，`ALL_CAPS` 这样的名字可能遭受意外的宏替换。

##### 示例

    // 某个头文件中：
    #define NE !=

    // 某个别的头文件中的别处：
    enum Coord { N, NE, NW, S, SE, SW, E, W };

    // 某个糟糕程序员的 .cpp 中的第三处：
    switch (direction) {
    case N:
        // ...
    case NE:
        // ...
    // ...
    }

##### 注解

不要仅仅因为常量曾经是宏，就使用 `ALL_CAPS` 作为常量。

##### 强制实施

对所有的 ALL CAPS 进行标记。对于老代码，则接受宏名字的 ALL CAPS 而标记所有的非 ALL-CAPS 的宏名字。

### <a name="Res-name-one"></a>ES.10: 每条声明中（仅）声明一个名字

##### 理由

每行一条声明的做法增加可读性并可避免与 C++ 的文法
相关的错误。这样做也为更具描述性的行尾注释留下了
空间。

##### 示例，不好

    char *p, c, a[7], *pp[7], **aa[10];   // 讨厌！

##### 例外

函数声明中可以包含多个函数参数声明。

##### 例外

结构化绑定（C++17）就是专门设计用于引入多个变量的：

    auto [iter, inserted] = m.insert_or_assign(k, val);
    if (inserted) { /* 已插入新条目 */ }

##### 示例

    template<class InputIterator, class Predicate>
    bool any_of(InputIterator first, InputIterator last, Predicate pred);

用 concept 则更佳：

    bool any_of(input_iterator auto first, input_iterator auto last, predicate auto pred);

##### 示例

    double scalbn(double x, int n);   // OK: x * pow(FLT_RADIX, n); FLT_RADIX 通常为 2

或者：

    double scalbn(    // 有改善: x * pow(FLT_RADIX, n); FLT_RADIX 通常为 2
        double x,     // 基数
        int n         // 指数
    );

或者：

    // 有改善: base * pow(FLT_RADIX, exponent); FLT_RADIX 通常为 2
    double scalbn(double base, int exponent);

##### 示例

    int a = 10, b = 11, c = 12, d, e = 14, f = 15;

在较长的声明符列表中，很容易忽视某个未能初始化的变量。

##### 强制实施

对具有多个声明符的变量或常量的声明式（比如 `int* p, q;`）进行标记。

### <a name="Res-auto"></a>ES.11: 使用 `auto` 来避免类型名字的多余重复

##### 理由

- 单纯的重复既麻烦又易错。
- 当使用 `auto` 时，所声明的实体的名字是处于声明的固定位置的，这增加了可读性。
- 函数模板声明的返回类型可能是某个成员类型。

##### 示例

考虑：

    auto p = v.begin();      // vector<DataRecord>::iterator
    auto z1 = v[3];          // 产生 DataRecord 的副本
    auto& z2 = v[3];         // 避免复制
    const auto& z3 = v[3];   // const 并避免复制
    auto h = t.future();
    auto q = make_unique<int[]>(s);
    auto f = [](int x) { return x + 10; };

以上都避免了写下冗长又难记的类型，它们是编译器已知的，但程序员则可能会搞错。

##### 示例

    template<class T>
    auto Container<T>::first() -> Iterator;   // Container<T>::Iterator

##### 例外

当使用初始化式列表，而所需要的确切类型是已知的，同时某个初始化式可能需要转换时，应当避免使用 `auto`。

##### 示例

    auto lst = { 1, 2, 3 };   // lst 是一个 initializer_list
    auto x{1};   // x 是一个 int（C++17；在 C++11 中则为 initializer_list）

##### 注解

C++20 的情况是，我们可以（而且应该）使用概念来更加明确地说明所推断的类型：

    // ...
    forward_iterator auto p = algo(x, y, z);

##### 示例（C++17）

    std::set<int> values;
    // ...
    auto [ position, newly_inserted ] = values.insert(5);   // 展开 std::pair 的成员

##### 强制实施

对声明中多余的类型名字进行标记。

### <a name="Res-reuse"></a>ES.12: 不要在嵌套作用域中重用名字

##### 理由

这样很容易把所用的是哪个变量搞混。
会造成维护问题。

##### 示例，不好

    int d = 0;
    // ...
    if (cond) {
        // ...
        d = 9;
        // ...
    }
    else {
        // ...
        int d = 7;
        // ...
        d = value_to_be_returned;
        // ...
    }

    return d;

这是个大型的 `if` 语句，很容易忽视在内部作用域中所引入的新的 `d`。
这是一种已知的 BUG 来源。
这种在内部作用域中的名字重用有时候被称为“遮蔽“。

##### 注解

当函数变得过大和过于复杂时，遮蔽是一种主要的问题。

##### 示例

语言不允许在函数的最外层块中遮蔽函数参数：

    void f(int x)
    {
        int x = 4;  // 错误：重用函数参数的名字

        if (x) {
            int x = 7;  // 允许，但不好
            // ...
        }
    }

##### 示例，不好

把成员名重用为局部变量也会造成问题：

    struct S {
        int m;
        void f(int x);
    };

    void S::f(int x)
    {
        m = 7;    // 对成员赋值
        if (x) {
            int m = 9;
            // ...
            m = 99; // 对局部变量赋值
            // ...
        }
    }

##### 例外

我们经常在派生类中重用基类中的函数名：

    struct B {
        void f(int);
    };

    struct D : B {
        void f(double);
        using B::f;
    };

这样做是易错的。
例如，要是忘了 using 声明式的话，`d.f(1)` 的调用就不会找到 `int` 版本的 `f`。

??? 我们需要为类层次中的遮蔽/隐藏给出专门的规则吗？

##### 强制实施

- 对嵌套局部作用域中的名字重用进行标记。
- 对成员函数中将成员名重用为局部变量进行标记。
- 对把全局名字重用为局部变量或成员的名字进行标记。
- 对在派生类中重用（除函数名之外的）基类成员名进行标记。

### <a name="Res-always"></a>ES.20: 坚持为对象进行初始化

##### 理由

避免发生“设值前使用”的错误及其所关联的未定义行为。
避免由复杂的初始化的理解所带来的问题。
简化重构。

##### 示例

    void use(int arg)
    {
        int i;   // 不好: 未初始化的变量
        // ...
        i = 7;   // 初始化 i
    }

错了，`i = 7` 并不是 `i` 的初始化；它是向其赋值。而且 `i` 也可能在 `...` 的部分中被读取。更好的做法是：

    void use(int arg)   // OK
    {
        int i = 7;   // OK: 初始化
        string s;    // OK: 默认初始化
        // ...
    }

##### 注释

我们有意让*总是进行初始化*规则比*对象在使用前必须设值*的语言规则更强。
后者是较为宽松的规则，虽然能够识别出技术上的 BUG，不过：

- 它会导致较不可读的代码，
- 它鼓励人们在比所需的更大的作用域中声明名字，
- 它会导致较难于阅读的代码，
- 它会因为鼓励复杂的代码而导致出现逻辑 BUG，
- 它会妨碍进行重构。

而*总是进行初始化*规则则是以提升可维护性为目标的一条风格规则，同样也是保护避免出现“设值前使用”错误的规则。

##### 示例

这个例子经常被当成是用来展示需要更宽松的初始化规则的例子。

    widget i;    // "widget" 是一个初始化操作昂贵的类型，可能是一种大型 POD
    widget j;

    if (cond) {  // 不好: i 和 j 进行了“延迟”初始化
        i = f1();
        j = f2();
    }
    else {
        i = f3();
        j = f4();
    }

这段代码是无法简单重写为用初始化式来对 `i` 和 `j` 进行初始化的。
注意，对于带有默认构造函数的类型来说，试图延后初始化只会导致变为一次默认初始化之后跟着一次赋值的做法。
这种例子的一种更加流行的理由是“效率”，不过可以检查出是否出现“设置前使用”错误的编译器，同样可以消除任何多余的双重初始化。

假定 `i` 和 `j` 之间存在某种逻辑关联，则这种关联可能应当在函数中予以表达：

    pair<widget, widget> make_related_widgets(bool x)
    {
        return (x) ? {f1(), f2()} : {f3(), f4()};
    }

    auto [i, j] = make_related_widgets(cond);    // C++17

如果除此之外 `make_related_widgets` 函数是多余的，
可以使用 lambda [ES.28](#Res-lambda-init) 来消除之：

    auto [i, j] = [x] { return (x) ? pair{f1(), f2()} : pair{f3(), f4()} }();    // C++17

用一个值代表 `uninitialized` 只是一种问题的症状，而不是一种解决方案：

    widget i = uninit;  // 不好
    widget j = uninit;

    // ...
    use(i);         // 可能发生设值前使用
    // ...

    if (cond) {     // 不好: i 和 j 进行了“延迟”初始化
        i = f1();
        j = f2();
    }
    else {
        i = f3();
        j = f4();
    }

这样的话编译器甚至无法再简单地检测出“设值前使用”。而且我们也在 widget 的状态空间中引入了复杂性：哪些操作对 `uninit` 的 widget 是有效的，哪些不是？

##### 注解

几十年来，精明的程序员中都流行进行复杂的初始化。
这样做也是一种错误和复杂性的主要来源。
而许多这样的错误都是在最初实现之后的多年之后的维护过程中所引入的。

##### 示例

本条规则涵盖成员变量。

    class X {
    public:
        X(int i, int ci) : m2{i}, cm2{ci} {}
        // ...

    private:
        int m1 = 7;
        int m2;
        int m3;

        const int cm1 = 7;
        const int cm2;
        const int cm3;
    };

编译器能够标记 `cm3` 为未初始化（因其为 `const`），但它无法发觉 `m3` 缺少初始化。
通常来说，以很少不恰当的成员初始化来消除错误，要比缺乏初始化更有价值，
而且优化器是可以消除冗余的初始化的（比如紧跟在赋值之前的初始化）。

##### 例外

当声明一个即将从输入进行初始化的对象时，其初始化就可能导致发生双重初始化。
不过，应当注意这也可能造成输入之后留下未初始化的数据——而这已经是一种错误和安全攻击的重大来源：

    constexpr int max = 8 * 1024;
    int buf[max];         // OK, 但是可疑: 未初始化
    f.read(buf, max);

由于数组和 `std::array` 的有所限制的初始化规则，它们提供了对于需要这种例外的大多数有意义的例子。

某些情况下，这个数组进行初始化的成本可能是显著的。
但是，这样的例子确实倾向于留下可访问到的未初始化变量，因而应当严肃对待它们。

    constexpr int max = 8 * 1024;
    int buf[max] = {};   // 某些情况下更好
    f.read(buf, max);

如果可行的话，应当用某个已知不会溢出的库函数。例如：

    string s;   // s 默认初始化为 ""
    cin >> s;   // s 进行扩充以持有字符串

不要把用于输入操作的简单变量作为本条规则的例外：

    int i;   // 不好
    // ...
    cin >> i;

在并不罕见的情况下，当输入目标和输入操作分开（其实不应该）时，就带来了发生“设值前使用”的可能性。

    int i2 = 0;   // 更好，假设 0 是 i2 可接受的值
    // ...
    cin >> i2;

优秀的优化器应当能够识别输入操作并消除这种多余的操作。

##### 注解

有时候，可以用 lambda 作为初始化式以避免未初始化变量：

    error_code ec;
    Value v = [&] {
        auto p = get_value();   // get_value() 返回 pair<error_code, Value>
        ec = p.first;
        return p.second;
    }();

还可以是：

    Value v = [] {
        auto p = get_value();   // get_value() 返回 pair<error_code, Value>
        if (p.first) throw Bad_value{p.first};
        return p.second;
    }();

**参见**: [ES.28](#Res-lambda-init)

##### 强制实施

- 标记出每个未初始化的变量。
  不要对具有默认构造函数的自定义类型的变量进行标记。
- 检查未初始化的缓冲区是否在声明后*立即*进行了写入。
  将未初始化变量作为一个非 `const` 的引用参数进行传递可以被假定为向变量进行的写入。

### <a name="Res-introduce"></a>ES.21: 不要在确实需要使用变量（或常量）之前就引入它

##### 理由

可读性。限制变量可以被使用的范围。

##### 示例

    int x = 7;
    // ... 这里没有对 x 的使用 ...
    ++x;

##### 强制实施

对离其首次使用很远的声明进行标记。

### <a name="Res-init"></a>ES.22: 要等到获得了用以初始化变量的值之后才声明变量

##### 理由

可读性。限制变量可以被使用的范围。避免“设值前使用”的风险。初始化通常比赋值更加高效。

##### 示例，不好

    string s;
    // ... 此处没有 s 的使用 ...
    s = "what a waste";

##### 示例，不好

    SomeLargeType var;  // 很难读的驼峰变量

    if (cond)   // 某个不简单的条件
        Set(&var);
    else if (cond2 || !cond3) {
        var = Set2(3.14);
    }
    else {
        var = 0;
        for (auto& e : something)
            var += e;
    }

    // 使用 var; 可以仅通过控制流而静态地保证这并不会过早进行

如果 `SomeLargeType` 的默认初始化并非过于昂贵的话就没什么问题。
不过，程序员可能十分想知道是否所有的穿过这个条件迷宫的路径都已经被覆盖到了。
如果没有的话，就存在一个“设值前使用”的 BUG。这是维护工作的一个陷阱。

对于具有必要复杂性的初始化式，也包括 `const` 变量的初始化式，应当考虑使用 lambda 来表达它；参见 [ES.28](#Res-lambda-init)。

##### 强制实施

- 如果具有默认初始化的声明在其首次被读取前就进行赋值，则对其进行标记。
- 对于任何在未初始化变量之后且在其使用之前进行的复杂计算进行标记。

### <a name="Res-list"></a>ES.23: 优先使用 `{}` 初始化式语法

##### 理由

优先使用 `{}`。`{}` 初始化的规则比其他形式的初始化更简单，更通用，更少歧义，而且更安全。

仅当你确定不存在窄化转换时才可使用 `=`。对于内建算术类型，`=` 仅和 `auto` 一起使用。

避免 `()` 初始化，它会导致解析中的歧义。

##### 示例

    int x {f(99)};
    int y = x;
    vector<int> v = {1, 2, 3, 4, 5, 6};

##### 例外

对于容器来说，存在用 `{...}` 给出元素列表而用 `(...)` 给出大小的传统做法：

    vector<int> v1(10);    // vector 有 10 个具有默认值 0 的元素
    vector<int> v2{10};    // vector 有 1 个值为 10 的元素

    vector<int> v3(1, 2);  // vector 有 1 个值为 2 的元素
    vector<int> v4{1, 2};  // vector 有 2 个值为 1 和 2 的元素

##### 注解

`{}` 初始化式不允许进行窄化转换（这点通常都很不错），并允许使用显式构造函数（这没有问题，我们的意图就是初始化一个新变量）。

##### 示例

    int x {7.9};   // 错误: 发生窄化
    int y = 7.9;   // OK: y 变为 7. 希望编译器给出了警告消息
    int z = gsl::narrow_cast<int>(7.9);  // OK: 这个正是你想要的

##### 注解

`{}` 初始化可以用于几乎所有的初始化；而其他的初始化则不行：

    auto p = new vector<int> {1, 2, 3, 4, 5};   // 初始化 vector
    D::D(int a, int b) :m{a, b} {   // 成员初始化式（比如说 m 可能是 pair）
        // ...
    };
    X var {};   // 初始化 var 为空
    struct S {
        int m {7};   // 成员的默认初始化
        // ...
    };

由于这个原因，以 `{}` 进行初始化通常被称为“统一初始化”，
（但很可惜存在少数不符合规则的例外）。

##### 注解

对以 `auto` 声明的变量用单个值进行的初始化，比如 `{v}`，直到 C++17 之前都还具有令人意外的含义。
C++17 的规则多少会少些意外：

    auto x1 {7};        // x1 是一个值为 7 的 int
    auto x2 = {7};      // x2 是一个具有一个元素 7 的 initializer_list<int>

    auto x11 {7, 8};    // 错误: 两个初始化式
    auto x22 = {7, 8};  // x22 是一个具有元素 7 和 8 的 initializer_list<int>

如果确实需要一个 `initializer_list<T>` 的话，可以使用 `={...}`：

    auto fib10 = {1, 1, 2, 3, 5, 8, 13, 21, 34, 55};   // fib10 是一个列表

##### 注解

`={}` 进行的是复制初始化，而 `{}` 则进行直接初始化。
与在复制初始化和直接初始化自身之间存在的区别类似的是，这里也可能带来一些意外情况。
`{}` 可以接受 `explicit` 构造函数；而 `={}` 则不能。例如：

    struct Z { explicit Z() {} };

    Z z1{};     // OK: 直接初始化，使用的是 explicit 构造函数
    Z z2 = {};  // 错误: 复制初始化，不能使用 explicit 构造函数

除非特别要求禁止使用显式构造函数，否则都应当使用普通的 `{}` 初始化。

##### 示例

    template<typename T>
    void f()
    {
        T x1(1);    // T 以 1 进行初始化
        T x0();     // 不好: 函数声明（一般都是一个错误）

        T y1 {1};   // T 以 1 进行初始化
        T y0 {};    // 默认初始化 T
        // ...
    }

**参见**: [讨论](#???)

##### 强制实施

- 当使用 `=` 初始化算术类型并发生窄化转换时予以标记。
- 当使用 `()` 初始化语法但实际上是声明式时予以标记。（许多编译器已经可就此给出警告。）

### <a name="Res-unique"></a>ES.24: 用 `unique_ptr<T>` 来保存指针

##### 理由

使用 `std::unique_ptr` 是避免泄漏的最简单方法。它是可靠的，它
利用类型系统完成验证所有权安全性的大部分工作，它
增加可读性，而且它没有或近乎没有运行时成本。

##### 示例

    void use(bool leak)
    {
        auto p1 = make_unique<int>(7);   // OK
        int* p2 = new int{7};            // 不好: 可能泄漏
        // ... 未对 p2 赋值 ...
        if (leak) return;
        // ... 未对 p2 赋值 ...
        vector<int> v(7);
        v.at(7) = 0;                    // 抛出异常
        delete p2;                      // 避免泄漏已太晚了
        // ...
    }

当 `leak == true` 时，`p2` 所指向的对象就会泄漏，而 `p1` 所指向的对象则不会。
当 `at()` 抛出异常时也是同样的情况。两种情况下，都没能到达 `delete p2` 语句。

##### 强制实施

寻找作为这些函数的目标的原生指针：`new`，`malloc()`，或者可能返回这类指针的函数。

### <a name="Res-const"></a>ES.25: 应当将对象声明为 `const` 或 `constexpr`，除非后面需要修改其值

##### 理由

这样的话你就不会误改掉这个值。而且这种方式可能会给编译器的带来优化机会。

##### 示例

    void f(int n)
    {
        const int bufmax = 2 * n + 2;  // 好: 无法意外改掉 bufmax
        int xmax = n;                  // 可疑: xmax 是不是会改掉？
        // ...
    }

##### 强制实施

查看变量是不是真的被改动过，若并非如此就进行标记。
不幸的是，也许不可能检测出某个非 `const` 是不是
*有意*要改动，还是仅仅是没被改动而已。

### <a name="Res-recycle"></a>ES.26: 不要用一个变量来达成两个不相关的目的

##### 理由

可读性和安全性。

##### 示例，不好

    void use()
    {
        int i;
        for (i = 0; i < 20; ++i) { /* ... */ }
        for (i = 0; i < 200; ++i) { /* ... */ } // 不好: i 重复使用了
    }

+##### 注解

也许你想把一个缓冲区当做暂存器来重复使用以作为一种优化措施，但即便如此也请尽可能限定该变量的作用域，还要当心不要导致由于遗留在重用的缓冲区中的数据而引发的 BUG，这是安全性 BUG 的一种常见来源。

    void write_to_file()
    {
        std::string buffer;             // 以避免每次循环重复中的重新分配
        for (auto& o : objects) {
            // 第一部分工作。
            generate_first_string(buffer, o);
            write_to_file(buffer);

            // 第二部分工作。
            generate_second_string(buffer, o);
            write_to_file(buffer);

            // 等等...
        }
    }

##### 强制实施

标记被重复使用的变量。

### <a name="Res-stack"></a>ES.27: 使用 `std::array` 或 `stack_array` 作为栈上的数组

##### 理由

它们是可读的，而且不会隐式转换为指针。
它们不会和内建数组的非标准扩展相混淆。

##### 示例，不好

    const int n = 7;
    int m = 9;

    void f()
    {
        int a1[n];
        int a2[m];   // 错误: 并非 ISO C++
        // ...
    }

##### 注解

`a1` 的定义是合法的 C++ 而且一直都是。
存在大量的这类代码。
不过它是易错的，尤其当它的界并非局部时更是如此。
而且它也是一种“流行”的错误来源（缓冲区溢出，数组退化而成的指针，等等）。
而 `a2` 的定义符合 C 但不符合 C++，而且被认为存在安全性风险。

##### 示例

    const int n = 7;
    int m = 9;

    void f()
    {
        array<int, n> a1;
        stack_array<int> a2(m);
        // ...
    }

##### 强制实施

- 对具有非常量界的数组（C 风格的 VLA）作出标记。
- 对具有非局部的常量界的数组作出标记。

### <a name="Res-lambda-init"></a>ES.28: 为复杂的初始化（尤其是 `const` 变量）使用 lambda

##### 理由

它可以很好地封装局部的初始化，包括对仅为初始化所需的临时变量进行清理，而且避免了创建不必要的非局部而且无法重用的函数。它对于应当为 `const` 的变量也可以工作，不过必须先进行一些初始化。

##### 示例，不好

    widget x;   // 应当为 const, 不过:
    for (auto i = 2; i <= N; ++i) {          // 这是由 x 的
        x += some_obj.do_something_with(i);  // 初始化所需的
    }                                        // 一段任意长的代码
    // 自此开始，x 应当为 const，不过我们无法在这种风格的代码中做到这点

##### 示例，好

    const widget x = [&] {
        widget val;                                // 假定 widget 具有默认构造函数
        for (auto i = 2; i <= N; ++i) {            // 这是由 x 的
            val += some_obj.do_something_with(i);  // 初始化所需的
        }                                          // 一段任意长的代码
        return val;
    }();

如果可能的话，应当将条件缩减成一个后续的简单集合（比如一个 `enum`），并避免把选择和初始化相互混合起来。

##### 强制实施

很难。最多是某种启发式方案。查找跟随某个未初始化变量之后的循环中向其赋值。

### <a name="Res-macros"></a>ES.30: 不要用宏来操纵程序文本

##### 理由

宏是 BUG 的一个主要来源。
宏不遵守常规的作用域和类型规则。
宏保证会让人读到的东西和编译器见到的东西不一样。
宏使得工具的建造复杂化。

##### 示例，不好

    #define Case break; case   /* 不好 */

这个貌似无害的宏会把某个大写的 `C` 替换为小写的 `c` 导致一个严重的控制流错误。

##### 注解

这条规则并不禁止在 `#ifdef` 等部分中使用用于“配置控制”的宏。

将来，模块可能会消除配置控制中对宏的需求。

##### 注解

此规则也意味着不鼓励使用 `#` 进行字符串化和使用 `##` 进行连接。
照例，宏有一些“无害”的用途，但即使这些也会给工具带来麻烦，
例如自动完成器、静态分析器和调试器。
通常，使用花式宏的欲望是过于复杂的设计的标志。
另外，`＃` 和 `##` 促进了宏的定义和使用：

    #define CAT(a, b) a ## b
    #define STRINGIFY(a) #a

    void f(int x, int y)
    {
        string CAT(x, y) = "asdf";   // 不好: 工具难以处理（也很丑陋）
        string sx2 = STRINGIFY(x);
        // ...
    }

有使用宏进行低级字符串操作的变通方法。例如：

    string s = "asdf" "lkjh";   // 普通的字符串文字连接

    enum E { a, b };

    template<int x>
    constexpr const char* stringify()
    {
        switch (x) {
        case a: return "a";
        case b: return "b";
        }
    }

    void f(int x, int y)
    {
        string sx = stringify<x>();
        // ...
    }

这不像定义宏那样方便，但是易于使用、零开销，并且是类型化的和作用域化的。

将来，静态反射可能会消除对程序文本操作的预处理器的最终需求。

##### 强制实施

见到并非仅用于源代码控制（比如 `#ifdef`）的宏时应当大声尖叫。

### <a name="Res-macros2"></a>ES.31: 不要用宏来作为常量或“函数”

##### 理由

宏是 BUG 的一个主要来源。
宏不遵守常规的作用域和类型规则。
宏不遵守常规的参数传递规则。
宏保证会让人读到的东西和编译器见到的东西不一样。
宏使得工具的建造复杂化。

##### 示例，不好

    #define PI 3.14
    #define SQUARE(a, b) (a * b)

即便我们并未在 `SQUARE` 中留下这个众所周知的 BUG，也存在多种表现好得多的替代方式；比如：

    constexpr double pi = 3.14;
    template<typename T> T square(T a, T b) { return a * b; }

##### 强制实施

见到并非仅用于源代码控制（比如 `#ifdef`）的宏时应当大声尖叫。

### <a name="Res-ALL_CAPS"></a>ES.32: 对所有的宏名采用 `ALL_CAPS` 命名方式

##### 理由

遵循约定。可读性。区分宏。

##### 示例

    #define forever for (;;)   /* 非常不好 */

    #define FOREVER for (;;)   /* 仍然很邪恶，但至少对人来说是可见的 */

##### 强制实施

见到小写的宏时应当大声尖叫。

### <a name="Res-MACROS"></a>ES.33: 如果必须使用宏的话，请为之提供唯一的名字

##### 理由

宏并不遵守作用域规则。

##### 示例

    #define MYCHAR        /* 不好，最终将会和别人的 MYCHAR 相冲突 */

    #define ZCORP_CHAR    /* 还是不好，但冲突的机会较小 */

##### 注解

如果可能就应当避免使用宏：[ES.30](#Res-macros)，[ES.31](#Res-macros2)，以及 [ES.32](#Res-ALL_CAPS)。
然而，存在亿万行的代码中包含宏，以及一种使用并过度使用宏的长期传统。
如果你被迫使用宏的话，请使用长名字，而且应当带有唯一前缀（比如你的组织机构的名字）以减少冲突的可能性。

##### 强制实施

对较短的宏名给出警告。

### <a name="Res-ellipses"></a> ES.34: 不要定义（C 风格的）变参函数

##### 理由

它并非类型安全。
而且需要杂乱的满是强制转换和宏的代码才能正确工作。

##### 示例

    #include <cstdarg>

    // "severity" 后面跟着以零终结的 char* 列表；将 C 风格字符串写入 cerr
    void error(int severity ...)
    {
        va_list ap;             // 一个持有参数的神奇类型
        va_start(ap, severity); // 参数启动："severity" 是 error() 的第一个参数

        for (;;) {
            // 将下一个变量看作 char*；没有检查：经过伪装的强制转换
            char* p = va_arg(ap, char*);
            if (!p) break;
            cerr << p << ' ';
        }

        va_end(ap);             // 参数清理（不能忘了这个）

        cerr << '\n';
        if (severity) exit(severity);
    }

    void use()
    {
        error(7, "this", "is", "an", "error", nullptr);
        error(7); // 崩溃
        error(7, "this", "is", "an", "error");  // 崩溃
        const char* is = "is";
        string an = "an";
        error(7, "this", "is", an, "error"); // 崩溃
    }

**替代方案**: 重载。模板。变参模板。

    #include <iostream>

    void error(int severity)
    {
        std::cerr << '\n';
        std::exit(severity);
    }

    template<typename T, typename... Ts>
    constexpr void error(int severity, T head, Ts... tail)
    {
        std::cerr << head;
        error(severity, tail...);
    }

    void use()
    {
        error(7); // 不会崩溃！
        error(5, "this", "is", "not", "an", "error"); // 不会崩溃！

        std::string an = "an";
        error(7, "this", "is", "not", an, "error"); // 不会崩溃！

        error(5, "oh", "no", nullptr); // 编译器报错！不需要 nullptr。
    }

##### 注解

这基本上就是 `printf` 的实现方式。

##### 强制实施

- 对 C 风格的变参函数的定义作出标记。
- 对 `#include <cstdarg>` 和 `#include <stdarg.h>` 作出标记。

## ES.expr: 表达式

表达式对值进行操作。

### <a name="Res-complicated"></a>ES.40: 避免复杂的表达式

##### 理由

复杂的表达式是易错的。

##### 示例

    // 不好: 在子表达式中藏有赋值
    while ((c = getc()) != -1)

    // 不好: 在一个子表达式中对两个非局部变量进行了赋值
    while ((cin >> c1, cin >> c2), c1 == c2)

    // 有改善，但可能仍然过于复杂
    for (char c1, c2; cin >> c1 >> c2 && c1 == c2;)

    // OK: 若 i 和 j 并非别名
    int x = ++i + ++j;

    // OK: 若 i != j 且 i != k
    v[i] = v[j] + v[k];

    // 不好: 子表达式中“隐藏”了多个赋值
    x = a + (b = f()) + (c = g()) * 7;

    // 不好: 依赖于经常被误解的优先级规则
    x = a & b + c * d && e ^ f == 7;

    // 不好: 未定义行为
    x = x++ + x++ + ++x;

这些表达式中有几个是无条件不好的（比如说依赖于未定义行为）。其他的只不过过于复杂和不常见，即便是优秀的程序员匆忙中也可能会误解或者忽略其中的某个问题。

##### 注解

C++17 收紧了有关求值顺序的规则
（除了赋值中从右向左，以及函数实参求值顺序未指明外均为从左向右，[参见 ES.43](#Res-order)），
但这并不影响复杂表达式很容易引起混乱的事实。

##### 注解

程序员应当了解并运用表达式的基本规则。

##### 示例

    x = k * y + z;             // OK

    auto t1 = k * y;           // 不好: 不必要的啰嗦
    x = t1 + z;

    if (0 <= x && x < max)   // OK

    auto t1 = 0 <= x;        // 不好: 不必要的啰嗦
    auto t2 = x < max;
    if (t1 && t2)            // ...

##### 强制实施

很麻烦。多复杂的表达式才能被当成复杂的？把计算写成每条语句一个操作同样是让人混乱的。需要考虑的有：

- 副作用：（对于某种非局部性定义，）多个非局部变量上发生副作用是值得怀疑的，尤其是当这些副作用是在不同的子表达式中时
- 向别名变量的写入
- 超过 N 个运算符（N 应当为多少？）
- 依赖于微妙的优先级规则
- 使用了未定义行为（我们是否应当识别所有的未定义行为？）
- 实现定义的行为？
- ???

### <a name="Res-parens"></a>ES.41: 对运算符优先级不保准时应使用括号

##### 理由

避免错误。可读性。不是每个人都能记住运算符表格。

##### 示例

    const unsigned int flag = 2;
    unsigned int a = flag;

    if (a & flag != 0)  // 不好: 含义为 a&(flag != 0)

注意：我们建议程序员了解算术运算和逻辑运算的优先级表，但应当考虑当按位逻辑运算和其他运算符混合使用时需要采用括号。

    if ((a & flag) != 0)  // OK: 按预期工作

##### 注解

你应当了解足够的知识以避免在这样的情况下需要括号：

    if (a < 0 || a <= max) {
        // ...
    }

##### 强制实施

- 当按位逻辑运算符合其他运算符组合时进行标记。
- 当赋值运算符不是最左边的运算符时进行标记。
- ???

### <a name="Res-ptr"></a>ES.42: 保持单纯直接的指针使用方式

##### 理由

复杂的指针操作是一种重大的错误来源。

##### 注解

代之以使用 `gsl::span`。
指针[只应当指代单个对象](#Ri-array)。
指针算术是脆弱而易错的，是许多许多糟糕的 BUG 和安全漏洞的来源。
`span` 是一种用于访问数组对象的带有边界检查的安全类型。
以常量为下标来访问已知边界的数组，编译器可以进行验证。

##### 示例，不好

    void f(int* p, int count)
    {
        if (count < 2) return;

        int* q = p + 1;    // 不好

        ptrdiff_t d;
        int n;
        d = (p - &n);      // OK
        d = (q - p);       // OK

        int n = *p++;      // 不好

        if (count < 6) return;

        p[4] = 1;          // 不好

        p[count - 1] = 2;  // 不好

        use(&p[0], 3);     // 不好
    }

##### 示例，好

    void f(span<int> a) // 好多了：函数声明中使用了 span
    {
        if (a.size() < 2) return;

        int n = a[0];      // OK

        span<int> q = a.subspan(1); // OK

        if (a.size() < 6) return;

        a[4] = 1;          // OK

        a[a.size() - 1] = 2;  // OK

        use(a.data(), 3);  // OK
    }

##### 注解

用变量做下标，对于工具和人类来说都是很难将其验证为安全的。
`span` 是一种用于访问数组对象的带有运行时边界检查的安全类型。
`at()` 是可以保证单次访问进行边界检查的另一种替代方案。
如果需要用迭代器来访问数组的话，应使用构造于数组之上的 `span` 所提供的迭代器。

##### 示例，不好

    void f(array<int, 10> a, int pos)
    {
        a[pos / 2] = 1; // 不好
        a[pos - 1] = 2; // 不好
        a[-1] = 3;    // 不好（但易于被工具查出） - 没有替代方案，请勿这样做
        a[10] = 4;    // 不好（但易于被工具查出） - 没有替代方案，请勿这样做
    }

##### 示例，好

使用 `span`：

    void f1(span<int, 10> a, int pos) // A1: 将参数类型改为使用 span
    {
        a[pos / 2] = 1; // OK
        a[pos - 1] = 2; // OK
    }

    void f2(array<int, 10> arr, int pos) // A2: 增加局部的 span 并使用之
    {
        span<int> a = {arr.data(), pos};
        a[pos / 2] = 1; // OK
        a[pos - 1] = 2; // OK
    }

使用 `at()`：

    void f3(array<int, 10> a, int pos) // 替代方案 B: 用 at() 进行访问
    {
        at(a, pos / 2) = 1; // OK
        at(a, pos - 1) = 2; // OK
    }

##### 示例，不好

    void f()
    {
        int arr[COUNT];
        for (int i = 0; i < COUNT; ++i)
            arr[i] = i; // 不好，不能使用非常量索引
    }

##### 示例，好

使用 `span`：

    void f1()
    {
        int arr[COUNT];
        span<int> av = arr;
        for (int i = 0; i < COUNT; ++i)
            av[i] = i;
    }

使用 `span` 和基于范围的 `for`：

    void f1a()
    {
         int arr[COUNT];
         span<int, COUNT> av = arr;
         int i = 0;
         for (auto& e : av)
             e = i++;
    }

使用 `at()` 进行访问：

    void f2()
    {
        int arr[COUNT];
        int i = 0;
        for (int i = 0; i < COUNT; ++i)
            at(arr, i) = i;
    }

使用基于范围的 `for`：

    void f3()
    {
         int arr[COUNT];
         for (auto& e : arr)
             e = i++;
    }

##### 注解

工具可以提供重写能力，以将涉及动态索引表达式的数组访问替换为使用 `at()` 进行访问：

    static int a[10];

    void f(int i, int j)
    {
        a[i + j] = 12;      // 不好，可以重写为 ...
        at(a, i + j) = 12;  // OK - 带有边界检查
    }

##### 示例

把数组转变为指针（语言基本上总会这样做），移除了进行检查的机会，因此应当予以避免

    void g(int* p);

    void f()
    {
        int a[5];
        g(a);        // 不好：是要传递一个数组吗？
        g(&a[0]);    // OK：传递单个对象
    }

如果要传递数组的话，应该这样：

    void g(int* p, size_t length);  // 老的（危险）代码

    void g1(span<int> av); // 好多了：改动了 g()。

    void f()
    {
        int a[5];
        span<int> av = a;

        g(av.data(), av.size());   // OK, 如果没有其他选择的话
        g1(a);                     // OK - 这里没有退化，而是使用了隐式的 span 构造函数
    }

##### 强制实施

- 对任何在指针类型的表达式上进行的产生指针类型的值的算术运算进行标记。
- 对任何数组类型的表达式或变量（无论是静态数组还是 `std::array`）上进行索引的表达式，若其索引不是值为从 `0` 到数组上界之内的编译期常量表达式，则进行标记。
- 对任何可能依赖于从数组类型向指针类型的隐式转换的表达式进行标记。

本条规则属于[边界安全性剖面配置](#SS-bounds)。

### <a name="Res-order"></a>ES.43: 避免带有未定义的求值顺序的表达式

##### 理由

你没办法搞清楚这种代码会做什么。可移植性。
即便它做到了对你可能有意义的事情，它在别的编译器（比如你的编译器的下个版本）或者不同的优化设置中也可能会做出不同的事情。

##### 注解

C++17 收紧了有关求值顺序的规则：
除了赋值中从右向左，以及函数实参求值顺序未指明外均为从左向右。

不过，要记住你的代码可能是由 C++17 之前的编译器进行编译的（比如通过复制粘贴），请勿自作聪明。

##### 示例

    v[i] = ++i;   //  其结果是未定义的

一条不错经验法则是，你不应当在一个表达式中两次读取你所写入的值。

##### 强制实施

可以由优秀的分析器检测出来。

### <a name="Res-order-fct"></a>ES.44: 不要对函数参数求值顺序有依赖

##### 理由

因为这种顺序是未定义的。

##### 注解

C++17 收紧了有关求值顺序的规则，但函数实参求值顺序仍然是未指明的。

##### 示例

    int i = 0;
    f(++i, ++i);

在 C++17 之前，其行为是未定义的。因此其行为可能是任何事（比如 `f(2, 2)`）。
自 C++17 起，中这段代码没有未定义行为，但仍未指定是哪个实参被首先求值。这个调用会是 `f(0, 1)` 或 `f(1, 0)`，但你不知道是哪个。

##### 示例

重载运算符可能导致求值顺序问题：

    f1()->m(f2());          // m(f1(), f2())
    cout << f1() << f2();   // operator<<(operator<<(cout, f1()), f2())

在 C++17 中，这些例子将按预期工作（自左向右），而赋值则按自右向左求值（`=` 正是自右向左绑定的）

    f1() = f2();    // C++14 中为未定义行为；C++17 中 f2() 在 f1() 之前求值

##### 强制实施

可以由优秀的分析器检测出来。

### <a name="Res-magic"></a>ES.45: 避免“魔法常量”，采用符号化常量

##### 理由

表达式中内嵌的无名的常量很容易被忽略，而且经常难于理解：

##### 示例

    for (int m = 1; m <= 12; ++m)   // 请勿如此: 魔法常量 12
        cout << month[m] << '\n';

不是所有人都知道一年中有 12 个月份，号码是 1 到 12。更好的做法是：

    // 月份索引值为 1..12
    constexpr int first_month = 1;
    constexpr int last_month = 12;

    for (int m = first_month; m <= last_month; ++m)   // 好多了
        cout << month[m] << '\n';

更好的做法是，不要暴露常量：

    for (auto m : month)
        cout << m << '\n';

##### 强制实施

标记代码中的字面量。让 `0`，`1`，`nullptr`，`\n'`，`""`，以及某个确认列表中的其他字面量通过检查。

### <a name="Res-narrowing"></a>ES.46: 避免丢失数据（窄化、截断）的算术转换

##### 理由

窄化转换会销毁信息，通常是不期望发生的。

##### 示例，不好

关键的例子就是基本的窄化：

    double d = 7.9;
    int i = d;    // 不好: 窄化: i 变为了 7
    i = (int) d;  // 不好: 我们打算声称这样的做法仍然不够明确

    void f(int x, long y, double d)
    {
        char c1 = x;   // 不好: 窄化
        char c2 = y;   // 不好: 窄化
        char c3 = d;   // 不好: 窄化
    }

##### 注解

指导方针支持库提供了一个 `narrow_cast` 操作，用以指名发生窄化是可接受的，以及一个 `narrow`（“窄化判定”）当窄化将会损失合法值时将会抛出一个异常：

    i = gsl::narrow_cast<int>(d);   // OK (明确需要): 窄化: i 变为了 7
    i = gsl::narrow<int>(d);        // OK: 抛出 narrowing_error

其中还包含了一些含有损失的算术强制转换，比如从负的浮点类型到无符号整型类型的强制转换：

    double d = -7.9;
    unsigned u = 0;

    u = d;                               // 不好：发生窄化
    u = gsl::narrow_cast<unsigned>(d);   // OK (明确需要): u 变为了 4294967289
    u = gsl::narrow<unsigned>(d);        // OK：抛出 narrowing_error

##### 注解

这条规则不适用于[按语境转换为 bool](https://en.cppreference.com/w/cpp/language/implicit_conversion#Contextual_conversions) 的情形：

    if (ptr) do_something(*ptr);   // OK：ptr 被用作条件
    bool b = ptr;                  // 不好：发生窄化

##### 强制实施

优良的分析器可以检测到所有的窄化转换。不过，对所有的窄化转换都进行标记将带来大量的误报。建议的做法是：

- 标记出所有的浮点向整数转换（可能只有 `float`->`char` 和 `double`->`int`。这里有问题！需要数据支持）。
- 标记出所有的 `long`->`char`（我怀疑 `int`->`char` 非常常见。这里有问题！需要数据支持）。
- 在函数参数上发生的窄化转换特别值得怀疑。

### <a name="Res-nullptr"></a>ES.47: 使用 `nullptr` 而不是 `0` 或 `NULL`

##### 理由

可读性。最小化意外：`nullptr` 不可能和 `int` 混淆。
`nullptr` 还有一个严格定义的（非常严格）类型，且因此
可以在类型推断可能在 `NULL` 或 `0` 上犯错的场合中仍能
正常工作。

##### 示例

考虑：

    void f(int);
    void f(char*);
    f(0);         // 调用 f(int)
    f(nullptr);   // 调用 f(char*)

##### 强制实施

对用作指针的 `0` 和 `NULL` 进行标记。可以用简单的程序变换来达成这种变换。

### <a name="Res-casts"></a>ES.48: 避免强制转换

##### 理由

强制转换是众所周知的错误来源，它们使得一些优化措施变得不可靠。

##### 示例，不好

    double d = 2;
    auto p = (long*)&d;
    auto q = (long long*)&d;
    cout << d << ' ' << *p << ' ' << *q << '\n';

你觉得这段代码会打印出什么呢？结果最好由实现定义。我得到的是

    2 0 4611686018427387904

加上这些

    *q = 666;
    cout << d << ' ' << *p << ' ' << *q << '\n';

得到的是

    3.29048e-321 666 666

奇怪吗？我很庆幸程序没有崩溃掉。

##### 注解

写下强制转换的程序员通常认为他们知道所做的是什么事情，
或者写出强制转换能让程序“更易读”。
而实际上，他们这样经常会禁止掉使用值的一些一般规则。
如果存在正确的函数的话，重载决议和模板实例化通常都能挑选出正确的函数。
如果没有的话，则可能本应如此，而不应该进行某种局部的修补（强制转换）。

##### 注解

强制转换在系统编程语言中是必要的。例如，否则我们怎么
才能把设备寄存器的地址放入一个指针呢？然而，强制转换
却被严重过度使用了，而且也是一种主要的错误来源。

当你觉得需要进行大量强制转换时，可能存在一个基本的设计问题。

[类型剖面配置](#Pro-type-reinterpretcast) 禁止使用 `reinterpret_cast` 和 C 风格强制转换。

不要以强制转换为 `(void)` 来忽略 `[[nodiscard]]` 返回值。
当你有意要丢弃这种返回值时，应当首先深入思考这是不是确实是个好主意（通常，这个函数或者使用了 `[[nodiscard]]` 的返回类型的作者，当初确实是有充分理由的）。
要是你仍然觉得这样做是合适的，而且你的代码评审者也同意的话，使用 `std::ignore =` 来关闭这个警告，这既简单，可移植，也易于 grep。

##### 替代方案

强制转换被广泛（误）用了。现代 C++ 已经提供了一些规则和语言构造，消除了许多语境中对强制转换的需求，比如

- 使用模板
- 使用 `std::variant`
- 借助良好定义的，安全的，指针类型之间的隐式转换
- 使用 `std::ignore =` 来忽略 `[[nodiscard]]` 值

##### 强制实施

- 对包括向 `void` 在内的所有 C 风格强制转换进行标记。
- 对使用 `Type(value)` 的函数风格强制转换进行标记。应代之以使用不会发生窄化的 `Type{value}`。（参见 [ES.64](#Res-construct)。）
- 对指针类型之间的[同一强制转换](#Pro-type-identitycast)，若其中的源类型和目标类型相同(#Pro-type-identitycast)则进行标记。
- 对可以作为[隐式转换](#Pro-type-implicitpointercast)的显示指针强制转换进行标记。

### <a name="Res-casts-named"></a>ES.49: 当必须使用强制转换时，使用具名的强制转换

##### 理由

可读性。避免错误。
具名的强制转换比 C 风格或函数风格的强制转换更加特殊，允许编译器捕捉到某些错误。

具名的强制转换包括：

- `static_cast`
- `const_cast`
- `reinterpret_cast`
- `dynamic_cast`
- `std::move` // `move(x)` 是指代 `x` 的右值引用
- `std::forward` // `forward<T>(x)` 是指代 `x` 的左值或右值引用（取决于 `T`）
- `gsl::narrow_cast` // `narrow_cast<T>(x)` 就是 `static_cast<T>(x)`
- `gsl::narrow` // `narrow<T>(x)` 在当 `static_cast<T>(x) == x` 时即为 `static_cast<T>(x)` 否则会抛出 `narrowing_error`

##### 示例

    class B { /* ... */ };
    class D { /* ... */ };

    template<typename D> D* upcast(B* pb)
    {
        D* pd0 = pb;                        // 错误：不存在从 B* 向 D* 的隐式转换
        D* pd1 = (D*)pb;                    // 合法，但干了什么？
        D* pd2 = static_cast<D*>(pb);       // 错误：D 并非派生于 B
        D* pd3 = reinterpret_cast<D*>(pb);  // OK：你自己负责！
        D* pd4 = dynamic_cast<D*>(pb);      // OK：返回 nullptr
        // ...
    }

这个例子是从真实世界的 BUG 合成的，其中 `D` 曾经派生于 `B`，但某个人重构了继承层次。
C 风格的强制转换很危险，因为它可以进行任何种类的转换，使我们丧失了今后受保护不犯错的机会。

##### 注解

当在类型之间进行没有信息丢失的转换时（比如从 `float` 到
`double` 或者从 `int32` 到 `int64`），可以代之以使用花括号初始化。

    double d {some_float};
    int64_t i {some_int32};

这样做明确了有意进行类型转换，而且同样避免了
发生可能导致精度损失的结果的类型转换。（比如说，
试图用这种风格来从 `double` 初始化 `float` 会导致
编译错误。）

##### 注解

`reinterpret_cast` 可以很基础，但其基础用法（如将机器地址转化为指针）并不是类型安全的：

    auto p = reinterpret_cast<Device_register>(0x800);  // 天生危险

##### 强制实施

- 对包括向 `void` 在内的所有 C 风格强制转换进行标记。
- 对使用 `Type(value)` 的函数风格强制转换进行标记。应代之以使用不会发生窄化的 `Type{value}`。（参见 [ES.64](#Res-construct)。）
- [类型剖面配置](#Pro-type-reinterpretcast)禁用了 `reinterpret_cast`。
- [类型剖面配置](#Pro-type-arithmeticcast)对于在算术类型之间使用 `static_cast` 时给出警告。

### <a name="Res-casts-const"></a>ES.50: 不要强制掉 `const`

##### 理由

这是在 `const` 上说谎。
若变量确实声明为 `const`，修改它将导致未定义的行为。

##### 示例，不好

    void f(const int& x)
    {
        const_cast<int&>(x) = 42;   // 不好
    }

    static int i = 0;
    static const int j = 0;

    f(i); // 暗藏的副作用
    f(j); // 未定义的行为

##### 示例

有时候，你可能倾向于借助 `const_cast` 来避免代码重复，比如两个访问函数仅在是否 `const` 上有区别而实现相似的情况。例如：

    class Bar;

    class Foo {
    public:
        // 不好，逻辑重复
        Bar& get_bar()
        {
            /* 获取 my_bar 的非 const 引用前后的复杂逻辑 */
        }

        const Bar& get_bar() const
        {
            /* 获取 my_bar 的 const 引用前后的相同的复杂逻辑 */
        }
    private:
        Bar my_bar;
    };

应当改为共享实现。通常可以直接让非 `const` 函数来调用 `const` 函数。不过当逻辑复杂的时候这可能会导致下面这样的模式，仍然需要借助于 `const_cast`：

    class Foo {
    public:
        // 不大好，非 const 函数调用 const 版本但借助于 const_cast
        Bar& get_bar()
        {
            return const_cast<Bar&>(static_cast<const Foo&>(*this).get_bar());
        }
        const Bar& get_bar() const
        {
            /* 获取 my_bar 的 const 引用前后的复杂逻辑 */
        }
    private:
        Bar my_bar;
    };

虽然这个模式如果恰当应用的话是安全的（因为调用方必然以一个非 `const` 对象来开始），但这并不理想，因为其安全性无法作为检查工具的规则而自动强制实施。

换种方式，可以优先将公共代码放入一个公共辅助函数中，并将之作为模板以使其推断 `const`。这完全不会用到 `const_cast`：

    class Foo {
    public:                         // 好
              Bar& get_bar()       { return get_bar_impl(*this); }
        const Bar& get_bar() const { return get_bar_impl(*this); }
    private:
        Bar my_bar;

        template<class T>           // 好，推断出 T 是 const 还是非 const
        static auto& get_bar_impl(T& t)
            { /* 获取 my_bar 的可能为 const 的引用前后的复杂逻辑 */ }
    };

注意：不要在模板中编写大型的非待决代码，因为它将导致代码爆炸。例如，进一步改进是当 `get_bar_impl` 的全部或部分代码是非待决代码时将之重构移出到一个公共的非模板函数中去，这可能会使代码大小显著变小。

##### 例外

当调用 `const` 不正确的函数时，你可能需要强制掉 `const`。
应当优先将这种函数包装到内联的 `const` 正确的包装函数中，以将强制转换封装到一处中。

##### 示例

有时候，“强制掉 `const`”是为了允许对本来无法改动的对象中的某种临时性的信息进行更新操作。
其例子包括进行缓存，备忘，以及预先计算等。
这样的例子，通常可以通过使用 `mutable` 或者通过一层间接进行处理，而同使用 `const_cast` 一样甚或比之更好。

考虑为昂贵操作将之前所计算的结果保留下来：

    int compute(int x); // 为 x 计算一个值；假设这是昂贵的

    class Cache {   // 为 int->int 操作实现一种高速缓存的某个类型
    public:
        pair<bool, int> find(int x) const;   // 有针对 x 的值吗？
        void set(int x, int v);             // 使 y 成为针对 x 的值
        // ...
    private:
        // ...
    };

    class X {
    public:
        int get_val(int x)
        {
            auto p = cache.find(x);
            if (p.first) return p.second;
            int val = compute(x);
            cache.set(x, val); // 插入针对 x 的值
            return val;
        }
        // ...
    private:
        Cache cache;
    };

这里的 `get_val()` 逻辑上是个常量，因此我们想使其成为 `const` 成员。
为此我们仍然需要改动 `cache`，因此人们有时候会求助于 `const_cast`：

    class X {   // 基于强制转换的可疑的方案
    public:
        int get_val(int x) const
        {
            auto p = cache.find(x);
            if (p.first) return p.second;
            int val = compute(x);
            const_cast<Cache&>(cache).set(x, val);   // 很难看
            return val;
        }
        // ...
    private:
        Cache cache;
    };

幸运的是，有一种更好的方案：
将 `cache` 称为即便对于 `const` 对象来说也是可改变的：

    class X {   // 更好的方案
    public:
        int get_val(int x) const
        {
            auto p = cache.find(x);
            if (p.first) return p.second;
            int val = compute(x);
            cache.set(x, val);
            return val;
        }
        // ...
    private:
        mutable Cache cache;
    };

另一种替代方案是存储指向 `cache` 的指针：

    class X {   // OK，但有点麻烦的方案
    public:
        int get_val(int x) const
        {
            auto p = cache->find(x);
            if (p.first) return p.second;
            int val = compute(x);
            cache->set(x, val);
            return val;
        }
        // ...
    private:
        unique_ptr<Cache> cache;
    };

这个方案最灵活，但需要显式进行 `*cache` 的构造和销毁
（最可能发生于 `X` 的构造函数和析构函数中）。

无论采用哪种形式，在多线程代码中都需要保护对 `cache` 的数据竞争，可能需要使用一个 `std::mutex`。

##### 强制实施

- 标记 `const_cast`。
- 本条规则属于[类型安全性剖面配置](#Pro-type-constcast)。

### <a name="Res-range-checking"></a>ES.55: 避免发生对范围检查的需要

##### 理由

无法溢出的构造时不会溢出的（而且通常运行得更快）：

##### 示例

    for (auto& x : v)      // 打印 v 的所有元素
        cout << x << '\n';

    auto p = find(v, x);   // 在 v 中寻找 x

##### 强制实施

查找显式的范围检查，并启发式地给出替代方案建议。

### <a name="Res-move"></a>ES.56: 仅在确实需要明确移动某个对象到别的作用域时才使用 `std::move()`

##### 理由

我们用移动而不是复制，以避免发生重复并提升性能。

一次移动通常会遗留一个空对象（[C.64](#Rc-move-semantic)），这可能令人意外甚至很危险，因此我们试图避免从左值进行移动（它们可能随后会被访问到）。

##### 注解

当来源是右值（比如 `return` 的值或者函数的结果）时就会隐式地进行移动，因此请不要在这些情况下明确写下 `move` 而无意义地使代码复杂化。可以代之以编写简短的返回值的函数，这样的话无论是函数的返回还是调用方的返回值接收，都会很自然地得到优化。

一般来说，遵循本文档中的指导方针（包括不要让变量的作用域无必要地变大，编写返回值的简短函数，返回局部变量等），有助于消除大多数对显式使用 `std::move` 的需要。

显式的 `move` 需要用于把某个对象明确移动到另一个作用域，尤其是将其传递给某个“接收器”函数，以及移动操作自身（移动构造函数，移动赋值运算符）和交换（`swap`）操作的实现之中。

##### 示例，不好

    void sink(X&& x);   // sink 接收 x 的所有权

    void user()
    {
        X x;
        // 错误: 无法将作者绑定到右值引用
        sink(x);
        // OK: sink 接收了 x 的内容，x 随即必须假定为空
        sink(std::move(x));

        // ...

        // 可能是个错误
        use(x);
    }

通常来说，`std::move()` 都用做某个 `&&` 形参的实参。
而这点之后，应当假定对象已经被移走（参见 [C.64](#Rc-move-semantic)），而直到首次向它设置某个新值之前，请勿再次读取它的状态。

    void f()
    {
        string s1 = "supercalifragilisticexpialidocious";

        string s2 = s1;             // ok, 接收了一个副本
        assert(s1 == "supercalifragilisticexpialidocious");  // ok

        // 不好, 如果你打算保留 s1 的值的话
        string s3 = move(s1);

        // 不好, assert 很可能会失败, s1 很可能被改动了
        assert(s1 == "supercalifragilisticexpialidocious");
    }

##### 示例

    void sink(unique_ptr<widget> p);  // 将 p 的所有权传递给 sink()

    void f()
    {
        auto w = make_unique<widget>();
        // ...
        sink(std::move(w));               // ok, 交给 sink()
        // ...
        sink(w);    // 错误: unique_ptr 经过严格设计，你无法复制它
    }

##### 注解

`std::move()` 经过伪装的向 `&&` 的强制转换；其自身并不会移动任何东西，但会把具名的对象标记为可被移动的候选者。
语言中已经了解了对象可以被移动的一般情况，尤其是从函数返回时，因此请不要用多余的 `std::move()` 使代码复杂化。

绝不要仅仅因为听说过“这样更加高效”就使用 `std::move()`。
通常来说，请不要相信那些没有支持数据的有关“效率”的断言。(???).
通常来说，请不要无理由地使代码复杂化。(??)
绝不要在 const 对象上 `std::move()`，它只会暗中将其转变成一个副本（参见 [Meyers15](#Meyers15) 的条款 23)。

##### 示例，不好

    vector<int> make_vector()
    {
        vector<int> result;
        // ... 加载 result 的数据
        return std::move(result);       // 不好; 直接写 "return result;" 即可
    }

绝不要写 `return move(local_variable);`，这是因为语言已经知道这个变量是移动的候选了。
在这段代码中用 `move` 并不会带来帮助，而且可能实际上是有害的，因为它创建了局部变量的一个额外引用别名，而在某些编译器中这回对 RVO（返回值优化）造成影响。

##### 示例，不好

    vector<int> v = std::move(make_vector());   // 不好; 这个 std::move 完全是多余的

绝不在返回值上使用 `move`，如 `x = move(f());`，其中的 `f` 按值返回。
语言已经知道返回值是临时对象而且可以被移动。

##### 示例

    void mover(X&& x)
    {
        call_something(std::move(x));         // ok
        call_something(std::forward<X>(x));   // 不好, 请勿对右值引用 std::forward
        call_something(x);                    // 可疑  为什么不用std:: move?
    }

    template<class T>
    void forwarder(T&& t)
    {
        call_something(std::move(t));         // 不好, 请勿对转发引用 std::move
        call_something(std::forward<T>(t));   // ok
        call_something(t);                    // 可疑, 为什么不用 std::forward?
    }

##### 强制实施

- 对于 `std::move(x)` 的使用，当 `x` 是右值，或者语言已经将其当做右值，这包括 `return std::move(local_variable);` 以及在按值返回的函数上的 `std::move(f())`，进行标记
- 当没有接受 `const S&` 的函数重载来处理左值时，对接受 `S&&` 参数的函数进行标记。
- 若实参经过 `std::move` 传递给形参则进行标记，除非形参的类型为右值引用 `X&&`，或者类型是只能移动的而该形参为按值传递。
- 当对转发引用（`T&&` 其中 `T` 为模板参数类型）使用 `std::move` 时进行标记。应当代之以使用 `std::forward`。
- 当对并非非 const 右值引用的变量使用 `std::move` 时进行标记。（这是前一条规则的更一般的情况，以覆盖非转发的情况。）
- 当对右值引用（`X&&` 其中 `X` 为非模板形参类型）使用 `std::forward` 时进行标记。应当代之以使用 `std::move`。
- 当对并非转发引用使用 `std::forward` 时进行标记。（这是前一条规则的更一般的情况，以覆盖非移动的情况。）
- 如果对象潜在地被移动走之后的下一个操作是 `const` 操作的话，则进行标记；首先应当交错进行一个非 `const` 操作，最好是赋值，以首先对对象的值进行重置。

### <a name="Res-new"></a>ES.60: 避免在资源管理函数之外使用 `new` 和 `delete`

##### 理由

应用程序代码中的直接资源管理既易错又麻烦。

##### 注解

通常也被称为“禁止裸 `new`！”规则。

##### 示例，不好

    void f(int n)
    {
        auto p = new X[n];   // n 个默认构造的 X
        // ...
        delete[] p;
    }

`...` 部分中的代码可能导致 `delete` 永远不会发生。

**参见**: [R: 资源管理](#S-resource)

##### 强制实施

对裸的 `new` 和裸的 `delete` 进行标记。

### <a name="Res-del"></a>ES.61: 用 `delete[]` 删除数组，用 `delete` 删除非数组对象

##### 理由

这正是语言的要求，而且所犯的错误将导致资源释放的错误以及内存破坏。

##### 示例，不好

    void f(int n)
    {
        auto p = new X[n];   // n 个默认初始化的 X
        // ...
        delete p;   // 错误: 仅仅删除了对象 p，而并未删除数组 p[]
    }

##### 注解

这个例子不仅像上前一个例子一样违反了[禁止裸 `new` 规则](#Res-new)，它还有更多的问题。

##### 强制实施

- 如果 `new` 和 `delete` 在同一个作用域中的话，就可以标记出现错误。
- 如果 `new` 和 `delete` 出现在构造函数/析构函数对之中的话，就可以标记出现错误。

### <a name="Res-arr2"></a>ES.62: 不要在不同的数组之间进行指针比较

##### 理由

这样做的结果是未定义的。

##### 示例，不好

    void f()
    {
        int a1[7];
        int a2[9];
        if (&a1[5] < &a2[7]) {}       // 不好: 未定义
        if (0 < &a1[5] - &a2[7]) {}   // 不好: 未定义
    }

##### 注解

这个例子中有许多问题。

##### 强制实施

???

### <a name="Res-slice"></a>ES.63: 不要产生切片

##### 理由

切片——亦即使用赋值或初始化而只对对象的一部分进行复制——通常会导致错误，
这是因为对象总是被当成是一个整体。
在罕见的进行蓄意的切片的代码中，其代码会让人意外。

##### 示例

    class Shape { /* ... */ };
    class Circle : public Shape { /* ... */ Point c; int r; };

    Circle c {{0, 0}, 42};
    Shape s {c};    // 仅复制构造了 Circle 中的 Shape 部分
    s = c;          // 仅复制赋值了 Circle 中的 Shape 部分

    void assign(const Shape& src, Shape& dest)
    {
        dest = src;
    }
    Circle c2 {{1, 1}, 43};
    assign(c, c2);   // 噢，传递的并不是整个状态
    assert(c == c2); // 如果提供复制操作，就也得提供比较操作，
                     //   但这里很可能返回 false

这样的结果是无意义的，因为不会把中心和半径从 `c` 复制给 `s`。
针对这个的第一条防线是[将基类 `Shape` 定义为不允许这样做](#Rc-copy-virtual)。

##### 替代方案

如果确实需要切片的话，应当为之定义一个明确的操作。
这会避免读者产生混乱。
例如：

    class Smiley : public Circle {
        public:
        Circle copy_circle();
        // ...
    };

    Smiley sm { /* ... */ };
    Circle c1 {sm};  // 理想情况下由 Circle 的定义所禁止
    Circle c2 {sm.copy_circle()};

##### 强制实施

针对切片给出警告。

### <a name="Res-construct"></a>ES.64: 使用 `T{e}` 写法来进行构造

##### 理由

对象构造语法 `T{e}` 明确了所需进行的构造。
对象构造语法 `T{e}` 不允许发生窄化。
`T{e}` 是唯一安全且通用的由表达式 `e` 构造一个 `T` 类型的值的表达式。
强制转换的写法 `T(e)` 和 `(T)e` 既不安全也不通用。

##### 示例

对于内建类型，构造的写法保护了不发生窄化和重解释

    void use(char ch, int i, double d, char* p, long long lng)
    {
        int x1 = int{ch};     // OK，但多余
        int x2 = int{d};      // 错误：double->int 窄化；如果需要的话应使用强制转换
        int x3 = int{p};      // 错误：指针->int；如果确实需要的话应使用 reinterpret_cast
        int x4 = int{lng};    // 错误：long long->int 窄化；如果需要的话应使用强制转换

        int y1 = int(ch);     // OK，但多余
        int y2 = int(d);      // 不好：double->int 窄化；如果需要的话应使用强制转换
        int y3 = int(p);      // 不好：指针->int；如果确实需要的话应使用 reinterpret_cast
        int y4 = int(lng);    // 不好：long long->int 窄化；如果需要的话应使用强制转换

        int z1 = (int)ch;     // OK，但多余
        int z2 = (int)d;      // 不好：double->int 窄化；如果需要的话应使用强制转换
        int z3 = (int)p;      // 不好：指针->int；如果确实需要的话应使用 reinterpret_cast
        int z4 = (int)lng;    // 不好：long long->int 窄化；如果需要的话应使用强制转换
    }

整数和指针之间的转换，在使用 `T(e)` 和 `(T)e` 时是由实现定义的，
而且在不同整数和指针大小的平台之间不可移植。

##### 注解

[避免强制转换](#Res-casts)（显式类型转换），如果必须要做的话[优先采用具名强制转换](#Res-casts-named)。

##### 注解

当没有歧义时，可以不写 `T{e}` 中的 `T`。

    complex<double> f(complex<double>);

    auto z = f({2*pi,1});

##### 注解

对象构造语法是最通用的[初始化式语法](#Res-list)。

##### 例外

`std::vector` 和其他的容器是在 `{}` 作为对象构造语法之前定义的。
考虑：

    vector<string> vs {10};                           // 十个空字符串
    vector<int> vi1 {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};  // 十个元素 1..10
    vector<int> vi2 {10};                             // 一个值为 10 的元素

如何得到包含十个默认初始化的 `int` 的 `vector`？

    vector<int> v3(10); // 十个值为 0 的元素

使用 `()` 而不是 `{}` 作为元素数量是一种约定（源于 1980 年代早期），很难改变，
但仍然是一个设计错误：对于元素类型与元素数量可能发生混淆的容器，必须解决
其中的歧义。
约定的方案是将 `{10}` 解释为单个元素的列表，而用 `(10)` 来指定大小。

不应当在新代码中重复这个错误。
可以定义一个类型来表示元素的数量：

    struct Count { int n; };

    template<typename T>
    class Vector {
    public:
        Vector(Count n);                     // n 个默认构造的元素
        Vector(initializer_list<T> init);    // init.size() 个元素
        // ...
    };

    Vector<int> v1{10};
    Vector<int> v2{Count{10}};
    Vector<Count> v3{Count{10}};    // 这里仍有一个很小的问题

剩下的主要问题就是为 `Count` 找个合适的名字了。

##### 强制实施

标记 C 风格的 `(T)e` 和函数式风格的 `T(e)` 强制转换。

### <a name="Res-deref"></a>ES.65: 不要解引用无效指针

##### 理由

解引用如 `nullptr` 这样的无效指针是未定义的行为，通常会导致程序立刻崩溃，
产生错误结果，或者内存被损坏。

##### 注解

本条规则是显然并且广为知晓的语言规则，但其可能难于遵守。
这需要良好的编码风格，程序库支持，以及静态分析来消除违反情况而不耗费大量开销。
这正是 [C++ 的类型和资源安全性模型](#Stroustrup15)中所讨论的主要部分。

**参见**：

- 使用 [RAII](#Rr-raii) 以避免生存期问题。
- 使用 [unique_ptr](#Rf-unique_ptr) 以避免生存期问题。
- 使用 [shared_ptr](#Rf-shared_ptr) 以避免生存期问题。
- 当不可能出现 `nullptr` 时应使用[引用](#Rf-ptr-ref)。
- 使用 [not_null](#Rf-nullptr) 以尽早捕捉到预期外的 `nullptr`。
- 使用[边界剖面配置](#SS-bounds)以避免范围错误。

##### 示例

    void f()
    {
        int x = 0;
        int* p = &x;

        if (condition()) {
            int y = 0;
            p = &y;
        } // p 失效

        *p = 42;            // 不好，若走了上面的分支则 p 无效
    }

为解决这个问题，要么应当扩展指针打算指代的这个对象的生存期，要么应当缩短指针的生存期（将解引用移动到所指代的对象生存期结束之前进行）。

    void f1()
    {
        int x = 0;
        int* p = &x;

        int y = 0;
        if (condition()) {
            p = &y;
        }

        *p = 42;            // OK，p 可指向 x 或 y，而它们都仍在作用域中
    }

不幸的是，大多数无效指针问题都更加难于定位且更加难于解决。

##### 示例

    void f(int* p)
    {
        int x = *p; // 不好：如何确定 p 是否有效？
    }

有大量的这种代码存在。
它们大多数都能工作（经过了大量的测试），但其各自是很难确定 `p` 是否可能为 `nullptr` 的。
后果就是，这同样是错误的一大来源。
有许多方案试图解决这个潜在问题：

    void f1(int* p) // 处理 nullptr
    {
        if (!p) {
            // 处理 nullptr（分配，返回，抛出，使 p 指向什么，等等
        }
        int x = *p;
    }

测试 `nullptr` 的做法有两个潜在的问题：

- 当遇到 `nullptr` 时应当做什么并不总是明确的
- 其测试可能多余并且相对比较昂贵
- 这个测试是为了保护某种违例还是所需逻辑的一部分并不明显

<!-- comment needed for code block after list -->

    void f2(int* p) // 声称 p 不应当为 nullptr
    {
        Assert(p);
        int x = *p;
    }

这样，仅当打开了断言检查时才会有所耗费，而且会向编译器/分析器提供有用的信息。
当 C++ 出现契约的直接支持后，还可以做的更好：

    void f3(int* p) // 声称 p 不应当为 nullptr
        [[expects: p]]
    {
        int x = *p;
    }

或者，还可以使用 `gsl::not_null` 来保证 `p` 不为 `nullptr`。

    void f(not_null<int*> p)
    {
        int x = *p;
    }

这些只是关于 `nullptr` 的处理办法。
要知道还有其他出现无效指针的方式。

##### 示例

    void f(int* p)  // 老代码，没使用 owner
    {
        delete p;
    }

    void g()        // 老代码：使用了裸 new
    {
        auto q = new int{7};
        f(q);
        int x = *q; // 不好：解引用了无效指针
    }

##### 示例

    void f()
    {
        vector<int> v(10);
        int* p = &v[5];
        v.push_back(99); // 可能重新分配 v 中的元素
        int x = *p; // 不好：解引用了潜在的无效指针
    }

##### 强制实施

本条规则属于[生存期安全性剖面配置](#SS-lifetime)

- 当对指向已经超出作用域的对象的指针进行解引用时进行标记
- 当对可能已经通过赋值 `nullptr` 而无效的指针进行解引用时进行标记
- 当对可能已经因 `delete` 而无效的指针进行解引用时进行标记
- 当对指向可能已经失效的容器元素的指针进行解引用时进行标记

## ES.stmt: 语句

语句控制了控制的流向（除了函数调用和异常抛出，它们是表达式）。

### <a name="Res-switch-if"></a>ES.70: 面临选择时，优先采用 `switch` 语句而不是 `if` 语句

##### 理由

- 可读性。
- 效率：`switch` 与常量进行比较，且通常比一个 `if`-`then`-`else` 链中的一系列测试获得更好的优化。
- `switch` 可以启用某种启发式的一致性检查。例如，是否某个 `enum` 的所有值都被覆盖了？如果没有的话，是否存在 `default`？

##### 示例

    void use(int n)
    {
        switch (n) {   // 好
        case 0:
            // ...
            break;
        case 7:
            // ...
            break;
        default:
            // ...
            break;
        }
    }

要好于：

    void use2(int n)
    {
        if (n == 0)   // 不好：以 if-then-else 链和一组常量进行比较
            // ...
        else if (n == 7)
            // ...
    }

##### 强制实施

对以 `if`-`then`-`else` 链条（仅）和常量进行比较的情况进行标记。

### <a name="Res-for-range"></a>ES.71: 面临选择时，优先采用范围式 `for` 语句而不是普通 `for` 语句

##### 理由

可读性。避免错误。效率。

##### 示例

    for (gsl::index i = 0; i < v.size(); ++i)   // 不好
        cout << v[i] << '\n';

    for (auto p = v.begin(); p != v.end(); ++p)   // 不好
        cout << *p << '\n';

    for (auto& x : v)    // OK
        cout << x << '\n';

    for (gsl::index i = 1; i < v.size(); ++i) // 接触了两个元素：无法作为范围式的 for
        cout << v[i] + v[i - 1] << '\n';

    for (gsl::index i = 0; i < v.size(); ++i) // 可能具有副作用：无法作为范围式的 for
        cout << f(v, &v[i]) << '\n';

    for (gsl::index i = 0; i < v.size(); ++i) { // 循环体中混入了循环变量：无法作为范围式 for
        if (i % 2 != 0)
            cout << v[i] << '\n'; // 输出奇数元素
    }

人类或优良的静态分析器也许可以确定，其实在 `f(v, &v[i])` 中的 `v` 的上并不真的存在副作用，因此这个循环可以被重写。

在循环体中“混入循环变量”的情况通常是最好进行避免的。

##### 注解

不要在范围式 `for` 循环中使用昂贵的循环变量副本：

    for (string s : vs) // ...

这将会对 `vs` 中的每个元素复制给 `s`。这样好一点：

    for (string& s : vs) // ...

更好的做法是，当循环变量不会被修改或复制时：

    for (const string& s : vs) // ...

##### 强制实施

查看循环，如果一个传统的循环仅会查看序列中的各个元素，而且其对这些元素所做的事中没有发生副作用，则将该循环重写为范围式的 `for` 循环。

### <a name="Res-for-while"></a>ES.72: 当存在显然的循环变量时，优先采用 `for` 语句而不是 `while` 语句

##### 理由

可读性：循环的全部逻辑都“直观可见”。循环变量的作用域是有限的。

##### 示例

    for (gsl::index i = 0; i < vec.size(); i++) {
        // 干活
    }

##### 示例，不好

    int i = 0;
    while (i < vec.size()) {
        // 干活
        i++;
    }

##### 强制实施

???

### <a name="Res-while-for"></a>ES.73: 当没有显然的循环变量时，优先采用 `while` 语句而不是 `for` 语句

##### 理由

可读性。

##### 示例

    int events = 0;
    for (; wait_for_event(); ++events) {  // 不好，含糊
        // ...
    }

这个“事件循环”会误导人，计数器 `events` 跟循环条件（`wait_for_event()`）并没有任何关系。
更好的做法是

    int events = 0;
    while (wait_for_event()) {      // 更好
        ++events;
        // ...
    }

##### 强制实施

对和 `for` 的条件不相关的 `for` 初始化式和 `for` 增量部分进行标记。

### <a name="Res-for-init"></a>ES.74: 优先在 `for` 语句的初始化部分中声明循环变量

参见 [ES.6](#Res-cond)

### <a name="Res-do"></a>ES.75: 避免使用 `do` 语句

##### 理由

可读性，避免错误。
其终止条件处于尾部（而这可能会被忽略），且其条件不会在第一时间进行检查。

##### 示例

    int x;
    do {
        cin >> x;
        // ...
    } while (x < 0);

##### 注解

确实有一些天才的例子中，`do` 语句是更简洁的方案，但有问题的更多。

##### 强制实施

标记 `do` 语句。

### <a name="Res-goto"></a>ES.76: 避免 `goto`

##### 理由

可读性，避免错误。存在对于人类更好的控制结构；`goto` 是用于机器生成的代码的。

##### 例外

跳出嵌套循环。
这种情况下应当总是向前跳出。

    for (int i = 0; i < imax; ++i)
        for (int j = 0; j < jmax; ++j) {
            if (a[i][j] > elem_max) goto finished;
            // ...
        }
    finished:
    // ...

##### 示例，不好

有相当数量的代码采用 C 风格的 goto-exit 惯用法：

    void f()
    {
        // ...
            goto exit;
        // ...
            goto exit;
        // ...
    exit:
        // ... 公共的清理代码 ...
    }

这是对析构函数的一种专门模仿。
应当将资源声明为带有清理的析构函数的包装类。
如果你由于某种原因无法用析构函数来处理所使用的各个变量的清理工作，
请考虑用 `gsl::finally()` 作为 `goto exit` 的一种简洁且更加可靠的替代方案。

##### 强制实施

- 标记 `goto`。更好的做法是标记出除了从嵌套内层循环中跳出到紧跟一组嵌套循环之后的语句的 `goto` 以外的所有 `goto`。

### <a name="Res-continue"></a>ES.77: 尽量减少循环中使用的 `break` 和 `continue`

##### 理由

在不平凡的循环体中，容易忽略掉 `break` 或 `continue`。

循环中的 `break` 和 `switch` 语句中的 `break` 的含义有很大的区别，
（而且循环中可以有 `switch` 语句，`switch` 的 `case` 中也可以有循环）。

##### 示例

    switch(x) {
    case 1 :
        while (/* 某种条件 */) {
            // ...
        break;
        } // 噢！打算 break switch 还是 break while？
    case 2 :
        // ...
        break;
    }

##### 替代方案

通常，需要 `break` 的循环都是作为一个函数（算法）的良好候选者，其 `break` 将会变为 `return`。

    // 原始代码：break 内部循环
    void use1()
    {
        std::vector<T> vec = {/* 初始化为一些值 */};
        T value;
        for (const T item : vec) {
            if (/* 某种条件 */) {
                value = item;
                break;
            }
        }
        /* 然后对 value 做些事 */
    }

    // 这样更好：创建一个函数使其从循环中返回
    T search(const std::vector<T> &vec)
    {
        for (const T &item : vec) {
            if (/* 某种条件 */) return item;
        }
        return T(); // 默认值
    }

    void use2()
    {
        std::vector<T> vec = {/* 初始化为一些值 */};
        T value = search(vec);
        /* 然后对 value 做些事 */
    }

通常，使用 `continue` 的循环都可以等价且同样简洁地用 `if` 语句来表达。

    for (int item : vec) { // 不好
        if (item%2 == 0) continue;
        if (item == 5) continue;
        if (item > 10) continue;
        /* 对 item 做些事 */
    }

    for (int item : vec) { // 好
        if (item%2 != 0 && item != 5 && item <= 10) {
            /* 对 item 做些事 */
        }
    }

##### 注解

如果你确实要打断一个循环，使用 `break` 通常比使用诸如[修改循环变量](#Res-loop-counter)或 [`goto`](#Res-goto) 等其他方案更好：

##### 强制实施

???

### <a name="Res-break"></a>ES.78: 不要依靠 `switch` 语句中的隐含直落行为

##### 理由

总是以 `break` 来结束非空的 `case`。意外地遗漏 `break` 是一种相当常见的 BUG。
蓄意的控制直落（fall through）是维护的噩梦，应该罕见并被明确标示出来。

##### 示例

    switch (eventType) {
    case Information:
        update_status_bar();
        break;
    case Warning:
        write_event_log();
        // 不好 - 隐式的控制直落
    case Error:
        display_error_window();
        break;
    }

单个语句带有多个 `case` 标签是可以的：

    switch (x) {
    case 'a':
    case 'b':
    case 'f':
        do_something(x);
        break;
    }

在 `case` 标签中使用返回语句也是可以的：

    switch (x) {
    case 'a':
        return 1;
    case 'b':
        return 2;
    case 'c':
        return 3;
    }

##### 例外

在罕见的直落被视为合适行为的情况中。应当明确标示，并使用 `[[fallthrough]]` 标注：

    switch (eventType) {
    case Information:
        update_status_bar();
        break;
    case Warning:
        write_event_log();
        [[fallthrough]];
    case Error:
        display_error_window();
        break;
    }

##### 注解

##### 强制实施

对所有从非空的 `case` 隐式发生的直落进行标记。

### <a name="Res-default"></a>ES.79: `default`（仅）用于处理一般情况

##### 理由

代码清晰性。
提升错误检测的机会。

##### 示例

    enum E { a, b, c, d };

    void f1(E x)
    {
        switch (x) {
        case a:
            do_something();
            break;
        case b:
            do_something_else();
            break;
        default:
            take_the_default_action();
            break;
        }
    }

此处很明显有一种默认的动作，而情况 `a` 和 `b` 则是特殊情况。

##### 示例

不过当不存在默认动作而只想处理特殊情况时怎么办呢？
这种情况下，应当使用空的 `default`，否则没办法知道你确实处理了所有情况：

    void f2(E x)
    {
        switch (x) {
        case a:
            do_something();
            break;
        case b:
            do_something_else();
            break;
        default:
            // 其他情况无需动作
            break;
        }
    }

如果没有 `default` 的话，维护者以及编译器可能会合理地假定你有意处理所有情况：

    void f2(E x)
    {
        switch (x) {
        case a:
            do_something();
            break;
        case b:
        case c:
            do_something_else();
            break;
        }
    }

你是忘记了情况 `d` 还是故意遗漏了它？
当有人向枚举中添加一种情况，而又未能对每个针对这些枚举符的 `switch` 中添加时，
容易出现这种遗忘 `case` 的情况。

##### 强制实施

针对某个枚举的 `switch` 语句，若其未能处理其所有枚举符且没有 `default`，则对其进行标记。
这样做对于某些代码库可能会产生大量误报；此时，可以仅标记那些处理了大多数情况而不是所有情况的 `switch` 语句
（这正是第一个 C++ 编译器曾经的策略）。

### <a name="Res-noname"></a>ES.84: 不要试图声明没有名字的局部变量

##### 理由

没有这种东西。
我们眼里看起来像是个无名变量的东西，对于编译器来说是一条由一个将会立刻离开作用域的临时对象所组成的语句。

##### 示例，不好

    void f()
    {
        lock_guard<mutex>{mx};   // 不好
        // ...
    }

这里声明了一个无名的 `lock_guard` 对象，它将在分号处立刻离开作用域。
这并不是一种少见的错误。
特别是，这个特别的例子会导致很难发觉的竞争条件。

##### 注解

无名函数实参是没问题的。

##### 强制实施

标记出仅有临时对象的语句。

### <a name="Res-empty"></a>ES.85: 让空语句显著可见

##### 理由

可读性。

##### 示例

    for (i = 0; i < max; ++i);   // 不好: 空语句很容易被忽略
    v[i] = f(v[i]);

    for (auto x : v) {           // 好多了
        // 空
    }
    v[i] = f(v[i]);

##### 强制实施

对并非块语句且不包含注释的空语句进行标记。

### <a name="Res-loop-counter"></a>ES.86: 避免在原生的 `for` 循环中修改循环控制变量

##### 理由

循环控制的第一行应当允许对循环中所发生的事情进行正确的推理。同时在循环的重复表达式和循环体之中修改循环计数器，是发生意外和 BUG 的一种经常性来源。

##### 示例

    for (int i = 0; i < 10; ++i) {
        // 未改动 i -- ok
    }

    for (int i = 0; i < 10; ++i) {
        //
        if (/* 某种情况 */) ++i; // 不好
        //
    }

    bool skip = false;
    for (int i = 0; i < 10; ++i) {
        if (skip) { skip = false; continue; }
        //
        if (/* 某种情况 */) skip = true;  // 有改善: 为两个概念使用了两个变量。
        //
    }

##### 强制实施

如果变量在循环控制的重复表达式和循环体中都潜在地进行更新（存在非 `const` 使用），则进行标记。

### <a name="Res-if"></a>ES.87: 请勿在条件上添加多余的 `==` 或 `!=`

##### 理由

这样可避免啰嗦，并消除了发生某些错误的机会。
有助于使代码风格保持一致性和协调性。

##### 示例

根据定义，`if` 语句，`while` 语句，以及 `for` 语句中的条件，选择 `true` 或 `false` 的取值。
数值与 `0` 相比较，指针值与 `nullptr` 相比较。

    // 这些都表示“当 p 不是 nullptr 时”
    if (p) { ... }            // 好
    if (p != 0) { ... }       // !=0 是多余的；不好：不要对指针用 0
    if (p != nullptr) { ... } // !=nullptr 是多余的，不建议如此

通常，`if (p)` 可解读为“如果 `p` 有效”，这正是程序员意图的直接表达，
而 `if (p != nullptr)` 则只是一种啰嗦的变通写法。

##### 示例

这条规则对于把声明式用作条件时尤其有用

    if (auto pc = dynamic_cast<Circle>(ps)) { ... } // 执行是按照 ps 指向某种 Circle 来进行的，好

    if (auto pc = dynamic_cast<Circle>(ps); pc != nullptr) { ... } // 不建议如此

##### 示例

要注意，条件中会实施向 `bool` 的隐式转换。
例如：

    for (string s; cin >> s; ) v.push_back(s);

这里会执行 `istream` 的 `operator bool()`。

##### 注解

明确地将整数和 `0` 进行比较通常并非是多余的。
因为（与指针和布尔值相反），整数通常都具有超过两个的有效值。
此外 `0`（零）还经常会用于代表成功。
因此，最好明确地进行比较。

    void f(int i)
    {
        if (i)            // 可疑
        // ...
        if (i == success) // 可能更好
        // ...
    }

一定要记住整数可以有超过两个值。

##### 示例，不好

众所周知，

    if(strcmp(p1, p2)) { ... }   // 这两个 C 风格的字符串相等吗？（错误！）

是一种常见的新手错误。
如果使用 C 风格的字符串，那么就必须好好了解 `<cstring>` 中的函数。
即便冗余地写为

    if(strcmp(p1, p2) != 0) { ... }   // 这两个 C 风格的字符串相等吗？（错误！）

也不会有效果。

##### 注解

表达相反的条件的最简单的方式就是使用一次取反：

    // 这些都表示“当 p 为 nullptr 时”
    if (!p) { ... }           // 好
    if (p == 0) { ... }       // ==0 是多余的；不好：不要对指针用 0
    if (p == nullptr) { ... } // ==nullptr 是多余的，不建议如此

##### 强制实施

容易，仅需检查条件中多余的 `!=` 和 `==` 的使用即可。

## <a name="SS-numbers"></a>算术

### <a name="Res-mix"></a>ES.100: 不要进行有符号和无符号混合运算

##### 理由

避免错误的结果。

##### 示例

    int x = -3;
    unsigned int y = 7;

    cout << x - y << '\n';  // 无符号结果，可能是 4294967286
    cout << x + y << '\n';  // 无符号结果：4
    cout << x * y << '\n';  // 无符号结果，可能是 4294967275

在更实际的例子中，这种问题更难于被发现。

##### 注解

不幸的是，C++ 使用有符号整数作为数组下标，而标准库使用无符号整数作为容器下标。
这妨碍了一致性。使用 `gsl::index` 来作为下标类型；[参见 ES.107](#Res-subscripts)。

##### 强制实施

- 编译器已知这种情况，有些时候会给出警告。
- （避免噪声）有符号/无符号的混合比较，若其一个实参是 `sizeof` 或调用容器的 `.size()` 而另一个是 `ptrdiff_t`，则不要进行标记。

### <a name="Res-unsigned"></a>ES.101: 使用无符号类型进行位操作

##### 理由

无符号类型支持位操作而没有符号位的意外。

##### 示例

    unsigned char x = 0b1010'1010;
    unsigned char y = ~x;   // y == 0b0101'0101;

##### 注解

无符号类型对于模算术也很有用。
不过，如果你想要进行模算术时，
应当按需添加代码注释以注明所依赖的回绕行为，因为这样的
代码会让许多程序员感觉意外。

##### 强制实施

- 一般来说基本不可能，因为标准库也使用了无符号下标。
  ???

### <a name="Res-signed"></a>ES.102: 使用有符号类型进行算术运算

##### 理由

因为大多数算术都假定是有符号的；
当 `y > x` 时，`x - y` 都会产生负数，除了罕见的情况下你确实需要模算术。

##### 示例

当你不期望时，无符号算术会产生奇怪的结果。
这在混合有符号和无符号算术时有其如此。

    template<typename T, typename T2>
    T subtract(T x, T2 y)
    {
        return x - y;
    }

    void test()
    {
        int s = 5;
        unsigned int us = 5;
        cout << subtract(s, 7) << '\n';       // -2
        cout << subtract(us, 7u) << '\n';     // 4294967294
        cout << subtract(s, 7u) << '\n';      // -2
        cout << subtract(us, 7) << '\n';      // 4294967294
        cout << subtract(s, us + 2) << '\n';  // -2
        cout << subtract(us, s + 2) << '\n';  // 4294967294
    }

我们这次非常明确发生了什么。
但要是你见到 `us - (s + 2)` 或者 `s += 2; ...; us - s` 时，你确实能够预计到打印的结果将是 `4294967294` 吗？

##### 例外

如果你确实需要模算术的话就使用无符号类型——
根据需要为其添加代码注释以说明其依赖溢出行为，因为这样的
代码会让许多程序员感觉意外。

##### 示例

标准库使用无符号类型作为下标。
内建数组则用有符号类型作为下标。
这不可避免地带来了意外（以及 BUG）。

    int a[10];
    for (int i = 0; i < 10; ++i) a[i] = i;
    vector<int> v(10);
    // 比较有符号和无符号数；有些编译器会警告，但我们不能警告
    for (gsl::index i = 0; i < v.size(); ++i) v[i] = i;

    int a2[-2];         // 错误：负的大小

    // OK，但 int 的数值（4294967294）过大，应当会造成一个异常
    vector<int> v2(-2);

使用 `gsl::index` 作为下标类型；[参见 ES.107](#Res-subscripts)。

##### 强制实施

- 对混合有符号和无符号算术进行标记。
- 对将无符号算术的结果作为有符号数赋值或打印进行标记。
- 对负数字面量（比如 `-2`）用作容器下标进行标记。
- （避免噪声）有符号/无符号的混合比较，若其一个实参是 `sizeof` 或调用容器的 `.size()` 而另一个是 `ptrdiff_t`，则不要进行标记。

### <a name="Res-overflow"></a>ES.103: 避免上溢出

##### 理由

上溢出通常会让数值算法变得没有意义。
将值增加超过其最大值将导致内存损坏和未定义的行为。

##### 示例，不好

    int a[10];
    a[10] = 7;   // 不好，数组边界上溢出

    for (int n = 0; n <= 10; ++n)
        a[n] = 9;   // 不好，数组边界上溢出

##### 示例，不好

    int n = numeric_limits<int>::max();
    int m = n + 1;   // 不好，数值上溢出

##### 示例，不好

    int area(int h, int w) { return h * w; }

    auto a = area(10'000'000, 100'000'000);   // 不好，数值上溢出

##### 例外

如果你确实需要模算术的话就使用无符号类型。

**替代方案**: 对于可以负担一些开销的关键应用，可以使用带有范围检查的整数和/或浮点类型。

##### 强制实施

???

### <a name="Res-underflow"></a>ES.104: 避免下溢出

##### 理由

将值减小超过其最小值将导致内存损坏和未定义的行为。

##### 示例，不好

    int a[10];
    a[-2] = 7;   // 不好

    int n = 101;
    while (n--)
        a[n - 1] = 9;   // 不好（两次）

##### 例外

如果你确实需要模算术的话就使用无符号类型。

##### 强制实施

???

### <a name="Res-zero"></a>ES.105: 避免除整数零

##### 理由

其结果是未定义的，很可能导致程序崩溃。

##### 注解

这同样适用于 `%`。

##### 示例，不好

    int divide(int a, int b)
    {
        // 不好, 应当进行检查（比如一条前条件）
        return a / b;
    }

##### 示例，好

    int divide(int a, int b)
    {
        // 好, 通过前条件进行处置（并当 C++ 支持契约后可以进行替换）
        Expects(b != 0);
        return a / b;
    }

    double divide(double a, double b)
    {
        // 好, 通过换为使用 double 来处置
        return a / b;
    }

**替代方案**: 对于可以负担一些开销的关键应用，可以使用带有范围检查的整数和/或浮点类型。

##### 强制实施

- 对以可能为零的整型值的除法进行标记。

### <a name="Res-nonnegative"></a>ES.106: 不要试图用 `unsigned` 来防止负数值

##### 理由

选用 `unsigned` 意味着对于包括模算术在内的整数的常规行为的许多改动，
它将抑制掉与溢出有关的警告，
并打开了与混合符号相关的错误的大门。
使用 `unsigned` 并不会真正消除负数值的可能性。

##### 示例

    unsigned int u1 = -2;   // 合法：u1 的值为 4294967294
    int i1 = -2;
    unsigned int u2 = i1;   // 合法：u2 的值为 4294967294
    int i2 = u2;            // 合法：i2 的值为 -2

真实代码中很难找出这样的（完全合法的）语法构造的问题，而它们是许多真实世界错误的来源。
考虑：

    unsigned area(unsigned height, unsigned width) { return height*width; } // [参见](#Ri-expects)
    // ...
    int height;
    cin >> height;
    auto a = area(height, 2);   // 当输入为 -2 时 a 为 4294967292

记住把 `-1` 赋值给 `unsigned int` 会变成最大的 `unsigned int`。
而且，由于无符号算术是模算术，其乘法并不会溢出，而是会发生回绕。

##### 示例

    unsigned max = 100000;    // “不小心写错了”，应该写 10'000
    unsigned short x = 100;
    while (x < max) x += 100; // 无限循环

要是 `x` 是个有符号的 `short` 的话，我们就会得到有关溢出的未定义行为的警告了。

##### 替代方案

- 使用有符号整数并检查 `x >= 0`
- 使用某个正整数类型
- 使用某个整数子值域类型
- `Assert(-1 < x)`

例如

    struct Positive {
        int val;
        Positive(int x) :val{x} { Assert(0 < x); }
        operator int() { return val; }
    };

    int f(Positive arg) { return arg; }

    int r1 = f(2);
    int r2 = f(-2);  // 抛出异常

##### 注解

???

##### 强制实施

参见 ES.100 的强制实施。

### <a name="Res-subscripts"></a>ES.107: 不要对下标使用 `unsigned`，优先使用 `gsl::index`

##### 理由

避免有符号和无符号混乱。
允许更好的优化。
允许更好的错误检测。
避免 `auto` 和 `int` 有关的陷阱。

##### 示例，不好

    vector<int> vec = /*...*/;

    for (int i = 0; i < vec.size(); i += 2)                    // 可能不够大
        cout << vec[i] << '\n';
    for (unsigned i = 0; i < vec.size(); i += 2)               // 有风险的回绕
        cout << vec[i] << '\n';
    for (auto i = 0; i < vec.size(); i += 2)                   // 可能不够大
        cout << vec[i] << '\n';
    for (vector<int>::size_type i = 0; i < vec.size(); i += 2) // 啰嗦
        cout << vec[i] << '\n';
    for (auto i = vec.size()-1; i >= 0; i -= 2)                // BUG
        cout << vec[i] << '\n';
    for (int i = vec.size()-1; i >= 0; i -= 2)                 // 可能不够大
        cout << vec[i] << '\n';

##### 示例，好

    vector<int> vec = /*...*/;

    for (gsl::index i = 0; i < vec.size(); i += 2)             // ok
        cout << vec[i] << '\n';
    for (gsl::index i = vec.size()-1; i >= 0; i -= 2)          // ok
        cout << vec[i] << '\n';

##### 注解

内建数组允许有符号的下标。
标准库容器使用无符号的下标。
因此没有完美的兼容解决方案（除非将来某一天，标准库容器改为使用有符号下标了）。
鉴于无符号和混合符号方面的已知问题，最好坚持使用（有符号）并且足够大的整数，而 `gsl::index` 保证了这点。

##### 示例

    template<typename T>
    struct My_container {
    public:
        // ...
        T& operator[](gsl::index i);    // 不是 unsigned
        // ...
    };

##### 示例

    ??? 演示改进后的代码生成和潜在可进行的错误检查 ???

##### 替代方案

可以代之以

- 使用算法
- 使用基于范围的 `for`
- 使用迭代器或指针

##### 强制实施

- 非常麻烦，因为标准库容器已经搞错了。
- （避免噪声）有符号/无符号的混合比较，若其一个实参是 `sizeof` 或调用容器的 `.size()` 而另一个是 `ptrdiff_t`，则不要进行标记。

# <a name="S-performance"></a>Per: 性能

??? 这一节应该放在主指南中吗 ???

本章节所包含的规则对于需要高性能和低延迟的人们很有价值。
就是说，这些规则是有关如何在可预测的短时段中尽可能使用更短时间和更少资源来完成任务的。
本章节中的规则要比（绝大）多数应用程序所需要的规则更多限制并更有侵入性。
请勿在一般的代码中盲目地尝试遵循这些规则：要达成低延迟的目标是需要进行一些额外工作的。

性能规则概览：

- [Per.1: 请勿进行无理由的优化](#Rper-reason)
- [Per.2: 请勿进行不成熟的优化](#Rper-Knuth)
- [Per.3: 请勿对非性能关键的代码进行优化](#Rper-critical)
- [Per.4: 不能假定复杂代码一定比简单代码更快](#Rper-simple)
- [Per.5: 不能假定低级代码一定比高级代码更快](#Rper-low)
- [Per.6: 请勿不进行测量就作出性能评断](#Rper-measure)
- [Per.7: 设计应当允许优化](#Rper-efficiency)
- [Per.10: 依赖静态类型系统](#Rper-type)
- [Per.11: 把计算从运行时转移到编译期](#Rper-Comp)
- [Per.12: 消除多余的别名](#Rper-alias)
- [Per.13: 消除多余的间接](#Rper-indirect)
- [Per.14: 最小化分配和回收的次数](#Rper-alloc)
- [Per.15: 请勿在关键逻辑分支中进行分配](#Rper-alloc0)
- [Per.16: 使用紧凑的数据结构](#Rper-compact)
- [Per.17: 在时间关键的结构中应当先声明最常用的成员](#Rper-struct)
- [Per.18: 空间即时间](#Rper-space)
- [Per.19: 进行可预测的内存访问](#Rper-access)
- [Per.30: 避免在关键路径中进行上下文切换](#Rper-context)

### <a name="Rper-reason"></a>Per.1: 请勿进行无理由的优化

##### 理由

如果没有必要优化的话，这样做的结果就是更多的错误和更高的维护成本。

##### 注解

一些人作出优化只是出于习惯或者因为感觉这很有趣。

???

### <a name="Rper-Knuth"></a>Per.2: 请勿进行不成熟的优化

##### 理由

经过精心优化的代码通常比未优化的代码更大而且更难修改。

???

### <a name="Rper-critical"></a>Per.3: 请勿对非性能关键的代码进行优化

##### 理由

对程序中并非性能关键的部分进行的优化，对于系统性能是没有效果的。

##### 注解

如果你的程序要耗费大量时间来等待 Web 或人的操作的话，对内存中的计算进行优化可能是没什么用处的。

换个角度来说：如果你的程序花费处理时间的 4% 来
计算 A 而花费 40% 的时间来计算 B，那对 A 的 50% 的改进
其影响只能和 B 的 5% 的改进相比。（如果你甚至不知道
A 或 B 到底花费了多少时间，参见 <a href="#Rper-reason">Per.1</a> 和 <a
href="#Rper-Knuth">Per.2</a>。）

### <a name="Rper-simple"></a>Per.4: 不能假定复杂代码一定比简单代码更快

##### 理由

简单的代码可能会非常快。优化器在简单代码上有时候会发生奇迹。

##### 示例，好

    // 清晰表达意图，快速执行

    vector<uint8_t> v(100000);

    for (auto& c : v)
        c = ~c;

##### 示例，不好

    // 试图更快，但通常会更慢

    vector<uint8_t> v(100000);

    for (size_t i = 0; i < v.size(); i += sizeof(uint64_t)) {
        uint64_t& quad_word = *reinterpret_cast<uint64_t*>(&v[i]);
        quad_word = ~quad_word;
    }

##### 注解

???

???

### <a name="Rper-low"></a>Per.5: 不能假定低级代码一定比高级代码更快

##### 理由

低级代码有时候会妨碍优化。优化器在高级代码上有时候会发生奇迹。

##### 注解

???

???

### <a name="Rper-measure"></a>Per.6: 请勿不进行测量就作出性能评断

##### 理由

性能领域充斥各种错误认识和伪习俗。
现代的硬件和优化器并不遵循这些幼稚的假设；即便是专家也会经常感觉意外。

##### 注解

要进行高质量的性能测量是很难的，而且需要采用专门的工具。

##### 注解

有些使用了 Unix 的 `time` 或者标准库的 `<chrono>` 的简单的微基准测量，有助于打破大多数明显的错误认识。
如果确实无法精确地测量完整系统的话，至少也要尝试对一些关键操作和算法进行测量。
性能剖析可以帮你发现系统的哪些部分是性能关键的。
你很可能会感觉意外。

???

### <a name="Rper-efficiency"></a>Per.7: 设计应当允许优化

##### 理由

因为我们经常需要对最初的设计进行优化。
因为忽略后续改进的可能性的设计是很难修改的。

##### 示例

来自 C（以及 C++）的标准：

    void qsort (void* base, size_t num, size_t size, int (*compar)(const void*, const void*));

什么情况会需要对内存进行排序呢？
时即上，我们需要对元素序列进行排序，通常它们存储于容器之中。
对 `qsort` 的调用抛弃了许多有用的信息（比如元素的类型），强制用户对其已知的信息
进行重复（比如元素的大小），并强制用户编写额外的代码（比如用于比较 `double` 的函数）。
这蕴含了程序员工作量的增加，易错，并剥夺了编译器为优化所需的信息。

    double data[100];
    // ... 填充 a ...

    // 对从地址 data 开始的 100 块 sizeof(double) 大小
    // 的内存，用由 compare_doubles 所定义的顺序进行排序
    qsort(data, 100, sizeof(double), compare_doubles);

从接口设计的观点来看，`qsort` 抛弃了有用的信息。

这样做可以更好（C++98）：

    template<typename Iter>
        void sort(Iter b, Iter e);  // sort [b:e)

    sort(data, data + 100);

这里，我们利用了编译器关于数组大小，元素类型，以及如何对 `double` 进行比较的知识。

而以 C++20 的话，我们还可以做得更好：

    // sortable 指定了 c 必须是一个
    // 可以用 < 进行比较的元素的随机访问序列
    void sort(sortable auto& c);

    sort(c);

其中的关键在于传递充分的信息以便能够选择一个好的实现。
这里给出的几个 `sort` 接口仍然有一个缺憾：
它们隐含地依赖于元素类型定义了小于（`<`）运算符。
为使接口完整，我们需要另一个接受比较准则的版本：

    // 用 r 比较 c 的元素
    template<random_access_range R, class C> requires sortable<R, C>
    void sort(R&& r, C c);

`sort` 的标准库规范提供了这两个版本和其他版本。

##### 注解

不成熟的优化被称为[一切罪恶之源](#Rper-Knuth)，但这并不是轻视性能的理由。
考虑如何使设计可以为改进而进行修正绝不是不成熟的，而性能改进则是一种常见的改进要求。
我们的目标是建立一组习惯，使缺省情况就能得到高效，可维护，且可优化的代码。
特别是，当你编写并非一次性实现细节的函数时，应当考虑

- 信息传递：
  优先采用能够为后续的实现改进带来充分信息的简洁[接口](#S-interfaces)。
  要注意信息会通过我们所提供的接口来流入和流出一个实现。
- 紧凑的数据：默认情况[使用紧凑的数据](#Rper-compact)，比如 `std::vector`，并[系统化地进行访问](#Rper-access)。
  如果你觉得需要一种有链接的结构的话，应尝试构造接口使这个结构不会被用户所看到。
- 函数的参数传递和返回：
  对可改变和不可变的数据加以区分。
  不要把资源管理的负担强加给用户。
  不要把假性的间接强加给用户。
  对通过接口传递信息采用[符合惯例的方式](#Rf-conventional)；
  不合惯例的，以及“优化过的”数据传递方式可能会严重影响后续的重新实现。
- 抽象：
  不要过度泛化；视图提供每一种可能用法（和误用），并把每个设计决策都（通过编译时或运行时间接）
  推迟到后面处理的设计，通常是复杂的，膨胀的，难于理解的混乱体。
  应当从具体的例子进行泛化，泛化时要保持性能。
  不要仅仅基于关于未来需求的推测而进行泛化。
  理想情况是零开销泛化。
- 程序库：
  使用带有良好接口的程序库。
  当没有可用的程序库时，就构建自己的，并模仿一个好程序库的接口风格。
  [标准库](#sl-the-standard-library)是寻找模仿的一个好的第一来源。
- 隔离：
  通过将你所选择的接口提供给你的代码来将其和杂乱和老旧风格的代码之间进行隔离。
  这有时候称为为有用或必须但杂乱的代码“提供包装”。
  不要让不良设计“渗入”你的代码中。

##### 示例

考虑：

    template<class ForwardIterator, class T>
    bool binary_search(ForwardIterator first, ForwardIterator last, const T& val);

`binary_search(begin(c), end(c), 7)` 能够得出 `7` 是否在 `c` 之中。
不过，它无法得出 `7` 在何处，或者是否有多于一个 `7`。

有时候仅把最小数量的信息传递回来（如这里的 `true` 或 `false`）是足够的，但一个好的接口会
向调用方传递其所需的信息。因此，标准库还提供了

    template<class ForwardIterator, class T>
    ForwardIterator lower_bound(ForwardIterator first, ForwardIterator last, const T& val);

`lower_bound` 返回第一个匹配元素（如果有）的迭代器，否则返回第一个大于 `val` 的元素的迭代器，找不到这样的元素时，返回 `last`。

不过 `lower_bound` 还是无法为所有用法返回足够的信息，因此标准库还提供了

    template<class ForwardIterator, class T>
    pair<ForwardIterator, ForwardIterator>
    equal_range(ForwardIterator first, ForwardIterator last, const T& val);

`equal_range` 返回迭代器的 `pair`，指定匹配的第一个和最后一个之后的元素。

    auto r = equal_range(begin(c), end(c), 7);
    for (auto p = r.first; p != r.second; ++p)
        cout << *p << '\n';

显然，这三个接口都是以相同的基本代码实现的。
它们不过是将基本的二叉搜索算法表现给用户的三种方式，
包含从最简单（“让简单的事情简单！”）
到返回完整但不总是必要的信息（“不要隐藏有用的信息”）。
自然，构造这样一组接口是需要经验和领域知识的。

##### 注解

接口的构建不要仅匹配你能想到的第一种实现和第一种用例。
一旦第一个初始实现完成后，应当进行复审；一旦它被部署出去，就很难对错误进行补救了。

##### 注解

对效率的需求并不意味着对[底层代码](#Rper-low)的需求。
高层代码并不必然缓慢或膨胀。

##### 注解

事物都是有成本的。
不要对成本过于偏执（当代计算机真的非常快），
但需要对你所使用的东西的成本的数量级有大致的概念。
例如，应当对
一次内存访问，
一次函数调用，
一次字符串比较，
一次系统调用，
一次磁盘访问，
以及一个通过网络的消息的成本有大致的概念。

##### 注解

如果你只能想到一种实现的话，可能你并没有某种能够设计一个稳定接口的东西。
有可能它只不过是实现细节——不是每段代码都需要一个稳定接口——停下来想一想。
一个有用的问题是
“如果这个操作需要用多个线程实现的话，需要什么样的接口呢？向量化？”

##### 注解

这条规则并不抵触[不要进行不成熟的优化](#Rper-Knuth)规则。
它对其进行了补充，鼓励程序员在有必要的时候，使后续的——适当并且成熟的——优化能够进行。

##### 强制实施

很麻烦。
也许查找 `void*` 函数参数能够找到妨碍后续优化的接口的例子。

### <a name="Rper-type"></a>Per.10: 依赖静态类型系统

##### 理由

类型违规，弱类型（比如 `void*`），以及低级代码（比如把序列当作独立字节进行操作）等会让优化器的工作变得困难很多。简单的代码通常比手工打造的复杂代码能够更好地优化。

???

### <a name="Rper-Comp"></a>Per.11: 把计算从运行时转移到编译期

##### 理由

减少代码大小和运行时间。
通过使用常量来避免数据竞争。
编译时捕获错误（并因而消除错误处理代码）。

##### 示例

    double square(double d) { return d*d; }
    static double s2 = square(2);    // 旧式代码：动态初始化

    constexpr double ntimes(double d, int n)   // 假定 0 <= n
    {
            double m = 1;
            while (n--) m *= d;
            return m;
    }
    constexpr double s3 {ntimes(2, 3)};  // 现代代码：编译期初始化

像 `s2` 的初始化这样的代码并不少见，尤其是比 `square()` 更复杂一些的初始化更是如此。
不过，与 `s3` 的初始化相比，它有两个问题：

- 我们得忍受运行时的一次函数调用的开销
- `s2` 在初始化开始前可能就被某个别的线程访问了。

注意：常量是不可能发生数据竞争的。

##### 示例

考虑一种流行的提供包装类的技术，在包装类自身之中存储小型对象，而把大型对象保存到堆上。

    constexpr int on_stack_max = 20;

    template<typename T>
    struct Scoped {     // 在 Scoped 中存储一个 T
            // ...
        T obj;
    };

    template<typename T>
    struct On_heap {    // 在自由存储中存储一个 T
            // ...
            T* objp;
    };

    template<typename T>
    using Handle = typename std::conditional<(sizeof(T) <= on_stack_max),
                        Scoped<T>,      // 第一种候选
                        On_heap<T>      // 第二种候选
                   >::type;

    void f()
    {
        Handle<double> v1;                   // double 在栈中
        Handle<std::array<double, 200>> v2;  // array 保存到自由存储里
        // ...
    }

假定 `Scoped` 和 `On_heap` 均提供了兼容的用户接口。
这里我们在编译时计算出了最优的类型。
对于选择所要调用的最优函数，也有类似的技术。

##### 注解

理想情况是，*不*试图在编译期执行所有的代码。
显然，大多数的运算都依赖于输入，因而它们没办法挪到编译期进行，
而除了这种逻辑限制外，实际情况是，复杂的编译期运算会严重增加编译时间，
并使调试变得复杂。
甚至编译期运算也可能使得代码变慢。
这种情况确实罕见，但当把一种通用运算分解为一组优化的子运算时，可能会导致指令高速缓存的效率变差。

##### 强制实施

- 找出可以（但尚不）是 constexpr 的简单函数。
- 找出调用时其全部实参均为常量表达式的函数。
- 找出可以为 constexpr 的宏。

### <a name="Rper-alias"></a>Per.12: 消除多余的别名

???

### <a name="Rper-indirect"></a>Per.13: 消除多余的间接

???

### <a name="Rper-alloc"></a>Per.14: 最小化分配和回收的次数

???

### <a name="Rper-alloc0"></a>Per.15: 请勿在关键逻辑分支中进行分配

???

### <a name="Rper-compact"></a>Per.16: 使用紧凑的数据结构

##### 理由

性能通常都是由内存访问次数所决定的。

???

### <a name="Rper-struct"></a>Per.17: 在时间关键的结构中应当先声明最常用的成员

???

### <a name="Rper-space"></a>Per.18: 空间即时间

##### 理由

性能通常都是由内存访问次数所决定的。

???

### <a name="Rper-access"></a>Per.19: 进行可预测的内存访问

##### 理由

性能对于 Cache 的性能非常敏感，而 Cache 算法则更喜欢对相邻数据进行的（通常是线性的）简单访问行为。

##### 示例

    int matrix[rows][cols];

    // 不好
    for (int c = 0; c < cols; ++c)
        for (int r = 0; r < rows; ++r)
            sum += matrix[r][c];

    // 好
    for (int r = 0; r < rows; ++r)
        for (int c = 0; c < cols; ++c)
            sum += matrix[r][c];

### <a name="Rper-context"></a>Per.30: 避免在关键路径中进行上下文切换

???

# <a name="S-concurrency"></a>CP: 并发与并行

我们经常想要我们的计算机同时运行许多任务（或者至少表现为同时运行它们）。
这有许多不同的原因（例如，需要仅用一个处理器等待许多事件，同时处理许多数据流，或者利用大量的硬件设施）
因此也由许多不同的用以表现并发和并行的基本设施。
我们这里将说明使用 ISO 标准 C++ 中用以表现基本并发和并行的设施的原则和规则。

线程是对并发和并行编程支持的机器级别的基础。
使用线程允许互不相关地运行程序的多个部分，
同时共享相同的内存。并发编程很麻烦，
因为在线程之间保护共享的数据说起来容易做起来难。
使现存的单线程代码可以并发执行，
可以通过策略性地添加 `std::async` 或 `std::thread` 这样简单做到，
也可能需要进行完全的重写，这依赖于原始代码是否是以线程友好
的方式编写的。

本文档中的并发/并行规则的设计有三个
目标：

- 有助于编写可以被修改为能够在线程环境中使用
  的代码。
- 展示使用标准库所提供的线程原语的简洁，
  安全的方式。
- 对当并发和并行无法提供所需要的性能增益时应当如何做
  提供指导。

同样重要的一点，是要注意 C++ 的并发仍然是未完成的工作。
C++11 引入了许多核心并发原语，C++14 和 C++17 对它们进行了改进，
而且在使 C++ 编写并发程序更加简单的方面
仍有许多关注。我们预计这里的一些与程序库相关
的指导方针会随着时间有大量的改动。

这一部分需要大量的工作（显然如此）。
请注意我们的规则是从相对非专家们入手的。
真正的专家们还请稍等；
我们欢迎贡献者，
但请为正在努力使他们的并发程序正确且高效的大多数程序员着想。

并发和并行规则概览：

- [CP.1: 假定你的代码将作为多线程程序的一部分而运行](#Rconc-multi)
- [CP.2: 避免数据竞争](#Rconc-races)
- [CP.3: 最小化可写数据的明确共享](#Rconc-data)
- [CP.4: 以任务而不是线程的角度思考](#Rconc-task)
- [CP.8: 不要为同步而使用 `volatile`](#Rconc-volatile)
- [CP.9: 只要可行，就使用工具对并发代码进行验证](#Rconc-tools)

**参见**：

- [CP.con: 并发](#SScp-con)
- [CP.coro: 协程](#SScp-coro)
- [CP.par: 并行](#SScp-par)
- [CP.mess: 消息传递](#SScp-mess)
- [CP.vec: 向量化](#SScp-vec)
- [CP.free: 无锁编程](#SScp-free)
- [CP.etc: 其他并发规则](#SScp-etc)

### <a name="Rconc-multi"></a>CP.1: 假定你的代码将作为多线程程序的一部分而运行

##### 理由

很难说现在或者未来什么时候会不会需要使用并发。
代码是会被重用的。
程序的其他使用了线程的部分可能会使用某个未使用线程的程序库。
请注意这条规则对于程序库代码来说最紧迫，而对独立的应用程序来说则最不紧迫。
不过，久而久之，代码片段可能出现在意想不到的地方。

##### 示例，不好

    double cached_computation(int x)
    {
        // 不好：这些静态变量导致多线程的使用情况中的数据竞争
        static int cached_x = 0.0;
        static double cached_result = COMPUTATION_OF_ZERO;

        if (cached_x != x) {
            cached_x = x;
            cached_result = computation(x);
        }
        return cached_result;
    }

虽然 `cached_computation` 在单线程环境中可以正确工作，但在多线程环境中，其两个 `static` 变量将导致数据竞争进而发生未定义的行为。

##### 示例，好

    struct ComputationCache {
        int cached_x = 0;
        double cached_result = COMPUTATION_OF_ZERO;

        double compute(int x) {
            if (cached_x != x) {
                cached_x = x;
                cached_result = computation(x);
            }
            return cached_result;
        }
    };

这里的缓存作为 `ComputationCache` 对象的成员数据保存，而非共享的静态状态。
这项重构本质上是将关注点委派给了调用方：
单线程程序可能仍会选择采用一个全局的 `ComputationCache` 对象，
而多线程程序可能会为每个线程准备一个 `ComputationCache`，或者为任意定义的“上下文”每个准备一个。
重构后的函数不再试图管理 `cached_x` 的分配。
这方面可以看做是对单一职责原则（SRP）的一次应用。

在这个特定的例子中，为线程安全性进行的重构同样改进了其在单线程程序中的可用性。
不难想象，某个单线程程序可能需要在程序的不同部分中使用两个 `ComputationCache` 实例，
并且它们并不会覆盖掉互相的缓冲数据。

还有其他的一些方法，可以为针对标准多线程环境（就是唯一的并发形式是 `std::thread` 的环境）编写的
代码添加线程安全性：

- 将状态变量标为 `thread_local` 而非 `static`。
- 实现并发控制逻辑，例如，用一个 `static std::mutex` 来保护对这两个 `static` 变量的访问。
- 拒绝在多线程环境中进行构建和/或运行。
- 提供两个实现，一个用在单线程环境中，另一个用在多线程环境中。

##### 例外

永远不会在多线程环境中执行的代码。

要小心的是：有许多例子，“认为”永远不会在多线程程序中执行的代码
却真的在多线程程序中执行了，通常是在多年以后。
一般来说，这种程序将导致进行痛苦的移除数据竞争的工作。
因此，确实有意不在多线程环境中执行的代码，应当清晰地进行标注，而且理想情况下应当利用编译或运行时的强制机制来提早检测到这种使用情况。

### <a name="Rconc-races"></a>CP.2: 避免数据竞争

##### 理由

不这样的话，则任何东西都不保证能工作，而且可能出现微妙的错误。

##### 注解

简而言之，当两个线程并发（未同步）地访问同一个对象，且至少一方为写入方（实施某个非 `const` 操作）时，就会出现数据竞争。
有关如何正确使用同步来消除数据竞争的更多信息，请求教于一本有关并发的优秀书籍（参见 [认真学习文献](#Rconc-literature)）。

##### 示例，不好

有大量存在数据竞争的例子，其中的一些现在这个时候就运行在
产品软件之中。一个非常简单的例子是：

    int get_id()
    {
      static int id = 1;
      return id++;
    }

这里的增量操作就是数据竞争的一个例子。这可能以许多方式导致发生错误，
包括：

- 线程 A 加载 `id` 的值，OS 上下文切换使 A 离开
  一段时间，其中有其他线程创建了上百个 ID。线程 A
  再次允许执行，而 `id` 被写回到那个位置，其值为 A
  所读取的 `id` 值加一。
- 线程 A 和 B 同时加载 `id` 并进行增量。它们都将获得
  相同的 ID。

局部静态变量是数据竞争的一种常见来源。

##### 示例，不好

    void f(fstream& fs, regex pattern)
    {
        array<double, max> buf;
        int sz = read_vec(fs, buf, max);            // 从 fs 读取到 buf 中
        gsl::span<double> s {buf};
        // ...
        auto h1 = async([&] { sort(std::execution::par, s); });     // 产生一个进行排序的任务
        // ...
        auto h2 = async([&] { return find_all(buf, sz, pattern); });   // 产生一个查找匹配的任务
        // ...
    }

这里，在 `buf` 的元素上有一个（很讨厌的）数据竞争（`sort` 既会读取也会写入）。
所有的数据竞争都很讨厌。
我们这里设法在栈上的数据上造成了数据竞争。
不是所有数据竞争都像这个这样容易找出来的。

##### 示例，不好

    // 未用锁进行控制的代码

    unsigned val;

    if (val < 5) {
        // ... 其他线程可能在这里改动 val ...
        switch (val) {
        case 0: // ...
        case 1: // ...
        case 2: // ...
        case 3: // ...
        case 4: // ...
        }
    }

这里，若编译器不知道 `val` 可能改变，它最可能将这个 `switch` 实现为一个有五个项目的跳转表。
然后，超出 `[0..4]` 范围的 `val` 值将会导致跳转到程序中的任何可能位置的地址，从那个位置继续执行。
真的，当你遇到数据竞争时什么都可能发生。
实际上可能更为糟糕：通过查看生成的代码，是可以确定这个走偏的跳转针对给定的值将会跳转到哪里的；
这就是一种安全风险。

##### 强制实施

有些事是可能做到的，至少要做一些事。
有一些商用和开源的工具试图处理这种问题，
但要注意的是任何工具解决方案都有其成本和盲点。
静态工具通常会有许多漏报，而动态工具则通常有显著的成本。
我们希望有更好的工具。
使用多个工具可以找到比单个工具更多的问题。

还有其他方式可以缓解发生数据竞争的机会：

- 避免全局数据
- 避免 `static` 变量
- 更多地使用栈上的具体类型（且不要过多地把指针到处传递）
- 更多地使用不可变数据（字面量，`constexpr`，以及 `const`）

### <a name="Rconc-data"></a>CP.3: 最小化可写数据的明确共享

##### 理由

如果不共享可写数据的话，就不会发生数据竞争。
越少进行共享，你就越少有机会忘掉对访问进行同步（而发生数据竞争）。
越少进行共享，你就越少有机会需要等待锁定（并改进性能）。

##### 示例

    bool validate(const vector<Reading>&);
    Graph<Temp_node> temperature_gradients(const vector<Reading>&);
    Image altitude_map(const vector<Reading>&);
    // ...

    void process_readings(const vector<Reading>& surface_readings)
    {
        auto h1 = async([&] { if (!validate(surface_readings)) throw Invalid_data{}; });
        auto h2 = async([&] { return temperature_gradients(surface_readings); });
        auto h3 = async([&] { return altitude_map(surface_readings); });
        // ...
        h1.get();
        auto v2 = h2.get();
        auto v3 = h3.get();
        // ...
    }

没有这些 `const` 的话，我们就必须为潜在的数据竞争而为在 `surface_readings` 上的所有异步函数调用进行复审。
使 `surface_readings` （对于这个函数）为 `const` 允许我们仅在函数体代码中进行推理。

##### 注解

不可变数据可以安全并高效地共享。
无须对其进行锁定：不可能在常量上发生数据竞争。
另请参见[CP.mess: 消息传递](#SScp-mess)和[CP.31: 优先采用按值传递](#Rconc-data-by-value)。

##### 强制实施

???

### <a name="Rconc-task"></a>CP.4: 以任务而不是线程的角度思考

##### 理由

`thread` 是一种实现概念，一种针对机器的思考方式。
任务则是一种应用概念，有时候你可以使任务和其他任务并发执行。
应用概念更容易进行推理。

##### 理由

    void some_fun(const std::string& msg)
    {
        std::thread publisher([=] { std::cout << msg; });      // 不好: 表达性不足
                                                               //       且更易错
        auto pubtask = std::async([=] { std::cout << msg; });  // OK
        // ...
        publisher.join();
    }

##### 注解

除了 `async()` 之外，标准库中的设施都是底层的，面向机器的，线程和锁层次的设施。
这是一种必须的基础，但我们不得不尝试提升抽象的层次：为提供生产率，可靠性，以及性能。
这对于采用更高层次的，更加面向应用的程序库（如果可能就建立在标准库设施上）的强有力的理由。

##### 强制实施

???

### <a name="Rconc-volatile"></a>CP.8: 不要为同步而使用 `volatile`

##### 理由

和其他语言不同，C++ 中的 `volatile` 并不提供原子性，不会在线程之间进行同步，
而且不会防止指令重排（无论编译器还是硬件）。
它和并发完全没有关系。

##### 示例，不好

    int free_slots = max_slots; // 当前的对象内存的来源

    Pool* use()
    {
        if (int n = free_slots--) return &pool[n];
    }

这里有一个问题：
这在单线程程序中是完全正确的代码，但若有两个线程执行，
并且在 `free_slots` 上发生竞争条件时，两个线程就可能拿到相同的值和 `free_slots`。
这（显然）是不好的数据竞争，受过其他语言训练的人可能会试图这样修正：

    volatile int free_slots = max_slots; // 当前的对象内存的来源

    Pool* use()
    {
        if (int n = free_slots--) return &pool[n];
    }

这并没有同步效果：数据竞争仍然存在！

C++ 对此的机制是 `atomic` 类型：

    atomic<int> free_slots = max_slots; // 当前的对象内存的来源

    Pool* use()
    {
        if (int n = free_slots--) return &pool[n];
    }

现在的 `--` 操作是原子性的，
而不是可能被另一个线程介入其独立操作之间的读-增量-写序列。

##### 替代方案

在某些其他语言中曾经使用 `volatile` 的地方使用 `atomic` 类型。
为更加复杂的例子使用 `mutex`。

##### 另见

[`volatile` 的（罕见）恰当用法](#Rconc-volatile2)

### <a name="Rconc-tools"></a>CP.9: 只要可行，就使用工具对并发代码进行验证

经验表明，让并发代码正确是特别难的，
而编译期检查、运行时检查和测试在找出并发错误方面，
并没有如同在顺序代码中找出错误时那么有效。
一些微妙的并发错误可能会造成显著的不良后果，包括内存破坏，死锁，和安全漏洞等。

##### 示例

    ???

##### 注解

线程安全性是一种有挑战性的任务，有经验的程序员通常可以做得更好一些：缓解这些风险的一种重要策略是运用工具。
现存有不少这样的工具，既有商用的也有开源的，既有研究性的也有产品级的。
遗憾的是，人们的需求和约束条件之间有巨大的差别，使我们无法给出特定的建议，
但我们可以提一些：

- 静态强制实施工具：[clang](http://clang.llvm.org/docs/ThreadSafetyAnalysis.html)
  和一些老版本的 [GCC](https://gcc.gnu.org/wiki/ThreadSafetyAnnotation)
  都提供了一些针对线程安全性性质的静态代码标注。
  统一地采用这项技术可以将许多种类的线程安全性错误变为编译时的错误。
  这些代码标注一般都是局部的（将某个特定成员变量标记为又一个特定的互斥体进行防护），
  且一般都易于学习使用。但与许多的静态工具一样，它经常会造成漏报；
  这些情况应当被发现但却被其忽略了。

- 动态强制实施工具：Clang 的 [Thread Sanitizer](http://clang.llvm.org/docs/ThreadSanitizer.html)（即 TSAN）
  是动态工具的一个强有力的例子：它改变你的程序的构建和执行，向其中添加对内存访问的簿记工作，
  精确地识别你的二进制程序的一次特定执行中发生的数据竞争。
  使用它的代价在内存上（多数情况为五到十倍），也拖慢 CPU（二到二十倍）。
  像这样的动态工具，在集成测试，金丝雀推送，以及在多个线程上操作的单元测试上实施是最好的。
  它是与工作负载相关的：一旦 TSAN 识别了一个问题，那它就是实际发生的数据竞争，
  但它只能识别出一次特定的执行之中发生的竞争。

##### 强制实施

对于特定的应用，应用的构建者来选择哪些支持工具是有价值的。

## <a name="SScp-con"></a>CP.con: 并发

这个部分所关注的是相对比较专门的通过共享数据进行多线程通信的用法。

- 有关并行算法，参见[并行](#SScp-par)。
- 有关不使用明确共享的任务间，通信参见[消息传递](#SScp-mess)。
- 有关向量并行代码，参见[向量化](#SScp-vec)。
- 有关无锁编程，参见[无锁](#SScp-free)。

并发规则概览：

- [CP.20: 使用 RAII，绝不使用普通的 `lock()`/`unlock()`](#Rconc-raii)
- [CP.21: 用 `std::lock()` 或 `std::scoped_lock` 来获得多个 `mutex`](#Rconc-lock)
- [CP.22: 绝不在持有锁的时候调用未知的代码（比如回调）](#Rconc-unknown)
- [CP.23: 把联结的 `thread` 看作是有作用域的容器](#Rconc-join)
- [CP.24: 把 `thread` 看作是全局的容器](#Rconc-detach)
- [CP.25: 优先采用 `gsl::joining_thread` 而不是 `std::thread`](#Rconc-joining_thread)
- [CP.26: 不要 `detach()` 线程](#Rconc-detached_thread)
- [CP.31: 少量数据在线程之间按值传递，而不是通过引用或指针传递](#Rconc-data-by-value)
- [CP.32: 用 `shared_ptr` 在无关的 `thread` 之间共享所有权](#Rconc-shared)
- [CP.40: 最小化上下文切换](#Rconc-switch)
- [CP.41: 最小化线程的创建和销毁](#Rconc-create)
- [CP.42: 不要无条件地 `wait`](#Rconc-wait)
- [CP.43: 最小化临界区的时间耗费](#Rconc-time)
- [CP.44: 记得为 `lock_guard` 和 `unique_lock` 命名](#Rconc-name)
- [CP.50: `mutex` 要和其所护卫的数据一起定义。一旦可能就使用 `synchronized_value<T>`](#Rconc-mutex)
- ??? 何时使用 spinlock
- ??? 何时使用 `try_lock()`
- ??? 何时应优先使用 `lock_guard` 而不是 `unique_lock`
- ??? 时序多工
- ??? 何时/如何使用 `new thread`

### <a name="Rconc-raii"></a>CP.20: 使用 RAII，绝不使用普通的 `lock()`/`unlock()`

##### 理由

避免源于未释放的锁定的令人讨厌的错误。

##### 示例，不好

    mutex mtx;

    void do_stuff()
    {
        mtx.lock();
        // ... 做一些事 ...
        mtx.unlock();
    }

或早或晚都会有人忘记 `mtx.unlock()`，在 `... 做一些事 ...` 中放入一个 `return`，抛出异常，或者别的什么。

    mutex mtx;

    void do_stuff()
    {
        unique_lock<mutex> lck {mtx};
        // ... 做一些事 ...
    }

##### 强制实施

标记对成员 `lock()` 和 `unlock()` 的调用。 ???

### <a name="Rconc-lock"></a>CP.21: 用 `std::lock()` 或 `std::scoped_lock` 来获得多个 `mutex`

##### 理由

避免在多个 `mutex` 上造成死锁。

##### 示例

下面将导致死锁：

    // 线程 1
    lock_guard<mutex> lck1(m1);
    lock_guard<mutex> lck2(m2);

    // 线程 2
    lock_guard<mutex> lck2(m2);
    lock_guard<mutex> lck1(m1);

代之以使用 `lock()`：

    // 线程 1
    lock(m1, m2);
    lock_guard<mutex> lck1(m1, defer_lock);
    lock_guard<mutex> lck2(m2, defer_lock);

    // 线程 2
    lock(m2, m1);
    lock_guard<mutex> lck2(m2, defer_lock);
    lock_guard<mutex> lck1(m1, defer_lock);

或者（这样更佳，但仅为 C++17）：

    // 线程 1
    scoped_lock<mutex, mutex> lck1(m1, m2);

    // 线程 2
    scoped_lock<mutex, mutex> lck2(m2, m1);

这样，`thread1` 和 `thread2` 的作者们仍然未在 `mutex` 的顺序上达成一致，但顺序不再是问题了。

##### 注解

在实际代码中，`mutex` 的命名很少便于程序员记得某种有意的关系和有意的获取顺序。
在实际代码中，`mutex` 并不总是便于在连续代码行中依次获取的。

##### 注解

在 C++17 中，可以编写普通的

    lock_guard lck1(m1, adopt_lock);

而让 `mutex` 类型被推断出来。

##### 强制实施

检测多个 `mutex` 的获取。
这一般来说是无法确定的，但找出常见的简单例子（比如上面这个）则比较容易。

### <a name="Rconc-unknown"></a>CP.22: 绝不在持有锁的时候调用未知的代码（比如回调）

##### 理由

如果不了解代码做了什么，就有死锁的风险。

##### 示例

    void do_this(Foo* p)
    {
        lock_guard<mutex> lck {my_mutex};
        // ... 做一些事 ...
        p->act(my_data);
        // ...
    }

如果不知道 `Foo::act` 会干什么（可能它是一个虚函数，调用某个还未编写的某个派生类成员），
它可能会（递归地）调用 `do_this` 因而在 `my_mutex` 上造成死锁。
可能它会在某个别的 `mutex` 上锁定而无法在适当的时间内返回，对任何调用了 `do_this` 的代码造成延迟。

##### 示例

“调用未知代码”问题的一个常见例子是调用了试图在相同对象上进行锁定访问的函数。
这种问题通常可以用 `recursive_mutex` 来解决。例如：

    recursive_mutex my_mutex;

    template<typename Action>
    void do_something(Action f)
    {
        unique_lock<recursive_mutex> lck {my_mutex};
        // ... 做一些事 ...
        f(this);    // f 将会对 *this 做一些事
        // ...
    }

如果如同其很可能做的那样，`f()` 调用了 `*this` 的某个操作的话，我们就必须保证在调用之前对象的不变式是满足的。

##### 强制实施

- 当持有非递归的 `mutex` 时调用虚函数则进行标记。
- 当持有非递归的 `mutex` 时调用回调则进行标记。

### <a name="Rconc-join"></a>CP.23: 把联结的 `thread` 看作是有作用域的容器

##### 理由

为了维护指针安全性并避免泄漏，需要考虑 `thread` 所使用的指针。
如果 `thread` 联结了，我们可以安全地把指向这个 `thread` 所在作用域及其外围作用域中的对象的指针传递给它。

##### 示例

    void f(int* p)
    {
        // ...
        *p = 99;
        // ...
    }
    int glob = 33;

    void some_fct(int* p)
    {
        int x = 77;
        joining_thread t0(f, &x);           // OK
        joining_thread t1(f, p);            // OK
        joining_thread t2(f, &glob);        // OK
        auto q = make_unique<int>(99);
        joining_thread t3(f, q.get());      // OK
        // ...
    }

`gsl::joining_thread` 是一种 `std::thread`，其析构函数进行联结且不可被 `detached()`。
这里的“OK”表明对象能够在 `thread` 可以使用指向它的指针时一直处于作用域（“存活”）。
`thread` 运行的并发性并不会影响这里的生存期或所有权问题；
这些 `thread` 可以仅仅被看成是从 `some_fct` 中调用的函数对象。

##### 强制实施

确保 `joining_thread` 不会 `detach()`。
之后，可以实施（针对局部对象的）常规的生存期和所有权强制实施方案。

### <a name="Rconc-detach"></a>CP.24: 把 `thread` 看作是全局的容器

##### 理由

为了维护指针安全性并避免泄漏，需要考虑 `thread` 所使用的指针。
如果 `thread` 脱离了，我们只可以安全地把指向静态和自由存储的对象的指针传递给它。

##### 示例

    void f(int* p)
    {
        // ...
        *p = 99;
        // ...
    }

    int glob = 33;

    void some_fct(int* p)
    {
        int x = 77;
        std::thread t0(f, &x);           // 不好
        std::thread t1(f, p);            // 不好
        std::thread t2(f, &glob);        // OK
        auto q = make_unique<int>(99);
        std::thread t3(f, q.get());      // 不好
        // ...
        t0.detach();
        t1.detach();
        t2.detach();
        t3.detach();
        // ...
    }

这里的“OK”表明对象能够在 `thread` 可以使用指向它的指针时一直处于作用域（“存活”）。
“bad”则表示 `thread` 可能在对象销毁之后使用指向它的指针。
`thread` 运行的并发性并不会影响这里的生存期或所有权问题；
这些 `thread` 可以仅仅被看成是从 `some_fct` 中调用的函数对象。

##### 注解

即便具有静态存储期的对象，在脱离的线程中的使用也会造成问题：
若是这个线程持续到程序终止，则它的运行可能与具有静态存储期的对象的销毁过程之间并发地发生，
而这样对这些对象的访问就可能发生竞争。

##### 注解

如果你[不 `detach()`](#Rconc-detached_thread) 并[使用 `gsl::joining_tread`](#Rconc-joining_thread) 的话，本条规则是多余的。
不过，将代码转化为遵循这些指导方针可能很困难，而对于第三方库来说更是不可能的。
这些情况下，这条规则对于生存期安全性和类型安全性来说就是必要的了。

一般来说是无法确定是否对某个 `thread` 执行了 `detach()` 的，但简单的常见情况则易于检测出来。
如果无法证明某个 `thread` 并没有 `detach()` 的话，我们只能假定它确实脱离了，且它的存活将超过其构造时所处于的作用域；
之后，可以实施（针对全局对象的）常规的生存期和所有权强制实施方案。

##### 强制实施

当试图将局部变量传递给可能 `detach()` 的线程时进行标记。

### <a name="Rconc-joining_thread"></a>CP.25: 优先采用 `gsl::joining_thread` 而不是 `std::thread`

##### 理由

`joining_thread` 是一种在其作用域结尾处进行联结的线程。
脱离的线程很难进行监管。
确保脱离的线程（和潜在脱离的线程）中没有错误则更加困难。

##### 示例，不好

    void f() { std::cout << "Hello "; }

    struct F {
        void operator()() const { std::cout << "parallel world "; }
    };

    int main()
    {
        std::thread t1{f};      // f() 在独立线程中执行
        std::thread t2{F()};    // F()() 在独立线程中执行
    }  // 请找出问题

##### 示例

    void f() { std::cout << "Hello "; }

    struct F {
        void operator()() const { std::cout << "parallel world "; }
    };

    int main()
    {
        std::thread t1{f};      // f() 在独立线程中执行
        std::thread t2{F()};    // F()() 在独立线程中执行

        t1.join();
        t2.join();
    }  // 剩下一个糟糕的 BUG

##### 注解

使“不死线程”成为全局的，将其放入外围作用域中，或者放入自由存储中，而不要 `detach()` 它们。
[不要 `detach`](#Rconc-detached_thread)。

##### 注解

因为老代码和第三方库也会使用 `std::thread`，本条规则可能很难引入。

##### 理由

标记 `std::thread` 的使用：

- 建议使用 `gsl::joining_thread` 或 C++20 的 `std:jthread`.
- 建议当其脱离时使其[“外放所有权”](#Rconc-detached_thread)到某个外围作用域中。
- 如果不明确线程是联结还是脱离，则给出警告。

### <a name="Rconc-detached_thread"></a>CP.26: 不要 `detach()` 线程

##### 理由

通常，需要存活超出线程创建的作用域的情况是来源于 `thread` 的任务所决定的，
但用 `detach` 来实现这点将造成更加难于对脱离的线程进行监控和通信。
特别是，要确保线程按预期完成或者按预期的时间存活变得更加困难（虽然不是不可能）。

##### 示例

    void heartbeat();

    void use()
    {
        std::thread t(heartbeat);             // 不联结；打算持续运行 heartbeat
        t.detach();
        // ...
    }

这是一种合理的线程用法，一般会使用 `detach()`。
但这里有些问题。
我们怎么监控脱离的线程以查看它是否存活呢？
心跳里边可能会出错，而在需要心跳的系统中，心跳丢失可能是非常严重的问题。
因此，我们需要与心跳线程进行通信
（例如，通过某个消息流，或者使用 `condition_variable` 的通知事件）。

一种替代的，而且通常更好的方案是，通过将其放入某个其创建点（或激活点）之外的作用域来控制其生存期。
例如：

    void heartbeat();

    gsl::joining_thread t(heartbeat);             // 打算持续运行 heartbeat

这个心跳，（除非出错或者硬件故障等情况）将在程序运行时一直运行。

有时候，我们需要将创建点和所有权点分离开：

    void heartbeat();

    unique_ptr<gsl::joining_thread> tick_tock {nullptr};

    void use()
    {
        // 打算在 tick_tock 存活期间持续运行 heartbeat
        tick_tock = make_unique(gsl::joining_thread, heartbeat);
        // ...
    }

##### 强制实施

标记 `detach()`。

### <a name="Rconc-data-by-value"></a>CP.31: 少量数据在线程之间按值传递，而不是通过引用或指针传递

##### 理由

对少量数据进行复制和访问要比使用某种锁定机制进行共享更廉价。
复制天然会带来唯一所有权（简化代码），并消除数据竞争的可能性。

##### 注解

对“少量”进行精确的定义是不可能的。

##### 示例

    string modify1(string);
    void modify2(string&);

    void fct(string& s)
    {
        auto res = async(modify1, s);
        async(modify2, s);
    }

`modify1` 的调用涉及两个 `string` 值的复制；而 `modify2` 的调用则不会。
另一方面，`modify1` 的实现和我们为单线程代码所编写的完全一样，
而 `modify2` 的实现则需要某种形式的锁定以避免数据竞争。
如果字符串很短（比如 10 个字符），对 `modify1` 的调用将会出乎意料地快；
基本上所有的代价都在 `thread` 的切换上。如果字符串很长（比如 1,000,000 个字符），对其两次复制
可能并不是一个好主意。

注意这个论点和 `async` 并没有任何关系。它同等地适用于任何对采用消息传递
还是共享内存的考虑之上。

##### 强制实施

???

### <a name="Rconc-shared"></a>CP.32: 用 `shared_ptr` 在无关的 `thread` 之间共享所有权

##### 理由

如果线程之间是无关的（就是说，互相不知道是否在相同作用域中，或者一个的生存期在另一个之内），
而它们需要共享需要删除的自由存储内存，`shared_ptr`（或者等价物）就是唯一的
可以保证正确删除的安全方式。

##### 示例

    ???

##### 注解

- 可以共享静态对象（比如全局对象），因为它并不像是需要某个线程来负责其删除那样被谁所拥有。
- 自由存储上不会被删除的对象可以进行共享。
- 由一个线程所拥有的对象可以安全地共享给另一个线程，只要第二个线程存活不会超过这个拥有者线程即可。

##### 强制实施

???

### <a name="Rconc-switch"></a>CP.40: 最小化上下文切换

##### 理由

上下文切换是昂贵的。

##### 示例

    ???

##### 强制实施

???

### <a name="Rconc-create"></a>CP.41: 最小化线程的创建和销毁

##### 理由

线程创建是昂贵的。

##### 示例

    void worker(Message m)
    {
        // 处理
    }

    void dispatcher(istream& is)
    {
        for (Message m; is >> m; )
            run_list.push_back(new thread(worker, m));
    }

这会为每个消息产生一个线程，而 `run_list` 则假定在它们完成后对这些任务进行销毁。

我们可以用一组预先创建的工作线程来处理这些消息：

    Sync_queue<Message> work;

    void dispatcher(istream& is)
    {
        for (Message m; is >> m; )
            work.put(m);
    }

    void worker()
    {
        for (Message m; m = work.get(); ) {
            // 处理
        }
    }

    void workers()  // 设立工作线程（这里是 4 个工作线程）
    {
        joining_thread w1 {worker};
        joining_thread w2 {worker};
        joining_thread w3 {worker};
        joining_thread w4 {worker};
    }

##### 注解

如果你的系统有一个好的线程池的话，就请使用它。
如果你的系统有一个好的消息队列的话，就请使用它。

##### 强制实施

???

### <a name="Rconc-wait"></a>CP.42: 不要无条件地 `wait`

##### 理由

没有条件的 `wait` 可能会丢失唤醒，或者唤醒时只会发现无事可做。

##### 示例，不好

    std::condition_variable cv;
    std::mutex mx;

    void thread1()
    {
        while (true) {
            // 做一些工作 ...
            std::unique_lock<std::mutex> lock(mx);
            cv.notify_one();    // 唤醒另一个线程
        }
    }

    void thread2()
    {
        while (true) {
            std::unique_lock<std::mutex> lock(mx);
            cv.wait(lock);    // 可能会永远阻塞
            // 做一些工作 ...
        }
    }

这里，如果某个其他 `thread` 消费了 `thread1` 的通知的话，`thread2` 将会永远等待下去。

##### 示例

    template<typename T>
    class Sync_queue {
    public:
        void put(const T& val);
        void put(T&& val);
        void get(T& val);
    private:
        mutex mtx;
        condition_variable cond;    // 这用于控制访问
        list<T> q;
    };

    template<typename T>
    void Sync_queue<T>::put(const T& val)
    {
        lock_guard<mutex> lck(mtx);
        q.push_back(val);
        cond.notify_one();
    }

    template<typename T>
    void Sync_queue<T>::get(T& val)
    {
        unique_lock<mutex> lck(mtx);
        cond.wait(lck, [this] { return !q.empty(); });    // 防止假性唤醒
        val = q.front();
        q.pop_front();
    }

这样当执行 `get()` 的线程被唤醒时，如果队列为空（比如因为别的线程在之前已经 `get()` 过了），
它将立刻回到睡眠中继续等待。

##### 强制实施

对所有没有条件的 `wait` 进行标记。

### <a name="Rconc-time"></a>CP.43: 最小化临界区的时间耗费

##### 理由

获取 `mutex` 时耗费的时间越短，其他 `thread` 不得不等待的机会就会越少，
而 `thread` 的挂起和恢复是昂贵的。

##### 示例

    void do_something() // 不好
    {
        unique_lock<mutex> lck(my_lock);
        do0();  // 预备：不需要锁定
        do1();  // 事务：需要锁定
        do2();  // 清理：不需要锁定
    }

这里我们持有的锁定比所需的要长：
我们不应该在必须锁定之前就获取锁定，而且应当在开始清理之前将其释放掉。
可以将其重写为：

    void do_something() // 不好
    {
        do0();  // 预备：不需要锁定
        my_lock.lock();
        do1();  // 事务：需要锁定
        my_lock.unlock();
        do2();  // 清理：不需要锁定
    }

但这样损害了安全性并且违反了[使用 RAII](#Rconc-raii) 规则。
我们可以为临界区添加语句块：

    void do_something() // OK
    {
        do0();  // 预备：不需要锁定
        {
            unique_lock<mutex> lck(my_lock);
            do1();  // 事务：需要锁定
        }
        do2();  // 清理：不需要锁定
    }

##### 强制实施

一般来说是不可能的。
对“裸” `lock()` 和 `unlock()` 进行标记。

### <a name="Rconc-name"></a>CP.44: 记得为 `lock_guard` 和 `unique_lock` 命名

##### 理由

无名的局部对象时临时对象，会立刻离开作用域。

##### 示例

    // 全局互斥体
    mutex m1;
    mutex m2;

    void f()
    {
        unique_lock<mutex>(m1); // (A)
        lock_guard<mutex> {m2}; // (B)
        // 关键区中的工作
    }

这个貌似足够有效，但其实并非如此。在 (A) 点，`m1` 是一个
默认构造的局部 `unique_lock`，它隐藏了全局的 `::m1`（且并未锁定它）。
在 (B) 点，构造了一个无名 `lock_guard` 临时对象并锁定了 `::m2`，
但它立即离开作用域并再次解锁了 `::m2`。
函数 `f()` 的余下部分中并没有锁定任何互斥体。

##### 强制实施

标记所有的无名 `lock_guard` 和 `unique_lock`。

### <a name="Rconc-mutex"></a>CP.50: `mutex` 要和其所保护的数据一起定义，只要可能就使用 `synchronized_value<T>`

##### 理由

对于读者来说，数据应该且如何被保护应当是显而易见的。这可以减少锁定错误的互斥体，或者互斥体未能被锁定的机会。

使用 `synchronized_value<T>` 保证了数据都带有互斥体，并且当访问数据时锁定正确的互斥体。
参见向某个未来的 TS 或 C++ 标准的修订版添加 `synchronized_value` [WG21 提案](http://wg21.link/p0290)。

##### 示例

    struct Record {
        std::mutex m;   // 访问其他成员之前应当获取这个 mutex
        // ...
    };

    class MyClass {
        struct DataRecord {
           // ...
        };
        synchronized_value<DataRecord> data; // 用互斥体保护数据
    };

##### 强制实施

??? 可能吗？

## <a name="SScp-coro"></a>CP.coro: 协程

这一部分关注协程的使用。

协程规则概览：

- [CP.51: 不要使用作为协程的有俘获 lambda 表达式](#Rcoro-capture)
- [CP.52: 不要在持有锁或其它同步原语时跨越挂起点](#Rcoro-locks)
- [CP.53: 协程的形参不能按引用传递](#Rcoro-reference-parameters)

### <a name="Rcoro-capture"></a>CP.51: 不要使用作为协程的有俘获 lambda 表达式

##### 理由

对于普通 lambda 来说正确的使用模式，对于协程 lambda 来说是高危的。很明显的变量俘获模式将会造成在首个挂起点之后访问已释放的内存，即便是带引用计数的智能指针和可复制类型也是如此。

lambda 表达式会产生一个带有存储的闭包对象，它通常在运行栈上，并会在某个时刻离开作用域。当闭包对象离开作用域时，它所俘获的也会离开作用域。普通 lambda 的执行在这个时间点都已经完成了，因此这并不是问题。闭包 lambda 则可能会在闭包对象已经销毁之后从挂起中恢复执行，而在这时其所有俘获都将变为“释放后使用”的内存访问。

##### 示例，不好

    int value = get_value();
    std::shared_ptr<Foo> sharedFoo = get_foo();
    {
      const auto lambda = [value, sharedFoo]() -> std::future<void>
      {
        co_await something();
        // "sharedFoo" 和 "value" 已被销毁
        // “共享”指针没有带来任何好处
      };
      lambda();
    } // lambda 闭包对象此时已离开作用域

##### 示例，更好

    int value = get_value();
    std::shared_ptr<Foo> sharedFoo = get_foo();
    {
      const auto lambda = [](auto sharedFoo, auto value) -> std::future<void>  // 以按值传参代替作为俘获
      {
        co_await something();
        // sharedFoo 和 value 此时仍然有效
      };
      lambda(sharedFoo, value);
    } // lambda 闭包对象此时已离开作用域

##### 示例，最佳

使用函数作为协程。

    std::future<void> Class::do_something(int value, std::shared_ptr<Foo> sharedFoo)
    {
      co_await something();
      // sharedFoo 和 value 此时仍然有效
    }

    void SomeOtherFunction()
    {
      int value = get_value();
      std::shared_ptr<Foo> sharedFoo = get_foo();
      do_something(value, sharedFoo);
    }

##### 强制实施

标记作为协程且具有非空俘获列表的 lambda 表达式。

### <a name="Rcoro-locks"></a>CP.52: 不要在持有锁或其它同步原语时跨越挂起点

##### 理由

这种模式会导致明显的死锁风险。某些种类的等待允许当前线程在异步操作完成前实施一些额外的工作。如果持有锁的线程实施了需要相同的所的工作，那它就会因为试图获取它已经持有的锁而发生死锁。

如果协程在某个与获得所的线程不同的另一个线程中完成，那就是未定义行为。即使协程中明确返回其原来的线程，仍然有可能在协程恢复之前抛出异常，并导致其锁定防护对象（lock guard）未能销毁。

##### 示例，不好

    std::mutex g_lock;

    std::future<void> Class::do_something()
    {
        std::lock_guard<std::mutex> guard(g_lock);
        co_await something(); // 危险：在持有锁时挂起协程
        co_await somethingElse();
    }

##### 示例，好

    std::mutex g_lock;

    std::future<void> Class::do_something()
    {
        {
            std::lock_guard<std::mutex> guard(g_lock);
            // 修改被锁保护的数据
        }
        co_await something(); // OK：锁已经在协程挂起前被释放
        co_await somethingElse();
    }

##### 注解

这种模式对于性能也不好。每当遇到比如 `co_await` 这样的挂起点时，都会停止当前函数的执行而去运行别的代码。而在协程恢复之前可能会经过很长时间。这个锁会在整个时间段中持有，并且无法被其他线程获得以进行别的工作。

##### 强制实施

标记所有未能在协程挂起前销毁的锁定防护。

### <a name="Rcoro-reference-parameters"></a>CP.53: 协程的形参不能按引用传递

##### 理由

一旦协程到达其第一个如 `co_await` 这样的挂起点，其同步执行的部分就会返回。这个位置之后，所有按引用传递的形参都是悬挂引用。此后对它们的任何使用都是未定义行为，可能包括向已释放的内存进行写入。

##### 示例，不好

    std::future<int> Class::do_something(const std::shared_ptr<int>& input)
    {
        co_await something();

        // 危险：对 input 的引用可能不再有效，可能已经是已释放内存
        co_return *input + 1;
    }

##### 示例，好

    std::future<int> Class::do_something(std::shared_ptr<int> input)
    {
        co_await something();
        co_return *input + 1; // input 是一个副本，且到此处仍有效
    }

##### 注解

这个问题并不适用于仅在第一个挂起点之前访问的引用形参。此后对函数的改动中可能会添加或移除挂起点，而这可能会再次引入这一类的缺陷。一些种类的协程，在协程执行第一行代码之前就会有挂起点，这种情况中的引用形参总是不安全的。一直采用按值传递的方式更安全，因为所复制的形参存活于协程的栈帧中，并在整个协程中都可以安全访问。

##### 注解

输出参数也有这种危险。[F.20: 对于“输出（out）”值，采用返回值优先于输出参数](#Rf-out) 不建议使用输出参数。协程应当完全避免输出参数。

##### 强制实施

标记协程的所有引用形参。

## <a name="SScp-par"></a>CP.par: 并行

这里的“并行”代表的是对许多数据项（或多或少）同时地（“并行进行”）实施某项任务。

并行规则概览：

- ???
- ???
- 适当的时候，优先采用标准库的并行算法
- 使用为并行设计的算法，而不是不必要地依赖于线性求值的算法

## <a name="SScp-mess"></a>CP.mess: 消息传递

标准库的设施是相当底层的，关注于使用 `thread`，`mutex`，`atomic` 等类型的贴近硬件的关键编程。
大多数人都不应该在这种层次上工作：它是易错的，而且开发很慢。
如果可能的话，应当使用高层次的设施：消息程序库，并行算法，以及向量化。
这一部分关注如何传递消息，以使程序员不必进行显式的同步。

消息传递规则概览：

- [CP.60: 使用 `future` 从并发任务返回值](#Rconc-future)
- [CP.61: 使用 `async()` 来产生并发任务](#Rconc-async)
- 消息队列
- 消息程序库

???? 是否应该有某个“使用 X 而不是 `std::async`”，其中 X 是某种更好说明的线程池？

??? 在未来的趋势下（甚至是现存的程序库），`std::async` 是否还是值得使用的并行设施？当有人想要对比如 `std::accumulate`（带上额外的累加前条件），或者归并排序进行并行化时，指导方针应该给出什么样的建议呢？

### <a name="Rconc-future"></a>CP.60: 使用 `future` 从并发任务返回值

##### 理由

`future` 为异步任务保持了常规函数调用的返回语义。
它没有显式的锁定，而且正确的（值）返回和错误的（异常）返回都能被简单处理。

##### 示例

    ???

##### 注解

???

##### 强制实施

???

### <a name="Rconc-async"></a>CP.61: 使用 `async()` 来产生并发任务

##### 理由

[R.12](#Rr-immediate-alloc) 告诉我们要避免原始所有权指针，与此相似，
我们也要尽可能避免原始线程和原始承诺（promise）。应使用诸如 `std::async` 之类的工厂函数，
它将处理线程的产生和重用，而不会讲原始线程暴露给你自己的代码。

##### 示例

    int read_value(const std::string& filename)
    {
        std::ifstream in(filename);
        in.exceptions(std::ifstream::failbit);
        int value;
        in >> value;
        return value;
    }

    void async_example()
    {
        try {
            std::future<int> f1 = std::async(read_value, "v1.txt");
            std::future<int> f2 = std::async(read_value, "v2.txt");
            std::cout << f1.get() + f2.get() << '\n';
        } catch (std::ios_base::failure & fail) {
            // 此处处理异常
        }
    }

##### 注解

不幸的是，`async()` 并不完美。比如说，它并不使用线程池，
这意味着它可能会因为资源耗尽而失败，而不会将你的任务放入队列以便随后执行。
不过，即便你不能用 `std::async`，你也应当优先编写自己的返回
`future` 的工厂函数，而非使用原始承诺。

##### 示例（不好）

这个例子展示了两种方式，都使用了 `std::future` 却未能避免对原始
`std::thread` 的管理。

    void async_example()
    {
        std::promise<int> p1;
        std::future<int> f1 = p1.get_future();
        std::thread t1([p1 = std::move(p1)]() mutable {
            p1.set_value(read_value("v1.txt"));
        });
        t1.detach(); // 恶行

        std::packaged_task<int()> pt2(read_value, "v2.txt");
        std::future<int> f2 = pt2.get_future();
        std::thread(std::move(pt2)).detach();

        std::cout << f1.get() + f2.get() << '\n';
    }

##### 示例（好）

这个例子展示一种方法，可以让你在当 `std::async` 自身
在产品中不可接受的情形中，模仿 `std::async` 所设立的
一般模式的方法。

    void async_example(WorkQueue& wq)
    {
        std::future<int> f1 = wq.enqueue([]() {
            return read_value("v1.txt");
        });
        std::future<int> f2 = wq.enqueue([]() {
            return read_value("v2.txt");
        });
        std::cout << f1.get() + f2.get() << '\n';
    }

所有为执行 `read_value` 的代码而产生的线程都被隐藏到对
`WorkQueue::enqueue` 的调用之内。用户代码仅需处理 `future` 对象，
无需处理原始 `thread`，`promise` 或 `packaged_task` 对象。

##### 强制实施

???

## <a name="SScp-vec"></a>CP.vec: 向量化

向量化是一种在不引入显式同步时并发地执行多个任务的技术。
它会并行地把某个操作实施与某个数据结构（向量，数组，等等）的各个元素之上。
向量化引人关注的性质在于它通常不需要对程序进行非局部的改动。
不过，向量化只对简单的数据结构和特别针对它所构造的算法能够得到最好的工作效果。

向量化规则概览：

- ???
- ???

## <a name="SScp-free"></a>CP.free: 无锁编程

使用 `mutex` 和 `condition_variable` 进行同步相对来说很昂贵。
而且可能导致死锁。
为了性能并消除死锁的可能性，有时候我们不得不使用麻烦的底层“无锁”设施，
它们依赖于对内存短暂地获得互斥（“原子性”）访问。
无锁编程也被用于实现如 `thread` 和 `mutex` 这样的高层并发机制。

无锁编程规则概览：

- [CP.100: 除非绝对必要，请勿使用无锁编程](#Rconc-lockfree)
- [CP.101: 不要信任你的硬件-编译器组合](#Rconc-distrust)
- [CP.102: 仔细研究文献](#Rconc-literature)
- 如何/何时使用原子
- 避免饥饿
- 使用无锁数据结构而不是手工构造的专门的无锁访问
- [CP.110: 不要为初始化编写你自己的双检查锁定](#Rconc-double)
- [CP.111: 当确实需要双检查锁定时应当采用惯用的模式](#Rconc-double-pattern)
- 如何/何时进行比较并交换（CAS）

### <a name="Rconc-lockfree"></a>CP.100: 除非绝对必要，请勿使用无锁编程

##### 理由

无锁编程容易出错，要求专家级的语言特性、机器架构和数据结构知识。

##### 示例，不好

    extern atomic<Link*> head;        // 共享的链表表头

    Link* nh = new Link(data, nullptr);    // 为进行插入制作一个连接
    Link* h = head.load();                 // 读取链表中的共享表头

    do {
        if (h->data <= data) break;        // 这样的话，就插入到别处
        nh->next = h;                      // 下一个元素是之前的表头
    } while (!head.compare_exchange_weak(h, nh));    // 将 nh 写入 head 或 h

请找出这里的 BUG。
通过测试找到它是非常困难的。
请阅读有关 ABA 问题的材料。

##### 例外

[原子变量](#???)可以简单并安全地使用，只要你所用的是顺序一致性内存模型（`memory_order_seq_cst`），而这是默认情况。

##### 注解

高层的并发机制，诸如 `thread` 和 `mutex`，是使用无锁编程来实现的。

**替代方案**: 使用由他人实现并作为程序库一部分的无锁数据结构。

### <a name="Rconc-distrust"></a>CP.101: 不要信任你的硬件-编译器组合

##### 理由

无锁编程所使用的底层硬件接口，是属于最难正确实现的，
而且属于最可能会发生最微妙的兼容性问题的领域。
如果你为了性能而进行无锁编程的话，你应当进行回归检查。

##### 注解

指令重排（静态和动态的）会让我们很难有效在这个层次上进行思考（尤其当你使用宽松的内存模型的时候）。
经验，（半）形式化的模型以及模型检查可以提供帮助。
测试——通常需要极端程度——是基础。
“不要飞得太靠近太阳。”

##### 强制实施

准备强有力的规则，使得当硬件，操作系统，编译器，和程序库发生任何改变都能重复测试以进行覆盖。

### <a name="Rconc-literature"></a>CP.102: 仔细研究文献

##### 理由

除了原子和少数其他的标准模式之外，无锁编程真的是只有专家才懂的议题。
在发布无锁代码给其他人使用之前，应当先成为一名专家。

##### 参考文献

- Anthony Williams: C++ concurrency in action. Manning Publications.
- Boehm, Adve, You Don't Know Jack About Shared Variables or Memory Models , Communications of the ACM, Feb 2012.
- Boehm, "Threads Basics", HPL TR 2009-259.
- Adve, Boehm, "Memory Models: A Case for Rethinking Parallel Languages and Hardware", Communications of the ACM, August 2010.
- Boehm, Adve, "Foundations of the C++ Concurrency Memory Model", PLDI 08.
- Mark Batty, Scott Owens, Susmit Sarkar, Peter Sewell, and Tjark Weber, "Mathematizing C++ Concurrency", POPL 2011.
- Damian Dechev, Peter Pirkelbauer, and Bjarne Stroustrup: Understanding and Effectively Preventing the ABA Problem in Descriptor-based Lock-free Designs. 13th IEEE Computer Society ISORC 2010 Symposium. May 2010.
- Damian Dechev and Bjarne Stroustrup: Scalable Non-blocking Concurrent Objects for Mission Critical Code. ACM OOPSLA'09. October 2009
- Damian Dechev, Peter Pirkelbauer, Nicolas Rouquette, and Bjarne Stroustrup: Semantically Enhanced Containers for Concurrent Real-Time Systems. Proc. 16th Annual IEEE International Conference and Workshop on the Engineering of Computer Based Systems (IEEE ECBS). April 2009.
- Maurice Herlihy, Nir Shavit, Victor Luchangco, Michael Spear, "The Art of Multiprocessor Programming", 2nd ed. September 2020

### <a name="Rconc-double"></a>CP.110: 不要为初始化编写你自己的双检查锁定

##### 理由

从 C++11 开始，静态局部变量是以线程安全的方式初始化的。当和 RAII 模式结合时，静态局部变量可以取代为初始化自己编写双检查锁定的需求。`std::call_once` 也可以达成相同的目的。请使用 C++11 的静态局部变量或者 `std::call_once` 来代替自己为初始化编写的双检查锁定。

##### 示例

使用 `std::call_once` 的例子。

    void f()
    {
        static std::once_flag my_once_flag;
        std::call_once(my_once_flag, []()
        {
            // 这个只做一次
        });
        // ...
    }

使用 C++11 的线程安全静态局部变量的例子。

    void f()
    {
        // 假定编译器遵循 C++11
        static My_class my_object; // 构造函数仅调用一次
        // ...
    }

    class My_class
    {
    public:
        My_class()
        {
            // 这个只做一次
        }
    };

##### 强制实施

??? 是否可能检测出这种惯用法？

### <a name="Rconc-double-pattern"></a>CP.111: 当确实需要双检查锁定时应当采用惯用的模式

##### 理由

双检查锁定是很容易被搞乱的。如果确实需要编写自己的双检查锁定，而不顾规则 [CP.110: 不要为初始化编写你自己的双检查锁定](#Rconc-double)和规则 [CP.100: 除非绝对必要，请勿使用无锁编程](#Rconc-lockfree)，那么应当采用惯用的模式。

使用双检查锁定模式而不违反[CP.110: 不要为初始化编写你自己的双检查锁定](#Rconc-double)的情形，出现于当某个非线程安全的动作既困难也罕见，并且存在某个快速且线程安全的测试可以用于保证该动作并不需要实施的情况，但反过来的情况则无法保证。

##### 示例，不好

使用 `volatile` 并不能使得第一个检查线程安全，另见[CP.200: `volatile` 仅用于和非 C++ 内存进行通信](#Rconc-volatile2)

    mutex action_mutex;
    volatile bool action_needed;

    if (action_needed) {
        std::lock_guard<std::mutex> lock(action_mutex);
        if (action_needed) {
            take_action();
            action_needed = false;
        }
    }

##### 示例，好

    mutex action_mutex;
    atomic<bool> action_needed;

    if (action_needed) {
        std::lock_guard<std::mutex> lock(action_mutex);
        if (action_needed) {
            take_action();
            action_needed = false;
        }
    }

这对于正确调校的内存顺序可能会带来好处，其中的获取加载要比顺序一致性加载更加高效

    mutex action_mutex;
    atomic<bool> action_needed;

    if (action_needed.load(memory_order_acquire)) {
        lock_guard<std::mutex> lock(action_mutex);
        if (action_needed.load(memory_order_relaxed)) {
            take_action();
            action_needed.store(false, memory_order_release);
        }
    }

##### 强制实施

??? 是否可能检测出这种惯用法？

## <a name="SScp-etc"></a>CP.etc: 其他并发规则

这些规则不适于简单的分类：

- [CP.200: `volatile` 仅用于和非 C++ 内存进行通信](#Rconc-volatile2)
- [CP.201: ??? 信号](#Rconc-signal)

### <a name="Rconc-volatile2"></a>CP.200: `volatile` 仅用于和非 C++ 内存进行通信

##### 理由

`volatile` 用于涉指那些与“非 C++”代码之间共享的对象，或者不遵循 C++ 内存模型的硬件。

##### 示例

    const volatile long clock;

这说明了一个被某个时钟电路不断更新的寄存器。
`clock` 为 `volatile` 是因为其值将会在没有使用它的 C++ 程序的任何动作下被改变。
例如，两次读取 `clock` 经常会产生两个不同的值，因此优化器最好不要将下面代码中的第二个读取操作优化掉：

    long t1 = clock;
    // ... 这里没有对 clock 的使用 ...
    long t2 = clock;

`clock` 为 `const` 是因为程序不应当试图写入 `clock`。

##### 注解

除非你是在编写直接操作硬件的最底层代码，否则应当把 `volatile` 当做是某种深奥的功能特性并最好避免使用。

##### 示例

通常 C++ 代码接受的 `volatile` 内存是由别处所拥有的（硬件或其他语言）：

    int volatile* vi = get_hardware_memory_location();
        // 注意：我们获得了指向别人的内存的指针
        // volatile 说明“请特别尊重地对待”

有时候 C++ 代码会分配 `volatile` 内存，并通过故意地暴露一个指针来将其共享给“别人”（硬件或其他语言）：

    static volatile long vl;
    please_use_this(&vl);   // 暴露对这个的一个引用给“别人”（不是 C++）

##### 示例，不好

`volatile` 局部变量几乎都是错误的——既然是短暂的，它们如何才能共享给其他语言或硬件呢？
因为相同的理由，这几乎同样强有力地适用于成员变量。

    void f()
    {
        volatile int i = 0; // 不好，volatile 局部变量
        // etc.
    }

    class My_type {
        volatile int i = 0; // 可以的，volatile 成员变量
        // etc.
    };

##### 注解

于其他一些语言不通，C++ 中的 `volatile` [和同步没有任何关系](#Rconc-volatile)。

##### 强制实施

- 对 `volatile T` 的局部成员变量进行标记；几乎肯定你应当用 `atomic<T>` 进行代替。
- ???

### <a name="Rconc-signal"></a>CP.201: ??? 信号

???UNIX 信号处理??? 也许值得提及异步信号安全有多么微弱，以及如何同信号处理器进行通信（也许最好应当“完全避免”）

# <a name="S-errors"></a>E: 错误处理

错误处理涉及：

- 检测某个错误
- 将有关错误的信息传递给某个处理代码
- 维持程序的某个有效状态
- 避免资源泄漏

不可能做到从所有的错误中恢复。如果从某个错误进行恢复是不可能的话，以明确定义的方式迅速“脱离”则是很重要的。错误处理的策略必须简单，否则就会成为更糟糕错误的来源。未经测试和很少被执行的错误处理代码自身也是许多 BUG 的来源。

以下规则是设计用以帮助避免几种错误的：

- 类型违规（比如对 `union` 和强制转换的误用）
- 资源泄漏（包括内存泄漏）
- 边界错误
- 生存期错误（比如在对象被 `delete` 后访问它）
- 复杂性错误（可能由于过于复杂的想法表达而导致的逻辑错误）
- 接口错误（比如通过接口传递了预期外的值）

错误处理规则概览：

- [E.1: 在设计中尽早开发错误处理策略](#Re-design)
- [E.2: 通过抛出异常来明示函数无法完成其所赋予的任务](#Re-throw)
- [E.3: 仅使用异常来进行错误处理](#Re-errors)
- [E.4: 围绕不变式来设计错误处理策略](#Re-design-invariants)
- [E.5: 让构造函数建立不变式，若其无法做到则抛出异常](#Re-invariant)
- [E.6: 使用 RAII 来避免泄漏](#Re-raii)
- [E.7: 明示前条件](#Re-precondition)
- [E.8: 明示后条件](#Re-postcondition)

- [E.12: 当函数不可能或不能接受以 `throw` 来退出时，使用 `noexcept`](#Re-noexcept)
- [E.13: 不要在作为某个对象的直接所有者时抛出异常](#Re-never-throw)
- [E.14: 应当使用为目的所设计的自定义类型（而不是内建类型）作为异常](#Re-exception-types)
- [E.15: 按值抛出并按引用捕获类型层次中的异常](#Re-exception-ref)
- [E.16: 析构函数，回收函数，`swap`，以及异常类型的复制/移动构造决不能失败](#Re-never-fail)
- [E.17: 不要试图在每个函数中捕获每个异常](#Re-not-always)
- [E.18: 最小化对 `try`/`catch` 的显式使用](#Re-catch)
- [E.19: 当没有合适的资源包装时，使用 `final_action` 对象来表达清理动作](#Re-finally)

- [E.25: 当不能抛出异常时，模拟 RAII 来进行资源管理](#Re-no-throw-raii)
- [E.26: 当不能抛出异常时，考虑采取快速失败](#Re-no-throw-crash)
- [E.27: 当不能抛出异常时，系统化地使用错误代码](#Re-no-throw-codes)
- [E.28: 避免基于全局状态（比如 `errno`）的错误处理](#Re-no-throw)

- [E.30: 请勿使用异常说明](#Re-specifications)
- [E.31: 恰当地对 `catch` 子句排序](#Re_catch)

### <a name="Re-design"></a>E.1: 在设计中尽早开发错误处理策略

##### 理由

在一个系统中改造翻新一种一致且完整的处理错误和资源泄漏的策略是很难的。

### <a name="Re-throw"></a>E.2: 通过抛出异常来明示函数无法完成其所赋予的任务

##### 理由

让错误处理有系统性，强健，而且避免重复。

##### 示例

    struct Foo {
        vector<Thing> v;
        File_handle f;
        string s;
    };

    void use()
    {
        Foo bar {{Thing{1}, Thing{2}, Thing{monkey}}, {"my_file", "r"}, "Here we go!"};
        // ...
    }

这里，`vector` 和 `string` 的构造函数可能无法为其元素分配足够的内存，`vector` 的构造函数可能无法复制其初始化式列表中的 `Thing`，而 `File_handle` 可能无法打开所需的文件。
这些情况中，它们都会抛出异常来让 `use()` 的调用者来处理。
如果 `use()` 可以处理这种构造 `bar` 的故障的话，它可以用 `try`/`catch` 来控制。
这些情况下，`Foo` 的构造函数都会在把控制传递给试图创建 `Foo` 的任何代码之前恰当地销毁以及构造的内存。
注意它并不存在可以包含错误代码的返回值。

`File_handle` 的构造函数可以这样定义：

    File_handle::File_handle(const string& name, const string& mode)
        : f{fopen(name.c_str(), mode.c_str())}
    {
        if (!f)
            throw runtime_error{"File_handle: could not open " + name + " as " + mode"}
    }

##### 注解

人们通常说异常应当用于表明意外的事件和故障。
不过这里面存在一点循环，“什么是意外的？”
例如：

- 无法满足的前条件
- 无法构造对象的构造函数（无法建立类的[不变式](#Rc-struct)）
- 越界错误（比如 `v[v.size()] = 7`）
- 无法获得资源（比如网络未连接）

相较而言，终止某个普通的循环则不是意外的。
除非这个循环本应当是无限循环，否则其终止就是正常且符合预期的。

##### 注解

不要用 `throw` 仅仅作为从函数中返回值的另一种方式。

##### 例外

某些系统，比如在执行开始之前就需要保证以（通常很短的）常量最大时间来执行动作的硬实时系统，这样的系统只有当存在工具可以支持精确地预测从一次 `throw` 中恢复的最大时间时才能使用异常。

**参见**: [RAII](#Re-raii)

**参见**: [讨论](#Sd-noexcept)

##### 注解

在你决定你无法负担或者不喜欢基于异常的错误处理之前，请看一看[替代方案](#Re-no-throw-raii)；
它们各自都有自己的复杂性和问题。
同样地，只要可能的话，就应该进行测量之后再发表有关效率的言论。

### <a name="Re-errors"></a>E.3: 仅使用异常来进行错误处理

##### 理由

以保持错误处理和“常规代码”互相分离。
C++ 实现都倾向于基于假定异常的稀有而进行优化。

##### 示例，请勿如此

    // 请勿如此: 异常并未用于错误处理
    int find_index(vector<string>& vec, const string& x)
    {
        try {
            for (gsl::index i = 0; i < vec.size(); ++i)
                if (vec[i] == x) throw i;  // 找到了 x
        }
        catch (int i) {
            return i;
        }
        return -1;   // 未找到
    }

这种代码要比显然的替代方式更加复杂，而且极可能运行慢得多。
在 `vector` 中寻找一个值是没什么意外情况的。

##### 强制实施

可能应该是启发式措施。
查找从 `catch` 子句“漏掉”的异常值。

### <a name="Re-design-invariants"></a>E.4: 围绕不变式来设计错误处理策略

##### 理由

要使用一个对象，它必须处于某个（正式或非正式通过不变式所定义的）有效的状态，而要从错误中恢复，每个还未销毁的对象也必须处于有效的状态。

##### 注解

[不变式](#Rc-struct)是对象的成员的逻辑条件，构造函数必须进行建立，且为公开的成员函数所假定。

##### 强制实施

???

### <a name="Re-invariant"></a>E.5: 让构造函数建立不变式，若其无法做到则抛出异常

##### 理由

遗留仍未建立不变式的对象将会带来麻烦。
不是任何成员函数都可以对其进行调用。

##### 示例

    class Vector {  // 非常简单的 double 向量
        // 当 elem != nullptr 时 elem 指向 sz 个 double
    public:
        Vector() : elem{nullptr}, sz{0}{}
        Vector(int s) : elem{new double[s]}, sz{s} { /* 元素的初始化 */ }
        ~Vector() { delete [] elem; }
        double& operator[](int s) { return elem[s]; }
        // ...
    private:
        owner<double*> elem;
        int sz;
    };

类不变式——这里以代码注释说明——是由构造函数建立的。
当 `new` 无法分配所需的内存时将抛出异常。
各运算符，尤其是下标运算符，都是依赖于这个不变式的。

**参见**: [当构造函数无法构造有效对象时，应当抛出异常](#Rc-throw)

##### 强制实施

对带有 `private` 状态但没有（公开，受保护或私有的）构造函数的类进行标记。

### <a name="Re-raii"></a>E.6: 使用 RAII 来避免泄漏

##### 理由

资源泄漏通常是不可接受的。
手工的资源释放很易出错。
RAII（Resource Acquisition Is Initialization，资源获取即初始化）是最简单，最系统化的避免泄漏方案。

##### 示例

    void f1(int i)   // 不好: 可能会泄漏
    {
        int* p = new int[12];
        // ...
        if (i < 17) throw Bad{"in f()", i};
        // ...
    }

我们可以在抛出异常前小心地释放资源：

    void f2(int i)   // 笨拙且易错: 显式的释放
    {
        int* p = new int[12];
        // ...
        if (i < 17) {
            delete[] p;
            throw Bad{"in f()", i};
        }
        // ...
    }

这样很啰嗦。在更大型的可能带有多个 `throw` 的代码中，显式的释放将变得重复且易错。

    void f3(int i)   // OK: 通过资源包装来进行资源管理（请见下文）
    {
        auto p = make_unique<int[]>(12);
        // ...
        if (i < 17) throw Bad{"in f()", i};
        // ...
    }

注意即使 `throw` 是在所调用的函数中暗中发生，这也能正常工作：

    void f4(int i)   // OK: 通过资源包装来进行资源管理（请见下文）
    {
        auto p = make_unique<int[]>(12);
        // ...
        helper(i);   // 可能抛出异常
        // ...
    }

除非你确实需要指针语义，否则还是应当使用局部的资源对象：

    void f5(int i)   // OK: 通过局部对象来进行资源管理
    {
        vector<int> v(12);
        // ...
        helper(i);   // 可能抛出异常
        // ...
    }

这即简单又安全，而且通常更加高效。

##### 注解

当没有合适的资源包装，且定义一个适当的 RAII 对象/包装由于某种原因不可行时，
万不得已，可以使用 [`final_action` 对象](#Re-finally)来表达清理动作。

##### 注解

但是当我们所编写的程序不能使用异常时应当怎么办呢？
首先应当质疑这项假设；到处都有许多反异常的错误认识。
据我们所知，只有少量正当理由：

- 我们所在的系统太小，支持异常将会吃掉我们的 2K 内存的大部分。
- 我们所在的是硬实时系统，而且我们没有工具能保证异常会在所需时间内处理掉。
- 我们所在的系统中有成吨的遗留代码以难于理解的方式大量地使用指针
  （尤其是没有可识别的所有权策略），因此异常可能会造成泄露。
- 我们的 C++ 异常机制的实现不合理地糟糕
  （很慢，很耗内存，对于动态链接库无法正确工作，等等）。
  请向你的实现的供应商提出意见；如果没有用户提出意见，就不会出现改进。
- 如果我们质疑经理的古老智慧的话会被炒鱿鱼。

以上原因中只有第一条才是基础问题，因此一旦可能的话，还是要用异常来实现 RAII，或者设计你的 RAII 对象永不失败。
当无法使用异常时，可以模拟 RAII。
就是说，系统化地在对象构造之后检查其有效性，并且仍然在析构函数中释放所有的资源。
一种策略是为每个资源包装添加一个 `valid()` 操作：

    void f()
    {
        vector<string> vs(100);   // 非 std::vector: 添加了 valid()
        if (!vs.valid()) {
            // 处理错误或退出
        }

        ifstream fs("foo");   // 非 std::ifstream: 添加了 valid()
        if (!fs.valid()) {
            // 处理错误或退出
        }

        // ...
    } // 析构函数如常进行清理

显然这样做增加了代码大小，不允许隐式的“异常”（`valid()` 检查）传播，而且 `valid()` 检查可能被忘掉。
优先采用异常。

**参见**: [`noexcept` 的用法](#Re-noexcept)

##### 强制实施

???

### <a name="Re-precondition"></a>E.7: 明示前条件

##### 理由

避免接口错误。

**参见**: [前条件规则](#Ri-pre)

### <a name="Re-postcondition"></a>E.8: 明示后条件

##### 理由

避免接口错误。

**参见**: [后条件规则](#Ri-post)

### <a name="Re-noexcept"></a>E.12: 当函数不可能或不能接受以 `throw` 来退出时，使用 `noexcept`

##### 理由

使错误处理系统化，强健，且高效。

##### 示例

    double compute(double d) noexcept
    {
        return log(sqrt(d <= 0 ? 1 : d));
    }

这里，我们已知 `compute` 不会抛出异常，因为它仅由不会抛出异常的操作所组成。
通过将 `compute` 声明为 `noexcept`，让编译器和人类阅读者获得信息，使其更容易理解和操作 `compute`。

##### 注解

许多标准库函数都是 `noexcept` 的，这包括所有从 C 标准库中“继承”来的标准库函数。

##### 示例

    vector<double> munge(const vector<double>& v) noexcept
    {
        vector<double> v2(v.size());
        // ... 做一些事 ...
    }

这里的 `noexcept` 表明我不希望或无法处理无法构造局部的 `vector` 对象的情形。
也就是说，我认为内存耗尽是一种严重的设计错误（类比于硬件故障），因此我希望当其发生时让程序崩溃。

##### 注解

请勿使用传统的[异常说明](#Re-specifications)。

##### 参见

[讨论](#Sd-noexcept)。

### <a name="Re-never-throw"></a>E.13: 不要在作为某个对象的直接所有者时抛出异常

##### 理由

这可能导致一次泄漏。

##### 示例

    void leak(int x)   // 请勿如此: 可能泄漏
    {
        auto p = new int{7};
        if (x < 0) throw Get_me_out_of_here{};  // 可能泄漏 *p
        // ...
        delete p;   // 可能不会执行到这里
    }

避免这种问题的一种方法是坚持使用资源包装：

    void no_leak(int x)
    {
        auto p = make_unique<int>(7);
        if (x < 0) throw Get_me_out_of_here{};  // 将按需删除 *p
        // ...
        // 无须 delete p
    }

另一种（通常更好）的方案是使用一个局部变量来消除指针的显式使用：

    void no_leak_simplified(int x)
    {
        vector<int> v(7);
        // ...
    }

##### 注解

如果有需要清理的某个局部“东西”，但其并未表示为带有析构函数的对象，则这样的清理
也必须在 `throw` 之前完成。
有时候，[`finally()`](#Re-finally) 可以把这种不系统的清理变得更加可管理一些。

### <a name="Re-exception-types"></a>E.14: 应当使用为目的所设计的自定义类型（而不是内建类型）作为异常

##### 理由

自定义类型可以把有关某个错误的信息更好地传递给处理器。
这些信息可以编码到类型自身中，而类型则不大可能会和其他人的异常造成冲突。

##### 示例

    throw 7; // 不好

    throw "something bad";  // 不好

    throw std::exception(); // 不好 - 未提供信息

从 `std::exception` 派生，能够获得选择捕获特定异常或者通过 `std::exception` 进行通盘处理的灵活性：

    class MyException: public std::runtime_error
    {
    public:
        MyException(const string& msg) : std::runtime_error(msg) {}
        // ...
    };

    // ...

    throw MyException("something bad");  // 好

异常可以不必派生于 `std::exception`：

    class MyCustomError final {};  // 并未派生于 std::exception

    // ...

    throw MyCustomError{};  // 好 - 处理器必须捕获这个类型（或 ...）

当检测位置没有可以添加的有用信息时，可以使用派生于 `exception`
的库类型作为通用类型：

    throw std::runtime_error{"someting bad"}; // 好

    // ...

    throw std::invalid_argument("i is not even"); // 好

也可以使用 `enum` 类：

    enum class alert {RED, YELLOW, GREEN};

    throw alert::RED; // 好

##### 强制实施

识别针对内建类型和 `std::exception` 的 `throw`。

### <a name="Re-exception-ref"></a>E.15: 按值抛出并按引用捕获类型层次中的异常

##### 理由

按值（而非指针）抛出并按引用捕获，能避免进行复制，尤其是基类子对象的切片。

##### 示例，不好

    void f()
    {
        try {
            // ...
            throw new widget{}; // 请勿如此：抛出值而不要抛出原始指针
            // ...
        }
        catch (base_class e) {  // 请勿如此: 可能造成切片
            // ...
        }
    }

可以代之以引用：

    catch (base_class& e) { /* ... */ }

或者（通常更好的）`const` 引用：

    catch (const base_class& e) { /* ... */ }

大多数处理器并不会改动异常，一般情况下我们都会[建议使用 `const`](#Res-const)。

##### 注解

对于如一个 `enum` 值这样的小型值类型来说，按值捕获是合适的。

##### 注解

重新抛出已捕获的异常应当使用 `throw;` 而非 `throw e;`。使用 `throw e;` 将会抛出 `e` 的一个新副本（并于异常被 `catch (const std::exception& e)` 捕获时切片成静态类型 `std::exception`），而并非重新抛出原来的 `std::runtime_error` 类型的异常。（但请关注[请勿试图在每个函数中捕获所有的异常](#Re-not-always)，以及[尽可能减少 `try`/`catch` 的显式使用](#Re-catch)。)

##### 强制实施

- 对按值捕获具有虚函数的类型进行标记。
- 对抛出原始指针进行标记。

### <a name="Re-never-fail"></a>E.16: 析构函数，回收函数，`swap`，以及异常类型的复制/移动构造决不能失败

##### 理由

如果析构函数，`swap`，内存回收，或者尝试复制/移动异常对象时会失败，就是说如果它会通过异常而退出，或者根本不会实施其所需的动作，我们就将不知道应当如何编写可靠的程序。

##### 示例，请勿如此

    class Connection {
        // ...
    public:
        ~Connection()   // 请勿如此: 非常糟糕的析构函数
        {
            if (cannot_disconnect()) throw I_give_up{information};
            // ...
        }
    };

##### 注解

许多人都曾试图编写违反这条规则的可靠代码，比如当网络连接“拒绝关闭”的情形。
尽我们所知，没人曾找到一个做到这点的通用方案。
虽然偶尔对于非常特殊的例子，你可以通过设置某个状态以便进行将来的清理的方式绕过它。
比如说，我们可能将一个不打算关闭的 socket 放入一个“故障 socket”列表之中，
让其被某个定期的系统状态清理所检查处理。
我们见过的每个这种例子都是易错的，专门的，而且通常有 BUG。

##### 注解

标准库假定析构函数，回收函数（比如 `operator delete`），和 `swap` 都不会抛出异常。当它们这样做时，基本的标准库不变式将会被打破。

##### 注解

- 回收函数，包括 `operator delete`，必须为 `noexcept`。
- `swap` 函数必须为 `noexcept`。
- 大多数的析构函数都是缺省隐含为 `noexcept` 的。
- 而且，[应该使移动操作为 `noexcept`](#Rc-move-noexcept)。
- 当编写用作异常类型的类型时，确保其复制构造函数不为 `noexcept`。一般来说我们没法机制化地强制这一点，因为我们并不了解一个类型是否有意作为一种异常类型。
- 尝试避免抛出复制构造函数不为 `noexcept` 的类型。一般来说我们没法机制化地强制这一点，因为即便 `throw std::string(...)` 也可能抛异常，虽然实际上并不会。

##### 强制实施

- 识别会 `throw` 的析构函数，回收操作，和 `swap`。
- 识别不为 `noexcept` 的这类操作。

**参见**: [讨论](#Sd-never-fail)

### <a name="Re-not-always"></a>E.17: 不要试图在每个函数中捕获每个异常

##### 理由

如果函数无法对异常进行有意义的恢复动作，其捕获这个异常就导致复杂性和浪费。
要让异常传播直到遇到一个可以处理它的函数。
要用 [RAII](#Re-raii) 来处理栈回溯路径上的清理操作。

##### 示例，请勿如此

    void f()   // 不好
    {
        try {
            // ...
        }
        catch (...) {
            // 不做任何事
            throw;   // 传播异常
        }
    }

##### 强制实施

- 标记嵌套的 `try` 块。
- 对带有过高的 `try` 块/函数比率的源代码文件进行标记。 (??? 问题：定义“过高”)

### <a name="Re-catch"></a>E.18: 最小化对 `try`/`catch` 的显式使用

##### 理由

`try`/`catch` 很啰嗦，而且非平凡的使用是易错的。
`try`/`catch` 可以作为对非系统化和/或低级的资源管理或错误处理的一个信号。

##### 示例，不好

    void f(zstring s)
    {
        Gadget* p;
        try {
            p = new Gadget(s);
            // ...
            delete p;
        }
        catch (Gadget_construction_failure) {
            delete p;
            throw;
        }
    }

这段代码很混乱。
可能在 `try` 块中的裸指针上发生泄漏。
不是所有的异常都被处理了。
`delete` 一个构造失败的对象几乎肯定是一个错误。
更好的是：

    void f2(zstring s)
    {
        Gadget g {s};
    }

##### 替代方案

- 合适的资源包装以及 [RAII](#Re-raii)
- [`finally`](#Re-finally)

##### 强制实施

??? 很难，需要启发式方法

### <a name="Re-finally"></a>E.19: 当没有合适的资源包装时，使用 `final_action` 对象来表达清理动作

##### 理由

[GSL](#gsl-guidelines-support-library) 提供的 `finally` 要比 `try`/`catch` 更不啰嗦且难于搞错。

##### 示例

    void f(int n)
    {
        void* p = malloc(n);
        auto _ = gsl::finally([p] { free(p); });
        // ...
    }

##### 注解

`finally` 没有 `try`/`catch` 那样混乱，但它仍然比较专门化。
优先采用[适当的资源管理对象](#Re-raii)。
万不得已考虑使用 `finally`。

##### 注解

相对于老式的 [`goto exit;` 技巧](#Re-no-throw-codes)来说，使用 `finally` 是处理并非系统化的资源管理中的清理工作的
更加系统化并且相当简洁的方案。

##### 强制实施

启发式措施：检测 `goto exit;`。

### <a name="Re-no-throw-raii"></a>E.25: 当不能抛出异常时，模拟 RAII 来进行资源管理

##### 理由

即便没有异常，[RAII](#Re-raii) 通常仍然是最佳且最系统化的处理资源的方式。

##### 注解

使用异常进行错误处理，是 C++ 中唯一完整且系统化的处理非局部错误的方式。
特别是，非侵入地对对象构造的失败进行报告需要使用异常。
以无法被忽略的方式报告错误需要使用异常。
如果无法使用异常，应当尽你所能模拟它们的使用。

大量对异常的惧怕是被误导的。
当用在并非充斥指针和复杂控制结构的代码中的违例情形时，
异常处理几乎总是（在时间和空间上）可以负担，且几乎总会导致更好的代码。
当然这假定存在一个优良的异常处理机制实现，而这并非在所有系统上都存在。
存在另一些情况并不适用于上述问题，但由于其他原因而无法使用异常。
一个例子是一些硬实时系统：必须在固定的时间之内完成操作并得到错误或正确的响应。
在没有适当的时间评估工具的条件下，很难对异常作出保证。
这样的系统（比如飞控系统）通常也会禁止使用动态（堆）内存。

因此，对于错误处理的主要指导方针还是“使用异常和 [RAII](#Re-raii)。”
本节所处理的情况是，要么你没有高效的异常实现，
或者要面对大量的老式代码
（比如说，大量的指针，不明确定义的所有权，以及大量的不系统化的基于错误代码检查的错误处理）
而且向其引入简单且系统化的错误处理的做法不可行。

在宣称不能使用异常或者抱怨它们成本过高之前，应当考虑一下使用[错误代码](#Re-no-throw-codes)的例子。
请考虑使用错误代码的成本和复杂性。
如果你担心性能的话，请进行测量。

##### 示例

假定你想要编写

    void func(zstring arg)
    {
        Gadget g {arg};
        // ...
    }

当这个 `g` 并未正确构造时，`func` 将以一个异常退出。
当无法抛出异常时，我们可以通过向 `Gadget` 添加 `valid()` 成员函数来模拟 RAII 风格的资源包装：

    error_indicator func(zstring arg)
    {
        Gadget g {arg};
        if (!g.valid()) return gadget_construction_error;
        // ...
        return 0;   // 零代表“正常”
    }

显然问题现在变成了调用者必须记得测试其返回值。考虑添加 `[[nodiscard]]` 以鼓励这样的做法。

**参见**: [讨论](#Sd-???)

##### 强制实施

（仅）对于这种想法的特定版本是可能的：比如检查资源包装的构造后进行系统化的 `valid()` 测试。

### <a name="Re-no-throw-crash"></a>E.26: 当不能抛出异常时，考虑采取快速失败

##### 理由

如果你无法做好错误恢复的话，至少你可以在发生更多后续的损害之前拜托出来。

**参见**：[模拟 RAII](#Re-no-throw-raii)

##### 注解

当你无法系统化地进行错误处理时，考虑以“程序崩溃”作为对任何无法局部处理的错误的回应。
就是说，如果你无法在检测到错误的函数的上下文中处理它，则调用 `abort()`，`quick_exit()`，
或者相似的某个将触发某种系统重启的函数。

在具有大量进程和/或大量计算机的系统中，你总要预计到并处理这些关键程序崩溃，
比如说源于硬件故障而引发。
这种情况下，“程序崩溃”只不过把错误处理留给了系统的下一个层次。

##### 示例

    void f(int n)
    {
        // ...
        p = static_cast<X*>(malloc(n * sizeof(X)));
        if (!p) abort();     // 当内存耗尽时 abort
        // ...
    }

大多数程序都无法得体地处理内存耗尽。这大略上等价于

    void f(int n)
    {
        // ...
        p = new X[n];    // 当内存耗尽时抛出异常（默认情况会调用 terminate）
        // ...
    }

通常，在退出之前将“崩溃”的原因记录日志是个好主意。

##### 强制实施

很难对付

### <a name="Re-no-throw-codes"></a>E.27: 当不能抛出异常时，系统化地使用错误代码

##### 理由

系统化地使用任何错误处理策略都能最小化忘记处理错误的机会。

**参见**：[模拟 RAII](#Re-no-throw-raii)

##### 注解

需要处理几个问题：

- 如何从函数向外传递错误指示？
- 如何在错误退出函数之前释放所有资源？
- 使用什么来作为错误指示？

通常，返回错误指示意味着返回两个值：其结果以及一个错误指示。
错误指示可以是对象的一部分，例如对象可以带有 `valid()` 指示，
也可以返回一对值。

##### 示例

    Gadget make_gadget(int n)
    {
        // ...
    }

    void user()
    {
        Gadget g = make_gadget(17);
        if (!g.valid()) {
                // 错误处理
        }
        // ...
    }

这种方案符合[模拟 RAII 资源管理](#Re-no-throw-raii)。
`valid()` 函数可以返回一个 `error_indicator`（比如说 `error_indicator` 枚举的某个成员）。

##### 示例

要是我们无法或者不想改动 `Gadget` 类型呢？
这种情况下，我们只能返回一对值。
例如：

    std::pair<Gadget, error_indicator> make_gadget(int n)
    {
        // ...
    }

    void user()
    {
        auto r = make_gadget(17);
        if (!r.second) {
                // 错误处理
        }
        Gadget& g = r.first;
        // ...
    }

可见，`std::pair` 是一种可能的返回类型。
某些人则更喜欢专门的类型。
例如：

    Gval make_gadget(int n)
    {
        // ...
    }

    void user()
    {
        auto r = make_gadget(17);
        if (!r.err) {
                // 错误处理
        }
        Gadget& g = r.val;
        // ...
    }

倾向于专门返回类型的一种原因是为其成员提供命名，而不是使用多少有些隐秘的 `first` 和 `second`,
而且可以避免与 `std::pair` 的其他使用相混淆。

##### 示例

通常，在错误退出之前必须进行清理。
这样做是很混乱的：

    std::pair<int, error_indicator> user()
    {
        Gadget g1 = make_gadget(17);
        if (!g1.valid()) {
            return {0, g1_error};
        }

        Gadget g2 = make_gadget(17);
        if (!g2.valid()) {
            cleanup(g1);
            return {0, g2_error};
        }

        // ...

        if (all_foobar(g1, g2)) {
            cleanup(g2);
            cleanup(g1);
            return {0, foobar_error};
        }

        // ...

        cleanup(g2);
        cleanup(g1);
        return {res, 0};
    }

模拟 RAII 可能不那么简单，尤其是在带有多个资源和多种可能错误的函数之中。
一种较为常见的技巧是把清理都集中到函数末尾以避免重复（注意这里本不必为 `g2` 增加一层作用域，但是编译 `goto` 版本却需要它）：

    std::pair<int, error_indicator> user()
    {
        error_indicator err = 0;
        int res = 0;

        Gadget g1 = make_gadget(17);
        if (!g1.valid()) {
            err = g1_error;
            goto g1_exit;
        }

        {
            Gadget g2 = make_gadget(31);
            if (!g2.valid()) {
                err = g2_error;
                goto g2_exit;
            }

            if (all_foobar(g1, g2)) {
                err = foobar_error;
                goto exit;
            }

            // ...

        g2_exit:
            if (g2.valid()) cleanup(g2);
        }

    g1_exit:
        if (g1.valid()) cleanup(g1);
        return {res,err};
    }

函数越大，这种技巧就越有吸引力。
`finally` 可以[略微缓解这个问题](#Re-finally)。
而且，程序变得越大，系统化地采用一中基于错误指示的错误处理策略就越加困难。

我们[优先采用基于异常的错误处理](#Re-throw)，并建议[保持函数短小](#Rf-single)。

**参见**: [讨论](#Sd-???)

**参见**: [返回多个值](#Rf-out-multi)

##### 强制实施

很难对付。

### <a name="Re-no-throw"></a>E.28: 避免基于全局状态（比如 `errno`）的错误处理

##### 理由

全局状态难于管理，且易于忘记检查。
你上次检查 `printf()` 的返回值是什么时候了？

**参见**：[模拟 RAII](#Re-no-throw-raii)

##### 示例，不好

    int last_err;

    void f(int n)
    {
        // ...
        p = static_cast<X*>(malloc(n * sizeof(X)));
        if (!p) last_err = -1;     // 当内存耗尽时发生的错误
        // ...
    }

##### 注解

C 风格的错误处理就是基于全局变量 `errno` 的，因此基本上不可能完全避免这种风格。

##### 强制实施

很难对付。

### <a name="Re-specifications"></a>E.30: 请勿使用异常说明

##### 理由

异常说明使得错误处理变得脆弱，隐含一些运行时开销，并且已经从 C++ 标准中被删除了。

##### 示例

    int use(int arg)
        throw(X, Y)
    {
        // ...
        auto x = f(arg);
        // ...
    }

当 `f()` 抛出了不同于 `X` 和 `Y` 的异常时将会执行未预期异常处理器，其默认将终止程序。
这没什么问题，但假定我们检查过着并不会发生而 `f` 则被改写为抛出某个新异常 `Z`，
这样将导致程序崩溃，除非我们改写 `use()`（并重新测试所有东西）。
障碍在于 `f()` 可能在某个我们无法控制的程序库中，而对于新的异常 `use()`
没办法对其做任何事，或者对其完全不感兴趣。
我们可以改写 `use()` 使其传递 `Z` 出去，但这样的话 `use()` 的调用方可能也需要被改写。
如此事态将很快变得无法掌控。
或者，我们可以在 `use()` 中添加一个 `try`-`catch` 以将 `Z` 映射为某种可以接受的异常。
这种方法也会很快变得无法掌控。
注意，对异常集合的改动通常都发生在系统的最底层
（比如说，当改换了网络库或者某种中间件时），因此改变将沿着冗长的调用链“冒泡上浮”。
在大型代码库中，这将意味着直到最后一个使用方也被改写之前，没人可以更新某个库到新版本。
如果 `use()` 是某个库的一部分，则也许不可能对其进行更新，因为其改动可能影响到未知的客户代码。

而让异常继续传递直到其到达某个潜在可以处理它的函数的策略，已经在多年的实践中得到了证明。

##### 注解

静态强制检查异常说明并不会带来任何好处。
相关例子请参见 [Stroustrup94](#Stroustrup94)。

##### 注解

当不会抛出异常时，请使用 [`noexcept`](#Re-noexcept)。

##### 强制实施

标记每个异常说明。

### <a name="Re_catch"></a>E.31: 恰当地对 `catch` 子句排序

##### 理由

`catch` 子句是以其出现顺序依次求值的，而其中一个可能会隐藏掉另一个。

##### 示例，不好

    void f()
    {
        // ...
        try {
                // ...
        }
        catch (Base& b) { /* ... */ }
        catch (Derived& d) { /* ... */ }
        catch (...) { /* ... */ }
        catch (std::exception& e) { /* ... */ }
    }

若 `Derived` 派生自 `Base` 则 `Derived` 的处理器永远不会被执行。
“捕获任何东西”的处理器保证 `std::exception` 的处理器永远不会被执行。

##### 强制实施

标记出所有的“隐藏处理器”。

# <a name="S-const"></a>Con: 常量与不可变性

常量是不会出现竞争条件的。
当大量的对象不会改变它们的值时，对程序进行推理将变得更容易。
承诺“不改动”作为参数所传递对象的接口，极大地提升了可读性。

常量规则概览：

- [Con.1: 缺省情况下，对象应当是不可变的](#Rconst-immutable)
- [Con.2: 缺省情况下，成员函数应当为 `const`](#Rconst-fct)
- [Con.3: 缺省情况下，应当传递指向 `const` 对象的指针或引用](#Rconst-ref)
- [Con.4: 构造之后不再改变其值的对象应当以 `const` 来定义](#Rconst-const)
- [Con.5: 以 `constexpr` 来定义可以在编译期计算的值](#Rconst-constexpr)

### <a name="Rconst-immutable"></a>Con.1: 缺省情况下，对象应当是不可变的

##### 理由

不可变对象更易于进行推理，应仅当需要改动对象的值时，才使之为非 `const` 对象。
避免出现意外造成的或者很难发觉的值的改变。

##### 示例

    for (const int i : c) cout << i << '\n';    // 仅进行读取: const

    for (int i : c) cout << i << '\n';          // 不好: 仅进行读取

##### 例外

按值传递的函数参数很少被改动，但也很少被声明为 `const`。
为了避免造成混淆和大量的误报，不要对函数参数实施这条规则。。

    void f(const char* const p); // 迂腐
    void g(const int i) { ... }  // 迂腐

注意，函数参数是局部变量，其改动也是局部的。

##### 强制实施

- 标记未发生改动的非 `const` 变量（排除参数以避免误报）

### <a name="Rconst-fct"></a>Con.2: 缺省情况下，成员函数应当为 `const`

##### 理由

除非成员函数会改变对象的可观察状态，否则它应当标记为 `const`。
这样做更精确地描述了设计意图，具有更佳的可读性，编译器可以识别更多的错误，而且有时能够带来更多的优化机会。

##### 示例，不好

    class Point {
        int x, y;
    public:
        int getx() { return x; }    // 不好，应当为 const，它并不改变对象的状态
        // ...
    };

    void f(const Point& pt)
    {
        int x = pt.getx();          // 错误，无法通过编译，因为 getx 并未标记为 const
    }

##### 注解

传递非 `const` 的指针或引用并非天生就是不好的，
但应当只有在所调用的函数预计会修改这个对象时才这样做。
代码的读者必须假定接受“普通的” `T*` 或 `T&` 的函数都将会修改其所指代的对象。
如果它现在不会，那它可能以后会，且无需强制要求重新编译。

##### 注解

有些代码和程序库提供的函数是声明为 `T*`，
但这些函数并不会修改这个 `T`。
这对于进行代码现代化转换的人们来说是个问题。
你可以：

- 如果倾向于长期解决方案的话，将程序库更新为 `const` 正确的；
- “强制掉 `const`”（[最好避免这样做](#Res-casts-const)）；
- 提供包装函数。

例如：

    void f(int* p);   // 老代码：f() 并不会修改 `*p`
    void f(const int* p) { f(const_cast<int*>(p)); } // 包装函数

注意，这种包装函数的方案是一种补丁，只能在无法修改 `f()` 的声明时才使用它，
比如当它属于某个你无法修改的程序库时。

##### 注解

`const` 成员函数可以改动 `mutable` 对象的值，或者通过某个指针成员改动对象的值。
一种常见用法是来维护一个缓存以避免重复进行复杂的运算。
例如，这里的 `Date` 缓存（记住）了其字符串表示，以简化其重复使用：

    class Date {
    public:
        // ...
        const string& string_ref() const
        {
            if (string_val == "") compute_string_rep();
            return string_val;
        }
        // ...
    private:
        void compute_string_rep() const;    // 计算字符串表示并将其存入 string_val
        mutable string string_val;
        // ...
    };

另一种说法是 `const` 特性不会传递。
通过 `const` 成员函数改动 `mutable` 成员的值和通过非 `const` 指针来访问的对象的值
是有可能的。
由类负责确保这样的改动仅当根据其语义（不变式）对于其用户有意义时
才会发生。

**参见**：[PImpl](#Ri-pimpl)

##### 强制实施

- 如果未标记为 `const` 的成员函数并未对任何成员变量实施非 `const` 操作的话，对其进行标记。

### <a name="Rconst-ref"></a>Con.3: 缺省情况下，应当传递指向 `const` 对象的指针或引用

##### 理由

避免所调用的函数意外地改变了这个值。
如果被调用的函数不会改动状态的话，对程序的推理将变得容易得多。

##### 示例

    void f(char* p);        // f 会不会修改 *p?（假定它会修改）
    void g(const char* p);  // g 不会修改 *p

##### 注解

传递指向非 `const` 对象的指针或引用并不是天生就有问题的，
不过只有当所调用的函数本就有意改动对象时才能这样做。

##### 注解

[请勿强制掉 `const`](#Res-casts-const)。

##### 强制实施

- 如果函数并未修改以指向非 `const` 的指针或引用传递的对象，则对其进行标记。
- 如果函数（利用强制转换）修改了以指向 `const` 的指针或引用传递的对象，则对其进行标记。

### <a name="Rconst-const"></a>Con.4: 构造之后不再改变其值的对象应当以 `const` 来定义

##### 理由

避免意外地改变对象的值。

##### 示例

    void f()
    {
        int x = 7;
        const int y = 9;

        for (;;) {
            // ...
        }
        // ...
    }

既然 `x` 并非 `const`，我们就必须假定它可能在循环中的某处会被修改。

##### 强制实施

- 标记并未被修改的非 `const` 变量。

### <a name="Rconst-constexpr"></a>Con.5: 以 `constexpr` 来定义可以在编译期计算的值

##### 理由

更好的性能，更好的编译期检查，受保证的编译期求值，竞争条件可能性为零。

##### 示例

    double x = f(2);            // 可能在运行时求值
    const double y = f(2);      // 可能在运行时求值
    constexpr double z = f(2);  // 除非 f(2) 可在编译期求值，否则会报错

##### 注解

参见 F.4。

##### 强制实施

- 对带有常量表达式初始化式的 `const` 定义进行标记。

# <a name="S-templates"></a>T: 模板和泛型编程

泛型编程是使用以类型、值和算法进行参数化的类型和算法所进行的编程。
在 C++ 中，泛型编程是以“模板”语言机制来提供支持的。

泛型函数的参数是以针对参数类型及其所涉及的值的规定的集合来刻画的。
在 C++ 中，这些规定是以被称为概念（concept）的编译期谓词来表达的。

模板还可用于进行元编程；亦即由编译期代码所组成的程序。

泛型编程的中心是“概念”；亦即以编译时谓词表现的对于模板参数的要求。
C++20 已经将“概念”标准化了，不过是在 GCC 6.1 中以一种略有不同的语法首次提供的。

模板使用的规则概览：

- [T.1: 用模板来提升代码的抽象层次](#Rt-raise)
- [T.2: 用模板来表达适用于许多参数类型的算法](#Rt-algo)
- [T.3: 用模板来表达容器和范围](#Rt-cont)
- [T.4: 用模板来表达对语法树的操作](#Rt-expr)
- [T.5: 结合泛型和面向对象技术来增强它们的能力，而不是它们的成本](#Rt-generic-oo)

概念使用的规则概览：

- [T.10: 为所有模板实参指明概念](#Rt-concepts)
- [T.11: 尽可能采用标准概念](#Rt-std-concepts)
- [T.12: 对于局部变量，优先采用概念名而不是 `auto`](#Rt-auto)
- [T.13: 对于简单的单类型参数概念，优先采用简写形式](#Rt-shorthand)
- ???

概念定义的规则概览：

- [T.20: 避免没有有意义的语义的“概念”](#Rt-low)
- [T.21: 为概念提出一组完整的操作要求](#Rt-complete)
- [T.22: 为概念指明公理](#Rt-axiom)
- [T.23: 通过添加新的使用模式，从更一般情形的概念中区分出提炼后的概念](#Rt-refine)
- [T.24: 用标签类或特征类来区分仅在语义上存在差别的概念](#Rt-tag)
- [T.25: 避免互补性的约束](#Rt-not)
- [T.26: 优先采用使用模式而不是简单的语法来定义概念](#Rt-use)
- [T.30: 节制地采用概念求反（`!C<T>`）来表示微小差异](#Rt-???)
- [T.31: 节制地采用概念析取（disjunction）（`C1<T> || C2<T>`）来表示备选项](#Rt-???)
- ???

模板接口的规则概览：

- [T.40: 使用函数对象向算法传递操作](#Rt-fo)
- [T.41: 在模板的概念上仅提出基本的性质要求](#Rt-essential)
- [T.42: 使用模板别名来简化写法并隐藏实现细节](#Rt-alias)
- [T.43: 优先使用 `using` 而不是 `typedef` 来定义别名](#Rt-using)
- [T.44: （如果可行）使用函数模板来对类模板的参数类型进行推断](#Rt-deduce)
- [T.46: 要求模板参数至少是半正规的](#Rt-regular)
- [T.47: 避免用常用名字命名高度可见的无约束模板](#Rt-visible)
- [T.48: 如果你的编译器不支持概念的话，可以用 `enable_if` 来模拟](#Rt-concept-def)
- [T.49: 尽可能避免类型擦除](#Rt-erasure)

模板定义的规则概览：

- [T.60: 最小化模板的上下文依赖性](#Rt-depend)
- [T.61: 请勿对成员进行过度参数化（SCARY）](#Rt-scary)
- [T.62: 将无依赖的类模板成员置于一个非模板基类之中](#Rt-nondependent)
- [T.64: 用特化来提供类模板的其他实现](#Rt-specialization)
- [T.65: 用标签分派来提供函数的其他实现](#Rt-tag-dispatch)
- [T.67: 用特化来提供不规则类型的其他实现](#Rt-specialization2)
- [T.68: 在模板中用 `{}` 而不是 `()` 以避免歧义](#Rt-cast)
- [T.69: 在模板中，请勿进行未限定的非成员函数调用，除非有意将之作为定制点](#Rt-customization)

模板和类型层次的规则概览：

- [T.80: 请勿不成熟地对类层次进行模板化](#Rt-hier)
- [T.81: 请勿混合类层次和数组](#Rt-array) // ??? 放在“类型层次”部分
- [T.82: 当不想要虚函数时，可以将类层次线性化](#Rt-linear)
- [T.83: 请勿声明虚的成员函数模板](#Rt-virtual)
- [T.84: 使用非模板的核心实现来提供 ABI 稳定的接口](#Rt-abi)
- [T.??: ????](#Rt-???)

变参模板的规则概览：

- [T.100: 当需要可以接受可变数量的多种类型参数的函数时，使用变参模板](#Rt-variadic)
- [T.101: ??? 如何向变参模板传递参数 ???](#Rt-variadic-pass)
- [T.102: ??? 如何处理变参模板的参数 ???](#Rt-variadic-process)
- [T.103: 请勿对同质参数列表使用变参模板](#Rt-variadic-not)
- [T.??: ????](#Rt-???)

元编程的规则概览：

- [T.120: 仅当确实需要时才使用模板元编程](#Rt-metameta)
- [T.121: 模板元编程主要用于模拟概念机制](#Rt-emulate)
- [T.122: 用模板（通常为模板别名）来在编译期进行类型运算](#Rt-tmp)
- [T.123: 用 `constexpr` 函数来在编译期进行值运算](#Rt-fct)
- [T.124: 优先使用标准库的模板元编程设施](#Rt-std-tmp)
- [T.125: 当需要标准库之外的模板元编程设施时，使用某个现存程序库](#Rt-lib)
- [T.??: ????](#Rt-???)

其他模板规则概览：

- [T.140: 若操作可被重用，则应为其命名](#Rt-name)
- [T.141: 当仅在一个地方需要一个简单的函数对象时，使用无名的 lambda](#Rt-lambda)
- [T.142: 使用模板变量以简化写法](#Rt-var)
- [T.143: 请勿编写并非有意非泛型的代码](#Rt-nongeneric)
- [T.144: 请勿特化函数模板](#Rt-specialize-function)
- [T.150: 用 `static_assert` 来检查类是否与概念相符](#Rt-check-class)
- [T.??: ????](#Rt-???)

## <a name="SS-GP"></a>T.gp: 泛型编程

泛型编程是利用以类型、值和算法进行了参数化的类型和算法所进行的编程。

### <a name="Rt-raise"></a>T.1: 用模板来提升代码的抽象层次

##### 理由

通用性。重用。效率。鼓励用户类型的定义一致性。

##### 示例，不好

概念上说，以下要求是错误的，因为我们所要求的 `T` 不止是如“可以增量”或“可以进行加法”这样的非常低级的概念：

    template<typename T>
        requires Incrementable<T>
    T sum1(vector<T>& v, T s)
    {
        for (auto x : v) s += x;
        return s;
    }

    template<typename T>
        requires Simple_number<T>
    T sum2(vector<T>& v, T s)
    {
        for (auto x : v) s = s + x;
        return s;
    }

由于假设 `Incrementable` 并不支持 `+`，而 `Simple_number` 也不支持 `+=`，我们对 `sum1` 和 `sum2` 的实现者过度约束了。
而且，这种情况下也错过了一次通用化的机会。

##### 示例

    template<typename T>
        requires Arithmetic<T>
    T sum(vector<T>& v, T s)
    {
        for (auto x : v) s += x;
        return s;
    }

通过假定 `Arithmetic` 同时要求 `+` 和 `+=`，我们约束 `sum` 的使用者来提供完整的算术类型。
这并非是最小的要求，但它为算法的实现者更多所需的自由度，并确保了任何 `Arithmetic` 类型
都可被用于各种各样的算法。

为达成额外的通用性和可重用性，我们还可以用更通用的 `Container` 或 `Range` 概念而不是仅投入到 `vector` 一个容器上。

##### 注解

如果我们所定义的模板提出的要求，完全是由一个算法的一个实现所要求的操作
（比如说仅要求 `+=` 而不同时要求 `=` 和 `+`），而没有别的要求，就会对维护者提出过度的约束。
我们的目标是最小化模板参数的要求，但一个实现的绝对最小要求很少能作为有意义的概念。

##### 注解

可以用模板来表现几乎任何东西（它是图灵完备的），但（利用模板进行）泛型编程的目标在于，
使操作和算法对于一组带有相似语义性质的类型有效地进行通用化。

##### 强制实施

- 对带有“过于简单”的要求（诸如不用概念而直接使用特定的运算符）的算法进行标记。
- 不要对“过于简单”的概念本身进行标记；它们也许只不过是更有用的概念的构造块。

### <a name="Rt-algo"></a>T.2: 用模板来表达适用于许多参数类型的算法

##### 理由

通用性。最小化源代码数量。互操作性。重用。

##### 示例

这正是 STL 的基础所在。一个 `find` 算法可以很轻易地同任何输入范围一起工作：

    template<typename Iter, typename Val>
        // requires Input_iterator<Iter>
        //       && Equality_comparable<Value_type<Iter>, Val>
    Iter find(Iter b, Iter e, Val v)
    {
        // ...
    }

##### 注解

除非确实需要多于一种模板参数类型，否则请不要使用模板。
请勿过度抽象。

##### 强制实施

??? 很难办，可能需要人来进行

### <a name="Rt-cont"></a>T.3: 用模板来表达容器和范围

##### 理由

容器需要一种元素类型，而将之表示为一个模板参数则是通用的，可重用的，且类型安全的做法。
这样也避免了采用脆弱的或者低效的变通手段。约定：这正是 STL 的做法。

##### 示例

    template<typename T>
        // requires Regular<T>
    class Vector {
        // ...
        T* elem;   // 指向 sz 个 T
        int sz;
    };

    Vector<double> v(10);
    v[7] = 9.9;

##### 示例，不好

    class Container {
        // ...
        void* elem;   // 指向 sz 个某种类型的元素
        int sz;
    };

    Container c(10, sizeof(double));
    ((double*) c.elem)[7] = 9.9;

这样做无法直接表现出程序员的意图，而且向类型系统和优化器隐藏了程序的结构。

把 `void*` 隐藏在宏之中只会掩盖问题，并引入新的发生混乱的机会。

**例外**: 当需要 ABI 稳定的接口时，也许你不得不提供一个基本实现，在基于它来呈现（类型安全的）目标。
参见[稳定的基类](#Rt-abi)。

##### 强制实施

- 对出现于低级实现代码之外的 `void*` 和强制转换进行标记。

### <a name="Rt-expr"></a>T.4: 用模板来表达对语法树的操作

##### 理由

???

##### 示例

    ???

**例外**: ???

### <a name="Rt-generic-oo"></a>T.5: 结合泛型和面向对象技术来增强它们的能力，而不是它们的成本

##### 理由

泛型和面向对象技术是互补的。

##### 示例

静态能够帮助动态：使用静态多态来实现动态多态的接口：

    class Command {
        // 纯虚函数
    };

    // 实现
    template</*...*/>
    class ConcreteCommand : public Command {
        // 实现虚函数
    };

##### 示例

动态有助于静态：提供通用的，便利的，静态绑定的接口，但内部进行动态派发，这样就可以提供统一的对象布局。
这样的例子包括如 `std::shared_ptr` 的删除器的类型擦除。（不过[请勿过度使用类型擦除](#Rt-erasure)。）

    #include <memory>

    class Object {
    public:
        template<typename T>
        Object(T&& obj)
            : concept_(std::make_shared<ConcreteCommand<T>>(std::forward<T>(obj))) {}

        int get_id() const { return concept_->get_id(); }

    private:
        struct Command {
            virtual ~Command() {}
            virtual int get_id() const = 0;
        };

        template<typename T>
        struct ConcreteCommand final : Command {
            ConcreteCommand(T&& obj) noexcept : object_(std::forward<T>(obj)) {}
            int get_id() const final { return object_.get_id(); }

        private:
            T object_;
        };

        std::shared_ptr<Command> concept_;
    };

    class Bar {
    public:
        int get_id() const { return 1; }
    };

    struct Foo {
    public:
        int get_id() const { return 2; }
    };

    Object o(Bar{});
    Object o2(Foo{});

##### 注解

类模板之中，非虚函数仅会在被使用时才会实例化——而虚函数则每次都会实例化。
这会导致代码大小膨胀，而且可能因为实例化从不需要的功能而导致对通用类型的过度约束。
请避免这样做，虽然标准的刻面类犯过这种错误。

##### 参见

- ref ???
- ref ???
- ref ???

##### 强制实施

参见参考条目以获得更具体的规则。

## <a name="SS-concepts"></a>T.concepts: 概念规则

概念是一种 C++20 语言设施，用于为模板参数指定要求。
在考虑泛型编程，以及未来的 C++ 程序库（无论标准的还是其他的）的基础时，
概念都是关键性的。

本部分假定有概念支持。

概念使用的规则概览：

- [T.10: 为所有模板实参指明概念](#Rt-concepts)
- [T.11: 尽可能采用标准概念](#Rt-std-concepts)
- [T.12: 优先采用概念名而不是 `auto`](#Rt-auto)
- [T.13: 对于简单的单类型参数概念，优先采用简写形式](#Rt-shorthand)
- ???

概念定义的规则概览：

- [T.20: 避免没有有意义的语义的“概念”](#Rt-low)
- [T.21: 为概念提出一组完整的操作要求](#Rt-complete)
- [T.22: 为概念指明公理](#Rt-axiom)
- [T.23: 通过添加新的使用模式，从更一般情形的概念中区分出提炼后的概念](#Rt-refine)
- [T.24: 用标签类或特征类来区分仅在语义上存在差别的概念](#Rt-tag)
- [T.25: 避免互补性的约束](#Rt-not)
- [T.26: 优先采用使用模式而不是简单的语法来定义概念](#Rt-use)
- ???

## <a name="SS-concept-use"></a>T.con-use: 概念的使用

### <a name="Rt-concepts"></a>T.10: 为所有模板实参指明概念

##### 理由

正确性和可读性。
针对模板参数所假定的含义（包括语法和语义），是模板接口的基础部分。
使用概念能够显著改善模板的文档和错误处理。
为模板参数指定概念是一种强力的设计工具。

##### 示例

    template<typename Iter, typename Val>
        requires input_iterator<Iter>
                 && equality_comparable_with<value_type_t<Iter>, Val>
    Iter find(Iter b, Iter e, Val v)
    {
        // ...
    }

也可以等价地用更为简洁的方式：

    template<input_iterator Iter, typename Val>
        requires equality_comparable_with<value_type_t<Iter>, Val>
    Iter find(Iter b, Iter e, Val v)
    {
        // ...
    }

##### 注解

普通的 `typename`（或 `auto`）是受最少约束的概念。
应当仅在只能假定“这是一个类型”的罕见情况中使用它们。
通常，这只会在当我们（用模板元编程代码）操作纯粹的表达式树，并推迟进行类型检查时才会需要。

**参考**: TC++PL4

##### 强制实施

对没有概念的模板类型参数进行标记。

### <a name="Rt-std-concepts"></a>T.11: 尽可能采用标准概念

##### 理由

“标准”概念（即由 [GSL](#gsl-guidelines-support-library) 和 ISO 标准自身所提供的概念)，
避免了我们思考自己的概念，它们比我们匆忙中能够想出来的要好得多，而且还提升了互操作性。

##### 注解

如果你不是要创建一个新的泛型程序库的话，大多数所需的概念都已经在标准库中定义过了。

##### 示例

    template<typename T>
        // 请勿定义这个: <iterator> 中已经有 sortable
    concept Ordered_container = Sequence<T> && Random_access<Iterator<T>> && Ordered<Value_type<T>>;

    void sort(Ordered_container auto& s);

这个 `Ordered_container` 貌似相当合理，但它和标准库中的 `sortable` 概念非常相似。
它是更好？更正确？它真的精确地反映了标准对于 `sort` 的要求吗？
直接使用 `sortable` 则更好而且更简单：

    void sort(sortable auto& s);   // 更好

##### 注解

在我们推进一个包含概念的 ISO 标准的过程中，“标准”概念的集合是不断演进的。

##### 注解

设计一个有用的概念是很有挑战性的。

##### 强制实施

很难。

- 查找无约束的参数，使用“非常规”或非标准的概念的模板，以及使用“自造的”又没有公理的概念的目标。
- 开发一种概念识别工具（例如，参考[一种早期实验](http://www.stroustrup.com/sle2010_webversion.pdf)）。

### <a name="Rt-auto"></a>T.12: 对于局部变量，优先采用概念名而不是 `auto`

##### 理由

`auto` 是最弱的概念。概念的名字会比仅用 `auto` 传达出更多的意义。

##### 示例

    vector<string> v{ "abc", "xyz" };
    auto& x = v.front();        // 不好
    String auto& s = v.front(); // 好（String 是 GSL 的一个概念）

##### 强制实施

- ???

### <a name="Rt-shorthand"></a>T.13: 对于简单的单类型参数概念，优先采用简写形式

##### 理由

可读性。直接表达意图。

##### 示例

这样来表达“`T` 是一种 `sortable`”：

    template<typename T>       // 正确但很啰嗦：“参数的类型
        requires sortable<T>   // 为 T，这是某个 sortable
    void sort(T&);             // 类型的名字”

    template<sortable T>       // 有改善：“参数的类型
    void sort(T&);             // 为 Sortable 的类型 T”

    void sort(sortable auto&); // 最佳方式：“参数为 sortable”

越简练的版本越符合我们的说话方式。注意许多模板不在需要使用 `template` 关键字了。

##### 强制实施

- 当人们从 `<typename T>` and `<class T>` 写法进行转换时，使用简短形式是不可行的。
- 之后，如果声明中首先引入了一个 `typename`，之后又用简单的单类型参数概念对其进行约束的话，就对其进行标记。

## <a name="SS-concepts-def"></a>T.concepts.def: 概念定义规则

定义恰当的概念并不简单。
概念是用于表现应用领域中的基本概念的（正如其名“概念”）。
只是把用在某个具体的类或算法的参数上的一组语法约束聚在一起，并不是概念所设计的用法，
也无法获得这个机制的全部好处。

显然，定义概念对于那些可以使用某个实现（比如 C++20 或更新版本）的代码是最有用的，
不过定义概念本身就是一种有益的设计技巧，有助于发现概念上的错误并清理实现中的各种概念。

### <a name="Rt-low"></a>T.20: 避免没有有意义的语义的“概念”

##### 理由

概念是用于表现语义的观念的，比如“数”，元素的“范围”，以及“全序的”等等。
简单的约束，比如“带有 `+` 运算符”和“带有 `>` 运算符”，是无法独立进行有意义的运用的，
而仅应当被用作有意义的概念的构造块，而不是在用户代码中使用。

##### 示例，不好

    template<typename T>
    // 不好，不充分
    concept Addable = requires(T a, T b) { a+b; };

    template<Addable N>
    auto algo(const N& a, const N& b) // 使用两个数值
    {
        // ...
        return a + b;
    }

    int x = 7;
    int y = 9;
    auto z = algo(x, y);   // z = 16

    string xx = "7";
    string yy = "9";
    auto zz = algo(xx, yy);   // zz = "79"

也许拼接是有意进行的。不过更可能的是一种意外。而对减法进行同等的定义则会导致可接受类型的集合的非常不同。
这个 `Addable` 违反了加法应当可交换的数学法则：`a+b == b+a`。

##### 注解

给出有意义的语义的能力，在于定义真正的概念的特征，而不是仅给出语法约束。

##### 示例

    template<typename T>
    // 假定数值的运算符 +、-、* 和 / 都遵循常规的数学法则
    concept Number = requires(T a, T b) { a+b; a-b; a*b; a/b; };

    template<Number N>
    auto algo(const N& a, const N& b)
    {
        // ...
        return a + b;
    }

    int x = 7;
    int y = 9;
    auto z = algo(x, y);   // z = 16

    string xx = "7";
    string yy = "9";
    auto zz = algo(xx, yy);   // 错误：string 不是 Number

##### 注解

带有多个操作的概念要远比单个操作的概念更少和类型发生意外匹配的机会。

##### 强制实施

- 对在其他 `concept` 的定义之外使用的但操作 `concept` 进行标记。
- 对表现为模拟单操作 `concept` 的 `enable_if` 的使用进行标记。

### <a name="Rt-complete"></a>T.21: 为概念提出一组完整的操作要求

##### 理由

易于理解。
提升互操作性。
帮助实现者和维护者。

##### 注解

这是对一般性规则[必须让概念有语义上的意义](#Rt-low)的一个专门的变体。

##### 示例，不好

    template<typename T> concept Subtractable = requires(T a, T b) { a-b; };

这个是没有语义作用的。
你至少还需要 `+` 来让 `-` 有意义和有用处。

完整集合的例子有

- `Arithmetic`: `+`, `-`, `*`, `/`, `+=`, `-=`, `*=`, `/=`
- `Comparable`: `<`, `>`, `<=`, `>=`, `==`, `!=`

##### 注解

无论我们是否使用了概念的直接语言支持，本条规则都适用。
这是一种一般性的设计规则，即便对于非模板也同样适用：

    class Minimal {
        // ...
    };

    bool operator==(const Minimal&, const Minimal&);
    bool operator<(const Minimal&, const Minimal&);

    Minimal operator+(const Minimal&, const Minimal&);
    // 没有其他运算符

    void f(const Minimal& x, const Minimal& y)
    {
        if (!(x == y)) { /* ... */ }    // OK
        if (x != y) { /* ... */ }       // 意外！错误

        while (!(x < y)) { /* ... */ }  // OK
        while (x >= y) { /* ... */ }    // 意外！错误

        x = x + y;          // OK
        x += y;             // 意外！错误
    }

这是最小化的设计，但会使用户遇到意外或受到限制。
可能它还会比较低效。

这条规则支持这样的观点：概念应当反映（数学上）协调的一组操作。

##### 示例

    class Convenient {
        // ...
    };

    bool operator==(const Convenient&, const Convenient&);
    bool operator<(const Convenient&, const Convenient&);
    // ... 其他比较运算符 ...

    Convenient operator+(const Convenient&, const Convenient&);
    // ... 其他算术运算符 ...

    void f(const Convenient& x, const Convenient& y)
    {
        if (!(x == y)) { /* ... */ }    // OK
        if (x != y) { /* ... */ }       // OK

        while (!(x < y)) { /* ... */ }  // OK
        while (x >= y) { /* ... */ }    // OK

        x = x + y;     // OK
        x += y;        // OK
    }

定义所有的运算符也许很麻烦，但并不困难。
理想情况下，语言应当默认提供比较运算符以支持这条规则。

##### 强制实施

- 如果类所支持的运算符是运算符集合的“奇异”子集，比如有 `==` 但没有 `!=` 或者有 `+` 但没有 `-`，就对其进行标记。
  确实，`std::string` 也是“奇异”的，但要修改它太晚了。

### <a name="Rt-axiom"></a>T.22: 为概念指明公理

##### 理由

有意义或有用的概念都有语义上的含义。
以非正式、半正式或正式的方式表达这些语义可以使概念对于读者更加可理解，而且对其进行表达的工作也能发现一些概念上的错误。
对语义的说明是一种强大的设计工具。

##### 示例

    template<typename T>
        // 假定数值的运算符 +、-、* 和 / 都遵循常规的数学法则
        // axiom(T a, T b) { a + b == b + a; a - a == 0; a * (b + c) == a * b + a * c; /*...*/ }
        concept Number = requires(T a, T b) {
            {a + b} -> convertible_to<T>;
            {a - b} -> convertible_to<T>;
            {a * b} -> convertible_to<T>;
            {a / b} -> convertible_to<T>;
        };

##### 注解

这是一种数学意义上的公理：可以假定成立而无需证明。
通常公理是无法证明的，而即便可以证明，通常也超出了编译器的能力。
一条公理也许并不是通用的，不过模板作者可以假定其对于其所实际使用的所有输入均成立（类似于一种前条件）。

##### 注解

这种上下文中的公理都是布尔表达式。
参见 [Palo Alto TR](#S-references) 中的例子。
当前，C++ 并不支持公理（即便 ISO Concepts TS 也不支持），我们不得不长期用代码注释来给出它们。
语言支持一旦出现，公理前面的 `//` 就可以删掉了。

##### 注解

GSL 中的概念都具有恰当定义的语义；请参见 Palo Alto TR 和 Ranges TS。

##### 例外

一个处于开发之中的新“概念”的早期版本，经常会仅仅定义了一组简单的约束而并没有恰当指定的语义。
为其寻找正确的语义可能是很费功夫和时间的。
不过不完整的约束集合仍然是很有用的：

    // 对于一般二叉树的平衡器
    template<typename Node> concept Balancer = requires(Node* p) {
        add_fixup(p);
        touch(p);
        detach(p);
    };

这样 `Balancer` 就必须为树的 `Node` 至少提供这些操作，
但我们还是无法指定详细的语义，因为一种新种类的平衡树可能需要更多的操作，
而在设计的早期阶段，很难确定把对于所有节点的精确的一般语义所确定下来。

不完整的或者没有恰当指定的语义的“概念”仍然是很有用的。
比如说，它可能允许在初始的试验之中进行某些检查。
不过，请不要把它当作是稳定的。
它的每个新的用例都可能导致不完整概念的改善。

##### 强制实施

- 在概念定义的代码注释中寻找单词“axiom”。

### <a name="Rt-refine"></a>T.23: 通过添加新的使用模式，从更一般情形的概念中区分出提炼后的概念

##### 理由

否则编译器是无法自动对它们进行区分的。

##### 示例

    template<typename I>
    // 注：<iterator> 中定义了 input_iterator
    concept Input_iter = requires(I iter) { ++iter; };

    template<typename I>
    // 注：<iterator> 中定义了 forward_iterator
    concept Fwd_iter = Input_iter<I> && requires(I iter) { iter++; };

编译器可以基于所要求的操作的集合（这里为前缀 `++`）来确定提炼关系。
这样做减少了这些类型的实现者的负担，
因为他们不再需要任何特殊的声明来“打入概念内部”了。
如果两个概念具有完全相同的要求的话，它们在逻辑上就是等价的（不存在提炼）。

##### 强制实施

- 对与已经出现的另一个概念具有完全相同的要求的概念进行标记（它们中不存在更精炼的概念）。
  为对它们进行区分，参见 [T.24](#Rt-tag)。

### <a name="Rt-tag"></a>T.24: 用标签类或特征类来区分仅在语义上存在差别的概念

##### 理由

要求相同的语法但具有不同语义的两个概念之间会造成歧义，除非程序员对它们进行区分。

##### 示例

    template<typename I>    // 提供随机访问的迭代器
    // 注：<iterator> 中定义了 random_access_iterator
    concept RA_iter = ...;

    template<typename I>    // 提供对连续数据的随机访问的迭代器
    // 注：<iterator> 中定义了 contiguous_iterator
    concept Contiguous_iter =
        RA_iter<I> && is_contiguous_v<I>;  // 使用 is_contiguous 特征

程序员（在程序库中）必须适当地定义（特征） `is_contiguous`。

把标签类包装到概念中可以得到这个方案的更简单的表达方式：

    template<typename I> concept Contiguous = is_contiguous_v<I>;

    template<typename I>
    concept Contiguous_iter = RA_iter<I> && Contiguous<I>;

程序员（在程序库中）必须适当地定义（特征） `is_contiguous`。

##### 注解

特征可以是特征类或者类型特征。
它们可以是用户定义的，或者标准库中的。
优先采用标准库中的特征。

##### 强制实施

- 编译器会将对相同的概念的有歧义的使用标记出来。
- 对相同的概念定义进行标记。

### <a name="Rt-not"></a>T.25: 避免互补性的约束

##### 理由

清晰性。可维护性。
用否定来表达的具有互补要求的函数是很脆弱的。

##### 示例

最初，人们总会试图定义带有互补要求的函数：

    template<typename T>
        requires !C<T>    // 不好
    void f();

    template<typename T>
        requires C<T>
    void f();

这样会好得多：

    template<typename T>   // 通用模板
        void f();

    template<typename T>   // 用概念进行特化
        requires C<T>
    void f();

仅当 `C<T>` 无法满足时，编译器将会选择无约束的模板。
如果你并不想（或者无法）定义无约束版本的
`f()` 的话，你可以删掉它。

    template<typename T>
    void f() = delete;

编译器将会选取这个重载，或者给出一个适当的错误。

##### 注解

很不幸，互补约束在 `enable_if` 代码中很常见：

    template<typename T>
    enable_if<!C<T>, void>   // 不好
    f();

    template<typename T>
    enable_if<C<T>, void>
    f();

##### 注解

有时候会（不正确地）把对单个要求的互补约束当做是可以接受的。
不过，对于两个或更多的要求来说，所需要的定义的数量是按指数增长的（2，4，8，16，……）：

    C1<T> && C2<T>
    !C1<T> && C2<T>
    C1<T> && !C2<T>
    !C1<T> && !C2<T>

这样，犯错的机会也会倍增。

##### 强制实施

- 对带有 `C<T>` 和 `!C<T>` 约束的函数对进行标记。

### <a name="Rt-use"></a>T.26: 优先采用使用模式而不是简单的语法来定义概念

##### 理由

其定义更可读，而且更直接地对应于用户需要编写的代码。
其中同时兼顾了类型转换。你再不需要记住所有的类型特征的名字。

##### 示例

你可能打算这样来定义概念 `Equality`：

    template<typename T> concept Equality = has_equal<T> && has_not_equal<T>;

显然，直接使用标准的 `equality_comparable` 要更好而且更容易，
但是——只是一个例子——如果你不得不定义这样的概念的话，应当这样：

    template<typename T> concept Equality = requires(T a, T b) {
        { a == b } -> std::convertible_to<bool>;
        { a != b } -> std::convertible_to<bool>;
        // axiom { !(a == b) == (a != b) }
        // axiom { a = b; => a == b }  // => 的意思是“意味着”
    };

而不是定义两个无意义的概念 `has_equal` 和 `has_not_equal` 仅用于帮助 `Equality` 的定义。
“无意义”的意思是我们无法独立地指定 `has_equal` 的语义。

##### 强制实施

???

## <a name="SS-temp-interface"></a>模板接口

这些年以来，使用模板的编程一直忍受着模板的接口及其实现之间的
微弱区分性。
在引入概念之前，这种区分是没有直接的语言支持的。
不过，模板的接口是一个关键概念——是用户和实现者之间的一种契约——而且应当进行周密的设计。

### <a name="Rt-fo"></a>T.40: 使用函数对象向算法传递操作

##### 理由

函数对象比“普通”的函数指针能够向接口传递更多的信息。
一般来说，传递函数对象比传递函数指针能带来更好的性能。

##### 示例

    bool greater(double x, double y) { return x > y; }
    sort(v, greater);                                    // 函数指针：可能较慢
    sort(v, [](double x, double y) { return x > y; });   // 函数对象
    sort(v, std::greater{});                             // 函数对象

    bool greater_than_7(double x) { return x > 7; }
    auto x = find_if(v, greater_than_7);                 // 函数指针：不灵活
    auto y = find_if(v, [](double x) { return x > 7; }); // 函数对象：携带所需数据
    auto z = find_if(v, Greater_than<double>(7));        // 函数对象：携带所需数据

当然，也可以使用 `auto` 或概念来使这些函数通用化。例如：

    auto y1 = find_if(v, [](totally_ordered auto x) { return x > 7; }); // 要求一种有序类型
    auto z1 = find_if(v, [](auto x) { return x > 7; });                 // 期望类型带有 >

##### 注解

Lambda 会生成函数对象。

##### 注解

性能表现依赖于编译器和优化器技术。

##### 强制实施

- 标记以函数指针作为模板参数。
- 标记将函数指针作为模板的参数进行传递（存在误报风险）。

### <a name="Rt-essential"></a>T.41: 在模板的概念上仅提出基本的性质要求

##### 理由

保持接口的简单和稳定。

##### 示例

考虑一个带有（过度简化的）简单调试支持的 `sort`：

    void sort(sortable auto& s)  // 对序列 s 进行排序
    {
        if (debug) cerr << "enter sort( " << s <<  ")\n";
        // ...
        if (debug) cerr << "exit sort( " << s <<  ")\n";
    }

是否该把它重写为：

    template<sortable S>
        requires Streamable<S>
    void sort(S& s)  // 对序列 s 进行排序
    {
        if (debug) cerr << "enter sort( " << s <<  ")\n";
        // ...
        if (debug) cerr << "exit sort( " << s <<  ")\n";
    }

毕竟，`sortable` 里面并没有要求任何 `iostream` 支持。
而另一方面，在排序的基本概念中也没有任何东西是有关于调试的。

##### 注解

如果我们要求把用到的所有操作都在要求部分中列出的话，接口就会变得不稳定：
每当我们改动了调试设施，使用情况数据收集，测试支持，错误报告等等，
模板的定义就都得进行改动，而模板的所有使用方都不得不进行重新编译。
这样做很是累赘，而且在某些环境下是不可行的。

与之相反，如果我们在实现中使用了某个并未被概念检查提供保证的操作的话，
我们可能会遇到一个延后的编译时错误。

通过不对模板参数的那些不被认为是基本的性质进行概念检查，
我们把其检查推迟到了进行实例化的时候。
我们认为这是值得做出的折衷。

注意，使用非局部的，非待决的名字（比如 `debug` 和 `cerr`），同样会引入可能导致“神秘的”错误的某些上下文依赖。

##### 注解

可能很难确定一个类型的哪些性质是基本的，而哪些不是。

##### 强制实施

???

### <a name="Rt-alias"></a>T.42: 使用模板别名来简化写法并隐藏实现细节

##### 理由

提升可读性。
隐藏实现。
注意，模板别名取代了许多用于计算类型的特征。
它们也可以用于封装一个特征。

##### 示例

    template<typename T, size_t N>
    class Matrix {
        // ...
        using Iterator = typename std::vector<T>::iterator;
        // ...
    };

这免除了 `Matrix` 的用户必须了解其元素是存储于 `vector` 之中，而且也免除了用户重复书写 `typename std::vector<T>::`。

##### 示例

    template<typename T>
    void user(T& c)
    {
        // ...
        typename container_traits<T>::value_type x; // 不好，啰嗦
        // ...
    }

    template<typename T>
    using Value_type = typename container_traits<T>::value_type;

这免除了 `Value_type` 的用户必须了解用于实现 `value_type` 的技术。

    template<typename T>
    void user2(T& c)
    {
        // ...
        Value_type<T> x;
        // ...
    }

##### 注解

一种简洁的常用说法是：“包装特征！”

##### 强制实施

- 将 `using` 声明之外用于消除歧义的 `typename` 进行标记。
- ???

### <a name="Rt-using"></a>T.43: 优先使用 `using` 而不是 `typedef` 来定义别名

##### 理由

提升可读性：使用 `using` 时，新名字在前面，而不是被嵌在声明中的什么地方。
通用性：`using` 可用于模板别名，而 `typedef` 无法轻易作为模板。
一致性：`using` 在语法上和 `auto` 相似。

##### 示例

    typedef int (*PFI)(int);   // OK, 但很别扭

    using PFI2 = int (*)(int);   // OK, 更好

    template<typename T>
    typedef int (*PFT)(T);      // 错误

    template<typename T>
    using PFT2 = int (*)(T);   // OK

##### 强制实施

- 标记 `typedef` 的使用。不过这样会出现大量的“命中” :-(

### <a name="Rt-deduce"></a>T.44: （如果可行）使用函数模板来对类模板的参数类型进行推断

##### 理由

显式写出模板参数类型既麻烦又无必要。

##### 示例

    tuple<int, string, double> t1 = {1, "Hamlet", 3.14};   // 明确类型
    auto t2 = make_tuple(1, "Ophelia"s, 3.14);         // 更好；推断类型

注意这里用 `s` 后缀来确保字符串是 `std::string` 而不是 C 风格字符串。

##### 注解

既然你可以轻易写出 `make_T` 函数，编译器也可以。以后 `make_T` 函数也许将会变得多余。

##### 例外

有时候没办法对模板参数进行推断，而有时候你则想要明确指定参数：

    vector<double> v = { 1, 2, 3, 7.9, 15.99 };
    list<Record*> lst;

##### 注解

注意，C++17 强允许模板参数直接从构造函数参数进行推断，而使这条规则变得多余：
[构造函数的模板形参推断(Rev. 3)](http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2016/p0091r1.html)。
例如：

    tuple t1 = {1, "Hamlet"s, 3.14}; // 推断为：tuple<int, string, double>

##### 强制实施

当显式指定的类型与所使用的类型精确匹配时进行标记。

### <a name="Rt-regular"></a>T.46: 要求模板参数至少是半正规的

##### 理由

可读性。
避免意外和错误。
大多数用法都支持这样做。

##### 示例

    class X {
    public:
        explicit X(int);
        X(const X&);            // 复制
        X operator=(const X&);
        X(X&&) noexcept;        // 移动
        X& operator=(X&&) noexcept;
        ~X();
        // ... 没有别的构造函数了 ...
    };

    X x {1};              // 没问题
    X y = x;              // 没问题
    std::vector<X> v(10); // 错误: 没有默认构造函数

##### 注解

`SemiRegular` 要求可以默认构造。

##### 强制实施

- 对并非至少为 `SemiRegular` 的类型进行标记。

### <a name="Rt-visible"></a>T.47: 避免用常用名字命名高度可见的无约束模板

##### 示例

无约束的模板参数和任何东西都能完全匹配，因此这样的模板相对于需要少量转换的更特定类型来说可能更优先。
而当使用 ADL 时，这一点将会更加麻烦和危险。
而常用的名字则会让这种问题更易于出现。

##### 示例

    namespace Bad {
        struct S { int m; };
        template<typename T1, typename T2>
        bool operator==(T1, T2) { cout << "Bad\n"; return true; }
    }

    namespace T0 {
        bool operator==(int, Bad::S) { cout << "T0\n"; return true; }  // 与 int 比较

        void test()
        {
            Bad::S bad{ 1 };
            vector<int> v(10);
            bool b = 1 == bad;
            bool b2 = v.size() == bad;
        }
    }

这将会打印出 `T0` 和 `Bad`。

这里 `Bad` 中的 `==` 有意设计为造成问题，不过你是否在真实代码中发现过这个问题呢？
问题在于 `v.size()` 返回的是 `unsigned` 整数，因此需要进行转换才能调用局部的 `==`；
而 `Bad` 中的 `=` 则不需要任何转换。
实际的类型，比如标准库的迭代器，也可以被弄成类似的反社会倾向。

##### 注解

如果在相同的命名空间中定义了一个无约束模板类型，
这个无约束模板是可以通过 ADL 所找到的（如例子中所发生的一样）。
就是说，它是高度可见的。

##### 注解

这条规则应当是没有必要的，不过委员会在是否将无约束模板从 ADL 中排除上无法达成一致。

不幸的是，这会导致大量的误报；标准库也大量地违反了这条规则，它将许多无约束模板和类型都放入了单一的命名空间 `std` 之中。

##### 强制实施

如果定义模板的命名空间中同样定义了具体的类型，就对其进行标记（可能在我们有概念支持之前都是不可行的）。

### <a name="Rt-concept-def"></a>T.48: 如果你的编译器不支持概念的话，可以用 `enable_if` 来模拟

##### 理由

因为这是我们没有直接的概念支持时能够做到的最好的方式。
`enable_if` 可被用于有条件地定义函数，以及用于在一组函数中进行选择。

##### 示例

    template<typename T>
    enable_if_t<is_integral_v<T>>
    f(T v)
    {
        // ...
    }

    // 等价于：
    template<Integral T>
    void f(T v)
    {
        // ...
    }

##### 注解

请当心[互补约束](#Rt-not)。
当用 `enable_if` 来模拟概念重载时，有时候会迫使我们使用这种易错的设计技巧。

##### 强制实施

???

### <a name="Rt-erasure"></a>T.49: 尽可能避免类型擦除

##### 理由

类型擦除通过在一个独立的编译边界之后隐藏类型信息而招致一层额外的间接。

##### 示例

    ???

**例外**: 有时候类型擦除是合适的，比如 `std::function`。

##### 强制实施

???

##### 注解

## <a name="SS-temp-def"></a>T.def: 模板定义

模板的定义式（无论是类还是函数）都可能包含任意的代码，因而只有相当范围的 C++ 编程技术才能覆盖这个主题。
不过，这个部分所关注的是特定于模板的实现的问题。
特别地说，这里关注的是模板定义式对其上下文之间的依赖。

### <a name="Rt-depend"></a>T.60: 最小化模板的上下文依赖性

##### 理由

便于理解。
最小化源于意外依赖的错误。
便于创建工具。

##### 示例

    template<typename C>
    void sort(C& c)
    {
        std::sort(begin(c), end(c)); // 必要并且有意义的依赖
    }

    template<typename Iter>
    Iter algo(Iter first, Iter last)
    {
        for (; first != last; ++first) {
            auto x = sqrt(*first); // 潜在的意外依赖：哪个 sqrt()？
            helper(first, x);      // 潜在的意外依赖：
                                   // helper 是基于 first 和 x 所选择的
            TT var = 7;            // 潜在的意外依赖：哪个 TT？
        }
    }

##### 注解

通常模板是放在头文件中的，因此相对于 `.cpp` 文件之中的函数来说，其上下文依赖更加会受到 `#include` 的顺序依赖的威胁。

##### 注解

让模板仅对其参数进行操作是一种把依赖减到最少的方式，但通常这是很难做到的。
比如说，一个算法通常会使用其他的算法，并且调用一些并非仅在参数上做动作的一些操作。
而且别再让我们从宏开始干活！

**参见**：[T.69](#Rt-customization)。

##### 强制实施

??? 很麻烦

### <a name="Rt-scary"></a>T.61: 请勿对成员进行过度参数化（SCARY）

##### 理由

不依赖于模板参数的成员，除非给定某个特定的模板参数，否则也是无法使用的。
这样会限制其使用，而且通常会增加代码大小。

##### 示例，不好

    template<typename T, typename A = std::allocator<T>>
        // requires Regular<T> && Allocator<A>
    class List {
    public:
        struct Link {   // 并未依赖于 A
            T elem;
            Link* pre;
            Link* suc;
        };

        using iterator = Link*;

        iterator first() const { return head; }

        // ...
    private:
        Link* head;
    };

    List<int> lst1;
    List<int, My_allocator> lst2;

这看起来没什么问题，但现在 `Link` 形成依赖于分配器（尽管它不使用分配器）。 这迫使冗余的实例化在某些现实场景中可能造成出奇的高的成本。
通常，解决方案是使用自己的最小模板参数集使嵌套类非局部化。

    template<typename T>
    struct Link {
        T elem;
        Link* pre;
        Link* suc;
    };

    template<typename T, typename A = std::allocator<T>>
        // requires Regular<T> && Allocator<A>
    class List2 {
    public:
        using iterator = Link<T>*;

        iterator first() const { return head; }

        // ...
    private:
        Link<T>* head;
    };

    List2<int> lst1;
    List2<int, My_allocator> lst2;

人们发现 `Link` 不再隐藏在列表中很可怕，所以我们命名这个技术为 [SCARY](http://www.open-std.org/jtc1/sc22/WG21/docs/papers/2009/n2911.pdf)。
引自该学术论文：“首字母缩略词 SCARY 描述了看似错误的赋值和初始化（受冲突的通用参数的约束），
但实际上使用了正确的实现（由于最小化的依赖而不受冲突的约束）。”

##### 注解

这同样适用于那些并不依赖于其全部模板形参的 lambda 表达式。

##### 强制实施

- 对并未依赖于全部模板形参的成员类型进行标记。
- 对并未依赖于全部模板形参的成员函数进行标记。
- 对并未依赖于全部模板形参的 lambda 表达式或变量模板进行标记。

### <a name="Rt-nondependent"></a>T.62: 将无依赖的类模板成员置于一个非模板基类之中

##### 理由

可以在使用基类成员时不需要指定模板参数，也不需要模板实例化。

##### 示例

    template<typename T>
    class Foo {
    public:
        enum { v1, v2 };
        // ...
    };

???

    struct Foo_base {
        enum { v1, v2 };
        // ...
    };

    template<typename T>
    class Foo : public Foo_base {
    public:
        // ...
    };

##### 注解

这条规则的更一般化的版本是，
“如果类模板的成员依赖于 M 个模板参数中的 N 个，就将它置于只有 N 个参数的基类之中。”
当 N == 1 时，可以如同 [T.61](#Rt-scary) 一样在一个基类和其外围作用域中的一个类之间进行选择。

??? 常量的情况如何？类的静态成员呢？

##### 强制实施

- 标记 ???

### <a name="Rt-specialization"></a>T.64: 用特化来提供类模板的其他实现

##### 理由

模板定义了通用接口。
特化是一种为这个接口提供替代实现的强大机制。

##### 示例

    ??? 字符串的特化 (==)

    ??? 表示特化？

##### 注解

???

##### 强制实施

???

### <a name="Rt-tag-dispatch"></a>T.65: 用标签分派来提供函数的其他实现

##### 理由

- 模板定义了通用接口。
- 标签派发允许我们基于参数类型的特定性质选择不同的实现。
- 性能。

##### 示例

这是 `std::copy` 的一个简化版本（忽略了非连续序列的可能性）

    struct pod_tag {};
    struct non_pod_tag {};

    template<class T> struct copy_trait { using tag = non_pod_tag; };   // T 不是“朴素数据”

    template<> struct copy_trait<int> { using tag = pod_tag; };         // int 是“朴素数据”

    template<class Iter>
    Out copy_helper(Iter first, Iter last, Iter out, pod_tag)
    {
        // 使用 memmove
    }

    template<class Iter>
    Out copy_helper(Iter first, Iter last, Iter out, non_pod_tag)
    {
        // 使用调用复制构造函数的循环
    }

    template<class Iter>
    Out copy(Iter first, Iter last, Iter out)
    {
        return copy_helper(first, last, out, typename copy_trait<Value_type<Iter>>::tag{})
    }

    void use(vector<int>& vi, vector<int>& vi2, vector<string>& vs, vector<string>& vs2)
    {
        copy(vi.begin(), vi.end(), vi2.begin()); // 使用 memmove
        copy(vs.begin(), vs.end(), vs2.begin()); // 使用调用复制构造函数的循环
    }

这是一种进行编译时算法选择的通用且有力的技巧。

##### 注解

当可以广泛使用 `concept` 之后，这样的替代实现就可以直接进行区分了：

    template<class Iter>
        requires Pod<Value_type<Iter>>
    Out copy_helper(In, first, In last, Out out)
    {
        // 使用 memmove
    }

    template<class Iter>
    Out copy_helper(In, first, In last, Out out)
    {
        // 使用调用复制构造函数的循环
    }

##### 强制实施

???

### <a name="Rt-specialization2"></a>T.67: 用特化来提供不规则类型的其他实现

##### 理由

???

##### 示例

    ???

##### 强制实施

???

### <a name="Rt-cast"></a>T.68: 在模板中用 `{}` 而不是 `()` 以避免歧义

##### 理由

`()` 会带来文法歧义。

##### 示例

    template<typename T, typename U>
    void f(T t, U u)
    {
        T v1(T(u));    // 错误：啊，v1 是函数而不是变量
        T v2{u};       // 清晰：显然是变量
        auto x = T(u); // 不清晰：构造还是强制转换？
    }

    f(1, "asdf"); // 不好：从 const char* 强制转换为 int

##### 强制实施

- 标记 `()` 初始化式。
- 标记函数风格的强制转换。

### <a name="Rt-customization"></a>T.69: 在模板中，请勿进行未限定的非成员函数调用，除非有意将之作为定制点

##### 理由

- 仅提供预计之内的灵活性。
- 避免源于意外的环境改变的威胁。

##### 示例

主要有三种方法使调用代码对模板进行定制化。

    template<class T>
        // 调用成员函数
    void test1(T t)
    {
        t.f();    // 要求 T 提供 f()
    }

    template<class T>
    void test2(T t)
        // 不带限定地调用非成员函数
    {
        f(t);     // 要求 f(/*T*/) 在调用方的作用域或者 T 的命名空间中可用
    }

    template<class T>
    void test3(T t)
        // 调用一个“特征”
    {
        test_traits<T>::f(t); // 要求定制化 test_traits<>
                              // 以获得非默认的函数和类型
    }

特征通常是用以计算一个类型的类型别名，
用以计算一个值的 `constexpr` 函数，
或者针对用户的类型进行特化的传统的特征模板。

##### 注解

当你打算为依赖于某个模板类型参数的值 `t` 调用自己的辅助函数 `helper(t)` 时，
请将函数放入一个 `::detail` 命名空间中，并把调用限定为 `detail::helper(t);`。
无限定的调用将成为一个定制点，它将会调用处于 `t` 的类型所在命名空间中的任何 `helper` 函数；
这可能会导致诸如[意外地调用了无约束函数模板](#Rt-visible)这样的问题。

##### 强制实施

- 在模板中，如果非成员函数的无限定调用传递了具有依赖类型的变量，而在该模板的命名空间中存在相同名字的非成员函数，则对其进行标记。

## <a name="SS-temp-hier"></a>T.temp-hier: 模板和类型层次规则：

模板是 C++ 为泛型编程提供支持的基石，而类层次则是 C++ 为面向对象编程提供
支持的基石。
这两种语言机制可以有效地组合起来，但必须避免一些设计上的陷阱。

### <a name="Rt-hier"></a>T.80: 请勿不成熟地对类层次进行模板化

##### 理由

使一个带有许多函数，尤其是有许多虚函数的类层次进行模板化，会导致代码膨胀。

##### 示例，不好

    template<typename T>
    struct Container {         // 这是一个接口
        virtual T* get(int i);
        virtual T* first();
        virtual T* next();
        virtual void sort();
    };

    template<typename T>
    class Vector : public Container<T> {
    public:
        // ...
    };

    Vector<int> vi;
    Vector<string> vs;

把 `sort` 定义为容器的成员函数可能是一个比较糟糕的主意，不过这样做并不鲜见，而且它是一个展示不应当做的事情的好例子。

在这之中，编译器不知道 `Vector<int>::sort()` 是不是会被调用，因此它必须为之生成代码。
`Vector<string>::sort()` 也与此相似。
除非这两个函数被调用，否则这就是代码爆炸。
不难想象当一个带有几十个成员函数和几十个派生类的类层次被大量实例化时会怎么样。

##### 注解

许多情况下都可以通过不为基类进行参数化而提供一个稳定的接口；
参见[“稳定的基类”](#Rt-abi)和 [OO 与 GP](#Rt-generic-oo)。

##### 强制实施

- 对依赖于模板参数的虚函数进行标记。 ??? 误报

### <a name="Rt-array"></a>T.81: 请勿混合类层次和数组

##### 理由

派生类的数组可以隐式地“退化”成指向基类的指针，并带来潜在的灾难性后果。

##### 示例

假定 `Apple` 和 `Pear` 是两种 `Fruit`。

    void maul(Fruit* p)
    {
        *p = Pear{};     // 把一个 Pear 放入 *p
        p[1] = Pear{};   // 把一个 Pear 放入 p[1]
    }

    Apple aa [] = { an_apple, another_apple };   // aa 包含的是 Apple （显然！）

    maul(aa);
    Apple& a0 = &aa[0];   // 是 Pear 吗？
    Apple& a1 = &aa[1];   // 是 Pear 吗？

`aa[0]` 可能会变为 `Pear`（并且没进行过强制转换！）。
当 `sizeof(Apple) != sizeof(Pear)` 时，对 `aa[1]` 的访问就是并未跟数组中的对象的适当起始位置进行对齐的。
这里出现了类型违例，以及很可能出现的内存损坏。
决不要写这样的代码。

注意，`maul()` 违反了 [`T*` 应指向独立对象的规则](#Rf-ptr)。

**替代方案**: 使用适当的（模板化）容器：

    void maul2(Fruit* p)
    {
        *p = Pear{};   // 把一个 Pear 放入 *p
    }

    vector<Apple> va = { an_apple, another_apple };   // va 包含的是 Apple （显然！）

    maul2(va);       // 错误: 无法把 vector<Apple> 转换为 Fruit*
    maul2(&va[0]);   // 这是你明确要做的

    Apple& a0 = &va[0];   // 是 Pear 吗？

注意，`maul2()` 中的赋值违反了[避免发生切片的规则](#Res-slice)。

##### 强制实施

- 对这种恐怖的东西进行检测！

### <a name="Rt-linear"></a>T.82: 当不想要虚函数时，可以将类层次线性化

##### 理由

???

##### 示例

    ???

##### 强制实施

???

### <a name="Rt-virtual"></a>T.83: 请勿声明虚的成员函数模板

##### 理由

C++ 是不支持这样做的。
如果支持的话，就只能等到连接时才能生成 VTBL 了。
而且一般来说，各个实现还要搞定动态连接的问题。

##### 示例，请勿如此

    class Shape {
        // ...
        template<class T>
        virtual bool intersect(T* p);   // 错误：模板不能为虚
    };

##### 注解

我们保留这条规则是因为人们总是问这个问题。

##### 替代方案

双派发或访问器模式，或者计算出所要调用的函数。

##### 强制实施

编译器会处理这个问题。

### <a name="Rt-abi"></a>T.84: 使用非模板的核心实现来提供 ABI 稳定的接口

##### 理由

提升代码的稳定性。
避免代码爆炸。

##### 示例

这个应当是基类：

    struct Link_base {   // 稳定
        Link_base* suc;
        Link_base* pre;
    };

    template<typename T>   // 模板化的包装带来了类型安全性
    struct Link : Link_base {
        T val;
    };

    struct List_base {
        Link_base* first;   // 第一个元素（如果有）
        int sz;             // 元素数量
        void add_front(Link_base* p);
        // ...
    };

    template<typename T>
    class List : List_base {
    public:
        void put_front(const T& e) { add_front(new Link<T>{e}); }   // 隐式强制转换为 Link_base
        T& front() { static_cast<Link<T>*>(first).val; }   // 显式强制转换回 Link<T>
        // ...
    };

    List<int> li;
    List<string> ls;

这样的话就只有一份用于对 `List` 的元素进行入链和解链的操作的代码了。
而类 `Link` 和 `List` 除了进行类型操作之外什么也没做。

除了使用一个独立的“base”类型外，另一种常用的技巧是对 `void` 或 `void*` 进行特化，并让针对 `T` 的通用模板成为在从或向 `void` 的核心实现进行强制转换的一层类型安全封装。

**替代方案**: 使用一个 [PImpl](#Ri-pimpl) 实现。

##### 强制实施

???

## <a name="SS-variadic"></a>T.var: 变参模板规则

???

### <a name="Rt-variadic"></a>T.100: 当需要可以接受可变数量的多种类型参数的函数时，使用变参模板

##### 理由

变参模板是做到这点的最通用的机制，而且既高效又类型安全。请不要使用 C 的变参。

##### 示例

    ??? printf

##### 强制实施

- 对用户代码中 `va_arg` 的使用进行标记。

### <a name="Rt-variadic-pass"></a>T.101: ??? 如何向变参模板传递参数 ???

##### 理由

???

##### 示例

    ??? 当心仅能移动参数和引用参数

##### 强制实施

???

### <a name="Rt-variadic-process"></a>T.102: 如何处理变参模板的参数

##### 理由

???

##### 示例

    ??? 转发参数，类型检查，引用

##### 强制实施

???

### <a name="Rt-variadic-not"></a>T.103: 请勿对同质参数列表使用变参模板

##### 理由

存在更加正规的给出同质序列的方式，比如使用 `initializer_list`。

##### 示例

    ???

##### 强制实施

???

## <a name="SS-meta"></a>T.meta: 模板元编程（TMP）

模板提供了一种编译期编程的通用机制。

元编程，是其中至少一项输入或者一项输出是类型的编程。
模板提供了编译期的图灵完备（除了内存消耗外）的鸭子类型系统。
其语法和所需技巧都相当可怕。

### <a name="Rt-metameta"></a>T.120: 仅当确实需要时才使用模板元编程

##### 理由

模板元编程很难做对，它会拖慢编译速度，而且通常很难维护。
不过，现实世界有些例子中模板元编程提供了比其他专家级的汇编代码替代方案还要更好的性能。
而且，也存在现实世界的例子用模板元编程做到比运行时代码更好地表达基本设计意图的情况。
例如，当确实需要在编译期进行 AST 操作时，（比如说对矩阵操作进行可选的折叠），C++ 中可能没有其他的实现方式。

##### 示例，不好

    ???

##### 示例，不好

    enable_if

请使用概念来替代它。不过请参见[如何在没有语言支持时模拟概念](#Rt-emulate)。

##### 示例

    ??? 好例子

**替代方案**: 如果结果是一个值而不是类型，请使用 [`constexpr` 函数](#Rt-fct)。

##### 注解

当你觉得需要把模板元编程代码隐藏到宏之中时，你可能已经跑得太远了。

### <a name="Rt-emulate"></a>T.121: 模板元编程主要用于模拟概念机制

##### 理由

不能使用 C++20 时，我们需要用 TMP 来模拟它。
对概念给出要求的用例（比如基于概念进行重载）是 TMP 的最常见（而且最简单）的用法。

##### 示例

    template<typename Iter>
        /*requires*/ enable_if<random_access_iterator<Iter>, void>
    advance(Iter p, int n) { p += n; }

    template<typename Iter>
        /*requires*/ enable_if<forward_iterator<Iter>, void>
    advance(Iter p, int n) { assert(n >= 0); while (n--) ++p;}

##### 注解

这种代码使用概念时将更加简单：

    void advance(random_access_iterator auto p, int n) { p += n; }

    void advance(forward_iterator auto p, int n) { assert(n >= 0); while (n--) ++p;}

##### 强制实施

???

### <a name="Rt-tmp"></a>T.122: 用模板（通常为模板别名）来在编译期进行类型运算

##### 理由

模板元编程是在编译期进行类型生成的受到直接支持和部分正规化的唯一方式。

##### 注解

“特征（Trait）”技术基本上在计算类型方面被模板别名所代替，而在计算值方面则被 `constexpr` 函数所代替。

##### 示例

    ??? 大型对象 / 小型对象的优化

##### 强制实施

???

### <a name="Rt-fct"></a>T.123: 用 `constexpr` 函数来在编译期进行值运算

##### 理由

函数是用于表达计算一个值的最显然和传统的方式。
通常 `constexpr` 函数都比其他的替代方式具有更少的编译期开销。

##### 注解

“特征（Trait）”技术基本上在计算类型方面被模板别名所代替，而在计算值方面则被 `constexpr` 函数所代替。

##### 示例

    template<typename T>
        // requires Number<T>
    constexpr T pow(T v, int n)   // 幂/指数
    {
        T res = 1;
        while (n--) res *= v;
        return res;
    }

    constexpr auto f7 = pow(pi, 7);

##### 强制实施

- 对产生值的模板元程序进行标记。它们应当被替换成 `constexpr` 函数。

### <a name="Rt-std-tmp"></a>T.124: 优先使用标准库的模板元编程设施

##### 理由

标准中所定义的设施，诸如 `conditional`，`enable_if`，以及 `tuple` 等，是可移植的，可以假定为大家所了解。

##### 示例

    ???

##### 强制实施

???

### <a name="Rt-lib"></a>T.125: 当需要标准库之外的模板元编程设施时，使用某个现存程序库

##### 理由

要搞出高级的 TMP 设施是很难的，而使用一个库则可让你进入某个（有希望收到支持的）社区。
只有当确实不得不编写自己的“高级 TMP 支持”时，才应当这样做。

##### 示例

    ???

##### 强制实施

???

## <a name="SS-temp-other"></a>其他模板规则

### <a name="Rt-name"></a>T.140: 若操作可被重用，则应为其命名

参见 [F.10](#Rf-name)

### <a name="Rt-lambda"></a>T.141: 当仅在一个地方需要一个简单的函数对象时，使用无名的 lambda

参见 [F.11](#Rf-lambda)

### <a name="Rt-var"></a>T.142?: 使用模板变量以简化写法

##### 理由

改善可读性。

##### 示例

    ???

##### 强制实施

???

### <a name="Rt-nongeneric"></a>T.143: 请勿编写并非有意非泛型的代码

##### 理由

一般性。可重用性。请勿无必要地陷入技术细节之中；请使用最广泛可用的设施。

##### 示例

用 `!=` 而不是 `<` 来比较迭代器；`!=` 可以在更多对象上正常工作，因为它并未蕴含有序性。

    for (auto i = first; i < last; ++i) {   // 通用性较差
        // ...
    }

    for (auto i = first; i != last; ++i) {   // 好; 通用性较强
        // ...
    }

当然，范围式 `for` 在符合需求的时候当然是更好的选择。

##### 示例

使用能够提供所需功能的最接近基类的类。

    class Base {
    public:
        Bar f();
        Bar g();
    };

    class Derived1 : public Base {
    public:
        Bar h();
    };

    class Derived2 : public Dase {
    public:
        Bar j();
    };

    // 不好，除非确实有特别的原因来将之仅限制为 Derived1 对象
    void my_func(Derived1& param)
    {
        use(param.f());
        use(param.g());
    }

    // 好，仅使用 Base 的接口，且保证了这个类型
    void my_func(Base& param)
    {
        use(param.f());
        use(param.g());
    }

##### 强制实施

- 对使用 `<` 而不是 `!=` 的迭代器比较进行标记。
- 当存在 `x.empty()` 或 `x.is_empty()` 时，对 `x.size() == 0` 进行标记。`empty()` 比 `size()` 能够对于更多的容器工作，因为某些容器是不知道自己的大小的，甚至概念上就是大小无界的。
- 如果函数接受指向更加派生的类型的指针或引用，但仅使用了在某个基类中所声明的函数，则对其进行标记。

### <a name="Rt-specialize-function"></a>T.144: 请勿特化函数模板

##### 理由

根据语言规则，函数模板是无法被部分特化的。函数模板可以被完全特化，不过你基本上需要的都是进行重载而不是特化——因为函数模板特化并不参与重载，它们的行为和你想要的可能是不同的。少数情况下，应当通过你可以进行适当特化的类模板来进行真正的特化。

##### 示例

    ???

**例外**: 当确实有特化函数模板的恰当理由时，请只编写一个函数模板，并使它委派给一个类模板，然后对这个类模板进行特化（这提供了编写部分特化的能力）。

##### 强制实施

- 标记出所有的函数模板特化。代之以函数重载。

### <a name="Rt-check-class"></a>T.150: 用 `static_assert` 来检查类是否与概念相符

##### 理由

当你打算使一个类符合某个概念时，应该提早进行验证以减少麻烦。

##### 示例

    class X {
    public:
        X() = delete;
        X(const X&) = default;
        X(X&&) = default;
        X& operator=(const X&) = default;
        // ...
    };

在别的地方，也许是某个实现文件中，可以让编译器来检查 `X` 的所需各项性质：

    static_assert(Default_constructible<X>);    // 错误: X 没有默认构造函数
    static_assert(Copyable<X>);                 // 错误: 忘记定义 X 的移动构造函数了

##### 强制实施

不可行。

# <a name="S-cpl"></a>CPL: C 风格的编程

C 和 C++ 是联系很紧密的两门语言。
它们都是源于 1978 年的“经典 C”语言的，且从此之后就在 ISO 标准委员会中进行演化。
为了让它们保持兼容，我们做过许多努力，但它们各自都并非是对方的子集。

C 规则概览：

- [CPL.1: 优先使用 C++ 而不是 C](#Rcpl-C)
- [CPL.2: 当一定要用 C 时，应使用 C 和 C++ 的公共子集，并将 C 代码以 C++ 来编译](#Rcpl-subset)
- [CPL.3: 当一定要用 C 来作为接口时，应在使用这些接口的调用方代码中使用 C++](#Rcpl-interface)

### <a name="Rcpl-C"></a>CPL.1: 优先使用 C++ 而不是 C

##### 理由

C++ 提供更好的类型检查和更多的语法支持。
它能为高层的编程提供更好的支持，而且通常会产生更快速的代码。

##### 示例

    char ch = 7;
    void* pv = &ch;
    int* pi = pv;   // 非 C++
    *pi = 999;      // 覆盖了 &ch 附近的 sizeof(int) 个字节

针对在 C 中从 `void*` 或向它进行的隐式强制转换的相关规则比较麻烦而且并未强制实施。
特别是，这个例子违反了禁止把类型转换为具有更严格对齐的类型的规则。

##### 强制实施

使用 C++ 编译器。

### <a name="Rcpl-subset"></a>CPL.2: 当一定要用 C 时，应使用 C 和 C++ 的公共子集，并将 C 代码以 C++ 来编译

##### 理由

它们的子集语言，C 和 C++ 编译器都可以编译，而当作为 C++ 编译时，比“纯 C” 进行更好的类型检查。

##### 示例

    int* p1 = malloc(10 * sizeof(int));                      // 非 C++
    int* p2 = static_cast<int*>(malloc(10 * sizeof(int)));   // 非 C, C 风格的 C++
    int* p3 = new int[10];                                   // 非 C
    int* p4 = (int*) malloc(10 * sizeof(int));               // C 和 C++ 均可

##### 强制实施

- 当使用某种将代码作为 C 来编译的构建模式时进行标记。

  - C++ 将会确保代码是合法的 C++ 代码，除非使用了 C 扩展的编译器选项。

### <a name="Rcpl-interface"></a>CPL.3: 当一定要用 C 来作为接口时，应在使用这些接口的代码中使用 C++

##### 理由

C++ 比 C 的表达能力更强，而且为许多种类的编程都提供了更好的支持。

##### 示例

例如，为使用第三方 C 程序库或者 C 系统接口，可以使用 C 和 C++ 的公共子集来定义其底层接口，以获得更好的类型检查。
尽可能将底层接口封装到一个遵循了 C++ 指导方针的接口之中（以获得更好的抽象、内存安全性和资源安全性），并在 C++ 代码中使用这个 C++ 接口。

##### 示例

在 C++ 中可以调用 C：

    // C 中:
    double sqrt(double);

    // C++ 中:
    extern "C" double sqrt(double);

    sqrt(2);

##### 示例

在 C 中可以调用 C++：

    // C 中:
    X call_f(struct Y*, int);

    // C++ 中:
    extern "C" X call_f(Y* p, int i)
    {
        return p->f(i);   // 可能是虚函数调用
    }

##### 强制实施

不需要做什么。

# <a name="S-source"></a>SF: 源文件

区分声明（用作接口）和定义（用作实现）。
用头文件来表达接口并强调逻辑结构。

源文件规则概览：

- [SF.1: 如果你的项目还未采用别的约定的话，应当为代码文件使用后缀 `.cpp`，而对接口文件使用后缀 `.h`](#Rs-file-suffix)
- [SF.2: 头文件不能含有对象定义或非内联的函数定义](#Rs-inline)
- [SF.3: 对在多个源文件中使用的任何声明，都应使用头文件](#Rs-declaration-header)
- [SF.4: 在文件中的其他所有声明之前包含头文件](#Rs-include-order)
- [SF.5: `.cpp` 文件必须包含定义了它的接口的一个或多个头文件](#Rs-consistency)
- [SF.6: `using namespace` 指令，（仅）可以为迁移而使用，可以为基础程序库使用（比如 `std`），或者在局部作用域中使用](#Rs-using)
- [SF.7: 请勿在头文件中的全局作用域使用 `using namespace` 指令](#Rs-using-directive)
- [SF.8: 为所有的头文件使用 `#include` 防卫宏](#Rs-guards)
- [SF.9: 避免源文件的循环依赖](#Rs-cycles)
- [SF.10: 避免依赖于隐含地 `#include` 进来的名字](#Rs-implicit)
- [SF.11: 头文件应当是自包含的](#Rs-contained)
- [SF.12: 对相对于包含文件的文件优先采用引号形式的 `#include`，其他情况下采用角括号形式](#Rs-incform)

- [SF.20: 用 `namespace` 表示逻辑结构](#Rs-namespace)
- [SF.21: 请勿在头文件中使用无名（匿名）命名空间](#Rs-unnamed)
- [SF.22: 为所有的内部/不导出的实体使用无名（匿名）命名空间](#Rs-unnamed2)

### <a name="Rs-file-suffix"></a>SF.1: 如果你的项目还未采用别的约定的话，应当为代码文件使用后缀 `.cpp`，而对接口文件使用后缀 `.h`

参见 [NL.27](#Rl-file-suffix)

### <a name="Rs-inline"></a>SF.2: 头文件不能含有对象定义或非内联的函数定义

##### 理由

对受制于唯一定义规则的实体的包含将导致连接错误。

##### 示例

    // file.h:
    namespace Foo {
        int x = 7;
        int xx() { return x+x; }
    }

    // file1.cpp:
    #include <file.h>
    // ... 更多代码 ...

     // file2.cpp:
    #include <file.h>
    // ... 更多代码 ...

当连接 `file1.cpp` 和 `file2.cpp` 时将出现两个连接器错误。

**其他形式**: 头文件必须仅包含：

- `#include` 其他的头文件（可能包括包含防卫宏）
- 模板
- 类定义
- 函数声明
- `extern` 声明
- `inline` 函数定义
- `constexpr` 定义
- `const` 定义
- `using` 别名定义
- ???

##### 强制实施

根据以上白名单来检查。

### <a name="Rs-declaration-header"></a>SF.3: 对在多个源文件中使用的任何声明，都应使用头文件

##### 理由

可维护性。可读性。

##### 示例，不好

    // bar.cpp:
    void bar() { cout << "bar\n"; }

    // foo.cpp:
    extern void bar();
    void foo() { bar(); }

`bar` 的维护者在需要改变 `bar` 的类型时，无法找到其全部声明。
`bar` 的使用者不知道他所使用的接口是否完整和正确。顶多会从连接器获得一些（延迟的）错误消息。

##### 强制实施

- 对并未放入 `.h` 而在其他源文件中的实体声明进行标记。

### <a name="Rs-include-order"></a>SF.4: 在文件中的其他所有声明之前包含头文件

##### 理由

最小化上下文的依赖并增加可读性。

##### 示例

    #include <vector>
    #include <algorithm>
    #include <string>

    // ... 我自己的代码 ...

##### 示例，不好

    #include <vector>

    // ... 我自己的代码 ...

    #include <algorithm>
    #include <string>

##### 注解

这对于 `.h` 和 `.cpp` 文件都同样适用。

##### 注解

有一种论点是通过在打算保护的代码的*后面*再 `#include` 头文件，以此将代码同头文件中的声明式和宏等之间进行隔离
（如上面例子中标为“不好”之处）。
不过，

- 这只能对单个文件（在单个层次上）工作：如果采用这个技巧的头文件被别的头文件所包含，这个威胁就会再次出现。
- 命名空间（一个“实现命名空间”）可以针对许多的上下文依赖进行保护。
- 完全的保护和灵活性需要模块。

**参见**：

- [工作草案，C++ 的模块扩展](http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2016/n4592.pdf)
- [模块，组件化及其迁移](http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2016/p0141r0.pdf)

##### 强制实施

容易。

### <a name="Rs-consistency"></a>SF.5: `.cpp` 文件必须包含定义了它的接口的一个或多个头文件

##### 理由

这使得编译器可以提早进行一致性检查。

##### 示例，不好

    // foo.h:
    void foo(int);
    int bar(long);
    int foobar(int);

    // foo.cpp:
    void foo(int) { /* ... */ }
    int bar(double) { /* ... */ }
    double foobar(int);

这个错误直到调用了 `bar` 或 `foobar` 的程序的连接时才会被发现。

##### 示例

    // foo.h:
    void foo(int);
    int bar(long);
    int foobar(int);

    // foo.cpp:
    #include "foo.h"

    void foo(int) { /* ... */ }
    int bar(double) { /* ... */ }
    double foobar(int);   // 错误: 错误的返回类型

`foobar` 的返回类型错误在编译 `foo.cpp` 时立即就被发现了。
对 `bar` 的参数类型错误在连接时之前无法被发现，因为可能会有重载发生，但系统性地使用 `.h` 文件能够增加时其被程序员更早发现的可能性。

##### 强制实施

???

### <a name="Rs-using"></a>SF.6: `using namespace` 指令，（仅）可以为迁移而使用，可以为基础程序库使用（比如 `std`），或者在局部作用域中使用

##### 理由

`using namespace` 可能造成名字冲突，因而应当节制使用。
然而，将用户代码中的每个命名空间中的名字都进行限定并不总是能够做到（比如在转换过程中）
而且有时候命名空间非常基础，并且在代码库中广为使用，坚持进行限定将使其既啰嗦又分散注意力。

##### 示例

    #include <string>
    #include <vector>
    #include <iostream>
    #include <memory>
    #include <algorithm>

    using namespace std;

    // ...

显然地，大量使用了标准库，而且貌似没使用别的程序库，因此要求每一处带有使用 `std::`
会使人分散注意力。

##### 示例

使用 `using namespace std;` 导致程序员可能面临与标准库中的名字造成名字冲突

    #include <cmath>
    using namespace std;

    int g(int x)
    {
        int sqrt = 7;
        // ...
        return sqrt(x); // 错误
    }

不过，不大可能导致并非错误的名字解析，
假定使用 `using namespace std` 的人们都了解 `std` 以及这种风险。

##### 注解

`.cpp` 文件也是一种形式的局部作用域。
包含一条 `using namespace X` 的 N 行的 `.cpp` 文件中发生名字冲突的机会，
和包含一条 `using namespace X` 的 N 行的函数，
以及每个都包含一条 `using namespace X` 的总行数为 N 行的 M 个函数，没有多少差别。

##### 注解

[请勿在头文件全局作用域中使用 `using namespace`](#Rs-using-directive)。

##### 强制实施

对于单个源文件中，对不同命名空间的多个 `using namespace` 指令进行标记。

### <a name="Rs-using-directive"></a>SF.7: 请勿在头文件中的全局作用域使用 `using namespace`

##### 理由

这样做使 `#include` 一方无法有效地进行区分并使用其他方式。这还可能使所 `#include` 的头文件之间出现顺序依赖，它们以不同次序包含时可能具有不同的意义。

##### 示例

    // bad.h
    #include <iostream>
    using namespace std; // bad

    // user.cpp
    #include "bad.h"

    bool copy(/*... some parameters ...*/);    // some function that happens to be named copy

    int main()
    {
        copy(/*...*/);    // now overloads local ::copy and std::copy, could be ambiguous
    }

##### 注解

一个例外是 `using namespace std::literals;`。若要在头文件中使用
字符串字面量，则必须如此，而且根据[规则](http://eel.is/c++draft/over.literal)——用户必须以
`operator""_x` 来命名他们自己的 UDL——它们并不会与标准库相冲突。

##### 强制实施

标记头文件的全局作用域中的 `using namespace`。

### <a name="Rs-guards"></a>SF.8: 为所有的头文件使用 `#include` 防卫宏

##### 理由

避免文件被多次 `#include`。

为避免包含防卫宏的冲突，不要仅使用文件名来命名防卫宏。
确保还要包含一个关键词和好的区分词，比如头文件所属的程序库
或组件的名字。

##### 示例

    // file foobar.h:
    #ifndef LIBRARY_FOOBAR_H
    #define LIBRARY_FOOBAR_H
    // ... 声明 ...
    #endif // LIBRARY_FOOBAR_H

##### 强制实施

标记没有 `#include` 防卫的 `.h` 文件。

##### 注解

一些实现提供了如 `#pragma once` 这样的厂商扩展作为包含防卫宏的替代。
这并非标准且不可移植。它向程序中注入了宿主机器的文件系统的语义，
而且把你锁定到某个特定厂商。
我们的建议是编写 ISO C++：参见[规则 P.2](#Rp-Cplusplus)。

### <a name="Rs-cycles"></a>SF.9: 避免源文件的循环依赖

##### 理由

循环会使理解变得困难，并拖慢编译速度。
它们还会使（当其可用时）向利用语言支持的模块进行转换工作变得复杂。

##### 注解

要消除循环依赖；请勿仅仅用 `#include` 防卫宏来试图打破它们。

##### 示例，不好

    // file1.h:
    #include "file2.h"

    // file2.h:
    #include "file3.h"

    // file3.h:
    #include "file1.h"

##### 强制实施

对任何循环依赖进行标记。

### <a name="Rs-implicit"></a>SF.10: 避免依赖于隐含地 `#include` 进来的名字

##### 理由

避免意外。
避免当 `#include` 的头文件改变时改变一条 `#include`。
避免意外地变为依赖于所包含的头文件中的实现细节和逻辑上独立的实体。

##### 示例，不好

    #include <iostream>
    using namespace std;

    void use()
    {
        string s;
        cin >> s;               // 好
        getline(cin, s);        // 错误：getline() 未定义
        if (s == "surprise") {  // 错误：== 未定义
            // ...
        }
    }

`<iostream>` 暴露了 `std::string` 的定义（“为什么？”是一个有趣的问题），
但其并不必然是通过传递包含整个 `<string>` 头文件而做到这一点的，
这带来了常见的新手问题“为什么 `getline(cin,s);` 不成？”，
甚至偶尔出现的“`string` 无法用 `==` 来比较”。

其解决方案是明确地 `#include <string>`：

##### 示例，好

    #include <iostream>
    #include <string>
    using namespace std;

    void use()
    {
        string s;
        cin >> s;               // 好
        getline(cin, s);        // 好
        if (s == "surprise") {  // 好
            // ...
        }
    }

##### 注解

一些头文件正是用于从一些头文件中合并一组声明。
例如：

    // basic_std_lib.h:

    #include <string>
    #include <map>
    #include <iostream>
    #include <random>
    #include <vector>

用户只用一条 `#include` 就可以获得整组的声明了：

    #include "basic_std_lib.h"

本条反对隐式包含的规则并不防止这种特意的聚集包含。

##### 强制实施

强制实施将需要一些有关头文件中哪些是“导出”给用户所用而哪些是用于实现的知识。
在我们能用到模块之前没有真正的好方案。

### <a name="Rs-contained"></a>SF.11: 头文件应当是自包含的

##### 理由

易用性，头文件应当易于使用，且单独包含即可正常工作。
头文件应当对其所提供的功能进行封装。
避免让头文件的使用方来管理它的依赖项。

##### 示例

    #include "helpers.h"
    // helpers.h 依赖于 std::string 并已包含了 <string>

##### 注解

不遵守这条规则将导致头文件的使用方难于诊断所出现的错误。

##### 注解

头文件应当包含其所有依赖项。请小心使用相对路径，各 C++ 实现对于它们的含义是有分歧的。

##### 强制实施

以一项测试来验证头文件自身可通过编译，或者一个仅包含了该头文件的 cpp 文件可通过编译。

### <a name="Rs-incform"></a>SF.12: 对相对于包含文件的文件优先采用引号形式的 `#include`，其他情况下采用角括号形式

##### 理由

[标准](http://eel.is/c++draft/cpp.include) 向编译器提供了对于实现
使用角括号（`<>`）或引号（`""`）语法的 `#include` 的两种形式的灵活性。
各厂商利用了这点并采用了不同的搜索算法和指定包含路径的方法。

无论如何，指导方针是使用引号形式来（从同一个组件或项目中）包含那些存在于某个相对于含有这条 `#include` 语句的文件的相对路径中的文件，其他情况尽可能使用角括号形式。这样做鼓励明确表现出文件与包含它的文件之间的局部性，或当需要某种不同的搜索算法的情形。这样一眼就可以很容易明白头文件是从某个局部相对文件包含的，还是某个标准库头文件或别的搜索路径（比如另一个程序库或一组常用包含路径）中的某个头文件。

##### 示例

    // foo.cpp:
    #include <string>                // 来自标准程序库，要求使用 <> 形式
    #include <some_library/common.h> // 从另一个程序库中包含的，并非出于局部相对位置的文件；使用 <> 形式
    #include "foo.h"                 // 处于同一项目中局部相对于 foo.cpp 的文件，使用 "" 形式
    #include "foo_utils/utils.h"     // 处于同一项目中局部相对于 foo.cpp 的文件，使用 "" 形式
    #include <component_b/bar.h>     // 通过搜索路径定位到的处于同一项目中的文件，使用 <> 形式

##### 注解

不遵守这条可能会导致很难诊断的错误：由于包含时指定的错误的范围而选择了错误的文件。
例如，通常 `#include ""` 的搜索算法首先搜索存在于某个局部相对路径中的文件，因此使用这种形式来指代某个并非位于局部相对路径的文件，就一位置一旦在局部相对路径中出现了一个这样的文件（比如进行包含的文件被移动到了别的位置），它就会在原来所包含的文件之前被找到，并使包含文件集合以一种预料之外的方式被改变。

程序库作者们应当把它们的头文件放到一个文件夹中，然后让其客户使用相对路径来包含这些文件：`#include <some_library/common.h>`。

##### 强制实施

检测按 `""` 引用的头文件是否可以按 `<>` 引用。

### <a name="Rs-namespace"></a>SF.20: 用 `namespace` 表示逻辑结构

##### 理由

???

##### 示例

    ???

##### 强制实施

???

### <a name="Rs-unnamed"></a>SF.21: 请勿在头文件中使用无名（匿名）命名空间

##### 理由

在头文件中使用无名命名空间差不多都是一个 BUG。

##### 示例

    // 文件 foo.h:
    namespace
    {
        const double x = 1.234;  // 不好

        double foo(double y)     // 不好
        {
            return y + x;
        }
    }

    namespace Foo
    {
        const double x = 1.234; // 好

        inline double foo(double y)        // 好
        {
            return y + x;
        }
    }

##### 强制实施

- 对头文件中所使用的任何匿名命名空间进行标记。

### <a name="Rs-unnamed2"></a>SF.22: 为所有的内部/不导出的实体使用无名（匿名）命名空间

##### 理由

外部实体无法依赖于嵌套的无名命名空间中的实体。
考虑将实现源文件中的所有定义都放入无名命名空间中，除非它定义的是一个“外部/导出”实体。

##### 示例；不好

    static int f();
    int g();
    static bool h();
    int k();

##### 示例；好

    namespace {
        int f();
        bool h();
    }
    int g();
    int k();

##### 示例

API 类及其成员不能放在无名命名空间中；而在实现源文件中所定义的任何的“辅助”类或函数则应当放在无名命名空间作用域之中。

    ???

##### 强制实施

- ???

# <a name="S-stdlib"></a>SL: 标准库

如果只使用纯语言本身的话，任何开发任务都会变得很麻烦（无论以何种语言）。
如果使用了某个合适的程序库的话，则任何开发任务都会变得相当简单。

这些年来标准库一直在持续增长。
现在它在标准中的描述已经比语言功能特性的描述更大了。
因此，可能指导方针的库部分的规模最终将会增长等于甚至超过其他的所有部分。

<< ??? 我们需要另一个层次的规则编号 ??? >>

C++ 标准库组件概览：

- [SL.con: 容器](#SS-con)
- [SL.str: 字符串](#SS-string)
- [SL.io: I/O 流（iostream）](#SS-io)
- [SL.regex: 正则表达式](#SS-regex)
- [SL.chrono: 时间](#SS-chrono)
- [SL.C: C 标准库](#SS-clib)

标准库规则概览：

- [SL.1: 尽可能使用程序库](#Rsl-lib)
- [SL.2: 优先使用标准库而不是其他程序库](#Rsl-sl)
- [SL.3: 请勿向命名空间 `std` 中添加非标准实体](#sl-std)
- [SL.4: 以类型安全的方式使用标准库](#sl-safe)
- ???

### <a name="Rsl-lib"></a>SL.1: 尽可能使用程序库

##### 理由

节约时间。避免重复发明轮子。
避免重复他人的工作。
如果其他人的工作有了改进，则可以从中获得好处。
当你进行了改进之后可以帮助其他人。

### <a name="Rsl-sl"></a>SL.2: 优先使用标准库而不是其他程序库

##### 理由

了解标准库的人更多。
相对于你自己的代码或者大多数其他程序库来说，标准库更加倾向于稳定，进行了良好维护，而且广泛可用。

### <a name="sl-std"></a>SL.3: 请勿向命名空间 `std` 中添加非标准实体

##### 理由

向 `std` 中添加东西可能会改变本来是遵循标准的代码的含义。
添加到 `std` 的东西可能会与未来版本的标准产生冲突。

##### 示例

    ???

##### 强制实施

有可能，但很麻烦而且在一些平台上很可能导致一些问题。

### <a name="sl-safe"></a>SL.4: 以类型安全的方式使用标准库

##### 理由

因为，很显然，违反这条规则将导致未定义的行为，内存损坏，以及其他所有种类的糟糕的错误。

##### 注解

本条规则是半哲学性的元规则，需要许多具体规则予以支持。
我们需要将之作为对于更加专门的规则的总括。

更加专门的规则概览：

- [SL.4: 以类型安全的方式使用标准库](#sl-safe)

## <a name="SS-con"></a>SL.con: 容器

???

容器规则概览：

- [SL.con.1: 优先采用 STL 的 `array` 或 `vector` 而不是 C 数组](#Rsl-arrays)
- [SL.con.2: 除非有理由使用别的容器，否则默认情况应优先采用 STL 的 `vector`](#Rsl-vector)
- [SL.con.3: 避免边界错误](#Rsl-bounds)
- [SL.con.4: 请勿对非可平凡复制的实参使用 `memset` 或 `memcpy`](#Rsl-copy)

### <a name="Rsl-arrays"></a>SL.con.1: 优先采用 STL 的 `array` 或 `vector` 而不是 C 数组

##### 理由

C 数组不那么安全，而且相对于 `array` 和 `vector` 也没有什么优势。
对于定长数组，应使用 `std::array`，它传递给函数时并不会退变为指针并丢失其大小信息。
而且，和内建数组一样，栈上分配的 `std::array` 会在栈上保存它的各个元素。
对于变长数组，应使用 `std::vector`，它还可以改变大小并处理内存分配。

##### 示例

    int v[SIZE];                        // 不好

    std::array<int, SIZE> w;            // ok

##### 示例

    int* v = new int[initial_size];     // 不好，有所有权的原生指针
    delete[] v;                         // 不好，手工 delete

    std::vector<int> w(initial_size);   // ok

##### 注解

在不拥有而引用容器中的元素时使用 `gsl::span`。

##### 注解

在栈上分配的固定大小的数组和把元素都放在自由存储上的 `vector` 之间比较性能是没什么意义的。
你同样也可以在栈上的 `std::array` 和通过指针访问 `malloc()` 的结果之间进行这样的比较。
对于大多数代码来说，即便是栈上分配和自由存储分配之间的差异也没那么重要，但 `vector` 带来的便利和安全性却是重要的。
如果有人编写的代码中这种差异确实重要，那么他显然可以在 `array` 和 `vector` 之间做出选择。

##### 强制实施

- 如果 C 数组的声明所在的函数或类也声明了 STL 的某个容器（这是为了避免在老式的非 STL 代码中的大量警告噪音），则对其进行标记。修正：最少要把 C 数组改成 `std::array`。

### <a name="Rsl-vector"></a>SL.con.2: 除非有理由使用别的容器，否则默认情况应优先采用 STL 的 `vector`

##### 理由

`vector` 和 `array` 是仅有的能够提供以下各项优势的标准容器：

- 最快的通用访问（随机访问，还包括对于向量化友好性）；
- 最快的默认访问模式（从头到尾或从尾到头方式是对预读器友好的）；
- 最少的空间耗费（连续布局中没有每个元素的开销，而且是 cache 友好的）。

通常你都需要对容器进行元素的添加和删除，因此默认应当采用 `vector`；如果并不需要改动容器的大小的话，则应采用 `array`。

即便其他容器貌似更加合适，比如 `map` 的 O(log N) 查找性能，或者 `list` 的中部高效插入，对于几个 KB 以内大小的容器来说，`vector` 仍然经常性能更好。

##### 注解

`string` 不应当用作独立字符的容器。`string` 是文本字符串；如果需要字符的容器的话，应当采用 `vector</*char_type*/>` 或者 `array</*char_type*/>`。

##### 例外

如果你有正当的理由来使用别的容器的话，就请使用它。例如：

- 若 `vector` 满足你的需求，但你并不需要容器大小可变，则应当代之以 `array`。

- 若你需要支持字典式查找的容器并保证 O(K) 或 O(log N) 的查找效率，而且容器将会比较大（超过几个 KB），你需要经常进行插入使得维护有序的 `vector` 的开销不大可行，则请代之以使用 `unordered_map` 或者 `map`。

##### 注解

使用 `()` 初始化来将 `vector` 初始化为具有特定数量的元素。
使用 `{}` 初始化来以一个元素列表来对 `vector` 进行初始化。

    vector<int> v1(20);  // v1 具有 20 个值为 0 的元素（vector<int>{}）
    vector<int> v2 {20}; // v2 具有 1 个值为 20 的元素

[优先采用 `{}` 初始化式语法](#Res-list)。

##### 强制实施

- 如果 `vector` 构造之后大小不会改变（比如因为它是 `const` 或者因为没有对它调用过非 `const` 函数），则对其进行标记。修正：代之以使用 `array`。

### <a name="Rsl-bounds"></a>SL.con.3: 避免边界错误

##### 理由

越过已分配的元素的范围进行读写，通常都会导致糟糕的错误，不正确的结果，程序崩溃，以及安全漏洞。

##### 注解

应用于一组元素的范围的标准库函数，都有（或应当有）接受 `span` 的边界安全重载。
如 `vector` 这样的标准类型，在边界剖面配置下（以某种不兼容的方式，如添加契约）可以被修改为实施边界检查，或者使用 `at()`。

理想情况下，边界内保证应当可以被静态强制实行。
例如：

- 基于范围的 `for` 的循环不会越过其所针对的容器的范围
- `v.begin(),v.end()` 可以很容易确定边界安全性

这种循环和任何的等价的无检查或不安全的循环一样高效。

通常，可以用一个简单的预先检查来消除检查每个索引的需要。
例如

- 对于 `v.begin(),v.begin()+i`，`i` 可以很容易针对 `v.size()` 检查

这种循环比每次都待检查元素访问要快得多。

##### 示例，不好

    void f()
    {
        array<int, 10> a, b;
        memset(a.data(), 0, 10);         // 不好，且包含长度错误（length = 10 * sizeof(int)）
        memcmp(a.data(), b.data(), 10);  // 不好，且包含长度错误（length = 10 * sizeof(int)）
    }

而且，`std::array<>::fill()` 或 `std::fill()`，甚或是空的初始化式，都是比 `memset()` 更好的候选。

##### 示例，好

    void f()
    {
        array<int, 10> a, b, c{};       // c 被初始化为零
        a.fill(0);
        fill(b.begin(), b.end(), 0);    // std::fill()
        fill(b, 0);                     // std::ranges::fill()

        if ( a == b ) {
          // ...
        }
    }

##### Example

如果代码使用的是未修改的标准库，仍然有一些变通方案来以边界安全的方式使用 `std::array` 和 `std::vector`。代码中可以调用各个类的 `.at()` 成员函数，这将抛出 `std::out_of_range` 异常。或者，代码中可以调用 `at()` 自由函数，这将在边界违例时导致快速失败（或者某个自定义动作）。

    void f(std::vector<int>& v, std::array<int, 12> a, int i)
    {
        v[0] = a[0];        // 不好
        v.at(0) = a[0];     // OK（替代方案 1）
        at(v, 0) = a[0];    // OK（替代方案 2）

        v.at(0) = a[i];     // 不好
        v.at(0) = a.at(i);  // OK（替代方案 1）
        v.at(0) = at(a, i); // OK（替代方案 2）
    }

##### 强制实施

- 对于没有边界检查的标准库函数的任何调用都给出诊断消息。
  ??? 在这里添加一组禁用函数的连接列表

本条规则属于[边界剖面配置](#SS-bounds)。

### <a name="Rsl-copy"></a>SL.con.4: 请勿对非可平凡复制的实参使用 `memset` 或 `memcpy`

##### 理由

这样做会破坏对象语义（例如，其会覆写掉 `vptr`）。

##### 注解

`(w)memset`，`(w)memcpy`，`(w)memmove`，以及 `(w)memcmp` 与此相似。

##### 示例

    struct base {
        virtual void update() = 0;
    };

    struct derived : public base {
        void update() override {}
    };


    void f (derived& a, derived& b) // 虚表再见！
    {
        memset(&a, 0, sizeof(derived));
        memcpy(&a, &b, sizeof(derived));
        memcmp(&a, &b, sizeof(derived));
    }

应当代之以定义适当的默认初始化，复制，以及比较函数

    void g(derived& a, derived& b)
    {
        a = {};    // 默认初始化
        b = a;     // 复制
        if (a == b) do_something(a,b);
    }

##### 强制实施

- 对在不可平凡复制的类型使用这些函数进行标记

**TODO 注释**:

- 对于标准库的影响需要和 WG21 之间进行紧密的协调，即便不需要标准化也应当至少保证兼容性。
- 我们正在考虑为标准库（尤其是 C 标准库）中如 `memcmp` 这样的函数指定边界安全的重载，并在 GSL 中提供它们。
- 对于标准中没有进行完全的边界检查的现存函数和如 `vector` 这样的类型来说，我们的目标是在启用了边界剖面配置的代码中调用时，这些功能应当进行边界检查，而从遗留代码中调用时则没有检查，可能需要利用契约来实现（正由几个 WG21 成员进行提案工作）。

## <a name="SS-string"></a>SL.str: 字符串

文本处理是一个大的主题。
`std::string` 无法全部覆盖这些。
这一部分主要尝试澄清 `std::string` 和 `char*`、`zstring`、`string_view` 和 `gsl::span<char>` 之间的关系。
有关非 ASCII 字符集和编码的重要问题（比如 `wchar_t`，Unicode，以及 UTF-8 等）将在别处讨论。

**参见**：[正则表达式](#SS-regex)

在这里，我们用“字符序列”或“字符串”来代表（终将）作为文本来读取的字符序列。
We don't consider ???

字符串概览：

- [SL.str.1: 使用 `std::string` 以拥有字符序列](#Rstr-string)
- [SL.str.2: 使用 `std::string_view` 或 `gsl::span<char>` 以指代字符序列](#Rstr-view)
- [SL.str.3: 使用 `zstring` 或 `czstring` 以指代 C 风格、以零结尾的字符序列](#Rstr-zstring)
- [SL.str.4: 使用 `char*` 以指代单个字符](#Rstr-char*)
- [SL.str.5: 使用 `std::byte` 以指代并不必须表示字符的字节值](#Rstr-byte)

- [SL.str.10: 当需要实施相关于文化地域的操作时，使用 `std::string`](#Rstr-locale)
- [SL.str.11: 当需要改动字符串时，使用 `gsl::span<char>` 而不是 `std::string_view`](#Rstr-span)
- [SL.str.12: 为作为标准库的 `string` 类型的字符串字面量使用后缀 `s`](#Rstr-s)

**参见**：

- [F.24 span](#Rf-range)
- [F.25 zstring](#Rf-zstring)

### <a name="Rstr-string"></a>SL.str.1: 使用 `std::string` 以拥有字符序列

##### 理由

`string` 能够正确处理资源分配，所有权，复制，渐进扩容，并提供许多有用的操作。

##### 示例

    vector<string> read_until(const string& terminator)
    {
        vector<string> res;
        for (string s; cin >> s && s != terminator; ) // 读取一个单词
            res.push_back(s);
        return res;
    }

注意已经为 `string` 提供了 `>>` 和 `!=`（作为有用操作的例子），并且没有显示的内存分配，
回收，或者范围检查（`string` 会处理这些）。

C++17 中，我们可以使用 `string_view` 而不是 `const string&` 作为参数，以允许调用方更大的灵活性：

    vector<string> read_until(string_view terminator)   // C++17
    {
        vector<string> res;
        for (string s; cin >> s && s != terminator; ) // 读取一个单词
            res.push_back(s);
        return res;
    }

##### 示例，不好

不要使用 C 风格的字符串来进行需要不单纯的内存管理的操作：

    char* cat(const char* s1, const char* s2)   // 当心！
        // return s1 + '.' + s2
    {
        int l1 = strlen(s1);
        int l2 = strlen(s2);
        char* p = (char*)malloc(l1 + l2 + 2);
        strcpy(p, s1, l1);
        p[l1] = '.';
        strcpy(p + l1 + 1, s2, l2);
        p[l1 + l2 + 1] = 0;
        return p;
    }

我们搞对了吗？
调用者能记得要对返回的指针调用 `free()` 吗？
这段代码能通过安全性评审吗？

##### 注解

没有测量就不要假设 `string` 比底层技术慢，要记得并非所有代码都是性能攸关的。
[请勿进行不成熟的优化](#Rper-Knuth)

##### 强制实施

???

### <a name="Rstr-view"></a>SL.str.2: 使用 `std::string_view` 或 `gsl::span<char>` 以指代字符序列

##### 理由

`std::string_view` 或 `gsl::span<char>` 提供了简易且（潜在）安全的对字符序列的访问，并与序列的
分配和存储方式无关。

##### 示例

    vector<string> read_until(string_view terminator);

    void user(zstring p, const string& s, string_view ss)
    {
        auto v1 = read_until(p);
        auto v2 = read_until(s);
        auto v3 = read_until(ss);
        // ...
    }

##### 注解

`std::string_view`（C++17）是只读的。

##### 强制实施

???

### <a name="Rstr-zstring"></a>SL.str.3: 使用 `zstring` 或 `czstring` 以指代 C 风格、以零结尾的字符序列

##### 理由

可读性。
明确意图。
普通的 `char*` 可以是指向单个字符的指针，指向字符数组的指针，指向 C 风格（零结尾）字符串的指针，甚或是指向小整数的指针。
对这些情况加以区分能够避免误解和 BUG。

##### 示例

    void f1(const char* s); // s 可能是个字符串

我们所知的只不过是它可能是 nullptr 或者指向至少一个字符

    void f1(zstring s);     // s 是 C 风格字符串或者 nullptr
    void f1(czstring s);    // s 是 C 风格字符串常量或者 nullptr
    void f1(std::byte* s);  // s 是某个字节的指针（C++17）

##### 注解

除非确实有理由，否则不要把 C 风格的字符串转换为 `string`。

##### 注解

与其他的“普通指针”一样，`zstring` 不能表达所有权。

##### 注解

已经存在了上亿行的 C++ 代码，它们大多使用 `char*` 和 `const char*` 却并不注明其意图。
各种不同的方式都在使用它们，包括以之表示所有权，以及（代替 `void*`）作为通用的内存指针。
很难区分这些用法，因此这条指导方针很难被遵守。
而这是 C 和 C++ 程序中的最主要的 BUG 来源之一，因此一旦可行就遵守这条指导方针是值得的。

##### 强制实施

- 标记在 `char*` 上使用的 `[]`
- 标记在 `char*` 上使用的 `delete`
- 标记在 `char*` 上使用的 `free()`

### <a name="Rstr-char*"></a>SL.str.4: 使用 `char*` 以指代单个字符

##### 示例

现存代码中对 `char*` 的各种不同用法，是一种主要的错误来源。

##### 示例，不好

    char arr[] = {'a', 'b', 'c'};

    void print(const char* p)
    {
        cout << p << '\n';
    }

    void use()
    {
        print(arr);   // 运行时错误；可能非常糟糕
    }

数组 `arr` 并非 C 风格字符串，因为它不是零结尾的。

##### 替代方案

参见 [`zstring`](#Rstr-zstring)，[`string`](#Rstr-string)，以及 [`string_view`](#Rstr-view)。

##### 强制实施

- 标记在 `char*` 上使用的 `[]`

### <a name="Rstr-byte"></a>SL.str.5: 使用 `std::byte` 以指代并不必须表示字符的字节值

##### 理由

用 `char*` 来表示指向不一定是字符的东西的指针会造成混乱，
并会妨碍有价值的优化。

##### 示例

    ???

##### 注解

C++17

##### 强制实施

???

### <a name="Rstr-locale"></a>SL.str.10: 当需要实施相关于文化地域的操作时，使用 `std::string`

##### 理由

`std::string` 支持标准库的 [`locale` 功能](#Rstr-locale)

##### 示例

    ???

##### 注解

???

##### 强制实施

???

### <a name="Rstr-span"></a>SL.str.11: 当需要改动字符串时，使用 `gsl::span<char>` 而不是 `std::string_view`

##### 理由

`std::string_view` 是只读的。

##### 示例

???

##### 注解

???

##### 强制实施

编译器会标记出试图写入 `string_view` 的地方。

### <a name="Rstr-s"></a>SL.str.12: 为作为标准库的 `string` 类型的字符串字面量使用后缀 `s`

##### 理由

直接表达想法能够最小化犯错机会。

##### 示例

    auto pp1 = make_pair("Tokyo", 9.00);         // {C 风格字符串,double} 有意如此？
    pair<string, double> pp2 = {"Tokyo", 9.00};  // 稍微啰嗦
    auto pp3 = make_pair("Tokyo"s, 9.00);        // {std::string,double}    // C++14
    pair pp4 = {"Tokyo"s, 9.00};                 // {std::string,double}    // C++17

##### 强制实施

???

## <a name="SS-io"></a>SL.io: I/O 流（iostream）

`iostream` 是一种类型安全的，可扩展的，带格式的和无格式的流式 I/O 的 I/O 程序库。
它支持多种（且用户可扩展的）缓冲策略以及多种文化地域。
它可以用于进行便利的 I/O，内存读写（字符串流），
以及用户定义的扩展，诸如跨网络的流（asio：尚未标准化）。

I/O 流规则概览：

- [SL.io.1: 仅在必要时才使用字符层面的输入](#Rio-low)
- [SL.io.2: 当进行读取时，总要考虑非法输入](#Rio-validate)
- [SL.io.3: 优先使用 iostream 进行 I/O](#Rio-streams)
- [SL.io.10: 除非你使用了 `printf` 族函数，否则要调用 `ios_base::sync_with_stdio(false)`](#Rio-sync)
- [SL.io.50: 避免使用 `endl`](#Rio-endl)
- [???](#???)

### <a name="Rio-low"></a>SL.io.1: 仅在必要时才使用字符层面的输入

##### 理由

除非你确实仅处理单个的字符，否则使用字符级的输入将导致用户代码实施潜在易错的
且潜在低效的从字符进行标记组合的工作。

##### 示例

    char c;
    char buf[128];
    int i = 0;
    while (cin.get(c) && !isspace(c) && i < 128)
        buf[i++] = c;
    if (i == 128) {
        // ... 处理过长的字符串 ....
    }

更好的做法（简单得多而且可能更快）：

    string s;
    s.reserve(128);
    cin>>s;

而且额能并不需要 `reserve(128)`。

##### 强制实施

???

### <a name="Rio-validate"></a>SL.io.2: 当进行读取时，总要考虑非法输入

##### 理由

错误通常最好尽快处理。
如果输入无效，所有的函数都必须编写为对付不良的数据（而这并不现实）。

##### 示例

    ???

##### 强制实施

???

### <a name="Rio-streams"></a>SL.io.3: 优先使用 iostream 进行 I/O

##### 理由

`iosteam` 安全，灵活，并且可扩展。

##### 示例

    // 写出一个复数：
    complex<double> z{ 3,4 };
    cout << z << '\n';

`complex` 是一个用户定义的类型，而其 I/O 的定义无需改动 `iostream` 库。

##### 示例

    // 读取一系列复数：
    for (complex<double> z; cin>>z)
        v.push_back(z);

##### 例外

??? 性能 ???

##### 讨论：`iostream` vs. `printf()` 家族

人们通常说（并且通常是正确的）`printf` 家族比 `iostream` 有两个优势：
格式化的灵活性和性能。
这需要与 `iostream` 在处理用户定义类型方面的扩展性，针对安全性的违反方面的韧性，
隐含的内存管理，以及 `locale` 处理等优势之间进行权衡。

如果需要 I/O 性能的话，你几乎总能做到比 `printf()` 更好。

`gets()`，使用 `%s` 的 `scanf()`，和使用 `%s` 的 `printf()` 在安全性方面冒风险（容易遭受缓冲区溢出问题而且通常很易错）。
C11 定义了一些“可选扩展”，它们对其实参进行一些额外检查。
如果您的 C 程序库中包含 `gets_s()`、`scanf_s()` 和 `printf_s()`，它们也许是更安全的替代方案，但仍然并非是类型安全的。

##### 强制实施

可选地标记 `<cstdio>` 和 `<stdio.h>`。

### <a name="Rio-sync"></a>SL.io.10: 除非你使用了 `printf` 族函数，否则要调用 `ios_base::sync_with_stdio(false)`

##### 理由

`iostreams` 和 `printf` 风格的 I/O 之间的同步是由代价的。
`cin` 和 `cout` 默认是与 `printf` 相同步的。

##### 示例

    int main()
    {
        ios_base::sync_with_stdio(false);
        // ... 使用 iostreams ...
    }

##### 强制实施

???

### <a name="Rio-endl"></a>SL.io.50: 避免使用 `endl`

##### 理由

`endl` 操纵符大致相当于 `'\n'` 和 `"\n"`；
其最常用的情况只不过会以添加多余的 `flush()` 的方式拖慢程序。
与 `printf` 式输出相比，这种拖慢程度是比较显著的。

##### 示例

    cout << "Hello, World!" << endl;    // 两次输出操作和一次 flush
    cout << "hello, World!\n";          // 一次输出操作且没有 flush

##### 注解

对于 `cin`/`cout`（或同等设备）的交互来说，没什么原因必须进行冲洗；它们是自动进行的。
对于向文件写入来说，也很少需要 `flush`。

##### 注解

对于字符串流（指 `ostringstream`），插入一个 `endl` 完全等价于
插入一个 `'\n'` 字符，但正是这种情况下，`endl` 可能会明显比较慢。

`endl` *并不*关注产生平台专有的行结尾序列（比如 Windows 上的 `"\r\n"`）。
因此，字符串流的 `s << endl` 只会插入*单个* `'\n'` 字符。

##### 注解

除了（偶尔会比较重要的）性能问题外，
从 `"\\n"` 和 `endl` 之间进行选择基本上完全是审美问题。

## <a name="SS-regex"></a>SL.regex: 正则表达式

`<regex>` 是标准 C++ 的正则表达式库。
它支持许多正则表达式的模式约定。

## <a name="SS-chrono"></a>SL.chrono: 时间

`<chrono>`（在命名空间 `std::chrono` 中定义）提供了 `time_point` 和 `duration`，并同时提供了
用于以各种不同单位输出时间的函数。
它还提供了用于注册 `time_point` 的时钟。

## <a name="SS-clib"></a>SL.C: C 标准库

???

C 标准库规则概览：

- [SL.C.1: 请勿使用 setjmp/longjmp](#Rclib-jmp)
- [???](#???)
- [???](#???)

### <a name="Rclib-jmp"></a>SL.C.1: 请勿使用 setjmp/longjmp

##### 理由

`longjmp` 会忽略析构函数，由此使得依赖于 RAII 的所有资源管理策略全部失效。

##### 强制实施

标记出现的所有 `longjmp` 和 `setjmp`

# <a name="S-A"></a>A: 架构设计的观念

本部分包括有关高层次的架构性观念和程序库的观念。

架构性规则概览：

- [A.1: 分离稳定的代码和不稳定的代码](#Ra-stable)
- [A.2: 将潜在可复用的部分作为程序库](#Ra-lib)
- [A.4: 程序库之间不能有循环依赖](#Ra-dag)
- [???](#???)
- [???](#???)
- [???](#???)
- [???](#???)
- [???](#???)
- [???](#???)

### <a name="Ra-stable"></a>A.1: 分离稳定的代码和不稳定的代码

对较不稳定的代码进行隔离，有助于其单元测试，接口改进，重构，以及最终弃用。

### <a name="Ra-lib"></a>A.2: 将潜在可复用的部分作为程序库

##### 理由

##### 注解

程序库是一些共同进行维护，文档化，并发布的声明式和定义式的集合体。
程序库可以是一组头文件（“仅有头文件的程序库”），或者一组头文件加上一组目标文件构成。
你可以静态或动态地将程序库连接到程序中，或者你还可以 `#included` 仅头文件的库。

### <a name="Ra-dag"></a>A.4: 程序库之间不能有循环依赖

##### 理由

- 循环依赖导致构建过程变得复杂。
- 循环依赖难于理解，可能会引入不确定性（未定义行为）。

##### 注解

一个程序库可以在它的组件的定义之间包含循环引用。
例如：

    ???

不过，程序库不能对依赖于它的其他程序库产生依赖。

# <a name="S-not"></a>NR: 伪规则和错误的看法

本部分包含一些在不少地方流行的规则和指导方针，但是我们慎重地建议不要采纳它们。
我们完全了解这些规则曾经在某些时间和场合是有意义的，而且我们自己也曾经采用过它们。
不过，在我们所推荐并以各项指导方针所支持的编程风格的情况中，这些“伪规则”是有害的。

即便是今天，仍有一些情况下这些规则是有意义的。
比如说，缺少合适的工具支持会导致异常在硬实时系统中的不适用，
但请不要盲目地信任“通俗智慧”（比如有关“效率”的未经数据支持的观点）；
这种“智慧”也许是基于几十年前的信息，或者是来自于与 C++ 有非常不同性质的语言的经验
（比如 C 或者 Java）。

对于这些伪规则的替代方案的正面观点都在各个规则的“替代方案”部分中给出。

伪规则概览：

- [NR.1: 请勿坚持认为声明都应当放在函数的最上面](#Rnr-top)
- [NR.2: 请勿坚持使函数中只保留一个 `return` 语句](#Rnr-single-return)
- [NR.3: 请勿避免使用异常](#Rnr-no-exceptions)
- [NR.4: 请勿坚持把每个类定义放在其自己的源文件中](#Rnr-lots-of-files)
- [NR.5: 请勿采用两阶段初始化](#Rnr-two-phase-init)
- [NR.6: 请勿把所有清理操作放在函数末尾并使用 `goto exit`](#Rnr-goto-exit)
- [NR.7: 请勿使所有数据成员 `protected`](#Rnr-protected-data)
- ???

### <a name="Rnr-top"></a>NR.1: 请勿坚持认为声明都应当放在函数的最上面

##### 理由

“所有声明都在开头”的规则，是来自不允许在语句之后对变量和常量进行初始化的老编程语言的遗产。
这样做会导致更长的程序，以及更多由于未初始化的或者错误初始化的变量所导致的错误。

##### 示例，不好

    int use(int x)
    {
        int i;
        char c;
        double d;

        // ... 做一些事 ...

        if (x < i) {
            // ...
            i = f(x, d);
        }
        if (i < x) {
            // ...
            i = g(x, c);
        }
        return i;
    }

未初始化变量和其使用点的距离越长，出现 BUG 的机会就越大。
幸运的是，编译器可以发现许多“设值前使用”的错误。
不幸的是，编译器无法捕捉到所有这样的错误，而且一些 BUG 并不都像这个小例子中的这样容易发现。

##### 替代方案

- [坚持为对象进行初始化](#Res-always)。
- [ES.21: 不要在确实需要使用变量（或常量）之前就引入它](#Res-introduce)。

### <a name="Rnr-single-return"></a>NR.2: 请勿坚持使函数中只保留一个 `return` 语句

##### 理由

单返回规则会导致不必要地复杂的代码，并引入多余的状态变量。
特别是，单返回规则导致更难在函数开头集中进行错误检查。

##### 示例

    template<class T>
    //  requires Number<T>
    string sign(T x)
    {
        if (x < 0)
            return "negative";
        if (x > 0)
            return "positive";
        return "zero";
    }

为仅使用一个返回语句，我们得做类似这样的事：

    template<class T>
    //  requires Number<T>
    string sign(T x)        // 不好
    {
        string res;
        if (x < 0)
            res = "negative";
        else if (x > 0)
            res = "positive";
        else
            res = "zero";
        return res;
    }

这不仅更长，而且很可能效率更差。
越长越复杂的函数，对其进行变通就越是痛苦。
当然许多简单的函数因为它们本来就简单的逻辑都天然就只有一个 `return`。

##### 示例

    int index(const char* p)
    {
        if (!p) return -1;  // 错误指标：替代方案是 "throw nullptr_error{}"
        // ... 进行查找以找出 p 的索引
        return i;
    }

如果我们采纳这条规则的话，得做类似这样的事：

    int index2(const char* p)
    {
        int i;
        if (!p)
            i = -1;  // 错误指标
        else {
            // ... 进行查找以找出 p 的索引
        }
        return i;
    }

注意我们（故意地）违反了禁止未初始化变量的规则，因为这种风格通常都会导致这样。
而且，这种风格也会倾向于采用 [goto exit](#Rnr-goto-exit) 伪规则。

##### 替代方案

- 保持函数短小简单。
- 随意使用多个 `return` 语句（以及抛出异常）。

### <a name="Rnr-no-exceptions"></a>NR.3: 请勿避免使用异常

##### 理由

一般有四种主要的不用异常的理由：

- 异常是低效的
- 异常会导致泄漏和错误
- 异常的性能无法预测
- 异常处理的运行时支持耗费过多空间

我们没有能够满足所有人的解决这个问题的办法。
无论如何，针对异常的讨论已经持续了四十多年了。
一些语言没有异常就无法使用，而另一些并不支持异常。
这在使用和不使用异常的方面都造成了强大的传统，并导致激烈的争论。

不过，我们可以简要说明，为什么我们认为对于通用编程以及这里的指导方针的情况来说，
异常是最佳的候选方案。
简单的论据，无论支持还是反对，都是缺乏说服力的。
确实存在一些特殊的应用，其中异常就是不合适的。
（例如，硬实时系统，且缺乏可靠的对于异常处理的耗费进行估计的支持）。

我们依次来考虑针对异常的主要反对观点：

- 异常是低效的：
  和什么相比？
  当进行比较时，请确保处理了同样的错误集合，并且它们都进行了等价的处理。
  尤其是，不要对一个见到异常就立刻终止的程序和一个在记录错误日志之前
  小心地进行资源清理的程序之间进行比较。
  确实，某些系统的异常处理实现很糟糕；有时候，这样的实现迫使我们使用
  其他错误处理方案，但这并不是异常的基本问题。
  当使用某个有效的论据时——无论什么样的上下文——请小心你能拿出确实提供了所讨论的问题的
  内部情况的健全的数据。
- 异常会导致泄漏和错误。
  不会。
  如果你的程序时一大堆乱糟糟的指针而没有总体的资源管理策略，
  那么无论你干什么都会有问题。
  如果你的系统是由上百万行这样的代码构成的，
  那你可能是无法使用异常的，
  不过这是一个有关过度和放纵的使用指针的问题，而不是异常的问题。
  我们的观点是，你需要用 RAII 来让基于异常的错误处理变得简单且安全——比其他方案都要更简单和安全。
- 异常的性能无法预测。
  如果你是在硬实时系统上，而你必须确保一个任务要在给定的时间内完成，
  你需要一些工具来支撑这样的保证。
  就我们所知，还没有出现这样的工具（至少对大多数程序员没有）。
- 异常处理的运行时支持耗费过多空间
  小型（通常为嵌入式）系统中可能如此。
  不过在放弃异常之前，请考虑采用统一的利用错误码的错误处理将耗费的空间有多少，
  以及错误未被捕获将造成的损失由多少。

许多（可能是大多数）的和异常有关的问题都源自于需要和杂乱的老代码进行交互的历史性原因。

而支持使用异常的基本论点是：

- 它们把错误返回和普通返回进行了清晰的区分
- 它们无法被忘记或忽略
- 它们可以系统化地使用

请记住

- 异常是用于报告错误的（C++ 中；其他语言可能有异常的不同用法）。
- 异常不是用于可以局部处理的错误的。
- 不要试图在每个函数中捕获每一种异常（这样做是冗长的，臃肿的，而且会导致代码缓慢）。
- 异常不是用于那些当发生无法恢复的错误之后需要立即终止模块或系统的错误的。

##### 示例

    ???

##### 替代方案

- [RAII](#Re-raii)
- 契约/断言：使用 GSL 的 `Expects` 和 `Ensures`（直到对契约的语言支持可以使用）

### <a name="Rnr-lots-of-files"></a>NR.4: 请勿坚持把每个类定义放在其自己的源文件中

##### 理由

将每个类都放进其自己的文件所导致的文件数量难于管理，并会拖慢编译过程。
单个的类很少是一种良好的维护和发布的逻辑单位。

##### 示例

    ???

##### 替代方案

- 使用命名空间来包含逻辑上聚合的类和函数。

### <a name="Rnr-two-phase-init"></a>NR.5: 请勿采用两阶段初始化

##### 理由

将初始化拆分为两步会导致不变式的弱化，
更复杂的代码（必须处理半构造对象），
以及错误（当未能一致地正确处理半构造对象时）。

##### 示例，不好

    // 老式传统风格：有许多问题

    class Picture
    {
        int mx;
        int my;
        int * data;
    public:
        // 主要问题：构造函数未进行完全构造
        Picture(int x, int y)
        {
            mx = x;         // 也不好：在构造函数体中而非
                            // 成员初始化式中进行赋值
            my = y;
            data = nullptr; // 也不好：在构造函数中而非
                            // 成员初始化式中进行常量初始化
        }

        ~Picture()
        {
            Cleanup();
        }

        // ...

        // 不好：两阶段初始化
        bool Init()
        {
            // 不变式检查
            if (mx <= 0 || my <= 0) {
                return false;
            }
            if (data) {
                return false;
            }
            data = (int*) malloc(mx*my*sizeof(int));   // 也不好：拥有原始指针，还用了 malloc
            return data != nullptr;
        }

        // 也不好：没有理由让清理操作作为单独的函数
        void Cleanup()
        {
            if (data) free(data);
            data = nullptr;
        }
    };

    Picture picture(100, 0); // 此时 picture 尚未就绪可用
    // 这里将失败
    if (!picture.Init()) {
        puts("Error, invalid picture");
    }
    // 现在有一个无效的 picture 对象实例。

##### 示例，好

    class Picture
    {
        int mx;
        int my;
        vector<int> data;

        static int check_size(int size)
        {
            // 不变式检查
            Expects(size > 0);
            return size;
        }

    public:
        // 更好的方式是以一个 2D 的 Size 类作为单个形参
        Picture(int x, int y)
            : mx(check_size(x))
            , my(check_size(y))
            // 现在已知 x 和 y 为有效的大小
            , data(mx * my) // 出错时将抛出 std::bad_alloc
        {
            // 图片就绪可用
        }

        // 编译器生成的析构函数会完成工作。（另见 C.21）

        // ...
    };

    Picture picture1(100, 100);
    // picture1 已就绪可用……

    // y 并非有效大小值，
    // 缺省的契约违规行为将会调用 std::terminate
    Picture picture2(100, 0);
    // 不会抵达这里……

##### 替代方案

- 始终在构造函数中建立类不变式。
- 不要在需要对象之前就定义它。

### <a name="Rnr-goto-exit"></a>NR.6: 请勿把所有清理操作放在函数末尾并使用 `goto exit`

##### 理由

`goto` 是易错的。
这种技巧是进行 RAII 式的资源和错误处理的前异常时代的技巧。

##### 示例，不好

    void do_something(int n)
    {
        if (n < 100) goto exit;
        // ...
        int* p = (int*) malloc(n);
        // ...
        if (some_error) goto_exit;
        // ...
    exit:
        free(p);
    }

请找出其中的 BUG。

##### 替代方案

- 使用异常和 [RAII](#Re-raii)
- 对于非 RAII 资源，使用 [`finally`](#Re-finally)。

### <a name="Rnr-protected-data"></a>NR.7: 请勿使所有数据成员 `protected`

##### 理由

`protected` 数据是一种错误来源。
`protected` 数据可以被各种地方的无界限数量的代码所操纵。
`protected` 数据是在类层次中等价于全局对象的东西。

##### 示例

    ???

##### 替代方案

- [使成员数据 `public` 或者（更好地）`private`](#Rh-protected)。

# <a name="S-references"></a>RF: 参考材料

已经为 C++，尤其是对 C++ 的使用编写过了许多的编码标准、规则和指导方针。
它们中许多都

- 关注的是低级问题，比如标识符的拼写
- 是由 C++ 的新手编写的
- 将“禁止程序员作出不常见行为”作为其首要目标
- 将维持许多编译器的可移植性作为目标（有些已经是 10 年前的了）
- 是为了维持好几十年的代码库而编写的
- 是仅关注单一的应用领域的
- 只会产生反效果
- 被忽略了（程序员为了完成工作不得不忽略它们）

不良的编码标准要比没有编码标准还要差。
不过一组恰当的指导方针比没有标准要好得多：“形式即解放。”

我们为什么不能有一种允许所有我们想要的同时又禁止所有我们不期望的东西的语言（“完美的语言”）呢？
本质上说，这是由于可负担的语言（及其工具链）同时也要为那些需求与你不同的人提供服务，并且要为你今后比今天更多的需求提供服务。
而且，你的需求会随时间而改变，而为此你则需要采用一种通用语言。
今日貌似理想的语言在未来可能会变得过于受限了。

编码指导方针可以使语言能够适应于特定的需求。
因此，并不存在适用于每个人的单一编码风格。
我们预计不同的组织会提供更具限制性和更严格的编码风格附加规定。

参考材料部分：

- [RF.rules: 编码规则](#SS-rules)
- [RF.books: 带有编码指导方针的书籍](#SS-books)
- [RF.C++: C++ 编程 (C++11/C++14/C++17)](#SS-Cplusplus)
- [RF.web: 网站](#SS-web)
- [RS.video: 有关“当代 C++”的视频](#SS-vid)
- [RF.man: 手册](#SS-man)
- [RF.core: 核心指导方针相关材料](#SS-core)

## <a name="SS-rules"></a>RF.rules: 编码规则

- [AUTOSAR Guidelines for the use of the C++14 language in critical and safety-related systems v17.10](https://www.autosar.org/fileadmin/user_upload/standards/adaptive/17-10/AUTOSAR_RS_CPP14Guidelines.pdf)
- [Boost Library Requirements and Guidelines](http://www.boost.org/development/requirements.html).
  ???.
- [Bloomberg: BDE C++ Coding](https://github.com/bloomberg/bde/wiki/CodingStandards.pdf).
  着重强调了代码的组织和布局。
- Facebook: ???
- [GCC Coding Conventions](https://gcc.gnu.org/codingconventions.html).
  C++03 以及（相当）一部分向后兼容。
- [Google C++ Style Guide](https://google.github.io/styleguide/cppguide.html).
  面向 C++17 和（同样）较老的代码库。Google 的专家们现在正展开活跃的合作，以改进这里的各项指导方针，有希望能够合并这些成果，以使它们能够成为他们也同样推荐采纳的一组现代的通用指导方针。
- [JSF++: JOINT STRIKE FIGHTER AIR VEHICLE C++ CODING STANDARDS](http://www.stroustrup.com/JSF-AV-rules.pdf).
  文档编号 2RDU00001 Rev C. December 2005.
  针对飞行控制软件。
  针对硬实时。
  这意味着它需要非常多的限制（“程序如果发生故障就会有人挂掉”）。
  例如，飞机起飞后禁止进行任何自由存储的分配和回收（禁止内存溢出并禁止发生碎片化）。
  禁止使用异常（因为没有可用工具可以保证异常能够在固定的短时间段内被处理）。
  所使用的程序库必须是已被证明可以用于关键任务应用的。
  它和这个指导方针集合的相似性并不让人惊讶，因为 Bjarne Stroustrup 正是 JSF++ 的作者之一。
  建议采纳，但请注意其非常特定的关注领域。
- [MISRA C++ 2008: Guidelines for the use of the C++ language in critical systems] (https://www.misra.org.uk/Buyonline/tabid/58/Default.aspx)。
- [Using C++ in Mozilla Code](https://firefox-source-docs.mozilla.org/code-quality/coding-style/using_cxx_in_firefox_code.html).
  如其名称所示，它关注于跨许多（老）编译器的兼容性。
  因此，它是很具有限制性的。
- [Geosoft.no: C++ Programming Style Guidelines](http://geosoft.no/development/cppstyle.html).
  ???.
- [Possibility.com: C++ Coding Standard](http://www.possibility.com/Cpp/CppCodingStandard.html).
  ???.
- [SEI CERT: Secure C++ Coding Standard](https://wiki.sei.cmu.edu/confluence/x/Wnw-BQ).
  针对安全关键代码所编写的一组非常好的规则（还带有示例和原理说明）。
  它们的许多规则都广泛适用。
- [High Integrity C++ Coding Standard](http://www.codingstandard.com/).
- [llvm](http://llvm.org/docs/CodingStandards.html).
  有些简略，基于 C++14，而且是（有理由地）针对其应用领域的。
- ???

## <a name="SS-books"></a>RF.books: 带有编码指导方针的书籍

- [Meyers96](#Meyers96) Scott Meyers: _More Effective C++_. Addison-Wesley 1996.
- [Meyers97](#Meyers97) Scott Meyers: _Effective C++, Second Edition_. Addison-Wesley 1997.
- [Meyers01](#Meyers01) Scott Meyers: _Effective STL_. Addison-Wesley 2001.
- [Meyers05](#Meyers05) Scott Meyers: _Effective C++, Third Edition_. Addison-Wesley 2005.
- [Meyers15](#Meyers15) Scott Meyers: _Effective Modern C++_. O'Reilly 2015.
- [SuttAlex05](#SuttAlex05) Sutter and Alexandrescu: _C++ Coding Standards_. Addison-Wesley 2005. 与其说是一组规则，不如说是一组元规则。前 C++11 时代。
- [Stroustrup05](#Stroustrup05) Bjarne Stroustrup: [A rationale for semantically enhanced library languages](http://www.stroustrup.com/SELLrationale.pdf).
  LCSD05. October 2005.
- [Stroustrup14](#Stroustrup05) Stroustrup: [A Tour of C++](http://www.stroustrup.com/Tour.html).
  Addison Wesley 2014.
  每章的结尾都有一个包含一组建议的忠告部分。
- [Stroustrup13](#Stroustrup13) Stroustrup: [The C++ Programming Language (4th Edition)](http://www.stroustrup.com/4th.html).
  Addison Wesley 2013.
  每章的结尾都有一个包含一组建议的忠告部分。
- Stroustrup: [Style Guide](http://www.stroustrup.com/Programming/PPP-style.pdf)
  for [Programming: Principles and Practice using C++](http://www.stroustrup.com/programming.html).
  大多是一些低级的命名和代码布局规则。
  主要作为教学工具。

## <a name="SS-Cplusplus"></a>RF.C++: C++ 编程 (C++11/C++14)

- [TC++PL4](http://www.stroustrup.com/4th.html):
  面向有经验的程序员的，对 C++ 语言和标准库的全面彻底的描述。
- [Tour++](http://www.stroustrup.com/Tour.html):
  面向有经验的程序员的，对 C++ 语言和标准库的简介。
- [Programming: Principles and Practice using C++](http://www.stroustrup.com/programming.html):
  面向初学者和新手们的教材。

## <a name="SS-web"></a>RF.web: 网站

- [isocpp.org](https://isocpp.org)
- [Bjarne Stroustrup 的个人主页](http://www.stroustrup.com)
- [WG21](http://www.open-std.org/jtc1/sc22/wg21/)
- [Boost](http://www.boost.org)<a name="Boost"></a>
- [Adobe open source](https://opensource.adobe.com/)
- [Poco libraries](http://pocoproject.org/)
- Sutter's Mill?
- ???

## <a name="SS-vid"></a>RS.video: 有关“当代 C++”的视频

- Bjarne Stroustrup: [C++11?Style](http://channel9.msdn.com/Events/GoingNative/GoingNative-2012/Keynote-Bjarne-Stroustrup-Cpp11-Style). 2012.
- Bjarne Stroustrup: [The Essence of C++: With Examples in C++84, C++98, C++11, and?C++14](http://channel9.msdn.com/Events/GoingNative/2013/Opening-Keynote-Bjarne-Stroustrup). 2013
- [CppCon '14](https://isocpp.org/blog/2014/11/cppcon-videos-c9) 的全部演讲
- Bjarne Stroustrup: [The essence of C++](https://www.youtube.com/watch?v=86xWVb4XIyE) 在爱丁堡大学。2014
- Bjarne Stroustrup: [The Evolution of C++ Past, Present and Future](https://www.youtube.com/watch?v=_wzc7a3McOs). CppCon 2016 keynote.
- Bjarne Stroustrup: [Make Simple Tasks Simple!](https://www.youtube.com/watch?v=nesCaocNjtQ). CppCon 2014 keynote.
- Bjarne Stroustrup: [Writing Good C++14](https://www.youtube.com/watch?v=1OEu9C51K2A). CppCon 2015 keynote about the Core Guidelines.
- Herb Sutter: [Writing Good C++14... By Default](https://www.youtube.com/watch?v=hEx5DNLWGgA). CppCon 2015 keynote about the Core Guidelines.
- CppCon 15
- ??? C++ Next
- ??? Meting C++
- ??? more ???

## <a name="SS-man"></a>RF.man: 手册

- ISO C++ Standard C++11.
- ISO C++ Standard C++14.
- [ISO C++ Standard C++17](http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2016/n4606.pdf). 委员会草案。
- [Palo Alto "Concepts" TR](http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2012/n3351.pdf).
- [ISO C++ Concepts TS](http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2015/n4553.pdf).
- [WG21 Ranges report](http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2016/n4569.pdf). 草案。

## <a name="SS-core"></a>RF.core: 核心指导方针相关材料

这个部分包含一些用于展示核心指导方针及其背后的思想的有用材料：

- [Our documents directory](https://github.com/isocpp/CppCoreGuidelines/tree/master/docs)
- Stroustrup, Sutter, and Dos Reis: [A brief introduction to C++’s model for type- and resource-safety](http://www.stroustrup.com/resource-model.pdf). A paper with lots of examples.
- Sergey Zubkov: [a Core Guidelines talk](https://www.youtube.com/watch?v=DyLwdl_6vmU)
  and here are the [slides](http://2017.cppconf.ru/talks/sergey-zubkov). In Russian. 2017.
- Neil MacIntosh: [The Guideline Support Library: One Year Later](https://www.youtube.com/watch?v=_GhNnCuaEjo). CppCon 2016.
- Bjarne Stroustrup: [Writing Good C++14](https://www.youtube.com/watch?v=1OEu9C51K2A). CppCon 2015 keynote.
- Herb Sutter: [Writing Good C++14... By Default](https://www.youtube.com/watch?v=hEx5DNLWGgA). CppCon 2015 keynote.
- Peter Sommerlad: [C++ Core Guidelines - Modernize your C++ Code Base](https://www.youtube.com/watch?v=fQ926v4ZzAM). ACCU 2017.
- Bjarne Stroustrup: [No Littering!](https://www.youtube.com/watch?v=01zI9kV4h8c). Bay Area ACCU 2016.
  It gives some idea of the ambition level for the Core uidelines.

CppCon 的展示的幻灯片是可以获得的（其链接，还有上传的视频）。

极大欢迎对于这个列表的贡献。

## <a name="SS-ack"></a>鸣谢

感谢对规则、建议、支持信息和参考材料等等作出了各种贡献的许多人：

- Peter Juhl
- Neil MacIntosh
- Axel Naumann
- Andrew Pardoe
- Gabriel Dos Reis
- Zhuang, Jiangang (Jeff)
- Sergey Zubkov

请查看 github 的贡献者列表。

# <a name="S-profile"></a>Pro: 剖面配置

理想情况，我们应当遵循所有这些指导方针。
这样能够得到最简洁，最规范，最少易错性，而且通常是最快的代码。
不幸的是这通常是不可能的，因为我们不得不让代码适合于大型的代码库并使用一些现存的程序库。
常常是，这样的代码已经编写好几十年了，并且并不遵循这些指导方针。
我们必须以[渐次采纳](#S-modernizing)为目标。

无论采用何种渐次采纳的策略，我们都应当能够首先采用一些相关指导方针的集合来
处理某些问题的集合，遗留其他的以后处理。
当发现某些而不是全部的指导方针对于代码库有关时，以及当在某个专门化的应用领域
采用一组专门化的指导方针的集合时，会出现类似的“相关指导方针”的主意。
我们称这样的相关指导方针的集合为一个“剖面配置”。
我们对这种指导方针集合的目标是其内聚性，它们一起可以有助于我们达成某个特定的目标，如“消除范围错误”
或“静态类型安全性”。
每个剖面配置都被设计用于消除一个类别的错误。
而“随意”实施一些独立的规则，相对于提供确定的改善来说，更像是对代码库的破坏。

“剖面配置”是确定的并且可移植实施的规则（限制）子集，它们是专门设计以达成某项特定保证的。
“确定性”意味着它们仅需要进行局部分析，并且可以在一台计算机中进行实现（虽然并不比如此）。
“可移植实施性”表明它们和语言规则相似，因而程序员们可以期望不同实施工具对于相同的代码给出相同的答案。

编写成在这样的语言剖面配置下仍免于警告的代码，可以认为是遵循这个剖面配置的。
而遵从的代码则可以认为对于该剖面配置的目标安全属性来说是安全的。
遵从的代码不会成为这种性质的错误的根本原因，
虽然程序中可能从其他的代码引入这样的错误。
剖面配置还可能会引入一些额外的库类型，以简化遵从性并鼓励编写正确的代码。

剖面配置概览：

- [Pro.type: 类型安全性](#SS-type)
- [Pro.bounds: 边界安全性](#SS-bounds)
- [Pro.lifetime: 生存期安全性](#SS-lifetime)

未来，我们打算定义更多的剖面配置，并向现有剖面配置中添加更多的检查。
候选者有：

- 窄化算术提升和转换（可能会成为一个单独的安全算术剖面配置的一部分）
- 从负浮点数向无符号整型类型进行算术强制转换（同上）
- 经选择的未定义行为：从 Gabriel Dos Reis 为 WG21 研究小组开发的 UB 列表入手
- 经选择的未指明行为：处理可移植性问题。
- `const` 违反：大多数情况已经由编译器完成，但我们可以捕捉不适当的强制转换和 `const` 的不当使用。

剖面配置的开启是由实现所定义的；典型情况下，是在分析工具之中进行的设置。

要抑制对某个剖面配置检查，可以在语言构造上放一个 `suppress` 标注。例如：

    [[suppress(bounds)]] char* raw_find(char* p, int n, char x)    // 在 p[0]..p[n - 1] 中寻找 x
    {
        // ...
    }

这样 `raw_find()` 就可以在内存中到处爬了。
显然，进行抑制应当是非常罕见的。

## <a name="SS-type"></a>Pro.safety: 类型安全性剖面配置

这个剖面配置将能够简化正确使用类型的代码编写，并避免因疏忽产生类型双关。
它是关注于移除各种主要的类型违例的因素（包括对强制转换和联合的不安全使用）而达成这点的。

针对本部分的目的而言，
类型安全性被定义为这样的性质：对变量的使用不会不遵守其所定义的类型的规则。
通过类型 `T` 所访问的内存，不应该是某个实际上包含了无关类型 `U` 的对象的有效内存。
注意，当和[边界安全性](#SS-bounds)、[生存期安全性](#SS-lifetime)组合起来时，安全性才是完整的。

这个剖面配置的实现应当在源代码中识别出下列模式，将之作为不符合并给出诊断信息。

类型安全性剖面配置概览：

- <a name="Pro-type-avoidcasts"></a>Type.1: [避免强制转换](#Res-casts)：

  1. <a name="Pro-type-reinterpretcast"></a>请勿使用 `reinterpret_cast`；此为[避免强制转换](#Res-casts)和[优先使用具名的强制转换](#Res-casts-named)的严格的版本。
  2. <a name="Pro-type-arithmeticcast"></a>请勿在算术类型上使用 `static_cast`；此为[避免强制转换](#Res-casts)和[优先使用具名的强制转换](#Res-casts-named)的严格的版本。
  3. <a name="Pro-type-identitycast"></a>当源指针类型和目标类型相同时，请勿进行指针强制转换；此为[避免强制转换](#Res-casts)的严格的版本。
  4. <a name="Pro-type-implicitpointercast"></a>当指针转换可以隐式转换时，请勿使用指针强制转换；此为[避免强制转换](#Res-casts)的严格的版本。

- <a name="Pro-type-downcast"></a>Type.2: 请勿使用 `static_cast` 进行向下强制转换：
  [代之以使用 `dynamic_cast`](#Rh-dynamic_cast)。
- <a name="Pro-type-constcast"></a>Type.3: 请勿使用 `const_cast` 强制掉 `const`（亦即不要这样做）：
  [不要强制掉 `const`](#Res-casts-const)。
- <a name="Pro-type-cstylecast"></a>Type.4: 请勿使用 C 风格的强制转换 `(T)expression` 和函数式风格强制转换 `T(expression)`：
  优先使用[构造语法](#Res-construct)，[具名的强制转换](#Res-casts-named)，或 `T{expression}`。
- <a name="Pro-type-init"></a>Type.5: 请勿在初始化之前使用变量：
  [坚持进行初始化](#Res-always)。
- <a name="Pro-type-memberinit"></a>Type.6: 坚持初始化成员变量：
  [坚持进行初始化](#Res-always)，
  可以采用[默认构造函数](#Rc-default0)或者
  [默认成员初始化式](#Rc-in-class-initializer)。
- <a name="Pro-type-union"></a>Type.7: 避免裸 union：
  [代之以使用 `variant`](#Ru-naked)。
- <a name="Pro-type-varargs"></a>Type.8: 避免 varargs：
  [不要使用 `va_arg` 参数](#F-varargs)。

##### 影响

在类型安全性剖面配置下，你可以相信每个操作都将在有效的对象上进行。
可能抛出异常以报告无法（在编译时）被静态地检测到的错误。
要注意的是，这种类型安全性仅当我们同样具有[边界安全性](#SS-bounds)和[生存期安全性](#SS-lifetime)时才是完整的。
而没有这些保证的话，一个内存区域可能以与其所存储的单个或多个对象，或对象的一部分无关的方式被访问。

## <a name="SS-bounds"></a>Pro.bounds: 边界安全性剖面配置

这个剖面配置将能简化对于在分配的内存块的边界之中进行操作的编码工作。
它是通过关注于移除边界违例的主要根源——即指针算术和数组索引——而做到这点的。
这个剖面配置的核心功能之一就是限制指针只能指向单个对象而不是数组。

我们将边界安全性定义为这样一种性质：程序不通过一个对象来对分配给这个对象的内存范围之外的内存进行访问。
仅当边界安全性与[类型安全性](#SS-type)和[生存期安全性](#SS-lifetime)组合起来时才是完整的，
它们还会包含其他允许发生边界违例的不安全操作。

边界安全性剖面配置概览：

- <a name="Pro-bounds-arithmetic"></a>Bounds.1: 请勿使用指针算术。请使用 `span` 代替：
  [（仅）传递单个对象的指针](#Ri-array)，并[保持指针算术的简单性](#Res-ptr)。
- <a name="Pro-bounds-arrayindex"></a>Bounds.2: 仅使用常量表达式对数组进行索引操作：
  [（仅）传递单个对象的指针](#Ri-array)，并[保持指针算术的简单性](#Res-ptr)。
- <a name="Pro-bounds-decay"></a>Bounds.3: 避免数组向指针的退化：
  [（仅）传递单个对象的指针](#Ri-array)，并[保持指针算术的简单性](#Res-ptr)。
- <a name="Pro-bounds-stdlib"></a>Bounds.4: 请勿使用不进行边界检查的标准库函数和类型：
  [以类型安全的方式使用标准库](#Rsl-bounds)

##### 影响

边界安全性意味着，当访问对象（尤其是数组）时不会越过对象的内存分配范围。
这消除了一大类的隐伏且难于发现的错误，包括（不）著名的“缓冲区溢出”错误。
这避免了安全漏洞，以及（当越界写入时发生）内存损坏错误的大量来源。
即使越界访问“只是读取操作”，它也可能导致不变式的违反（当所访问的不是预期的类型时）
和“神秘的值”。

## <a name="SS-lifetime"></a>Pro.lifetime: 生存期安全性剖面配置

通过已经不指向任何东西的指针进行访问，是错误的一种主要来源，
而且在许多传统的 C 或 C++ 风格的编程中这很难避免。
例如，指针可能未初始化，值为 `nullptr`，指向越过指针范围，或者指向已删除的对象。

[参见此处的设计说明书的当前版本](https://github.com/isocpp/CppCoreGuidelines/blob/master/docs/Lifetime.pdf)

生存期安全性剖面配置概览：

- <a name="Pro-lifetime-invalid-deref"></a>Lifetime.1: 不要解引用无效指针：
  [检测或避免](#Res-deref)。

##### 影响

一旦强制实施了编码风格规则，静态分析，以及程序库支持的组合方案之后，本剖面配置将能

- 消除 C++ 中的恶劣错误的一种主要来源
- 消除潜在安全漏洞的一种主要来源
- 通过消除多余的“偏执”检查而改善性能
- 提升代码正确性的信心
- 通过强制遵循一种关键的 C++ 语言规则而避免未定义的行为

# <a name="S-gsl"></a>GSL: 指导方针支持库

GSL 是一个小型的程序库，其中的设施被设计用于支持本指导方针。
不使用这些设施的话，这些指导方针不得不变得对语言细节过于限制。

核心指导方针支持库是定义在 `gsl` 命名空间中的，其中的名字可能是对标准库和其他著名程序库的名字的别名。通过 `gsl` 命名空间进行的（编译期）间接，使我们可以进行试验，以及对这些支持设施提供局部变体。

GSL 只有头文件，可以在 [GSL: 指导方针支持库](https://github.com/Microsoft/GSL)找到。
支持库中的设施被设计为极为轻量化（零开销），它们相比于传统方案并不会带来任何开销。
当需要时，它们还可以用其他功能“工具化”（比如一些检查）来帮助进行诸如调试等任务。

各指导方针中，除了使用 GSL 中的类型之外，还使用了标准程序库（如 C++17）中的类型。
例如，我们假定有一个 `variant` 类型，但它当前尚未在 GSL 中。
总之，请使用[通过表决进入 C++17 的版本](http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2016/p0088r3.html)。

由于诸如当前 C++ 版本的限制等技术原因，您所使用的程序库中可能不支持下面列出的某些 GSL 类型。
请查阅您的 GSL 文档以获得更多信息。

对于以下的每个 GSL 类型，我们都为该类型给出了不变式。只要用户代码仅使用类型所提供的成员或自由函数（就是说，用户代码不会以违反任何其他指南规则的方式，绕过类型的接口来改动对象的值或位），该不变式均有效。

GSL 组件概览：

- [GSL.view: 视图](#SS-views)
- [GSL.owner](#SS-ownership)
- [GSL.assert: 断言](#SS-assertions)
- [GSL.util: 工具](#SS-utilities)
- [GSL.concept: 概念](#SS-gsl-concepts)

我们计划提供一个“ISO C++ 标准风格的”半正式的 GSL 规范。

我们依赖于 ISO C++ 标准库，并希望 GSL 的一些部分能够被吸收到标准库之中。

## <a name="SS-views"></a>GSL.view: 视图

这些类型使用户可以区分带有和没有所有权的指针，并区分指向单个对象的指针和指向序列的第一个元素的指针。

“视图”都不是所有者。

引用都不是所有者（参见 [R.4](#Rr-ref)）。注意：有许多机会能让引用存活超过其所指代的对象，如按引用返回局部变量，持有 vector 的某个元素的引用然后进行 `push_back`，绑定到 `std::max(x, y + 1)`，等等。生存期安全性剖面配置的目标就是处理这些事情，但即便如此 `owner<T&>` 也没有意义且不建议使用。

它们的名字基本上遵循 ISO 标准库风格（小写字母和下划线）：

- `T*` // `T*` 不是所有者，可能为 null；假定为指向单个元素。
- `T&` // `T&` 不是所有者，不可能为“null 引用”；引用总是绑定到对象上。

“原生指针”写法（如 `int*`）假定为具有其最常见的含义；亦即指向一个对象的指针，但并不拥有它。
所有者应当被转换为资源包装（如 `unique_ptr` 或 `vector<T>`），或标为 `owner<T*>`。

- `owner<T*>` // `T*`，拥有所指向/指代的对象；可能为 `nullptr`。

`owner` 用于对代码中有所有权的指针进行标记，它们无法更改为使用适当的资源包装。
其原因可能包括：

- 转换的成本。
- 需要为某个 ABI 使用指针。
- 这个指针时某种资源包装的实现的一部分。

`owner<T>` 和 `T` 的某种资源包装的区别在于它仍然需要明确进行 `delete`。

`owner<T>` 假定为指代自由存储（堆）上的某个对象。

当某个东西不应当为 `nullptr` 时，可以这样做：

- `not_null<T>` // `T` 通常是某个指针类型（例如 `not_null<int*>` 和 `not_null<owner<Foo*>>`），且不能为 `nullptr`。
  `T` 可以是 `==nullptr` 有意义的任何类型。

- `span<T>` // `[p:p+n)`，构造函数接受 `{p, q}` 和 `{p, n}`；`T` 为指针类型
- `span_p<T>` // `{p, predicate}` `[p:q)`，其中 `q` 为首个使 `predicate(*p)` 为真的元素

`span<T>` 指代零或更多可改动的 `T`，除非 `T` 为 `const` 类型。对 `span` 元素的所有访问，尤其是通过 `operator[]` 进行的访问，默认保证进行边界检查。

> 注：有提案将 GSL 的 `span`（起初叫做 `array_view`）加入 C++ 标准库且已被采纳（名字和接口有改动），唯一不同是 `std::span` 不提供边界检查保证。因此，GSL 修改了 `span` 的名字和接口以跟踪 `std::span`，并应当与 `std::span` 完全相同，而其仅有差别应当为，GSL 的 `span` 默认是完全边界安全的。如果边界检查影响其接口，那么应当通过 ISO C++ 委员会带回改动的提案，以保持 `gsl::span` 和 `std::span` 接口兼容。如果 `std::span` 未来的演化添加了边界检查，则 `gsl::span` 即可移除。

“指针算术”最好在 `span` 之内进行。
指向多个 `char` 但并非 C 风格字符串的 `char*`（比如指向某个输入缓冲区的指针）应当表示为一个 `span`。

- `zstring` // `char*`，假定为 C 风格字符串；亦即以零结尾的 `char` 的序列或者是 `nullptr`
- `czstring` // `const char*`，假定为 C 风格字符串；亦即以零结尾的 `const` `char` 的序列或者是 `nullptr`

逻辑上来说，最后两种别名是没有必要的，但我们并不总是依照逻辑的，它们可以在指向单个 `char` 的指针和指向 C 风格字符串的指针之间明确地进行区分。
并未假定为零结尾的字符序列应当是 `span<char>`，或当因 ABI 问题而不可能时是 `char*`，而不是 `zstring`。

对于不能为 `nullptr` 的 C 风格字符串，应使用 `not_null<zstring>`。 ??? 我们需要为 `not_null<zstring>` 命名吗？还是说它的难看是有用的？

## <a name="SS-ownership"></a>GSL.owner: 所有权指针

- `unique_ptr<T>` // 唯一所有权：`std::unique_ptr<T>`
- `shared_ptr<T>` // 共享所有权：`std::shared_ptr<T>`（引用计数指针）
- `stack_array<T>` // 栈分配数组。元素的数量在构造时确定并固定下来。其元素可改变，除非 `T` 为 `const` 类型。
- `dyn_array<T>` // ??? 有必要吗 ??? 堆分配数组。元素的数量在构造时确定并固定下来。
  其元素可改变，除非 `T` 为 `const` 类型。基本上这是一个进行分配并拥有其元素的 `span`。

## <a name="SS-assertions"></a>GSL.assert: 断言

- `Expects` // 前条件断言。当前放置于函数体内。今后应当移动到声明中。
  // `Expects(p)` 当不满足 `p == true` 时会终止程序
  // `Expects` 处于一组选项的控制之下（强制，错误消息，对终止程序的替代）
- `Ensures` // 后条件断言。当前放置于函数体内。今后应当移动到声明中。

现在这些断言还是宏（天呐！）而且必须（只）被用在函数定义式之内。
等待标准委员会对于契约和断言语法的确定。
参见使用属性语法的[契约提案](http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2016/p0380r1.pdf)，
比如说，`Expects(p)` 将变为 `[[expects: p]]`。

## <a name="SS-utilities"></a>GSL.util: 工具

- `finally` // `finally(f)` 创建一个 `final_action{f}`，其析构函数将执行 `f`
- `narrow_cast` // `narrow_cast<T>(x)` 就是 `static_cast<T>(x)`
- `narrow` // `narrow<T>(x)` 在满足无符号提升下的 `static_cast<T>(x) == x` 时为 `static_cast<T>(x)`，否则抛出 `narrowing_error`（例如，`narrow<unsigned>(-42)` 会抛出异常）
- `[[implicit]]` // 放在单参数构造函数上的“记号”，以明确说明它们并非显式构造函数。
- `move_owner` // `p = move_owner(q)` 含义为 `p = q` 但 ???
- `joining_thread` // RAII 风格版本的进行联结的 `std::thread`
- `index` // 用于进行所有的容器和数组索引的类型（当前是 `ptrdiff_t` 的别名）

## <a name="SS-gsl-concepts"></a>GSL.concept: 概念

这些概念（类型谓词）借用于
Andrew Sutton 的 Origin 程序库，
Range 提案，
以及 ISO WG21 的 Palo Alto TR。
其中许多都与已在 C++20 中成为 ISO C++ 标准的概念十分相似。

- `String`
- `Number`
- `Boolean`
- `Range` // C++20 中为 `std::ranges::range`
- `Sortable` // C++20 中为 `std::sortable`
- `EqualityComparable` // C++20 中为 `std::equality_comparable`
- `Convertible` // C++20 中为 `std::convertible_to`
- `Common` // C++20 中为 `std::common_with`
- `Integral` // C++20 中为 `std::integral`
- `SignedIntegral` // C++20 中为 `std::signed_integral`
- `SemiRegular` // C++20 中为 `std::semiregular`
- `Regular` // C++20 中为 `std::regular`
- `TotallyOrdered` // C++20 中为 `std::totally_ordered`
- `Function` // C++20 中为 `std::invocable`
- `RegularFunction` // C++20 中为 `std::regular_invocable`
- `Predicate` // C++20 中为 `std::predicate`
- `Relation` // C++20 中为 `std::relation`
- ...

### <a name="SS-gsl-smartptrconcepts"></a>GSL.ptr: 智能指针概念

- `Pointer` // 带有 `*`，`->`，`==`，以及默认构造的类型（默认构造被假定为设值为唯一的“null”值）
- `Unique_pointer` // 符合 `Pointer` 的类型，可移动但不可复制
- `Shared_pointer` // 符合 `Pointer` 的类型，可复制

# <a name="S-naming"></a>NL: 命名和代码布局建议

维持一致的命名和代码布局是很有用的。
即便不为其他原因，也可以减少“我的代码风格比你的好”这类的纷争。
然而，人们使用许多许多的不同代码风格，并狂热地坚持它们（的优缺点）。
而且，大多数的现实项目都包含来自于许多来源的代码，因而通常不可能把所有的代码都标准化为某个单一的代码风格。
经过许多的用户请求给予指导后，我们给出一组规则，当你没有更好的选择时可以使用它们，但真正的目标在于一致性，而不是任何一组特定的规则。
IDE 和工具可以提供辅助（当然也可能造成妨碍）。

命名和代码布局规则：

- [NL.1: 不要在代码注释中说明可以由代码来清晰表达的东西](#Rl-comments)
- [NL.2: 在代码注释中说明意图](#Rl-comments-intent)
- [NL.3: 保持代码注释简明干脆](#Rl-comments-crisp)
- [NL.4: 保持一种统一的缩进风格](#Rl-indent)
- [NL.5: 避免在名字中编码类型信息](#Rl-name-type)
- [NL.7: 使名字的长度大约正比于其作用域的长度](#Rl-name-length)
- [NL.8: 使用一种统一的命名风格](#Rl-name)
- [NL.9: 将 `ALL_CAPS`（全大写）仅用于宏的名字](#Rl-all-caps)
- [NL.10: 优先采用 `underscore_style`（下划线风格）的名字](#Rl-camel)
- [NL.11: 使字面量可阅读](#Rl-literals)
- [NL.15: 节制地使用空格](#Rl-space)
- [NL.16: 使用一种常规的类成员声明次序](#Rl-order)
- [NL.17: 使用从 K&R 衍生出的代码布局](#Rl-knr)
- [NL.18: 使用 C++ 风格的声明符布局](#Rl-ptr)
- [NL.19: 避免使用容易误读的名字](#Rl-misread)
- [NL.20: 不要把两个语句放在同一行中](#Rl-stmt)
- [NL.21: 每个声明式（仅）声明一个名字](#Rl-dcl)
- [NL.25: 请勿将 `void` 用作参数类型](#Rl-void)
- [NL.26: 采用符合惯例的 `const` 写法](#Rl-const)
- [NL.27: 为代码文件使用后缀 `.cpp`，而对接口文件使用后缀 `.h`](#Rl-file-suffix)

这些问题的大部分都是审美问题，程序员都有很强的个人倾向。
IDE 也都会提供某些默认方案和一组替代方案。
这些规则是作为缺省建议的，如果没有别的理由，请采用它们。

我们收到一些意见称命名和代码布局非常个人化和任意性，我们不应该试图为之“立法”。
我们并不是在“立法”（参见前一个段落）。
不过，我们也收到了大量的针对某些命名和代码布局约定的请求，要求当没有外来限制的时候应当采用它们。

更专门和详细的规则更加易于强制实施。

这些规则恐怕会和 [PPP Style Guide](http://www.stroustrup.com/Programming/PPP-style.pdf) 中的建议有很强的相似性，
它是为支持 Stroustrup 的 [Programming: Principles and Practice using C++](http://www.stroustrup.com/programming.html) 而编制的。

### <a name="Rl-comments"></a>NL.1: 不要在代码注释中说明可以由代码来清晰表达的东西

##### 理由

编译器不会读注释。
注释没有代码那么精确。
注释不会像代码那样进行一致地更新。

##### 示例，不好

    auto x = m * v1 + vv;   // 将 m 乘以 v1 并将其结果加上 vv

##### 强制实施

构建一个 AI 程序来解释口语英文文字，看看它所说的是否可以用 C++ 来更好地表达。

### <a name="Rl-comments-intent"></a>NL.2: 在代码注释中说明意图

##### 理由

代码表示的是做了什么，而不是想要做成什么。通常来说意图比实现能够更清晰简明地进行说明。

##### 示例

    void stable_sort(Sortable& c)
        // 对 c 根据由 < 决定的顺序进行排序，保持相等元素（由 == 定义）的
        // 原始相对顺序
    {
        // ... 相当多的不平常的代码行 ...
    }

##### 注解

如果代码注释和代码有冲突，则它们都可能是错的。

### <a name="Rl-comments-crisp"></a>NL.3: 保持代码注释简明干脆

##### 理由

冗长啰嗦会拖慢理解速度，而且到处散布在代码文件里也会让代码难于阅读。

##### 注解

使用明白易懂的英文。
也许我可以流利使用丹麦语，但大多数程序员不行；我的代码的维护者也不行。
避免使用网络用语，注意你的文法，标点，以及大小写。
目标是专业性，而不是“够酷”。

##### 强制实施

不可能。

### <a name="Rl-indent"></a>NL.4: 保持一种统一的缩进风格

##### 理由

可读性。避免“微妙的错误”。

##### 示例，不好

    int i;
    for (i = 0; i < max; ++i); // 可能出现的 BUG
    if (i == j)
        return i;

##### 注解

总是把 `if (...)`，`for (...)`，以及 `while (...)` 之后的语句进行缩进是一个好主意：

    if (i < 0) error("negative argument");

    if (i < 0)
        error("negative argument");

##### 强制实施

使用一种工具。

### <a name="Rl-name-type"></a>NL.5: 避免在名字中编码类型信息

##### 原理

当名字反映类型而不是功能时，它将变得难于为提供其功能而改变其所使用的类型。
而且，当改变变量的类型时，使用它的代码也得修改。
最小化无意进行的转换。

##### 示例，不好

    void print_int(int i);
    void print_string(const char*);

    print_int(1);          // 重复，人工进行类型匹配
    print_string("xyzzy"); // 重复，人工进行类型匹配

##### 示例，好

    void print(int i);
    void print(string_view);    // 对任意字符串式的序列都能工作

    print(1);              // 简洁，自动类型匹配
    print("xyzzy");        // 简洁，自动类型匹配

##### 注解

带有类型编码的名字要么啰嗦要么难懂。

    printS  // 打印一个 std::string
    prints  // 打印一个 C 风格字符串
    printi  // 打印一个 int

在无类型语言中曾经采用过像匈牙利记法这样的技巧来在名字中编码类型，但在像 C++ 这样的强静态类型语言中，这通常是不必要而且实际上是有害的，因为这些标注会过时（这些累赘和注释类似，而且和它们一样会烂掉），而且它们干扰了语言的恰当用法（应当代之以使用相同的名字和重载决议）。

##### 注解

一些代码风格会使用非常一般性的（而不是特定于类型的）前缀来代表变量的一般用法。

    auto p = new User();
    auto p = make_unique<User>();
    // 注："p" 并非是说“User 类型的原始指针”，
    //     而只是一般性的“这是一次间接访问”

    auto cntHits = calc_total_of_hits(/*...*/);
    // 注："cnt" 并非用于编码某个类型，
    //     而只是一般性的“这是某种东西的一个计数”

这样做是没有害处的，且并不属于本条指导方针，因为其并未编码类型信息。

##### 注解

一些代码风格会对成员和局部变量，以及全局变量之间进行区分。

    struct S {
        int m_;
        S(int m) : m_{abs(m)} { }
    };

这样做是没有害处的，且并不属于本条指导方针，因为其并未编码类型信息。

##### 注解

像 C++ 这样，一些代码风格对类型和非类型之间进行区分。
例如，对类型名字首字母大写，而函数和变量名字则不这样做。

    typename<typename T>
    class HashTable {   // 将 string 映射为 T
        // ...
    };

    HashTable<int> index;

这样做是没有害处的，且并不属于本条指导方针，因为其并未编码类型信息。

### <a name="Rl-name-length"></a>NL.7: 使名字的长度大约正比于其作用域的长度

**原理**: 作用域越大，搞混的机会和意外的名字冲突的机会就越大。

##### 示例

    double sqrt(double x);   // 返回 x 的平方根；x 必须是非负数

    int length(const char* p);  // 返回零结尾的 C 风格字符串的字符数量

    int length_of_string(const char zero_terminated_array_of_char[])    // 不好: 啰嗦

    int g;      // 不好: 全局变量具有密秘的名字

    int open;   // 不好: 全局变量使用短小且常用的名字

为指针使用 `p`，以及为浮点变量使用 `x` 是符合惯例的，在受限的作用域中不会造成混乱。

##### 强制实施

???

### <a name="Rl-name"></a>NL.8: 使用一种统一的命名风格

**原理**: 命名和命名风格的一致性会提高可读性。

##### 注解

命名风格有好多，当你使用多个程序库时，你无法遵循所有它们不同的命名约定。
应当选用一种“自有风格”，但保持“导入”的程序为其原有风格不变。

##### 示例

ISO 标准仅使用小写字母和数字，并用下划线进行词的连接。

- `int`
- `vector`
- `my_map`

避免使用双下划线 `__`。

##### 示例

[Stroustrup](http://www.stroustrup.com/Programming/PPP-style.pdf)：
采用 ISO 标准，但在自己的类型和概念上采用大写字母：

- `int`
- `vector`
- `My_map`

##### 示例

CamelCase：多词标识符的每个词首字母大写：

- `int`
- `vector`
- `MyMap`
- `myMap`

一些命名约定会将首字母大写，而另一些不会。

##### 注解

应当试图在对缩略词的使用和标识符长度上保持一致风格。

    int mtbf {12};
    int mean_time_between_failures {12}; // 你自己决定

##### 强制实施

除使用具有不同命名约定的程序库之外应当是可能做到的。

### <a name="Rl-all-caps"></a>NL.9: 将 `ALL_CAPS`（全大写）仅用于宏的名字

##### 理由

避免在宏和遵循作用域和类型规则的名字之间造成混乱。

##### 示例

    void f()
    {
        const int SIZE{1000};  // 不好，应代之以 'size'
        int v[SIZE];
    }

##### 注解

这条规则适用于非宏的符号常量：

    enum bad { BAD, WORSE, HORRIBLE }; // 不好

##### 强制实施

- 对带有小写字母的宏进行标记
- 对 `ALL_CAPS` 非宏名字进行标记

### <a name="Rl-camel"></a>NL.10: 优先采用 `underscore_style`（下划线风格）的名字

##### 理由

用下划线来分隔名字的各部分就是 C 和 C++ 的原始风格，并被用于 C++ 标准库中。

##### 注解

这条规则仅作为当你有选择权时的缺省方案。
通常你是没有什么选择权的，而只能遵循某个已经设立的风格以维持[一致性](#Rl-name)。
对一致性的需要优先于个人喜好。

这个推荐适用于[当你没有约束条件或者没有更好的想法时](#S-naming)的情况。
经过很多要求给予指导后，添加这个规则。

##### 示例

[Stroustrup](http://www.stroustrup.com/Programming/PPP-style.pdf)：
采用 ISO 标准，但在自己的类型和概念上采用大写字母：

- `int`
- `vector`
- `My_map`

##### 强制实施

不可能。

### <a name="Rl-literals"></a>NL.11: 使字面量可阅读

##### 理由

可读性。

##### 示例

用数字分隔符来避免长串的数字

    auto c = 299'792'458; // m/s2
    auto q2 = 0b0000'1111'0000'0000;
    auto ss_number = 123'456'7890;

##### 示例

需要清晰性时使用字面量后缀

    auto hello = "Hello!"s; // std::string
    auto world = "world";   // C 风格字符串
    auto interval = 100ms;  // 使用 <chrono>

##### 注解

不能在代码中到处当做[“魔法常量”](#Res-magic)一样乱用字面量，
但当定义它们时使它们更可读仍是个好主意。
在较长的整数串中很容易出现拼写错误。

##### 强制实施

标记长数字串。麻烦的是“长”的定义；也许应当是 7。

### <a name="Rl-space"></a>NL.15: 节制地使用空格

##### 理由

太多的空格会让文本更大更分散。

##### 示例，不好

    #include < map >

    int main(int argc, char * argv [ ])
    {
        // ...
    }

##### 示例

    #include <map>

    int main(int argc, char* argv[])
    {
        // ...
    }

##### 注解

一些 IDE 有其自己的看法，并会添加分散的空格。

这个推荐适用于[当你没有约束条件或者没有更好的想法时](#S-naming)的情况。
经过很多要求给予指导后，添加这个规则。

##### 注解

我们将恰当放置的空白评价为能够明显有助于可读性。但请勿过度。

### <a name="Rl-order"></a>NL.16: 使用一种常规的类成员声明次序

##### 理由

一种常规的成员次序会提高可读性。

以如下次序声明类

- 类型：类，枚举，别名（`using`）
- 构造函数，赋值，析构函数
- 函数
- 数据

采用先是 `public`，然后是 `protected`，之后是 `private` 的次序。

这个推荐适用于[当你没有约束条件或者没有更好的想法时](#S-naming)的情况。
经过很多要求给予指导后，添加这个规则。

##### 示例

    class X {
    public:
        // 接口
    protected:
        // 供派生类实现使用的不带检查的函数
    private:
        // 实现细节
    };

##### 示例

有时候，成员的默认顺序，与将公开接口从实现细节中分离出来的需求之间有冲突。
这种情况下，私有类型和函数可以和私有数据放在一起。

    class X {
    public:
        // 接口
    protected:
        // 供派生类实现使用的不带检查的函数
    private:
        // 实现细节（类型，函数和数据）
    };

##### 示例，不好

避免让具有某一种访问（如 `public`）的多个声明块被具有不同访问（如 `private`）的其他声明块分隔开。

    class X {
    public:
        void f();
    public:
        int g();
        // ...
    };

用宏来声明成员组的做法通常会导致违反所有的次序规则。
不过，宏的使用掩盖了其所表达的东西。

##### 强制实施

对背离上述建议次序的代码进行标记。将会有大量的老代码不符合这条规则。

### <a name="Rl-knr"></a>NL.17: 使用从 K&R 衍生出的代码布局

##### 理由

这正是 C 和 C++ 的原始代码布局。它很好地保持了纵向空间。它对不同语言构造（如函数和类）进行了很好的区分。

##### 注解

在 C++ 的语境中，这种风格通常被称为“Stroustrup”。

这个推荐适用于[当你没有约束条件或者没有更好的想法时](#S-naming)的情况。
经过很多要求给予指导后，添加这个规则。

##### 示例

    struct Cable {
        int x;
        // ...
    };

    double foo(int x)
    {
        if (0 < x) {
            // ...
        }

        switch (x) {
        case 0:
            // ...
            break;
        case amazing:
            // ...
            break;
        default:
            // ...
            break;
        }

        if (0 < x)
            ++x;

        if (x < 0)
            something();
        else
            something_else();

        return some_value;
    }

注意 `if` 和 `(` 之间有一个空格

##### 注解

每个语句，`if` 的分支，以及 `for` 的代码体都使用单独的代码行。

##### 注解

`class` 和 `struct` 的 `{` *并不*在单独的代码行上，但函数的 `{` 在单独的代码行上。

##### 注解

对你自定义的类型的名字进行首字母大写，以将其与标准库类型相区分。

##### 注解

不要对函数名大写。

##### 强制实施

如果想要强制实施的话，请使用某个 IDE 进行格式化。

### <a name="Rl-ptr"></a>NL.18: 使用 C++ 风格的声明符布局

##### 理由

C 风格的布局强调其在表达式中的用法和文法，而 C++ 风格强调的是类型。
对表达式用法的说辞并不适用于引用。

##### 示例

    T& operator[](size_t);   // OK
    T &operator[](size_t);   // 奇怪
    T & operator[](size_t);   // 不确定

##### 注解

这个推荐适用于[当你没有约束条件或者没有更好的想法时](#S-naming)的情况。
经过很多要求给予指导后，添加这个规则。

##### 强制实施

由于历史原因而不可能。

### <a name="Rl-misread"></a>NL.19: 避免使用容易误读的名字

##### 理由

可读性。
并非每个人都有能将字符轻易区分开的屏幕和打印机。
我们很容易搞混拼写相似和略微拼错的单词。

##### 示例

    int oO01lL = 6; // 不好

    int splunk = 7;
    int splonk = 8; // 不好：splunk 和 splonk 很容易搞混

##### 强制实施

???

### <a name="Rl-stmt"></a>NL.20: 不要把两个语句放在同一行中

##### 理由

可读性。
当一行里有多个语句时，相当容易忽视某个语句。

##### 示例

    int x = 7; char* p = 29;    // 请勿如此
    int x = 7; f(x);  ++x;      // 请勿如此

##### 强制实施

容易。

### <a name="Rl-dcl"></a>NL.21: 每个声明式（仅）声明一个名字

##### 理由

可读性。
最小化声明符语法造成的混乱。

##### 注解

相关细节，参见 [ES.10](#Res-name-one)、

### <a name="Rl-void"></a>NL.25: 请勿将 `void` 用作参数类型

##### 理由

这很啰嗦，而且仅在考虑 C 兼容性是才有必要。

##### 示例

    void f(void);   // 不好

    void g();       // 好多了

##### 注解

即便是 Dennis Ritchie 自己都认为 `void f(void)` 很讨厌。
你可以反驳称，在 C 中当函数原型很少见时禁止这样的代码：

    int f();
    f(1, 2, "weird but valid C89");   // 希望 f() 被定义为 int f(a, b, c) char* c; { /* ... */ }

可能造成很大的问题，但这并不适于 21 世纪和 C++。

### <a name="Rl-const"></a>NL.26: 采用符合惯例的 `const` 写法

##### 理由

更多程序员更加熟悉惯例写法。
大型代码库中的一致性。

##### 示例

    const int x = 7;    // OK
    int const y = 9;    // 不好

    const int *const p = nullptr;   // OK, 指向常量 int 的常量指针
    int const *const p = nullptr;   // 不好，指向常量 int 的常量指针

##### 注解

我们知道你可能会说“不好”的例子比标有“OK”的更符合逻辑，
但它们会让更多人搞混，尤其是那些依赖于采用了远为常用，符合惯例的 OK 风格的教学材料的新手们。

一如往常，请记住这些命名和代码布局规则的目标在于一致性，而审美则会有广泛的变化。

这个推荐适用于[当你没有约束条件或者没有更好的想法时](#S-naming)的情况。
经过很多要求给予指导后，添加这个规则。

##### 强制实施

标记用作类型的后缀的 `const`。

### <a name="Rl-file-suffix"></a>NL.27: 为代码文件使用后缀 `.cpp`，而对接口文件使用后缀 `.h`

##### 理由

这是一条历史悠久的约定。
不过一致性更加重要，因此如果你的项目用了别的约定的话，应当遵守它。

##### 注解

这项约定反应了一种常见使用模式：
头文件更容易和 C 语言共享使用，并可以作为 C++ 和 C 编译，它们通常用 `.h` 后缀，
并且对于有意要和 C 共用的头文件来说，让所有头文件都使用 `.h` 而不是别的扩展名要更加容易。
另一方面，实现文件则很少会和 C 共用，通常应当和 `.c` 文件相区别，
因此一般最好为所有的 C++ 实现文件用别的扩展名（如 `.cpp`）来命名。

特定的名字 `.h` 和 `.cpp` 并不是必要的（只是作为缺省建议），其他的名字也被广泛采用。
例子包括 `.hh`，`.C`，和 `.cxx` 等。请以类似方式使用这些名字。
本文档中我们把 `.h` 和 `.cpp` 作为头文件和实现文件的简便提法，
虽然实际上的扩展名可能是不同的。

也许你的 IDE（如果你使用的话）对后缀有较强的倾向。

##### 示例

    // foo.h:
    extern int a;   // 声明
    extern void foo();

    // foo.cpp:
    int a;   // 定义
    void foo() { ++a; }

`foo.h` 提供了 `foo.cpp` 的接口。最好避免全局变量。

##### 示例，不好

    // foo.h:
    int a;   // 定义
    void foo() { ++a; }

一个程序中两次 `#include <foo.h>` 将导致因为对唯一定义规则的两次违反而出现一个连接错误。

##### 强制实施

- 对不符合约定的文件名进行标记。
- 检查 `.h` 和 `.cpp`（或等价文件）遵循下列各规则。

# <a name="S-faq"></a>FAQ: 常见问题及其回答

本节中包括了对关于这些指导方针的常见问题的回答。

### <a name="Faq-aims"></a>FAQ.1: 这些指导方针的想要达成什么目标？

请参见<a href="#S-abstract">本页面开头</a>。这是一个开源项目，旨在为采用当今的 C++ 标准来编写 C++ 代码而维护的一组现代的权威指导方针。这些指导方针的设计是现代的，尽可能使机器可实施的，并且是为贡献和分支保持开放，以使各种组织机构可以便于将它们整合到其自己组织的编码指导方针之中。

### <a name="Faq-announced"></a>FAQ.2: 这项工作是何时何地首次公开的？

是在 [Bjarne Stroustrup 在他为 CppCon 2015 的开场主旨演讲，“Writing Good C++14”](https://isocpp.org/blog/2015/09/stroustrup-cppcon15-keynote)。另请参见[相应的 isocpp.org 博客条目](https://isocpp.org/blog/2015/09/bjarne-stroustrup-announces-cpp-core-guidelines)，关于类型和内存安全性指导方针的原理请参见 [Herb Sutter 的后续 CppCon 2015 演讲，“Writing Good C++14 ... By Default”](https://isocpp.org/blog/2015/09/sutter-cppcon15-day2plenary)。

### <a name="Faq-maintainers"></a>FAQ.3: 谁是这些指导方针的作者和维护者？

最初的主要作者和维护者是 Bjarne Stroustrup 和 Herb Sutter，而迄今为止的指导方针则是由来自 CERN，Microsoft，Morgan Stanley，以及许多其他组织机构的专家所贡献的。指导方针发布时，其正处于 "0.6" 状态，我们欢迎人们进行贡献。正如 Stroustrup 在其声明中所说：“我们需要帮助！”

### <a name="Faq-contribute"></a>FAQ.4: 我如何进行贡献呢？

参见 [CONTRIBUTING.md](https://github.com/isocpp/CppCoreGuidelines/blob/master/CONTRIBUTING.md)。我们感激志愿者的帮助！

### <a name="Faq-maintainer"></a>FAQ.5: 怎样成为一名编辑或维护者？

通过先进行大量贡献并使你的贡献被认可具有一致的质量。参见 [CONTRIBUTING.md](https://github.com/isocpp/CppCoreGuidelines/blob/master/CONTRIBUTING.md)。我们感激志愿者的帮助！

### <a name="Faq-iso"></a>FAQ.6: 这些指导方针被 ISO C++ 标准委员会采纳了吗？它们是否代表委员会的一致意见？

不是这样。这些指导方针不在标准之内。它们是为标准服务的，而当前维护的指导方针是为了更有效地使用当前的标准 C++的。我们的目标是使其与委员会所设计的标准保持同步。

### <a name="Faq-isocpp"></a>FAQ.7: 既然这些指导方针并不是委员会所采纳的，它们为何在 `github.com/isocpp` 之下呢？

因为 `isocpp` 是标准 C++ 基金会；而标准委员会的仓库则处于 [github.com/_cplusplus_](https://github.com/cplusplus) 之下。我们需要一个中立组织来持有版权和许可以明确其并不是由某个人或供应商所控制的。这个自然实体就是基金会，其设立是为了推进使用并持续更新对现代标准 C++ 的理解，以及推进标准委员会的工作。其所遵循的正是与 isocpp.org 为 [C++ FAQ](https://isocpp.org/faq) 所做的相同模式，它是有 Bjarne Stroustrup，Marshall Cline，和 Herb Sutter 所发起的工作，并以相同的方式贡献为了开放项目。

### <a name="Faq-cpp98"></a>FAQ.8: 会有 C++98 版本的指导方针吗？C++11 版本呢？

不会。这些指导方针的目标是更好地使用现代标准 C++，以及假定你有一个现代的遵循标准的编译器时如何进行代码编写的。

### <a name="Faq-language-extensions"></a>FAQ.9: 这些指导方针中会提出新的语言功能吗？

不会。这些指导方针的目标是更好地使用现代标准 C++，它们自我限定为仅建议使用这些功能。

### <a name="Faq-markdown"></a>FAQ.10: 这些指导方针的书写使用的是哪个版本的 Markdown？

这些编码指导方针使用的是 [CommonMark](http://commonmark.org)，以及 `<a>` HTML 锚定元素。

我们正在考虑以下这些来自 [GitHub Flavored Markdown (GFM)](https://help.github.com/articles/github-flavored-markdown/) 的扩展：

- 有围栏代码块（正在讨论是否统一使用缩进还是围栏代码块）
- 表格（我们虽然还没用到，但很需要它们，这是一种 GFM 扩展）

避免使用其他 HTML 标签和其他扩展。

注意：我们还没对这种风格达成一致。

### <a name="Faq-gsl"></a>FAQ.50: 什么是 GSL（指导方针支持程序库）？

GSL 是在指导方针中所指定的类型和别名的一个小集合。当写下本文时，对它们的说明还过于松散；我们计划添加一个 WG21 风格的接口规范来确保不同实现之间保持一致，并作为一项可能的标准化提案，按常规遵循标准委员会进行采纳、改进、修订或否决。

### <a name="Faq-msgsl"></a>FAQ.51: [github.com/Microsoft/GSL](https://github.com/Microsoft/GSL) 是 GSL 吗？

不是。它只是由 Microsoft 所贡献的第一个实现。我们鼓励其他供应商提供其他的实现，对该实现的分支和贡献也是被鼓励的。书写本文作为一项公开项目的一周中，已经出现了至少一个 GPLv3 的开源实现。我们计划制定一个 WG21 风格的接口规范来确保不同实现之间保持一致。

### <a name="Faq-gsl-implementation"></a>FAQ.52: 为何不在指导方针之中提供一个真正的 GSL 实现呢？

我们不愿去保佑某个特定的实现，因为我们不希望让人们以为只有一个实现，而疏忽大意地扼杀了其他并行的实现。而如果在指导方针中包含一个真正实现的话，无论是谁提供了它都会变得过于有影响力。我们更倾向于采用委员会的更具长期性的方案，即指定其接口而不是实现。但同时我们也需要至少存在一个实现；希望可以有很多。

### <a name="Faq-boost"></a>FAQ.53: 为什么不把 GSL 类型提交给 Boost 呢？

因为我们想要立刻使用它们，也因为我们想要在一旦标准库中出现了满足其需要的类型时立刻将它们撤销掉。

### <a name="Faq-gsl-iso"></a>FAQ.54: ISO C++ 标准委员会采纳了 GSL（指导方针支持程序库）吗？

没有。GSL 的存在只为提供少量标准库中还没有的类型和别名。如果委员会决定了（这些类型或者满足其需要的其他类型的）标准化的版本，就可以将它们从 GSL 中删除了。

### <a name="Faq-gsl-string-view"></a>FAQ.55: 既然你是尽可能使用标准类型，为什么 GSL 的 `span<char>` 同 Library Fundamentals 1 Technical Specification 和 C++17 工作文本中的 `string_view` 不同呢？为什么不使用委员会采纳的 `string_view`？

有关 C++ 标准库的视图的分类的统一观点是，“视图（view）”意味着“只读”，而“跨距（span）”意味着“可读写”。如果你只需要一组字符的不需要保证边界检查的只读视图，并且你可以用 C++17，那就使用 C++17 的 `std::string_view`。否则，如果你需要的是不需要保证边界检查的可读写视图，并且可以用 C++20，那就用 C++20 的 `std::span<char>`。否则，就用 `gsl::span<char>`。

### <a name="Faq-gsl-owner"></a>FAQ.56: `owner` 和提案的 `observer_ptr` 一样吗？

不一样。`owner` 有所有权，它是一个别名，而且适用于任何间接类型。而 `observer_ptr` 的主要意图则是明确某个*没有*所有权的指针。

### <a name="Faq-gsl-stack-array"></a>FAQ.57: `stack_array` 和标准的 `array` 一样吗？

不一样。`stack_array` 保证在栈上分配。虽然 `std::array` 直接在其自身内部包含存储，但 `array` 对象可以放在包括堆在内的任何地方。

### <a name="Faq-gsl-dyn-array"></a>FAQ.58: `dyn_array` 和 `vector` 或者提案的 `dynarray` 一样吗？

不一样。`dyn_array` 是不可改变大小的，是一种指代堆分配的固定大小数组的一种安全方式。与 `vector` 不同，它是为了取代数组 `new[]` 的。与委员会中提案的 `dynarray` 不同，它并不会参与编译器和语言的魔法，来在当它作为分配于栈上的对象的成员时也在栈上分配；它只不过指代一个“动态的”或基于堆的数组而已。

### <a name="Faq-gsl-expects"></a>FAQ.59: `Expects` 和 `assert` 一样吗？

不一样。它是一种对于契约前条件语言支持的占位符。

### <a name="Faq-gsl-ensures"></a>FAQ.60: `Ensures` 和 `assert` 一样吗？

不一样。它是一种对于契约后条件语言支持的占位符。

# <a name="S-libraries"></a>附录 A: 程序库

这个部分列出了一些推荐的程序库，并且特别推荐了其中的几个。

??? 这个对一般性指南来说合适吗？我觉得不是 ???

# <a name="S-modernizing"></a>附录 B: 代码的现代化转换

理想情况下，我们的所有代码都应当遵循全部的规则。
而实际情况则是，我们不得不对付大量的老代码：

- 那些在我们的指导方针被建立或者被了解到之前所编写的代码
- 那些依据老的或者不同的标准所编写的程序库
- 那些在“不寻常”的约束下编写的代码
- 那些我们还未来得及使其现代化的代码

如果有上百万行的新代码的话，“立刻改掉它们”的想法一般都是不现实的。
因此，我们需要一种方式能够渐进地对代码库进行现代化转换。

将老代码升级为现代风格可能是很让人却步的工作。
老代码通常即混乱（难于理解）又可以（在当前使用范围内）正确工作。
很有可能，原来的程序员并不在场，而且测试用例也不完整。
代码的混乱性显著地增大了为进行任何改动所需要的工作量和引入错误的风险。
通常，混乱的老代码的运行会有不必要的缓慢，因为它需要过期的编译器，并且无法得益于当代硬件的改进。
许多情况下，都需要某种自动进行“现代化转换”的工具支持来进行主要的升级工作。

对代码现代化转换的目的在于简化新功能的添加，简化维护工作，以及增加性能（吞吐量或延迟），和更好地利用当代的硬件能力。
而让代码“看起来更好”或“遵循现代风格”自身并不能成为改动的理由。
每一种改动都蕴含着风险，而是由过时的代码库则会蕴含一些成本（包含丢失机会带来的成本）。
成本的缩减必须超过风险。

如何做呢？

并不存在唯一的代码现代化转换的方案。
如何最好地执行，依赖于代码，更新的进度压力，开发者的背景，以及可用的工具。
下面是一些（非常一般的）想法：

- 理想情况是“对全部代码一起进行升级”。这将在最短的总时间内获得做大的好处。
  在大多数情况下，这也是不可能的。
- 我们可以对代码库以模块为单位进行转换，不过任何影响接口（尤其是 ABI）的规则，如 [使用 `span`](#SS-views)，都无法按模块来达成。
- 我们可以“自底向上”转换代码，并最先应用我们估计在给定的代码库上将会带来最大好处和最少麻烦的那些规则。
- 我们可以从关注接口开始，比如说，保证没有资源的泄漏，没有指针误用等。
  这可能会导致涉及整个代码库的一些改动，不过它们是最可能会带来巨大好处的改动。
  以后，隐藏在这些接口后面的代码可以渐进地进行现代化转换而不会影响其他的代码。

无论你选择哪种方式，都要注意，对指导方针的最高度的遵循性才会带来大多数的好处。
这些指导方针并不是一组无关规则的随机集合，并不能让你随意选取并期望取得成功。

我们衷心希望听到有关它们的使用经验，以及有关工具是如何使用的。
如果有分析工具（即便是代码变换工具）的支持的话，代码现代化转换后可以变得更快，更简单，而且更安全。

# <a name="S-discussion"></a>附录 C: 讨论

这个部分包含了对规则和规则集合的跟进材料。
尤其是，我们列出了更多的原理说明，更长的例子，以及对替代方案的探讨等。

### <a name="Sd-order"></a>讨论: 以成员的声明顺序进行成员变量的定义和初始化

成员变量总是以它们在类定义中的声明顺序进行初始化，因此在构造函数初始化列表中应当以该顺序来书写它们。以别的顺序书写它们只会让代码混淆，因为它并不会以你所见到的顺序来运行，而这会导致难于发现与顺序有关的 BUG。

    class Employee {
        string email, first, last;
    public:
        Employee(const char* firstName, const char* lastName);
        // ...
    };

    Employee::Employee(const char* firstName, const char* lastName)
      : first(firstName),
        last(lastName),
        // 不好: first 和 last 还未构造
        email(first + "." + last + "@acme.com")
    {}

在这个例子中，`email` 比 `first` 和 `last` 构造得早，因为它是先声明的。这意味着其构造函数对 `first` 和 `last` 的使用过早了——不只在它们被设为所需的值之前，而完全是在它们被构造之前就使用了。

如果类定义和构造函数体是在不同文件中的话，这种由成员变量声明顺序对构造函数的正确性造成的远距离影响将更难于发现。

**参考**：

[\[Cline99\]](#Cline99) §22.03-11, [\[Dewhurst03\]](#Dewhurst03) §52-53, [\[Koenig97\]](#Koenig97) §4, [\[Lakos96\]](#Lakos96) §10.3.5, [\[Meyers97\]](#Meyers97) §13, [\[Murray93\]](#Murray93) §2.1.3, [\[Sutter00\]](#Sutter00) §47

### <a name="Sd-init"></a>讨论：使用 `=`，`{}`，和 `()` 作为初始化式

???

### <a name="Sd-factory"></a>讨论: 当需要在初始化过程中使用“虚函数行为”时，使用工厂函数

如果你的设计需要从基类的构造函数或析构函数中对 `f` 或者 `g` 这样的函数向派生类进行虚函数派发的话，你其实需要的是其他技巧，比如后构造函数——一种必须由调用者调用以完成初始化过程的成员函数，它可以安全地调用 `f` 和 `g`，这是由于成员函数中的虚函数调用能够正常工作。“参考”部分中列出了一些这样的技巧。以下是一个不完整的可选项列表：

- _推卸责任：_ 仅仅给出文档说明，要求用户代码在对象构造之后必须立刻调用后初始化函数。
- _惰性后初始化：_ 在第一个调用的成员函数中进行。用基类中的一个布尔标记说明后初始化是否已经执行过。
- _使用虚基类语义：_ 语言规则要求由最终派生类的构造函数来决定调用哪个基类构造函数；你可以利用这点。（参见[\[Taligent94\]](#Taligent94)。）
- _使用工厂函数：_ 以这种方式，你可以轻易确保进行对后构造函数的调用。

以下是对最后一种选项的一个例子：

    class B {
    public:
        B()
        {
            /* ... */
            f(); // 不好: C.82：不要在构造函数和析构函数中调用虚函数
            /* ... */
        }

        virtual void f() = 0;
    };

    class B {
    protected:
        class Token {};

    public:
        // 需要公开构造函数以使 make_shared 可以访问它。
        // 通过要求一个 Token 达成受保护访问等级。
        explicit B(Token) { /* ... */ }  // 创建不完全初始化的对象
        virtual void f() = 0;

        template<class T>
        static shared_ptr<T> create()    // 创建共享对象的接口
        {
            auto p = make_shared<T>(typename T::Token{});
            p->post_initialize();
            return p;
        }

    protected:
        virtual void post_initialize()   // 构造之后立即调用
            { /* ... */ f(); /* ... */ } // 好: 虚函数分派是安全的
        }
    };


    class D : public B {                 // 某个派生类
    protected:
        class Token {};

    public:
        // 需要公开构造函数以使 make_shared 可以访问它。
        // 通过要求一个 Token 达成受保护访问等级。
        explicit D(Token) : B( B::Token{} ) {}
        void f() override { /* ... */ };

    protected:
        template<class T>
        friend shared_ptr<T> B::create();
    };

    shared_ptr<D> p = D::Create<D>();    // 创建一个 D 对象

这种设计需要遵守以下纪律：

- 像 `D` 这样的派生类不能暴露可公开调用的构造函数。否则的话，`D` 的使用者就能够创建 `D` 对象而不调用 `post_initialize` 了。
- 分配被限定为使用 `operator new`。不过，`B` 可以覆盖 `new`（参见 [SuttAlex05](#SuttAlex05) 条款 45 和 46）。
- `D` 必须定义一个带有与 `B` 所选择的相同的参数的构造函数。不过，定义多个重载的 `create` 可以缓和这个问题；而且还可以是这些重载对参数类型进行模板化。

一旦满足了上述要求，这个设计就可以保证对于任意完全构造的 `B` 的派生类对象，都将调用 `post_initialize`。`post_initialize` 不必是虚函数；它可以随意进行虚函数调用。

总之，不存在完美的后构造技巧。最差的方式是完全回避问题而只是让调用方来人工调用后构造函数。即便是最佳方案也需要采用一种不同的对象构造语法（易于进行编译期检查）以及需要派生类的作者的协作（这无法进行编译期进行检查）。

**参考**: [\[Alexandrescu01\]](#Alexandrescu01) §3, [\[Boost\]](#Boost), [\[Dewhurst03\]](#Dewhurst03) §75, [\[Meyers97\]](#Meyers97) §46, [\[Stroustrup00\]](#Stroustrup00) §15.4.3, [\[Taligent94\]](#Taligent94)

### <a name="Sd-dtor"></a>讨论: 基类的析构函数应当要么是 public 和 virtual，要么是 protected 且非 virtual

析构应不应该表现为虚函数？就是说，是否允许通过指向 `base` 类的指针来进行析构呢？如果是的话，`base` 的析构函数为被调用则必须是 public 的，而且必须 virtual，否则调用就会导致未定义行为。否则的话，它应当是 protected 的，这样就只有派生类可以在它们自己的析构函数中调用它，且应当是非 virtual 的，因为它并不需要表现为虚函数的行为。

##### 示例

基类的一般情况是为了具有 public 的派生类，因而调用方代码基本上可以确定要用到某种比如 `shared_ptr<base>` 这样的东西：

    class Base {
    public:
        ~Base();                   // 不好, 非 virtual
        virtual ~Base();           // 好
        // ...
    };

    class Derived : public Base { /* ... */ };

    {
        unique_ptr<Base> pb = make_unique<Derived>();
        // ...
    } // 只有当 ~Base 是虚函数时 ~pb 才会调用正确的析构函数

少数比如策略类这类的情况下，类被用作基类是为方便起见，而并非是其多态行为。建议将它们的析构函数作为 protected 和非 virtual 函数：

    class My_policy {
    public:
        virtual ~My_policy();      // 不好, public 并且 virtual
    protected:
        ~My_policy();              // 好
        // ...
    };

    template<class Policy>
    class customizable : Policy { /* ... */ }; // 注: private 继承

##### 注解

这个简单的指导方针演示了一种微妙的问题，而且反映了继承的现代用法以及面向对象的设计原则。

对于某个基类 `Base`，调用方代码可能通过指向 `Base` 的指针来销毁派生类对象，比如使用一个 `unique_ptr<Base>`。如果 `Base` 的析构函数是 public 的且非 virtual（默认情况），它就可能意外地在实际指向一个派生类对象的指针上进行调用，这种情况下想要进行的删除的行为是未定义的。这种状况曾导致一些老编码标准提出通用的要求，让所有基类析构函数都必须 virtual。这种做法杀伤力过大了（虽然这是常见的情况）；其实，规则应当是当且仅当基类析构函数是 public 时才要求它是 virtual 的。

编写一个基类就是在定义一种抽象（参见条款 35 到 37）。注意对于参与这个抽象的每个成员函数来说，你都需要作出以下决定：

- 它是否应当表现为虚函数。
- 它是应当对所有使用 `Base` 指针的调用方公开，还是作为隐藏的内部实现细节。

如条款 39 中所述，对于普通成员函数来说，其选择可以是：允许通过 `Base` 指针对其进行非虚调用（但当它调用了虚函数时可具有虚行为，比如在 NVI 或者模板方法模式中那样），进行虚调用，或者完全不能调用。NVI 模式是一种避免公开虚函数的技巧。

析构可以仅仅被看做是另一种操作，虽然它带有特殊的语义，并且非虚调用要么很危险要么是错误的。因而，对于基类析构函数来说，其选择有：允许通过 `Base` 指针进行虚函数调用，或者完全不能调用；“非虚调用”是不行的。这样的话，基类析构函数当其可以被调用（即为 public）时应当是 virtual 的，否则就为非 virtual。

注意 NVI 模式并不适用于析构函数，因为构造函数和析构函数无法进行深析构调用。（参见条款 39 和 55。）

推论：当编写基类时，一定要明确编写析构函数，因为隐式生成的析构函数是 public 和非 virtual 的。当预置函数体没问题是你当然可以用 `=default`，而仅仅为其指定正确的可见性和虚函数性质即可。

##### 例外

某些组件体系架构（如 COM 和 CORBA）并不适用标准的删除机制，而是为对象的处置设立了不同的方案。请遵循相应的模式和惯用法，并在适当时采纳本条指导方针。

再考虑一下这种罕见情况：

- `B` 既是一个基类，也是可以被实例化的具体类，因而其析构函数必须为 public 以便 `B` 的对象可以创建和销毁。
- 而 `B` 也没有虚函数，且并不打算按多态方式使用，因此虽然其析构函数是 public 它也不必是 virtual 的。

这样的话，虽然析构函数必须为 public，也会有很强的压力来阻止它变为 virtual，因为作为第一个 virtual 函数，若其所添加的功能永远不会被利用的话，它就会损害所有的运行时类型开销。

在这种罕见情况下，可以是析构函数 public 且非 virtual，但要明确说明其派生类对象绝不能当作 `B` 来多态地使用。我们对 `std::unary_function` 正是这样做的。

不过，一般来说应当避免具体的基类（参见条款 35）。例如，`unary_function` 不过是聚合了一组 typedef，它不可能会被有意单独实例化。给它提供 public 的析构函数完全没有任何意义；更好的设计应当是遵循本条款的建议来给它一个 protected 非虚析构函数猜到。

**参考**: [\[SuttAlex05\]](#SuttAlex05) Item 50, [\[Cargill92\]](#Cargill92) pp. 77-79, 207? [\[Cline99\]](#Cline99) §21.06, 21.12-13? [\[Henricson97\]](#Henricson97) pp. 110-114? [\[Koenig97\]](#Koenig97) Chapters 4, 11? [\[Meyers97\]](#Meyers97) §14? [\[Stroustrup00\]](#Stroustrup00) §12.4.2? [\[Sutter02\]](#Sutter02) §27? [\[Sutter04\]](#Sutter04) §18

### <a name="Sd-noexcept"></a>讨论: noexcept 的用法

???

### <a name="Sd-never-fail"></a>讨论: 虚构函数，回收函数和 swap 不允许失败

绝不能允许从虚构函数，资源回收函数（如 `operator delete`），或者 `swap` 函数中用 `throw` 来报告错误。如果这些操作可以失败的话，就几乎不可能编写有用的代码了，而且即便真的发生了某种错误，也几乎不可能有进行重试的任何意义。特别是，C++ 标准库是直截了当地禁止使用可能在析构函数中抛出异常的类型的。现在，大多数析构函数缺省就隐含带有 `noexcept` 了。

##### 示例

    class Nefarious {
    public:
        Nefarious() { /* 可能抛出异常的代码 */ }    // 好
        ~Nefarious() { /* 可能抛出异常的代码 */ }   // 不好, 可能抛出异常
        // ...
    };

1.  `Nefarious` 对象很难安全地使用，即便是作为局部变量也是如此：

        void test(string& s)
        {
            Nefarious n;          // 要有麻烦了
            string copy = s;      // 复制 string
        } // 先后销毁 copy 和 n

    这里，对 `s` 的复制可能抛出异常，且当其抛出了异常而 `n` 的析构函数也抛出了异常时，程序就会因调用 `std::terminate` 而退出，因为无法同时传播两个异常。

2.  以 `Nefarious` 为成员或者基类的类同样很难安全地使用，因为它们的析构函数必须调用 `Nefarious` 的析构函数，且同样遭受其糟糕行为的毒害：

        class Innocent_bystander {
            Nefarious member;     // 噢，毒害了外围类的析构函数
            // ...
        };

        void test(string& s)
        {
            Innocent_bystander i;  // 要有更多麻烦了
            string copy2 = s;      // 复制 string
        } // 依次销毁 copy 和 i

    这里，当 `copy2` 的构造中抛出了异常时，我们会遇到同样的问题，因为 `i` 的析构函数现在也会抛出异常，且因此会使我们调用 `std::terminate`。

3.  你也无法可靠地创建全局或静态的 `Nefarious` 对象：

        static Nefarious n;       // 噢，无法捕获任何析构函数异常

4.  你无法可靠地创建 `Nefarious` 的数组：

        void test()
        {
            std::array<Nefarious, 10> arr; // 这行代码会导致 std::terminate()
        }

    当出现可能抛出异常的析构函数时，数组的行为是未定义的，因为根本不可能发明出合理的回退行为。请想象一下：编译器如何才能生成用来构造 `arr` 的代码，如果第四个对象的构造函数抛出了异常，这段代码必须放弃，且在其清理模式中将试图调用已经构造完成的每个对象的析构函数……而这些析构函数中的一个或更多会抛出异常呢？不存在令人满意的答案。

5.  你无法在标准容器中使用 `Nefarious`：

        std::vector<Nefarious> vec(10);   // 这行代码会导致 std::terminate()

    标准库禁止其所使用的任何析构函数抛出异常。你无法把 `Nefarious` 对象存储到标准容器中，或者在标准库的任何其他组件上使用它们。

##### 注解

它们是绝不能失败的关键函数，因为在事务性编程中需要它们提供两种关键操作：当处理过程中遇到问题时撤回工作，以及当未发生问题时提交工作。如果没有办法可以用无失败操作来安全地撤回的话，就不可能实现无失败的回滚操作。如果没有办法可以用无失败操作（显然 `swap` 可以，但并不仅限于它）来安全地提交状态的改变的话，就不可能实现无失败的提交操作。

请考虑以下在 C++ 标准中所找到的建议和要求：

> 当在栈回溯过程中所调用的析构函数因为异常而退出时，将调用 terminate (15.5.1)。因此析构函数通常应当捕获异常，并防止它们被传播出析构函数。 --[\[C++03\]](#Cplusplus03) §15.2(3)
>
> C++ 标准库中所定义的任何析构函数（也包括用于实例化标准库模板的任何类型的析构函数）的操作都不会抛出异常。 --[\[C++03\]](#Cplusplus03) §17.4.4.8(3)

包括专门重载的 `operator delete` 和 `operator delete[]` 在内的回收函数也属于这一类别，因为一般它们也被用在清理过程，尤其是在异常处理过程中，用以对部分完成的工作进行撤回。
除了析构函数和回收函数之外，一般的错误安全性技术也依赖于永不失败的 `swap` 操作——这种情况下，它们不仅用于实现确保成功的回滚操作，也用于实现确保成功的提交操作。例如，以下是对类型 `T` 的一种惯用的 `operator=` 实现，它在复制构造之后，调用了无失败的 `swap`：

    T& T::operator=(const T& other)
    {
        auto temp = other;
        swap(temp);
        return *this;
    }

(另见条款 56。 ???)

幸运的是，当进行资源释放时，发生故障的范围肯定会比较小。如果使用异常作为错误报告机制的话，请确保这样的函数会处理其内部的处理中可能会产生的所有异常和其他错误。（对于异常，可以直接把你的析构函数中的所有相关部分都包围到一个 `try/catch(...)` 块中。）这点非常重要，因为析构函数可能会在某种紧要关头被调用，比如当无法分配某种系统资源（如内存、文件、锁、端口、窗口，或者其他系统对象）的时候。

当使用异常作为错误处理机制的时候，请始终明示这种行为，将这些函数声明为 `noexcept`。（参见条款 75。）

**参考**: [\[SuttAlex05\]](#SuttAlex05) Item 51; [\[C++03\]](#Cplusplus03) §15.2(3), §17.4.4.8(3)? [\[Meyers96\]](#Meyers96) §11? [\[Stroustrup00\]](#Stroustrup00) §14.4.7, §E.2-4? [\[Sutter00\]](#Sutter00) §8, §16? [\[Sutter02\]](#Sutter02) §18-19

## <a name="Sd-consistent"></a>统一对复制、移动和销毁操作进行定义

##### 理由

???

##### 注解

一旦定义了复制构造函数，就也得定义复制赋值运算符。

##### 注解

一旦定义了移动构造函数，就也得定义移动赋值运算符。

##### 示例

    class X {
    public:
        X(const x&) { /* stuff */ }

        // 不好: 未同时定义复制赋值运算符

        X(x&&) noexcept { /* stuff */ }

        // 不好: 未同时定义移动赋值运算符

        // ...
    };

    X x1;
    X x2 = x1; // ok
    x2 = x1;   // 陷阱：要么不能通过编译，要么会做出不好的事

一旦定义了析构函数，就不能再使用编译器所生成的复制或移动操作了；你可能需要定义或者抑制掉移动或复制操作。

    class X {
        HANDLE hnd;
        // ...
    public:
        ~X() { /* 自定义的行为，比如关闭 hnd */ }
        // 可疑: 未提到过复制或移动操作——hnd 会怎么样？
    };

    X x1;
    X x2 = x1; // 陷阱：要么不能通过编译，要么会做出不好的事
    x2 = x1;   // 陷阱：要么不能通过编译，要么会做出不好的事

如果定义了复制操作，且有任何基类或成员的诶性定义了移动操作的话，应当同样定义移动操作。

    class X {
        string s; // 定义了更高效的移动操作
        // ... 其他数据成员 ...
    public:
        X(const X&) { /* stuff */ }
        X& operator=(const X&) { /* stuff */ }

        //    不好: 并未一同定义移动构造函数和移动赋值
        //   （为何不把那些自定义的“stuff”重复一下呢？）
    };

    X test()
    {
        X local;
        // ...
        return local;  // 陷阱：可能会低效甚或产生错误的行为
    }

一旦定义了复制构造函数，复制赋值运算符，或者析构函数中额任何一个，你就可能需要也定义其他的。

##### 注解

如果需要定义这五个函数中的任何一个，这就意味着你需要得到与其预置行为不同的行为——而这五者则是非对称相关的。如下所述：

- 当编写或禁用复制构造函数或复制赋值运算符之一时，很可能需要对另一个同样对待：若其中之一有“特别的”任务，则很可能另一个也应当如此，因为这两个函数应当具有相似的效果。（参见条款 53，其中对这点进行专门的展开说明。）
- 当明确编写复制函数时，很可能需要编写析构函数：若复制构造函数所做的“特别的”任务为分配或复制某中资源（诸如内存、文件、socket 等），则需要在析构函数中对其进行回收。
- 当明确编写析构函数时，很可能需要明确编写或禁用复制操作：若不得不编写一个不平凡的析构函数的话，这通常是由于你需要人工释放对象所持有的某个资源。若是如此的话，很可能需要特别小心这些资源的复制，而你就需要关注对象进行复制和赋值的方式，或者完全禁止复制操作。

许多情况下，持有以 RAII 的“拥有者”对象恰当封装了的资源，是能够吧自己编写这些操作的需要消除掉的。（参见条款 13。）

应当优先采用编译器生成（包括 `=default`）的特殊成员；只有它们才被归类为“平凡的”，而且至少有一家主要的标准库供应商针对带有平凡特殊成员的类进行了大量地优化。这可能会成为一种常规实践。

**例外**: 当特殊函数的声明仅为了使其非公开或者为虚函数而没有特殊的语义时，它并不导致需要其他的特殊成员。
少数情况下，带有奇怪类型的成员（诸如引用成员）的类也是例外，因为它们的复制语义很古怪。
在持有引用的类中，你可能需要编写复制构造函数和赋值运算符，但预置的析构函数仍能够做出正确的处理。（需要注意，基本上使用引用成员几乎总是错误的。）

**参考**: [\[SuttAlex05\]](#SuttAlex05) Item 52; [\[Cline99\]](#Cline99) §30.01-14? [\[Koenig97\]](#Koenig97) §4? [\[Stroustrup00\]](#Stroustrup00) §5.5, §10.4? [\[SuttHysl04b\]](#SuttHysl04b)

资源管理规则概览：

- [提供强资源安全性；亦即，绝不让你认为是资源的任何东西发生泄漏](#Cr-safety)
- [绝不在持有未被句柄所拥有的资源时返回或抛出异常](#Cr-never)
- [“原生”的指针或引用不可能是资源句柄](#Cr-raw)
- [绝不让指针的生存期超过其所指向的对象](#Cr-outlive)
- [用模板来表现容器（和其他资源句柄）](#Cr-templates)
- [按值返回容器（依靠移动或复制消除来获得性能）](#Cr-value-return)
- [若类为资源句柄，则它需要构造函数，析构函数，复制以及移动操作](#Cr-handle)
- [若类为容器，则应为其提供一个初始化式列表构造函数](#Cr-list)

### <a name="Cr-safety"></a>讨论：提供强资源安全性；亦即，绝不让你认为是资源的任何东西发生泄漏

##### 理由

避免泄漏。泄漏会导致性能损耗，发生神秘的错误，系统崩溃，以及安全性的违犯。

**其他形式**: 使所有资源都表示为某种可以自我管理生存期的类的对象。

##### 示例

    template<class T>
    class Vector {
    private:
        T* elem;   // 自由存储中的 sz 个元素，由类对象所拥有
        int sz;
        // ...
    };

这个类是一个资源句柄。它管理各个 `T` 对象的生存期。为此，`Vector` 必然要对[一组特殊操作](???)（几个构造函数，析构函数，等等）进行定义或弃置。

##### 示例

    ??? “奇异的”非内存资源 ???

##### 强制实施

防止泄漏的基本技巧是让所有的资源都被某种带有回档析构函数的资源句柄所拥有。检查工具能够查找出“裸 `new`”。给定一组 C 风格的分配函数（如 `fopen()`），检查工具也能够查找出未被资源句柄管理的使用点。一般来说，可以带着怀疑看待“裸指针”，对其进行标记和分析。如果没有人为输入的话，时无法产生资源的完整列表的（“资源”的定义有些过于宽泛），不过可以用一个资源列表来对工具进行“参数化”。

### <a name="Cr-never"></a>讨论：绝不在持有未被句柄所拥有的资源时返回或抛出异常

##### 理由

这会导致泄漏。

##### 示例

    void f(int i)
    {
        FILE* f = fopen("a file", "r");
        ifstream is { "another file" };
        // ...
        if (i == 0) return;
        // ...
        fclose(f);
    }

当 `i == 0` 时 `a file` 的文件句柄就会泄漏。另一方面，`another file` 的 `ifstream` 则将会（在销毁时）正确关闭它的文件。如果你必须显式使用指针而不是带有特定语义的资源句柄的话，可以使用带有自定义删除器的 `unique_ptr` 或 `shared_ptr`：

    void f(int i)
    {
        unique_ptr<FILE, int(*)(FILE*)> f(fopen("a file", "r"), fclose);
        // ...
        if (i == 0) return;
        // ...
    }

这样更好：

    void f(int i)
    {
        ifstream input {"a file"};
        // ...
        if (i == 0) return;
        // ...
    }

##### 强制实施

检查器必须将任何“裸指针”当作可疑处理。
检查器可能必须依赖于人工提供的资源列表进行工作。
上手时，我们知道标准库容器，`string`，以及智能指针。
`span` 和 `string_view` 的使用能够提供巨大的帮助（它们并非资源句柄）。

### <a name="Cr-raw"></a>讨论：“原生”的指针或引用不可能是资源句柄

##### 理由

使得能够区分所有者和视图。

##### 注解

这和你如何“拼写”指针是两回事：`T*`，`T&`，`Ptr<T>` 和 `Range<T>` 都不是所有者。

### <a name="Cr-outlive"></a>讨论：绝不让指针的生存期超过其所指向的对象

##### 理由

避免极难找到的错误。这种指针的解引用时未定义行为，能够导致发生对类型系统的违犯。

##### 示例

    string* bad()   // 确实很坏
    {
        vector<string> v = { "This", "will", "cause", "trouble", "!" };
        // 导致指向已经销毁的对象（v）的已经销毁的成员的一个指针被泄漏出去
        return &v[0];
    }

    void use()
    {
        string* p = bad();
        vector<int> xx = {7, 8, 9};
        // 未定义行为: x 可能不是字符串 "This"
        string x = *p;
        // 未定义行为: 我们不知道在位置 p 上分配的到底是什么（如果有的话）
        *p = "Evil!";
    }

`v` 中的各个 `string` 都在 `bad()` 退出之时被销毁了， `v` 自身也是如此。其所返回的指针指向自由存储上的未分配内存。（由 `p` 所指向的）这块内存，在执行 `*p` 之时可能已经被重新分配了。此时很可能并不存在可以读取的 `string` 对象，而通过 `p` 进行写入则会轻易损坏某些无关类型的对象。

##### 强制实施

大多数编译器已经能对简单情况进行警告，而且它们带有可以更进一步的信息。将函数所返回的任何指针都当作是可疑的。用容器、资源句柄和视图（例如 `span`，它不是资源句柄）来减少需要检查的情形。上手时，可将带有析构函数的类都当作是资源句柄处理。

### <a name="Cr-templates"></a>讨论：用模板来表现容器（和其他资源句柄）

##### 理由

提供静态类型安全的元素操作。

##### 示例

    template<typename T> class Vector {
        // ...
        T* elem;   // 指向 sz 个 T 类型的元素
        int sz;
    };

### <a name="Cr-value-return"></a>讨论：按值返回容器（依靠移动或复制消除来获得性能）

##### 理由

简化代码并消除一种进行显式内存管理的需要。将对象递交给外围作用域，由此扩展其生存期。

**参见**：[F.20，有关“输出（Out）”值的一般条款](#Rf-out)

##### 示例

    vector<int> get_large_vector()
    {
        return ...;
    }

    auto v = get_large_vector(); //  按值返回没有问题，大多数现代编译器都会进行复制消除

##### 例外

见 [F.20](#Rf-out) 中的例外。

##### 强制实施

检查函数所返回额指针和引用，看看它们是否被赋值给资源句柄（如 `unique_ptr`）。

### <a name="Cr-handle"></a>讨论：若类为资源句柄，则它需要构造函数，析构函数，复制以及移动操作

##### 理由

以提供对资源的生存期的完全控制。以提供一组协调的对资源的操作。

##### 示例

    ??? 折腾指针

##### 注解

若所有的成员都为资源句柄，则尽可能要依赖预置的特殊操作。

    template<typename T> struct Named {
        string name;
        T value;
    };

现在 `Named` 带有一个默认构造函数，一个析构函数，以及高效的复制和移动操作，只要 `T` 也提供了它们。

##### 强制实施

一般来说，工具是无法知道类是否是资源句柄的。不过，如果类带有某种[默认操作](#SS-ctor)的话, 它就得拥有全部，而如果类中有成员为资源句柄的话，它也应被当做是资源句柄。

### <a name="Cr-list"></a>讨论：若类为容器，则应为其提供一个初始化式列表构造函数

##### 理由

提供一组初始元素是一种常见情形。

##### 示例

    template<typename T> class Vector {
    public:
        Vector(std::initializer_list<T>);
        // ...
    };

    Vector<string> vs { "Nygaard", "Ritchie" };

##### 强制实施

类怎么算作是容器呢？ ???

# <a name="S-tools"></a>附录 D: 支持工具

这个部分列出了直接支持采用 C++ 核心指导方针的一些工具。这个列表并非要穷尽那些有助于编写良好的 C++ 代码的工具。
如果一个工具被专门设计以支持并关联到 C++ 核心指导方针，那它就是包括进来的候选者。

### <a name="St-clangtidy"></a>工具: [Clang-tidy](http://clang.llvm.org/extra/clang-tidy/checks/list.html)

Clang-tidy 有一组专门用于强制实施 C++ 核心指导方针的规则。这些规则的命名模式为 `cppcoreguidelines-*`。

### <a name="St-cppcorecheck"></a>工具: [CppCoreCheck](https://docs.microsoft.com/en-us/visualstudio/code-quality/using-the-cpp-core-guidelines-checkers)

微软编译器的 C++ 代码分析中包含一组专门用于强制实施 C++ 核心指导方针的规则。

# <a name="S-glossary"></a>词汇表

这是在指导方针中用到的一些术语的相对非正式的定义
（基于 [Programming: Principles and Practice using C++](http://www.stroustrup.com/programming.html) 中的词汇表）。

有关 C++ 的许多主题的更多信息，可以在[标准 C++ 基金会的网站](https://isocpp.org) 找到。

- _ABI_: 应用二进制接口，对于特定硬件平台与操作系统的组合的一种规范。与 API 相对。
- _抽象类（abstract class）_: 不能直接用于创建对象的类；通常用于为派生类定义接口。
  当类带有纯虚函数或只有受保护的构造函数时，它就是抽象的。
- _抽象（abstraction）_: 对事物的描述，有选择并有意忽略（隐藏）了细节（如实现细节）；选择性忽略。
- _地址（address）_: 用以在计算机的内存中找到某个对象的值。
- _算法（algorithm）_: 用以解决某个问题的过程或公式；有限的一系列计算步骤以产生一个结果。
- _别名（alias）_: 指代某个对象的替代方式；通常为名字，指针，或者引用。
- _API_: 应用编程接口，一组构成不同软件之间之间的交互的函数。与 ABI 相对。
- _应用程序（application）_: 程序或程序的集合，用户将其看作一个实体。
- _近似（approximation）_: 事物（比如值或者设计），接近于完美的或者理想的（值或设计）。
  通常近似都是在理想情形中进行各种权衡的结果。
- _参数/实参（argument）_: 传递给函数或模板的值，其中以形参来进行访问。
- _数组（array）_: 同质元素序列，通常是数值，例如 `[0:max)`。
- _断言（assertion）_: 插入到程序中的语句，以声称（断言）在程序的这个位置某事物必定为真。
- _基类（base class）_: 目的是为从其进行派生的类型（比如它带有非 `final` 的虚函数），且有意仅间接使用该类型的对象（如通过指针）。\[严格地说，“基类”可被定义为“从之进行派生的类型”，但我们这里以类设计者意图的角度来给出定义\] 通常基类带有一个或更多的虚函数。
- _位（bit）_: 计算机中信息的基本单位。一个位的值可以为 0 或 1。
- _bug_: 程序中的错误。
- _字节（byte）_: 大多数计算机中进行寻址的基本单位。通常一个字节有 8 位。
- _类（class）_: 一种用户定义的类型，可以包含数据成员，函数成员，以及成员类型。
- _代码（code）_: 程序或程序的部分；有歧义地同时用于源代码和目标代码。
- _编译器（compiler）_: 一种将源代码变为目标代码的程序。
- _复杂度（complexity）_: 对某个问题构造解决方案的难度，或解决方案自身的一种难于精确定义的记法或度量。
  有时候复杂度只是（简单地）表示对执行某个算法所需操作的数量的估计。
- _计算（computation）_: 执行一些代码，通常接受一些输入并产生一些输出。
- _概念（concept）_: (1) 提法，想法；(2) 一组要求，通常针对模板参数。
- _具体类（concrete type）_: 并非基类的类型，且有意直接使用该类型的对象（而非仅通过指针/间接），其大小是已知的，通常可以按程序员的意图在任何地方分配（比如静态地在运行栈上分配）。
- _常量（constant）_: （在给定作用域中）不能改变的值；不可变。
- _构造函数（constructor）_: 初始化（“构造”）一个对象的操作。
  通常构造函数会建立起不变式，并且通常会获取对象被使用时所需的资源（并通常将由析构函数所释放）。
- _容器（container）_: 持有一些元素（其他对象）的对象。
- _复制（copy）_: 制造两个对象使其值比较为相等的操作。另见移动。
- _正确性（correctness）_: 如果程序或程序片段符合其说明，则其为正确的。
  不幸的是，说明可能不完整或不一致，或者也可能无法满足用户的合理预期。
  因此为了产生可接受的代码，我们有时候比仅仅遵守形式说明要做更多的事。
- _成本（cost）_: 产生一个程序，或者执行它的耗费（如开发时间，执行时间或空间等）。
  理想情况下，成本应当是复杂度的函数。
- _定制点（customization point）_:
- _数据（data）_: 计算中所用到的值。
- _调试（debugging）_: 寻找并移除程序中的错误的行为；通常远没有测试那样系统化。
- _声明式（declaration）_: 程序中对一个名字及其类型的说明。
- _定义式（definition）_: 实体的声明式，提供了程序使用该实体所需的所有信息。
  简化版定义：分配了内存额声明。
- _派生类（derived class）_: 派生自一个或多个基类的类。
- _设计（design）_: 对软件的某个片段应当如何运作以满足其说明的一个总体描述。
- _析构函数（destructor）_: 当对象销毁（如在作用域结束时）被隐式执行（调用）的操作。它通常进行资源的释放。
- _封装（encapsulation）_: 将某些事物（如实现细节）保护为私有的，不接受未授权的访问。
- _错误（error）_: 对程序行为的合理期望（通常表现为某种需求或者一份用户指南）和程序的实际行为之间的不一致。
- _可执行程序（executable）_: 预备在计算机上运行（执行）的程序。
- _功能蔓延（feature creep）_: 为“预防万一”而向程序添加过量的功能的倾向。
- _文件（file）_: 计算机中的持久信息的容器。
- _浮点数（floating-point number）_: 计算机对实数（如 7.93 和 10.78e–3）的近似。
- _函数（function）_: 命名的代码单元，可以从程序的不同部分执行（调用）；计算的逻辑单元。
- _泛型编程（generic programming）_: 关注于算法的设计和高效实现的一种编程风格。
  泛型算法能够对所有符合其要求的参数类型正确工作。在 C++ 中，泛型编程通常使用模板进行。
- _全局变量（global variable）_: 技术上说，命名空间作用域中的具名对象。
- _句柄（handle）_: 一个类，允许通过一个成员指针或引用来访问另一个对象。另见资源，复制，移动。
- _头文件（header）_: 包含用于在程序的各个部分中共享接口的声明的文件。
- _隐藏（hiding）_: 防止一个信息片段被直接看到或访问的行为。
  例如，嵌套（内部）作用域中的名字会防止外部（外围）作用域中相同的名字被直接使用。
- _理想的（ideal）_: 我们力争达成的事物的完美版本。我们经常不得不进行各种权衡最后获得一个近似。
- _实现（implementation）_: (1) 编写代码并测试的活动；(2) 用以实现一个程序的代码。
- _无限循环（infinite loop）_: 终止条件永不为真的循环。参见重复。
- _无限递归（infinite recursion）_: 无法终止的递归，直到机器耗尽内存无法维持其调用。
  在现实中这种递归不可能是无限的，它会因某种硬件错误而终止。
- _信息隐藏（information hiding）_: 分离接口和实现，以此将用户不感兴趣的实现细节隐藏起来，并提供一种抽象的活动。
- _初始化（initialize）_: 为一个对象给定其第一个（初始）值。
- _输入（input）_: 计算中所使用的值（比如函数参数以及通过键盘所输入的字符）。
- _整数（integer）_: 整数，比如 42 和 –99。
- _接口（interface）_: 一个或一组声明，说明了一个代码片段（比如函数或者类）应当如何进行调用。
- _不变式（invariant）_: 程序中的某些点必然总为真的事物；通常用于描述对象的状态（值的集合），或者循环进入其重复的语句之前的状态。
- _重复（iteration）_: 重复执行代码片段的行为；参见递归。
- _迭代器（iterator）_: 用以标识序列中的一个元素的对象。
- _ISO_: 国际标准化组织。C++ 语言是一项 ISO 标准：ISO/IEC 14882。更多信息请参考 [iso.org](http://iso.org)。
- _程序库（library）_: 类型、函数、类等等的集合，它们实现了一组设施（抽象），预备可能被用作不止一个程序的组成部分。
- _生存期（lifetime）_: 从对象的初始化直到它变为不可用（离开作用域，被删除，或程序终止）的时间。
- _连接器（linker）_: 用以将目标代码文件和程序库合并构成一个可执行程序的程序。
- _字面量（literal）_: 直接指定一个值的写法，比如 12 指定的是整数值“十二”。
- _循环（loop）_: 重复执行的代码片段；在 C++ 中，通常是 `for` 语句或者 `while` 语句。
- _移动（move）_: 将值从一个对象转移到另一个对象，并遗留一个表示“空”的值的操作。另见复制。
- _仅可移动类型（move-only type）_：可以移动但不能复制的具体类型。
- _可变的（mutable）_: 可以改动；不可变、常量和不变量的反义词。
- _对象（object）_: (1) 已经初始化的一块具有已知类型的内存区域，持有该类型的一个值；(2) 一块内存区域。
- _目标代码（object code）_: 编译器的输出，预备作为连接器的输入（连接器以其产生可执行代码）。
- _目标文件（object file）_: 包含目标代码的文件。
- _面向对象编程（object-oriented programming）_: （OOP）一种关注类和类层次的设计和使用的编程风格。
- _操作（operation）_: 能够实施某种活动的事物，比如函数或运算符。
- _输出（output）_: 由计算所产生的值（例如函数的结果，或者在屏幕上写下的一行行字符等）。
- _溢出（overflow）_: 产生无法被其预期目标所存储的值。
- _重载（overload）_: 定义两个函数或运算符，使其具有相同名字但不同的参数（操作数）类型。
- _覆盖（override）_: 在派生类中用声明和基类中的某个虚函数具有相同名字和参数类型的函数，以此使该函数可以通过由基类所定义的接口来进行调用。
- _所有者（owner）_: 负责释放某个资源的对象。
- _范式（paradigm）_: 设计和编程风格的一种多少有些做作的术语；通常会被用于（错误地）暗示有一种范式被其他的都更优秀。
- _形参（parameter）_: 对函数或模板的一个明确输入的声明。当进行调用时，函数可以通过其形参的名字来访问向其所传递的各个实参。
- _指针（pointer）_: (1) 值，用于标识内存中的一个有类型的对象；(2) 持有这种值的变量。
- _后条件（post-condition）_: 当从一个代码片段（如函数或者循环）退出时必须满足的条件。
- _前条件（pre-condition）_: 当进入一个代码片段（如函数或者循环）时必须满足的条件。
- _程序（program）_: 足够完整以便能够在计算机上执行的代码（可能带有关联的数据）。
- _编程（programming）_: 将问题的解决方案表现为代码的工艺。
- _编程语言（programming language）_: 用于表达程序的语言。
- _伪代码（pseudo code）_: 以非正式的写法而非编程语言所编写的对计算的一种描述。
- _纯虚函数（pure virtual function）_: 必须在派生类中予以覆盖的虚函数。
- _RAII_: （“资源获取即初始化，Resource Acquisition Is Initialization”）一种基于作用域进行资源管理的基本技术。
- _范围（range）_: 值的序列，可以以一个开始点和一个结尾点进行描述。例如，`[0:5)` 的意思是值 0，1，2，3，和 4。
- _递归（recursion）_: 函数调用其自身的行为；另见重复。
- _引用（reference）_: (1) 一种值，描述内存中具有类型的值的位置；(2) 持有这种值的变量。
- _正则表达式（regular expression）_: 对字符串的模式的一种表示法。
- _正规_: 可以进行相等性比较的半正规类型（参见 `std::regular` 概念）。进行复制之后，副本对象与原对象比较为相等。正规类型的行为与如 `int` 这样的内建类型相似，且可以用 `==` 进行比较。
  特别是，正规类型的对象可以进行复制，且复制的结果是与原对象比较为相等的一个独立对象。另见*半正规类型*。
- _要求（requirement）_: (1) 对程序或程序的一部分的预期行为的描述；(2) 对函数或模板对其参数所作出的假设的描述。
- _资源（resource）_: 获取而得的并随后必须被释放的事物，比如文件句柄，锁，或者内存。另见句柄，所有者。
- _舍入（rounding）_: 将一个值转换为某个较不精确类型的数学上最接近的值。
- _RTTI_: 运行时类型信息（Run-Time Type Information）。 ???
- _作用域（scope）_: 程序文本（源代码）的区域，在其中可以对一个名字进行涉指。
- _半正规（semiregular）_: 可复制的（也包括可移动的）且可默认构造的具体类型（参见 `std::semiregular` 概念）。复制的结果是一个与原对象具有相同的值的独立类型。半正规类型的行为与像 `int` 这样内建类型大致相似，但可能没有 `==` 运算符。另见*正规类型*。
- _序列（sequence）_: 可以以线性的顺序访问的一组元素。
- _软件（software）_: 代码片段及其关联数据的集合；通常可以和程序互换运用。
- _源代码（source code）_: 由程序员所生产的代码，（原则上）可以被其他程序员阅读。
- _源文件（source file）_: 包含源代码的文件。
- _规范（specification）_: 对代码片段应当做什么的描述。
- _标准（standard）_: 由官方承认的对某事物的定义，比如编程语言。
- _状态（state）_: 一组值。
- _STL_: 标准库中的容器，迭代器，以及算法部分。
- _字符串（string）_: 字符的序列。
- _风格（style）_: 旨在统一语言功能特征的使用的一组编程技巧；有时候以非常限定的方式来仅代表诸如命名和代码展现等的低层次规则。
- _子类型（subtype）_: 派生类型；一个类型具有另一个类型的所有（可能更多）的性质。
- _超类型（supertype）_: 基类型；一个类型具有另一个类型的性质的子集。
- _系统（system）_: (1) 用以在计算机上实施某种任务的一个或一组程序；(2) 对“操作系统”的简称，即计算机的基本执行环境及工具。
- _TS_: [技术规范](https://www.iso.org/deliverables-all.html type=ts)。技术规范所处理的是仍处于技术开发之中的工作，或者是认为这项工作以后可能会被同意采纳为国际标准，但并不会立即处理。技术规范的出版是为了其立即可用，也是为了提供一种获得反馈的方法。其目标是最终能够被转化并重新作为国际标准来出版。
- _模板（template）_: 由一个或多个的类型或（编译时）值进行参数化的类或函数；支持泛型编程的基本 C++ 语言构造。
- _测试（testing）_: 系统化地查找程序中的错误。
- _权衡（trade-off）_: 对多个设计和实现准则进行平衡的结果。
- _截断（truncation）_: 从一个类型转换为另一个无法精确表示被转换的值的类型时发生的信息损失。
- _类型（type）_: 为一个对象定义了一组可能的值和一组操作的事物。
- _未初始化的（uninitialized）_: 对象在初始化之前的（未定义的）状态。
- _单元（unit）_: (1) 为值赋予含义的一种标准度量（例如，距离单位 km）；(2) 较大的整体中的一个可区分的（比如命名的）部分。
- _用例（use case）_: 程序的某个特定（通常简化的）使用，以测试其功能并演示其目的。
- _值（value）_: 根据某个类型所解释的一组内存中的位。
- _值类型（value type）_：一些人用这个术语来表示正规或半正规类型。
- _变量（variable）_: 给定类型的具名对象；除非未初始化否则包含一个值。
- _虚函数（virtual function）_: 可在派生类中进行覆盖的成员函数。
- _字（word）_: 计算机中内存的基本单元，通常是用以持有一个整数的单元。

# <a name="S-unclassified"></a>To-do: 未分类的规则原型

这是我们的未完成列表。
以下各条目最终将成为规则或者规则的一部分。
或者，我们也会决定不需要做出改动并将条目移除。

- 禁止远距离友元关系
- 应不应该处理物理设计（文件里有什么）和大规模设计（程序库，程序库的组合）？
- 命名空间
- 避免在全局作用域中使用 using 指令（但允许如 std 或其他的“基础”命名空间（如 experimental））
- 命名空间应当有什么粒度？是（如 Sutter/Alexandrescu 所定义的）所有被设计为一同工作或者一同发布的类和函数，还是应该更窄或是更宽？
- 应该用内联命名空间吗（比如 `std::literals::*_literals`）？
- 避免隐式转换
- Const 成员函数应当是线程安全的……aka, 但我并不想真的改掉变量，只是在第一次调用它的时候向它赋一个值……argh
- 始终初始化变量，为成员变量使用初始化列表。
- 无论谁编写了接受或返回 `void*` 的公开接口，都应该上火刑。我曾经好多年都以它作为自己的个人喜好来着。 :)
- 尽可能应用 `const`：成员函数，变量，以及 `const_iterators`
- 使用 `auto`
- `(size)` vs. `{initializers}` vs. `{Extent{size}}`
- 不要过度抽象
- 不要沿着调用栈向下传递指针
- 通过函数底部退出
- 应当提供在多态之间进行选择的指导方针吗？是的。经典的（虚函数，引用语义） vs. Sean Parent 风格（值语义，类型擦除，类似 `std::function`） vs. CRTP/静态的？也许还需要 vs. 标签派发？
- 我们的指导方针是否应当在构造函数或析构函数中禁止进行虚函数调用？是的。许多人都禁止了，虽然我觉得这是 C++ 的一大优势 ??? -保留意见（D 走向 Java 之路太让我失望了）。有好的例子吗？
- 在 lambda 方面，在算法调用和其他回调场景中什么因素会影响决定使用 lambda 还是（局部？）类？
- 讨论一下 `std::bind`，Stephen T. Lavavej 对它有太多批评，使我开始觉得它是不是真的会在未来消失掉。应该建议以 lambda 代替它吗？
- 怎么处理泄漏的临时变量？ : `p = (s1 + s2).c_str();`
- 指针和迭代器的失效会导致悬挂指针：

        void bad()
        {
            int* p = new int[700];
            int* q = &p[7];
            delete p;

            vector<int> v(700);
            int* q2 = &v[7];
            v.resize(900);

            // ... 使用 q 和 q2 ...
        }

- LSP
- 私有继承 vs/and 成员
- 避免静态类成员变量（竞争条件，几乎就是全局变量）

- 使用 RAII 锁定保护（`lock_guard`，`unique_lock`，`shared_lock`），绝不直接调用 `mutex.lock` 和 `mutex.unlock`（RAII）
- 优先使用非递归锁（它们通常用作不良情况的变通手段，有开销）
- 联结（join）你的每个线程！（因为如果没被联结或脱离（detach）的话，析构函数会调用 `std::terminate`……有什么好理由来脱离线程吗？） -- ??? 支持库该不该为 `std::thread` 提供一个 RAII 包装呢？
- 当必须同时获取两个或更多的互斥体时，应当使用 `std::lock`（或者别的死锁免除算法？）
- 当使用 `condition_variable` 时，始终用一个互斥体来保护它（在互斥体外面设置原子 bool 的值的做法是错误的！），并对条件变量自身使用同一个互斥体。
- 绝不对 `std::atomic<user-defined-struct>` 使用 `atomic_compare_exchange_strong`（填充位中的区别会造成影响，而在循环中使用 `compare_exchange_weak` 则能够归于稳定的填充位）
- 单独的 `shared_future` 对象不是线程安全的：两个线程不能等待同一个 `shared_future` 对象（它们可以等待指代相同共享状态的 `shared_future` 的副本）
- 单独的 `shared_ptr` 对象不是线程安全的：不同的线程可以调用指代相同共享对象的*不同* `shared_ptr` 的非 `const` 成员函数，但当一个线程访问一个 `shared_ptr` 对象时，另一个线程不能调用相同 `shared_ptr` 对象的非 `const` 成员函数（如果确实需要，考虑代之以 `atomic_shared_ptr`）

- 算术相关规则

# 参考文献

- <a name="Abrahams01"></a>
  \[Abrahams01]: D. Abrahams. [Exception-Safety in Generic Components](http://www.boost.org/community/exception_safety.html).
- <a name="Alexandrescu01"></a>
  \[Alexandrescu01]: A. Alexandrescu. Modern C++ Design (Addison-Wesley, 2001).
- <a name="Cplusplus03"></a>
  \[C++03]: ISO/IEC 14882:2003(E), Programming Languages — C++ (updated ISO and ANSI C++ Standard including the contents of (C++98) plus errata corrections).
- <a name="Cargill92"></a>
  \[Cargill92]: T. Cargill. C++ Programming Style (Addison-Wesley, 1992).
- <a name="Cline99"></a>
  \[Cline99]: M. Cline, G. Lomow, and M. Girou. C++ FAQs (2ndEdition) (Addison-Wesley, 1999).
- <a name="Dewhurst03"></a>
  \[Dewhurst03]: S. Dewhurst. C++ Gotchas (Addison-Wesley, 2003).
- <a name="Henricson97"></a>
  \[Henricson97]: M. Henricson and E. Nyquist. Industrial Strength C++ (Prentice Hall, 1997).
- <a name="Koenig97"></a>
  \[Koenig97]: A. Koenig and B. Moo. Ruminations on C++ (Addison-Wesley, 1997).
- <a name="Lakos96"></a>
  \[Lakos96]: J. Lakos. Large-Scale C++ Software Design (Addison-Wesley, 1996).
- <a name="Meyers96"></a>
  \[Meyers96]: S. Meyers. More Effective C++ (Addison-Wesley, 1996).
- <a name="Meyers97"></a>
  \[Meyers97]: S. Meyers. Effective C++ (2nd Edition) (Addison-Wesley, 1997).
- <a name="Meyers01"></a>
  \[Meyers01]: S. Meyers. Effective STL (Addison-Wesley, 2001).
- <a name="Meyers05"></a>
  \[Meyers05]: S. Meyers. Effective C++ (3rd Edition) (Addison-Wesley, 2005).
- <a name="Meyers15"></a>
  \[Meyers15]: S. Meyers. Effective Modern C++ (O'Reilly, 2015).
- <a name="Murray93"></a>
  \[Murray93]: R. Murray. C++ Strategies and Tactics (Addison-Wesley, 1993).
- <a name="Stroustrup94"></a>
  \[Stroustrup94]: B. Stroustrup. The Design and Evolution of C++ (Addison-Wesley, 1994).
- <a name="Stroustrup00"></a>
  \[Stroustrup00]: B. Stroustrup. The C++ Programming Language (Special 3rdEdition) (Addison-Wesley, 2000).
- <a name="Stroustrup05"></a>
  \[Stroustrup05]: B. Stroustrup. [A rationale for semantically enhanced library languages](http://www.stroustrup.com/SELLrationale.pdf).
- <a name="Stroustrup13"></a>
  \[Stroustrup13]: B. Stroustrup. [The C++ Programming Language (4th Edition)](http://www.stroustrup.com/4th.html). Addison Wesley 2013.
- <a name="Stroustrup14"></a>
  \[Stroustrup14]: B. Stroustrup. [A Tour of C++](http://www.stroustrup.com/Tour.html).
  Addison Wesley 2014.
- <a name="Stroustrup15></a>
  \[Stroustrup15]: B. Stroustrup, Herb Sutter, and G. Dos Reis: [A brief introduction to C++'s model for type- and resource-safety](https://github.com/isocpp/CppCoreGuidelines/blob/master/docs/Introduction%20to%20type%20and%20resource%20safety.pdf).
- <a name="SuttHysl04b"></a>
  \[SuttHysl04b]: H. Sutter and J. Hyslop. [Collecting Shared Objects](https://web.archive.org/web/20120926011837/http://www.drdobbs.com/collecting-shared-objects/184401839) (C/C++ Users Journal, 22(8), August 2004).
- <a name="SuttAlex05"></a>
  \[SuttAlex05]: H. Sutter and A. Alexandrescu. C++ Coding Standards. Addison-Wesley 2005.
- <a name="Sutter00"></a>
  \[Sutter00]: H. Sutter. Exceptional C++ (Addison-Wesley, 2000).
- <a name="Sutter02"></a>
  \[Sutter02]: H. Sutter. More Exceptional C++ (Addison-Wesley, 2002).
- <a name="Sutter04"></a>
  \[Sutter04]: H. Sutter. Exceptional C++ Style (Addison-Wesley, 2004).
- <a name="Taligent94"></a>
  \[Taligent94]: Taligent's Guide to Designing Programs (Addison-Wesley, 1994).
