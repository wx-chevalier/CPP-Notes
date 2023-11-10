# C++ 中的结构体与类

分析 C++中的结构体与类，它们的相同之处：

- 结构体也可以包含函数，也可以定义 public、private、protected 数据成员；定义了结构体之后，可以用结构体名来创建对象。
- C++当中，结构体中可以有成员变量，可以有成员函数，可以从别的结构体继承，也可以被别的结构体继承，可以有虚函数。
- 总之，class 和 struct 的语法基本相同，从声明到使用，都很相似，但是 struct 的约束要比 class 多，理论上，struct 能做到的 class 都能做到，但 class 能做到的 stuct 却不一定做的到。

**再来说说两者的区别**：对于成员访问权限以及继承方式，class 中默认的是 private，而 struct 中则是 public。class 还可以用于表示模板类型，struct 则不行。

```cpp
struct A
{
public:
    A(){};
    virtual void Dynamic()
    {
        cout << "A" << endl;
    }
protected:
    void fun();
private:
    int m_Data;
};

struct B:public A
{
public:
    virtual void Dynamic()
    {
        cout << "B" << endl;
    }
};
int main()
{
    A * pa = new B;
    pa->Dynamic();
}
//原文链接：https://blog.csdn.net/Loving_Forever_/article/details/51483828
```

总结一下就是：

- **概念**：class 和 struct 的语法基本相同，从声明到使用，都很相似，但是 struct 的约束要比 class 多，理论上，struct 能做到的 class 都能做到，但 class 能做到的 stuct 却不一定做的到。
- **类型**：struct 是值类型，class 是引用类型，因此它们具有所有值类型和引用类型之间的差异。
- **效率**：由于堆栈的执行效率要比堆的执行效率高，但是堆栈资源却很有限，不适合处理逻辑复杂的大对象，因此 struct 常用来处理作为基类型对待的小对象，而 class 来处理某个商业逻辑。
- **关系**：struct 不仅能继承也能被继承 ，而且可以实现接口，不过 Class 可以完全扩展。内部结构有区别，struct 只能添加带参的构造函数，不能使用 abstract 和 protected 等修饰符，不能初始化实例字段。

关于 Class 性能好还是 Struct 性能好（换言堆性能好？还是栈性能好？） 那么什么时机该用呢 ，比较 struct 和 class 的不同，以下总结可以参考一下：

1. 在表示诸如点、矩形等主要用来存储数据的轻量级对象时，首选 struct。
2. 在表示数据量大、逻辑复杂的大对象时，首选 class。
3. 在表现抽象和多级别的对象层次时，class 是最佳选择
