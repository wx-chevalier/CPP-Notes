# C++ 函数基础

## 1. 函数概述

函数是执行特定操作的代码块，可以：

- 接受输入参数
- 返回输出值
- 封装可重用的操作

基本示例：

```cpp
// 简单的加法函数
int sum(int a, int b) {
    return a + b;
}

// 函数调用示例
int main() {
    int i = sum(10, 32);
    int j = sum(i, 66);
    cout << "The value of j is " << j << endl; // 108
}
```

## 2. 函数定义

### 2.1 基本组成部分

必需部分：

1. 返回类型（如 `int`、`void`、`auto`）
2. 函数名称
3. 参数列表（可以为空）

可选部分：

- `constexpr` - 编译时计算
- `extern`/`static` - 链接规范
- `inline` - 内联展开
- `noexcept` - 异常说明
- 成员函数专用：`const`、`volatile`、`virtual`、`override`、`final`、`static`

### 2.2 函数声明示例

```cpp
// 基本函数声明
int sum(int a, int b);

// constexpr函数示例
constexpr float exp(float x, int n) {
    return n == 0 ? 1 :
        n % 2 == 0 ? exp(x * x, n / 2) :
        exp(x * x, (n - 1) / 2) * x;
}

// inline函数示例
inline double Account::GetBalance() {
    return balance;
}
```

## 3. 函数参数

### 3.1 参数传递方式

1. 值传递（默认）
2. 引用传递

```cpp
// 引用传递
void DoSomething(std::string& input);

// const引用传递
void DoSomething(const std::string& input);

// 右值引用（C++11）
void DoSomething(std::string&& input);
```

### 3.2 默认参数

```cpp
// 正确：默认参数在末尾
int DoSomething(int num,
    string str = "Default",
    Allocator& alloc = defaultAllocator);
```

## 4. 函数返回值

### 4.1 返回类型声明

```cpp
// 传统返回类型
int traditional() { return 42; }

// 尾随返回类型（C++11）
auto trailing() -> int { return 42; }

// 自动推导返回类型（C++14）
auto deduced() { return 42; }
```

### 4.2 多值返回

1. 使用结构体

```cpp
struct Result {
    string name;
    int value;
};

Result getData() {
    return {"test", 42};
}
```

2. 使用 tuple/pair

```cpp
tuple<int, string, double> getData() {
    return {108, "text", 0.01};
}
```

3. 使用结构化绑定（C++17）

```cpp
auto [x, y, z] = getData(); // 自动解包tuple
```

## 5. 函数模板

```cpp
template<typename Lhs, typename Rhs>
auto Add(const Lhs& lhs, const Rhs& rhs) -> decltype(lhs + rhs) {
    return lhs + rhs;
}
```

## 6. 函数指针

推荐使用 typedef 简化函数指针声明：

```cpp
typedef int (*FunctionPtr)(int);
FunctionPtr getFunction();

// 等价于复杂形式：
int (*getFunction())(int);
```
