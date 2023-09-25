# 智能指针

在现代 C++ 编程中，标准库包含智能指针，该指针用于确保程序不存在内存和资源泄漏且是异常安全的。

# 智能指针的使用

智能指针是在 [<memory>](https://learn.microsoft.com/zh-cn/cpp/standard-library/memory?view=msvc-170) 头文件中的 `std` 命名空间中定义的。它们对 [RAII](https://learn.microsoft.com/zh-cn/cpp/cpp/object-lifetime-and-resource-management-modern-cpp?view=msvc-170) 或“获取资源即初始化”编程惯用法至关重要。此习惯用法的主要目的是确保资源获取与对象初始化同时发生，从而能够创建该对象的所有资源并在某行代码中准备就绪。实际上，RAII 的主要原则是为将任何堆分配资源（例如，动态分配内存或系统对象句柄）的所有权提供给其析构函数包含用于删除或释放资源的代码以及任何相关清理代码的堆栈分配对象。

大多数情况下，当初始化原始指针或资源句柄以指向实际资源时，会立即将指针传递给智能指针。在现代 C++ 中，原始指针仅用于范围有限的小代码块、循环或者性能至关重要且不会混淆所有权的 Helper 函数中。下面的示例将原始指针声明与智能指针声明进行了比较。

```cpp
void UseRawPointer()
{
    // Using a raw pointer -- not recommended.
    Song* pSong = new Song(L"Nothing on You", L"Bruno Mars");

    // Use pSong...

    // Don't forget to delete!
    delete pSong;
}


void UseSmartPointer()
{
    // Declare a smart pointer on stack and pass it the raw pointer.
    unique_ptr<Song> song2(new Song(L"Nothing on You", L"Bruno Mars"));

    // Use song2...
    wstring s = song2->duration_;
    //...

} // song2 is deleted automatically here.
```

如示例所示，智能指针是你在堆栈上声明的类模板，并可通过使用指向某个堆分配的对象的原始指针进行初始化。在初始化智能指针后，它将拥有原始的指针。这意味着智能指针负责删除原始指针指定的内存。智能指针析构函数包括要删除的调用，并且由于在堆栈上声明了智能指针，当智能指针超出范围时将调用其析构函数，尽管堆栈上的某处将进一步引发异常。

通过使用熟悉的指针运算符（`->` 和 `*`）访问封装指针，智能指针类将重载这些运算符以返回封装的原始指针。C++ 智能指针思路类似于在语言（如 C#）中创建对象的过程：创建对象后让系统负责在正确的时间将其删除。不同之处在于，单独的垃圾回收器不在后台运行；按照标准 C++ 范围规则对内存进行管理，以使运行时环境更快速更有效。

下面的示例演示了如何使用 C++ 标准库中的 unique_ptr 智能指针类型将指针封装到大型对象。

```cpp
class LargeObject
{
public:
    void DoSomething(){}
};

void ProcessLargeObject(const LargeObject& lo){}
void SmartPointerDemo()
{
    // Create the object and pass it to a smart pointer
    std::unique_ptr<LargeObject> pLarge(new LargeObject());

    //Call a method on the object
    pLarge->DoSomething();

    // Pass a reference to a method.
    ProcessLargeObject(*pLarge);

} //pLarge is deleted automatically when function block goes out of scope.
```

此示例演示如何使用智能指针执行以下关键步骤。

1. 将智能指针声明为一个自动（局部）变量。（不要对智能指针本身使用 **`new`** 或 `malloc` 表达式。）
2. 在类型参数中，指定封装指针的指向类型。
3. 在智能指针构造函数中将原始指针传递至 **`new`** 对象。（某些实用工具函数或智能指针构造函数可为你执行此操作。）
4. 使用重载的 `->` 和 `*` 运算符访问对象。
5. 允许智能指针删除对象。

智能指针的设计原则是在内存和性能上尽可能高效。例如，`unique_ptr` 中的唯一数据成员是封装的指针。这意味着，`unique_ptr` 与该指针的大小完全相同，不是四个字节就是八个字节。使用重载了 `*` 和 `->` 运算符的智能指针访问封装指针的速度不会明显慢于直接访问原始指针的速度。智能指针具有通过使用“点”表示法访问的成员函数。例如，一些 C++ 标准库智能指针具有释放指针所有权的重置成员函数。如果你想要在智能指针超出范围之前释放其内存将很有用，这会很有用，如以下示例所示：

```cpp
void SmartPointerDemo2()
{
    // Create the object and pass it to a smart pointer
    std::unique_ptr<LargeObject> pLarge(new LargeObject());

    //Call a method on the object
    pLarge->DoSomething();

    // Free the memory before we exit function block.
    pLarge.reset();

    // Do some other work...

}
```

智能指针通常提供直接访问其原始指针的方法。C++ 标准库智能指针拥有一个用于此目的的 get 成员函数，CComPtr 拥有一个公共的 p 类成员。通过提供对基础指针的直接访问，你可以使用智能指针管理你自己的代码中的内存，还能将原始指针传递给不支持智能指针的代码。

```cpp
void SmartPointerDemo4()
{
    // Create the object and pass it to a smart pointer
    std::unique_ptr<LargeObject> pLarge(new LargeObject());

    //Call a method on the object
    pLarge->DoSomething();

    // Pass raw pointer to a legacy API
    LegacyLargeObjectFunction(pLarge.get());
}
```

# 智能指针的类型

使用这些智能指针作为将指针封装为纯旧 C++ 对象 (POCO) 的首选项。

- `unique_ptr`
  只允许基础指针的一个所有者。除非你确信需要 `shared_ptr`，否则请将该指针用作 POCO 的默认选项。可以移到新所有者，但不会复制或共享。替换已弃用的 `auto_ptr`。与 `boost::scoped_ptr` 比较。`unique_ptr` 小巧高效；大小等同于一个指针且支持 rvalue 引用，从而可实现快速插入和对 C++ 标准库集合的检索。头文件：`<memory>`。
- `shared_ptr`
  采用引用计数的智能指针。如果你想要将一个原始指针分配给多个所有者（例如，从容器返回了指针副本又想保留原始指针时），请使用该指针。直至所有 `shared_ptr` 所有者超出了范围或放弃所有权，才会删除原始指针。大小为两个指针；一个用于对象，另一个用于包含引用计数的共享控制块。头文件：`<memory>`。
- `weak_ptr`
  结合 `shared_ptr` 使用的特例智能指针。`weak_ptr` 提供对一个或多个 `shared_ptr` 实例拥有的对象的访问，但不参与引用计数。如果你想要观察某个对象但不需要其保持活动状态，请使用该实例。在某些情况下，需要断开 `shared_ptr` 实例间的循环引用。头文件：`<memory>`。
