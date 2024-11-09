# C++ 类与结构体

## 1. 结构体（struct）

结构体是 C++ 中最简单的用户自定义类型，它可以将不同类型的数据组合成一个整体。

### 1.1 基本语法与成员初始化

```cpp
struct Person {
    // 使用成员初始化器设置默认值
    string name;
    int age = 0;
    float weight = 0.0f;

    // 默认构造函数
    Person() = default;

    // 自定义构造函数使用成员初始化列表
    Person(string n, int a, float w)
        : name(std::move(n))
        , age(a)
        , weight(w) {}
};
```

### 1.2 位域结构体

```cpp
struct PixelColor {
    // 位域不支持成员初始化器
    unsigned int red   : 8;
    unsigned int green : 8;
    unsigned int blue  : 8;

    PixelColor(uint8_t r = 0, uint8_t g = 0, uint8_t b = 0)
        : red(r)
        , green(g)
        , blue(b) {}
};
```

### 1.3 结构体的继承

```cpp
struct Animal {
    string name;
    int age = 0;

    Animal() = default;
    Animal(string n, int a)
        : name(std::move(n))
        , age(a) {}
};

struct Dog : Animal {
    string breed = "mixed";  // 默认值
    bool isVaccinated = false;

    Dog() = default;
    Dog(string n, int a, string b)
        : Animal(std::move(n), a)
        , breed(std::move(b)) {}
};
```

## 2. 类（class）

类是 C++ 的核心特性，它将数据和操作数据的方法封装在一起。

### 2.1 基本语法

```cpp
class Student {
private:
    string name;
    int studentId = 0;
    float GPA = 0.0f;
    vector<string> courses;  // 空 vector 不需要初始化器

public:
    Student() = default;
    Student(string n, int id, float gpa)
        : name(std::move(n))
        , studentId(id)
        , GPA(gpa) {}

    float getGPA() const { return GPA; }
};
```

### 2.2 继承与多态

```cpp
class Shape {
protected:
    Point2D position = {0, 0};  // 使用聚合初始化
    string name = "unknown";

public:
    Shape() = default;
    Shape(Point2D pos, string n)
        : position(pos)
        , name(std::move(n)) {}

    virtual double area() const = 0;
    virtual ~Shape() = default;
};

class Circle : public Shape {
private:
    double radius = 0.0;

public:
    Circle() = default;
    Circle(Point2D center, double r, string n)
        : Shape(center, std::move(n))
        , radius(r) {}

    double area() const override {
        return M_PI * radius * radius;
    }
};
```

## 3. 实际应用场景

### 3.1 适合使用结构体的场景

1. **数据传输对象（DTO）**

```cpp
struct NetworkConfig {
    string host = "localhost";
    uint16_t port = 8080;
    bool useTLS = false;
    int timeout_ms = 5000;

    NetworkConfig() = default;
    NetworkConfig(string h, uint16_t p)
        : host(std::move(h))
        , port(p) {}
};
```

2. **值类型对象**

```cpp
struct Point3D {
    double x = 0.0;
    double y = 0.0;
    double z = 0.0;

    Point3D() = default;
    Point3D(double xPos, double yPos, double zPos)
        : x(xPos)
        , y(yPos)
        , z(zPos) {}
};
```

### 3.2 适合使用类的场景

1. **资源管理**

```cpp
class FileHandler {
private:
    string filename;
    FILE* file = nullptr;
    bool isOpen = false;

public:
    explicit FileHandler(string fname)
        : filename(std::move(fname)) {
        open();
    }

    ~FileHandler() {
        close();
    }

    // 禁止拷贝
    FileHandler(const FileHandler&) = delete;
    FileHandler& operator=(const FileHandler&) = delete;
};
```

2. **业务逻辑封装**

```cpp
class BankAccount {
private:
    string accountNumber;
    double balance = 0.0;
    vector<string> transactionHistory;
    bool isLocked = false;

public:
    explicit BankAccount(string accNum)
        : accountNumber(std::move(accNum)) {}

    bool withdraw(double amount) {
        if (isLocked || amount <= 0 || balance < amount) {
            return false;
        }
        balance -= amount;
        transactionHistory.push_back("Withdraw: " + to_string(amount));
        return true;
    }
};
```

## 4. 最佳实践

1. **成员初始化**

   - 优先使用成员初始化器设置默认值
   - 构造函数使用成员初始化列表
   - 只在构造函数中处理非默认值情况

2. **构造函数**

   - 使用 `= default` 声明默认构造函数
   - 需要自定义时使用成员初始化列表
   - 使用 `std::move` 优化字符串等资源

3. **访问控制**
   - 结构体保持成员公开
   - 类遵循封装原则，成员私有

## 5. 注意事项

1. 成员初始化器的值会被构造函数的初始化列表覆盖
2. 成员的初始化顺序由声明顺序决定，不是初始化列表的顺序
3. 位域成员不能使用成员初始化器
4. 虚函数需要虚析构函数
5. 继承时要考虑基类的初始化
