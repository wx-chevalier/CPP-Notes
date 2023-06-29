# unique_ptr

unique_ptr 不共享它的指针。它无法复制到其他 unique_ptr，无法通过值传递到函数，也无法用于需要副本的任何 C++ 标准库算法。只能移动 unique_ptr。这意味着，内存资源所有权将转移到另一 unique_ptr，并且原始 unique_ptr 不再拥有此资源。我们建议你将对象限制为由一个所有者所有，因为多个所有权会使程序逻辑变得复杂。因此，当需要智能指针用于纯 C++ 对象时，可使用 unique_ptr，而当构造 unique_ptr 时，可使用 make_unique Helper 函数。

下图演示了两个 unique_ptr 实例之间的所有权转换。

![所有权变化](https://learn.microsoft.com/zh-cn/cpp/cpp/media/unique_ptr.png?view=msvc-170)

unique_ptr 在 C++ 标准库的 <memory> 标头中定义。它与原始指针一样高效，可在 C++ 标准库容器中使用。将 unique_ptr 实例添加到 C++ 标准库容器很有效，因为通过 unique_ptr 的移动构造函数，不再需要进行复制操作。

# 示例

以下示例演示如何创建 unique_ptr 实例并在函数之间传递这些实例。

```cpp
unique_ptr<Song> SongFactory(const std::wstring& artist, const std::wstring& title)
{
    // Implicit move operation into the variable that stores the result.
    return make_unique<Song>(artist, title);
}

void MakeSongs()
{
    // Create a new unique_ptr with a new object.
    auto song = make_unique<Song>(L"Mr. Children", L"Namonaki Uta");

    // Use the unique_ptr.
    vector<wstring> titles = { song->title };

    // Move raw pointer from one unique_ptr to another.
    unique_ptr<Song> song2 = std::move(song);

    // Obtain unique_ptr from function that returns by value.
    auto song3 = SongFactory(L"Michael Jackson", L"Beat It");
}
```

这些示例说明了 unique_ptr 的基本特征：可移动，但不可复制。“移动”将所有权转移到新 unique_ptr 并重置旧 unique_ptr。

以下示例演示如何创建 unique_ptr 实例并在向量中使用这些实例。

```cpp
void SongVector()
{
    vector<unique_ptr<Song>> songs;

    // Create a few new unique_ptr<Song> instances
    // and add them to vector using implicit move semantics.
    songs.push_back(make_unique<Song>(L"B'z", L"Juice"));
    songs.push_back(make_unique<Song>(L"Namie Amuro", L"Funky Town"));
    songs.push_back(make_unique<Song>(L"Kome Kome Club", L"Kimi ga Iru Dake de"));
    songs.push_back(make_unique<Song>(L"Ayumi Hamasaki", L"Poker Face"));

    // Pass by const reference when possible to avoid copying.
    for (const auto& song : songs)
    {
        wcout << L"Artist: " << song->artist << L"   Title: " << song->title << endl;
    }
}
```

在 range for 循环中，注意 unique_ptr 通过引用来传递。如果你尝试通过此处的值传递，由于删除了 unique_ptr 复制构造函数，编译器将引发错误。

以下示例演示如何初始化类成员 unique_ptr。

```cpp
class MyClass
{
private:
    // MyClass owns the unique_ptr.
    unique_ptr<ClassFactory> factory;
public:

    // Initialize by using make_unique with ClassFactory default constructor.
    MyClass() : factory (make_unique<ClassFactory>())
    {
    }

    void MakeClass()
    {
        factory->DoSomething();
    }
};
```

可使用 make_unique 将 unique_ptr 创建到数组，但无法使用 make_unique 初始化数组元素。

```cpp
// Create a unique_ptr to an array of 5 integers.
auto p = make_unique<int[]>(5);

// Initialize the array.
for (int i = 0; i < 5; ++i)
{
    p[i] = i;
    wcout << p[i] << endl;
}
```
