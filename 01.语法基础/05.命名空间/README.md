# 命名空间

命名空间是一个声明性区域，为其内部的标识符（类型、函数和变量等的名称）提供一个范围。命名空间用于将代码组织到逻辑组中，还可用于避免名称冲突，尤其是在基本代码包括多个库时。命名空间范围内的所有标识符彼此可见，而没有任何限制。命名空间之外的标识符可通过使用每个标识符的完全限定名（例如 std::vector<std::string> vec;）来访问成员，也可通过单个标识符的 using 声明 (using std::string) 或命名空间中所有标识符的 using 指令 (using namespace std;) 来访问成员。头文件中的代码应始终使用完全限定的命名空间名称。

下面的示例演示了一个命名空间声明和命名空间之外的代码可访问其成员的三种方法。

```c++
namespace ContosoData
{
    class ObjectManager
    {
    public:
        void DoSomething() {}
    };
    void Func(ObjectManager) {}
}

// 使用完全限定名
ContosoData::ObjectManager mgr;
mgr.DoSomething();
ContosoData::Func(mgr);

// 使用 using 声明，以将一个标识符引入范围：
using ContosoData::ObjectManager;
ObjectManager mgr;
mgr.DoSomething();

// 使用 using 指令，以将命名空间中的所有内容引入范围：
using namespace ContosoData;

ObjectManager mgr;
mgr.DoSomething();
Func(mgr);
```

# using 指令

通过 using 指令，可使用 namespace 中的所有名称，而不需要 namespace-name 为显式限定符。如果在一个命名空间中使用多个不同的标识符，则在实现文件中使用 using 指令（即 \*.cpp）；如果仅使用一个或两个标识符，则考虑 using 声明，以仅将这些标识符而不是命名空间中的所有标识符引入范围。如果本地变量的名称与命名空间变量的名称相同，则隐藏命名空间变量。使命名空间变量具有与全局变量相同的名称是错误的。

using 指令可以放置在 .cpp 文件的顶部（在文件范围内），或放置在类或函数定义内。

一般情况下，避免将 using 指令放置在头文件 (`*.h`) 中，因为任何包含该标头的文件都会将命名空间中的所有内容引入范围，这将导致非常难以调试的名称隐藏和名称冲突问题。在头文件中，始终使用完全限定名。如果这些名称太长，可以使用命名空间别名将其缩短。
