# C++ 智能指针

## 背景介绍

传统 C++ 中的内存管理一直是个棘手的问题：

- 手动管理内存容易造成内存泄漏
- 异常安全难以保证
- 所有权语义不清晰

为了解决这些问题，现代 C++ 引入了智能指针机制，它基于 RAII (Resource Acquisition Is Initialization) 原则，实现了自动的资源管理。

## 智能指针概述

智能指针是存在于 `<memory>` 头文件中的模板类，它们像常规指针一样工作，但能够自动管理底层内存的生命周期。主要特点：

- 自动内存管理
- 异常安全
- 清晰的所有权语义
- 与原始指针相近的性能

## 常见的智能指针类型

### 1. unique_ptr

最基础且最常用的智能指针，适用于独占资源的场景。

**特点：**

- 独占所有权
- 不可复制，但可以移动
- 零开销抽象（大小等同于原始指针）
- 是 POCO（Plain Old C++ Object）的默认选择

**使用场景：**

```cpp
void UseUniquePtr() {
    // 创建智能指针
    std::unique_ptr<Song> song(new Song("Nothing on You", "Bruno Mars"));

    // 使用对象
    song->play();

    // 无需手动释放内存
} // song 自动释放

// 现代写法（推荐）
auto song = std::make_unique<Song>("Nothing on You", "Bruno Mars");
```

### 2. shared_ptr

适用于需要共享资源所有权的场景。

**特点：**

- 支持多个指针共享同一个对象
- 采用引用计数机制
- 线程安全的引用计数
- 额外内存开销（两个指针大小）

**使用场景：**

```cpp
void UseSharedPtr() {
    // 创建共享指针
    auto sp1 = std::make_shared<Resource>();
    {
        auto sp2 = sp1; // 引用计数加1
        // sp2 使用资源
    } // sp2 销毁，引用计数减1
} // sp1 销毁，引用计数为0，资源被释放
```

### 3. weak_ptr

配合 shared_ptr 使用，用于解决循环引用问题。

**特点：**

- 不增加引用计数
- 可以观察但不拥有对象
- 可以检查所观察的对象是否仍然存在

**使用场景：**

```cpp
class Node {
    std::weak_ptr<Node> parent;    // 避免循环引用
    std::vector<std::shared_ptr<Node>> children;
};
```

## 最佳实践

1. **优先使用 unique_ptr**

   - 除非确实需要共享所有权，否则应该使用 unique_ptr
   - 使用 `std::make_unique` 创建（C++14 及以后）

2. **谨慎使用 shared_ptr**

   - 共享所有权会增加程序的复杂性
   - 使用 `std::make_shared` 创建以获得更好的性能

3. **避免直接使用 new**

   - 优先使用 make_unique/make_shared 工厂函数
   - 这样可以确保异常安全，并且代码更简洁

4. **使用 weak_ptr 打破循环引用**
   - 在观察者模式等场景中特别有用
   - 可以防止内存泄漏

## 注意事项

- 不要用 new 创建智能指针本身
- 避免同一原始指针创建多个智能指针
- 注意 shared_ptr 的性能开销
- 使用 make_xxx 函数而不是直接构造
