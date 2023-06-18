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
