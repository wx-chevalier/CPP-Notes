# RAII

与托管语言不同，C++ 没有自动回收垃圾，这是在程序运行时释放堆内存和其他资源的一个内部进程。C++ 程序负责将所有已获取的资源返回到操作系统。未能释放未使用的资源称为“泄漏”。在进程退出之前，泄漏的资源无法用于其他程序。特别是内存泄漏是 C 样式编程中 Bug 的常见原因。新式 C++ 通过声明堆栈上的对象，尽可能避免使用堆内存。当某个资源对于堆栈来说太大时，则它应由对象拥有。当该对象初始化时，它会获取它拥有的资源。然后，该对象负责在其析构函数中释放资源。在堆栈上声明拥有资源的对象本身。对象拥有资源的原则也称为“资源获取即初始化”(RAII)。

当拥有资源的堆栈对象超出范围时，会自动调用其析构函数。这样，C++ 中的垃圾回收与对象生存期密切相关，是确定性的。资源始终在程序中的已知点发布，你可以控制该点。仅类似 C++ 中的确定析构函数可公平处理内存和非内存资源。下面的示例显示了简单查询 `w`。它在函数范围内的堆栈上声明，并在函数块的末尾销毁。对象 `w` 没有资源（例如堆分配的内存）。它的唯一成员 `g` 本身在堆栈上声明，并且与 `w` 一起超出范围。`widget` 析构函数中不需要特殊代码。

```cpp
class widget {
private:
    gadget g;   // lifetime automatically tied to enclosing object
public:
    void draw();
};

void functionUsingWidget () {
    widget w;   // lifetime automatically tied to enclosing scope
                // constructs w, including the w.g gadget member
    // ...
    w.draw();
    // ...
} // automatic destruction and deallocation for w and w.g
  // automatic exception safety,
  // as if "finally { w.dispose(); w.g.dispose(); }"
```

在以下示例中，w 拥有内存资源，因此必须在其析构函数中具有代码才能删除内存。

```cpp
class widget
{
private:
    int* data;
public:
    widget(const int size) { data = new int[size]; } // acquire
    ~widget() { delete[] data; } // release
    void do_something() {}
};

void functionUsingWidget() {
    widget w(1000000);  // lifetime automatically tied to enclosing scope
                        // constructs w, including the w.data member
    w.do_something();

} // automatic destruction and deallocation for w and w.data
```

自 C++ 11 以来，有一种更好的方法可以编写前面的示例：使用标准库中的智能指针。智能指针可处理对其拥有的内存的分配和删除。使用智能指针将不需要在 widget 类中显式析构函数。

```cpp
#include <memory>
class widget
{
private:
    std::unique_ptr<int[]> data;
public:
    widget(const int size) { data = std::make_unique<int[]>(size); }
    void do_something() {}
};

void functionUsingWidget() {
    widget w(1000000);  // lifetime automatically tied to enclosing scope
                        // constructs w, including the w.data gadget member
    // ...
    w.do_something();
    // ...
} // automatic destruction and deallocation for w and w.data
```

使用智能指针进行内存分配，可以消除内存泄漏的可能性。此模型适用于其他资源，例如文件句柄或套接字。可以在你的类中以类似的方式管理自己的资源。C++ 的设计可确保对象在超出范围时被销毁。也就是说，它们在块被退出时以与构造相反的顺序被摧毁。销毁对象时，将按特定顺序销毁其基项和成员。在全局范围内在任何块之外声明的对象可能会导致问题。如果全局对象的构造函数引发异常，将很难调试。
