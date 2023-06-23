# C 与 C++ 中的结构体

C++ 中的类（Class）可以看做 C 语言中结构体（Struct）的升级版。结构体是一种构造类型，可以包含若干成员变量，每个成员变量的类型可以不同；可以通过结构体来定义结构体变量，每个变量拥有相同的性质。

- C 语言中的结构体不能为空，否则会报错
- C 语言中的结构体只涉及到数据结构，而不涉及到算法，也就是说在 C 中数据结构和算法是分离的。换句话说就是 C 语言中的结构体只能定义成员变量，但是不能定义成员函数。
- 在 C++中既可以定义成员变量又可以定义成员函数， C++中的结构体和类体现了数据结构和算法的结合。
- 虽然 C 语言的结构体中不能定义成员函数，但是却可以定义函数指针，不过函数指针本质上不是函数而是指针，所以总的来说 C 语言中的结构体只是一个复杂数据类型 ，只能定义成员变量，不能定义成员函数，不能用于面向对象编程。

C 语言中的 struct 只能包含变量，而 C++ 中的 class 除了可以包含变量，还可以包含函数。display() 是用来处理成员变量的函数，在 C 语言中，将它放在了 struct Student 外面，它和成员变量是分离的；而在 C++ 中，我们将它放在了 class Student 内部，使它和成员变量聚集在一起，看起来更像一个整体。结构体和类都可以看做一种由用户自己定义的复杂数据类型，在 C 语言中可以通过结构体名来定义变量，在 C++ 中可以通过类名来定义变量。不同的是，通过结构体定义出来的变量还是叫变量，而通过类定义出来的变量有了新的名称，叫做对象（Object）。

```c
// C
#include <stdio.h>

//定义结构体 Student
struct Student{
    //结构体包含的成员变量
    char *name;
    int age;
    float score;
};
//显示结构体的成员变量
void display(struct Student stu){
    printf("%s的年龄是 %d，成绩是 %f\n", stu.name, stu.age, stu.score);
}
int main(){
    struct Student stu1;
    //为结构体的成员变量赋值
    stu1.name = "小明";
    stu1.age = 15;
    stu1.score = 92.5;
    //调用函数
    display(stu1);
    return 0;
}

// C++
#include <stdio.h>

//通过class关键字类定义类
class Student{
public:
    //类包含的变量
    char *name;
    int age;
    float score;
    //类包含的函数
    void say(){
        printf("%s的年龄是 %d，成绩是 %f\n", name, age, score);
    }
};
int main(){
    //通过类来定义变量，即创建对象
    class Student stu1;  //也可以省略关键字class
    //为类的成员变量赋值
    stu1.name = "小明";
    stu1.age = 15;
    stu1.score = 92.5f;
    //调用类的成员函数
    stu1.say();
    return 0;
}
```

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
