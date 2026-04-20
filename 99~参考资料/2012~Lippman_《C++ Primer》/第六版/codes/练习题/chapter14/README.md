# 第 14 章 《C++中的代码重用 》 编程练习题之我解

## 14.1

**题：** Wine 类有一个 string 类对象成员(参见第 4 章)和一个 Pair 对象 (参见本章);其中前者用于存储葡萄酒的名称，而后者有 2 个 valarray<int>对象(参见本章)，这两个 valarray<int>对象分别保存了葡 萄酒的酿造年份和该年生产的瓶数。例如，Pair 的第 1 个 valarray<int>对 象可能为 1988、1992 和 1996 年，第 2 个 valarray<int>对象可能为 24、48 和 144 瓶。Wine 最好有 1 个 int 成员用于存储年数。另外，一些 typedef 可能 有助于简化编程工作:

![image-20220213133041457](https://assets.ng-tech.icu/item/image-20220213133041457.png)

这样，`PairArray` 表示的是类型 `Pair<std::valarray<int>, std::valarray<int> >`。使用包含来实现 `Wine` 类，并用一个简单的程序对其 进行测试。`Wine` 类应该有一个默认构造函数以及如下构造函数:

![image-20220213133131668](https://assets.ng-tech.icu/item/image-20220213133131668.png)

Wine 类应该有一个 `GetBottles()` 方法，它根据 Wine 对象能够存储几种年份(y)，提示用户输入年份和瓶数。方法 `Label()` 返回一个指向葡 萄酒名称的引用。`sum()` 方法返回 `Pair` 对象中第二个 `valarray<int>` 对象中的瓶数总和。

测试程序应提示用户输入葡萄酒名称、元素个数以及每个元素存储 的年份和瓶数等信息。程序将使用这些数据来构造一个 `Wine` 对象，然后 显示对象中保存的信息。

下面是一个简单的测试程序:

![image-20220213133308327](https://assets.ng-tech.icu/item/image-20220213133308327.png)

下面是该程序的运行情况:

![image-20220213133353832](https://assets.ng-tech.icu/item/image-20220213133353832.png)

## 14.2

**题：** 采用私有继承而不是包含来完成编程练习 1。同样，一些 typedef 可能会有所帮助，另外，您可能还需要考虑诸如下面这样的语句的含义:

![image-20220213133433724](https://assets.ng-tech.icu/item/image-20220213133433724.png)

您设计的类应该可以使用编程练习 1 中的测试程序进行测试。

## 14.3

**题：** 定义一个 QueueTp 模板。然后在一个类似于程序清单 14.12 的程 序中创建一个指向 Worker 的指针队列(参见程序清单 14.10 中的定 义)，并使用该队列来测试它。

## 14.4

**题：** Person 类保存人的名和姓。除构造函数外，它还有 Show( )方 法，用于显示名和姓。Gunslinger 类以 Person 类为虚基类派生而来，它包 含一个 Draw( )成员，该方法返回一个 double 值，表示枪手的拔枪时间。这个类还包含一个 int 成员，表示枪手枪上的刻痕数。最后，这个类还包 含一个 Show( )函数，用于显示所有这些信息。

PokerPlayer 类以 Person 类为虚基类派生而来。它包含一个 Draw()成 员，该函数返回一个 1~52 的随机数，用于表示扑克牌的值(也可以定 义一个 Card 类，其中包含花色和面值成员，然后让 Draw( )返回一个 Card 对象)。PokerPlayer 类使用 Person 类的 show( )函数。BadDude( )类从 Gunslinger 和 PokerPlayer 类公有派生而来。它包含 Gdraw( )成员(返回坏 蛋拔枪的时间)和 Cdraw( )成员(返回下一张扑克牌)，另外还有一个 合适的 Show( )函数。请定义这些类和方法以及其他必要的方法(如用于 设置对象值的方法)，并使用一个类似于程序清单 14.12 的简单程序对 它们进行测试。

## 14.5

**题：** 下面是一些类声明:

![image-20220213133617785](https://assets.ng-tech.icu/item/image-20220213133617785.png)

![image-20220213133650978](https://assets.ng-tech.icu/item/image-20220213133650978.png)

![image-20220213133709715](https://assets.ng-tech.icu/item/image-20220213133709715.png)

注意，该类层次结构使用了带虚基类的 MI，所以要牢记这种情况 下用于构造函数初始化列表的特殊规则。还需要注意的是，有些方法被 声明为保护的。这可以简化一些 highfink 方法的代码(例如，如果 highfink::ShowAll( )只是调用 fink::ShowAll( )和 manager::ShwAll( )，则它 将调用 abstr_emp::ShowAll( )两次)。请提供类方法的实现，并在一个程 序中对这些类进行测试。下面是一个小型测试程序:

![image-20220213133744235](https://assets.ng-tech.icu/item/image-20220213133744235.png)

![image-20220213133810320](https://assets.ng-tech.icu/item/image-20220213133810320.png)

- 为什么没有定义赋值运算符?
- 为什么要将 `ShowAll()` 和 `SetAll()` 定义为虚的?
- 为什么要将 `abstr_emp` 定义为虚基类?
- 为什么 `highfink` 类没有数据部分?
- 为什么只需要一个`operator<<()`版本?

如果使用下面的代码替换程序的结尾部分，将会发生什么情况?

![image-20220213133957275](https://assets.ng-tech.icu/item/image-20220213133957275.png)
