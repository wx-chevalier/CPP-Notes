# 什么是 STL？

除了从 C 标准库保留下来的一些功能之外，C++还提供了一个**基于模版**实现的**标准模版库（Standard Template Library, 简称 STL）**。

- STL 基于模版实现了一些面向序列数据的**表示**以及常用的**操作**
- STL 支持了一种**抽象**的编程模式

  - 隐藏了一些低级的程序元素，如数组、链表、循环等

![](https://ngte-superbed.oss-cn-beijing.aliyuncs.com/uPic/ha0DEWhyTTml.png)

## STL 包含什么？

**容器类模板**

容器用于存储**序列化的**数据，如：向量、队列、栈、集合等。

**算法（函数）模板**

算法用于对容器中的数据元素进行一些常用**操作**，如：排序、统计等。

**迭代器类模板**

- 迭代器实现了**抽象的指针**功能，它们指向容器中的数据元素，用于对容器中的数据元素进行遍历和访问。
- **迭代器是容器和算法之间的桥梁**：传给算法的不是容器，而是指向容器中元素的迭代器，算法通过迭代器实现对容器中数据元素的访问。这样使得算法与容器保持独立，从而提高算法的通用性。

![STL 的基本逻辑](https://cui-jiacai.gitbook.io/~gitbook/image?url=https%3A%2F%2F971000043-files.gitbook.io%2F%7E%2Ffiles%2Fv0%2Fb%2Fgitbook-x-prod.appspot.com%2Fo%2Fspaces%252F4jcp9JrFSUV0Enu5fcXK%252Fuploads%252F4yIi4UTdNW9C2vdGDA0D%252F%25E6%2588%25AA%25E5%25B1%258F2022-02-08%252020.57.44.png%3Falt%3Dmedia%26token%3D39377522-2a40-4233-8abd-296ba9c79229&width=768&dpr=4&quality=100&sign=5b914a35&sv=1)

# 基于 STL 编程的例子

```cpp
#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
using namespace std;

void display(int x) { cout << ' ' << x; return; }

int main()
{
    vector<int> v; //创建容器对象v，元素类型为int
    int x;
    cin >> x;
    while (x > 0) //生成容器v中的元素
    {
        v.push_back(x); //往容器v中增加一个元素
        cin >> x;
    }
    //利用算法max_element计算并输出容器v中的最大元素
    cout << "Max = " << *max_element(v.begin(),v.end()) << endl;
    //利用算法accumulate计算并输出容器v中所有元素的和
    cout << "Sum = " << accumulate(v.begin(),v.end(),0) << endl;
    //利用算法sort对容器v中的元素进行排序
    sort(v.begin(),v.end());
    //利用算法for_each输出排序结果
    cout << "Sorted result is:\n";
    for_each(v.begin(),v.end(),display);
    cout << '\n';
    return 0;
}
```

用 STL 来实现上述程序的功能不需要涉及一些低级的程序元素，如数组、链表、循环，使得程序设计变得更便捷。
