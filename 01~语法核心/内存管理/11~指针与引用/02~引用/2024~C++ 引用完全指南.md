# C++ 引用完全指南

## 1. 引用基础

引用是 C++中的一个重要特性，本质上是对象的别名。与指针相似，引用存储其他对象的内存地址，但提供了更安全、更简洁的语法。

### 1.1 基本语法

```cpp
// 基本声明语法
[storage-class-specifiers] [cv-qualifiers] type-specifiers [& or &&] [cv-qualifiers] identifier [=expression];

// 示例
int value = 42;
int& ref = value;         // lvalue引用
int&& rref = std::move(value);  // rvalue引用

// 多重声明
int &ref1, &ref2, *ptr, var;  // ref1和ref2是引用，ptr是指针，var是普通变量
```

### 1.2 引用的特性

1. **必须初始化**：

```cpp
int& ref;      // 错误：引用必须初始化
int value = 42;
int& ref = value;  // 正确
```

2. **不能重新绑定**：

```cpp
int a = 1, b = 2;
int& ref = a;
ref = b;  // 修改a的值为2，而不是重新绑定到b
```

3. **没有多级引用**：

```cpp
int value = 42;
int& ref = value;
int& & ref2 = ref;  // 错误：不存在引用的引用
```

## 2. 引用与指针的对比

### 2.1 语法差异

```cpp
int value = 42;

// 指针使用
int* ptr = &value;     // 需要取地址符
*ptr = 43;            // 需要解引用

// 引用使用
int& ref = value;      // 直接绑定
ref = 43;             // 直接使用
```

### 2.2 核心区别

1. **初始化要求**：

   - 引用必须初始化
   - 指针可以后续初始化

2. **空值语义**：

   - 引用不能为空
   - 指针可以为 nullptr

3. **内存占用**：
   - 引用通常不占额外内存
   - 指针需要存储地址（4 或 8 字节）

## 3. 常见使用场景

### 3.1 函数参数传递

```cpp
// 大对象：优先使用const引用
void processLargeObject(const BigObject& obj);

// 需要修改且可能为空：使用指针
void processNullableObject(Object* obj) {
    if (!obj) return;
    // 处理obj
}

// 需要修改且确保非空：使用引用
void processNonNullObject(Object& obj) {
    // 直接处理obj
}

// 现代C++中的字符串处理
void processString(std::string_view text);  // 比const string&更优
```

### 3.2 范围 for 循环

```cpp
vector<string> names = {"Alice", "Bob", "Charlie"};

// 只读遍历
for (const auto& name : names) {
    cout << name << endl;
}

// 修改元素
for (auto& name : names) {
    name = "Mr. " + name;
}
```

### 3.3 数据结构实现

```cpp
// 指针适合实现链表等数据结构
struct ListNode {
    int val;
    ListNode* next;
    ListNode(int x) : val(x), next(nullptr) {}
};

// 引用不适合这种场景，因为：
// 1. 必须立即初始化
// 2. 不能为空
// 3. 不能重新赋值
```

## 4. 现代 C++最佳实践

### 4.1 智能指针优先

```cpp
// 替代裸指针的动态内存管理
std::unique_ptr<Resource> unique;  // 独占所有权
std::shared_ptr<Resource> shared;  // 共享所有权
std::weak_ptr<Resource> weak;      // 弱引用
```

### 4.2 参数传递指南

```cpp
// 小对象：值传递
void process(int value);

// 大对象：const引用
void process(const BigObject& obj);

// 字符串：string_view
void process(std::string_view text);

// 需要修改：非const引用
void process(std::vector<int>& data);
```

### 4.3 现代替代方案

```cpp
// 可选引用
std::optional<std::reference_wrapper<T>> optRef;

// 视图类型
void processArray(std::span<const int> data);

// 多态场景
std::variant<Circle, Square, Triangle> shape;
```

## 5. 引用的缺陷

### 5.1 初始化限制

```cpp
class MyClass {
    int& ref;  // 成员引用必须在构造函数初始化列表中初始化
public:
    MyClass() { }  // 错误
    MyClass(int& val) : ref(val) { }  // 正确
};
```

### 5.2 容器限制

```cpp
// 不能创建引用容器
vector<int&> refs;  // 错误

// 解决方案：使用reference_wrapper
vector<reference_wrapper<int>> refs;  // 正确
```

### 5.3 生命周期问题

```cpp
int& getDanglingReference() {
    int local = 42;
    return local;  // 危险：返回局部变量的引用
}
```

## 6. 选择指南

1. **使用指针当：**

   - 需要表示空值
   - 需要动态内存管理
   - 需要改变指向
   - 实现复杂数据结构

2. **使用引用当：**

   - 确保对象存在
   - 作为函数参数避免拷贝
   - 在范围 for 循环中
   - 实现运算符重载

3. **使用现代替代品当：**
   - 需要智能内存管理（智能指针）
   - 需要字符串或数组视图（string_view/span）
   - 需要可选值语义（optional）

## 7. 注意事项

1. 避免引用成员变量（除非确实需要）
2. 注意生命周期管理
3. 优先使用 const 引用
4. 考虑现代 C++提供的专门工具
5. lambda 表达式中优先选择值捕获而非引用捕获
