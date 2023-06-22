# CMake 新旧版本对比

现代 CMake 指的是 CMake 3.x。古代 CMake 指的是 CMake 2.x。现代 CMake 和古代 CMake 相比，使用更方便，功能更强大。

## **古代 CMake 的命令行调用**

相信大家都很熟悉了，在使用古代 CMake 时，需要先手动创建一个名为`build`的文件夹，然后手动进入该文件夹，再`cmake ..`配置上一层文件夹的内容到`build`文件夹。在生成`Makefile`后，再使用`make`命令进行编译以及后续的安装，如下所示：

```bash
mkdir -p build
cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
make -j4
make install
```

## **现代 CMake 的命令行调用**

- 在现代 CMake 中，我们不再需要手动创建`build`文件夹，而是直接使用`cmake -B build`命令即可。

- `cmake --build build`命令则增强了**通用性**。

- - 在 Windows 系统中会自动调用 VS 的编译器去进行编译
  - 在 Linux 系统中会自动调用 make 命令进行编译

```bash
cmake -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --parallel 4
cmake --build build --target install
```

## **古代 CMake 的 CMakeLists.txt 写法**

在 CMakeLists.txt 文件的写法上，古代与现代也有不同。

如果我们想要找到 TBB 库，先要通过`list(APPEND CMAKE_MODULE_PATH "<path to FindTBB module>")`命令去网上下载下来，然后把它加到`CMAKE_MODULE_PATH`里。然后才能通过命令`find_package(TBB COMPONENTS tbb tbbmalloc)`继续寻找 TBB 库，还需通过 if 语句来判断是否找到，如果没有找到需要手动添加报错信息。

对于要生成的可执行文件（一个 target），我们需要手动通过`target_include_directories(myapp ${TBB_INCLUDE_DIRS})`来指定头文件目录，通过`target_compile_definitions(myapp ${TBB_DEFINITIONS})`添加定义目录，通过`target_link_libraries(myapp ${TBB_LIBRARIES})`来指定要链接的库。

```cmake
cmake_minimum_required (VERSION 2.8)
project(MyProject)

list(APPEND CMAKE_MODULE_PATH "<path to FindTBB module>")

find_package(TBB COMPONENTS tbb tbbmalloc)
if (NOT TBB_FOUND)
    message(FATAL__ERROR "TBB not found")
endif()

add_executable(myapp myapp)
target_include_directories(myapp ${TBB_INCLUDE_DIRS})
target_compile_definitions(myapp ${TBB_DEFINITIONS})
target_link_libraries(myapp ${TBB_LIBRARIES})
```

## **现代 CMake 的 CMakeLists.txt 写法**

在现代 CMake 中，如果在 TBB 的库中使用了`PUBLIC`，那么我们只要链接了 TBB 库，TBB 库上自带的头文件目录等等，就会传播给 myapp，不需要再一个个添加，**只需链接上 TBB 即可**。

```cmake
cmake_minimum_required (VERSION 3.12)
project(MyProject LANGUAGES CXX)

find_package(TBB COMPONENTS tbb tbbmalloc REQUIRED)

add_executable(myapp myapp)
target_link_libraries(myapp TBB::tbb TBB : : tbbmalloc)
```
