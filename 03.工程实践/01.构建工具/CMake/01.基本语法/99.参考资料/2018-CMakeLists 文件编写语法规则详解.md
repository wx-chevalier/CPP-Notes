> [原文地址](https://blog.csdn.net/zhanghm1995/article/details/80902807)

# CMakeLists 文件编写语法规则详解

CMake 要求要求工程主目录和所有存放源代码子目录下都要编写 CMakeLists.txt 文件，注意大小写（CM 大写，Lists 中 L 要大写且不要落下 s)。CMake 变量使用${}方式取值,但是在 IF 控制语句中是直接使用变量名，环境变量使用 \$ENV{} 方式取值,使用 `SET(ENV{VAR} VALUE)` 赋值

```sh
指令(参数1 参数2…)
```

参数使用括弧括起,参数之间使用空格或分号分开。以 ADD_EXECUTABLE 指令为例：

```sh
ADD_EXECUTABLE(hello main.c func.c)
# 或者
ADD_EXECUTABLE(hello main.c;func.c)
```

注：指令是大小写无关的,参数和变量是大小写相关的。我一般习惯用小写。

# 常见 CMakeLists.txt 中指令剖析

- cmake_minimum_required(VERSION 2.6)命令：规定 cmake 软件的最低版本。这行命令是可选的，可以不写这句话，但在有些情况下，如果 CMakeLists.txt 文件中使用了一些高版本 cmake 特有的一些命令时，就需要加上这一行，提醒用户升级到该版本之后再执行 cmake。

- project(<projectname>)命令：语法：project(projectname [cxx] [c] [java])，

  可以指定工程采用的语言，选项分别表示：C++, C, java，如不指定默认支持所有语言。指定项目的名称，在生成 VS 项目下即指明了生成的.sln 项目的文件名。项目最终编译生成的可执行文件不一定是这个项目名称，而是由另一条命令(add_executable)指定的。

  通过 project 命令，为整个代码项目定义了一个管理架构，这点可以通过在 VS 下建立项目的结构层次来帮助理解。VS 下所有的项目都是在解决方案（.sln 文件）下进行管理的，一个解决方案可以包含多个项目。cmake 中 project 命令正是定义了解决方案的名称，add_executable 和 add_library 命令都会生成一个项目，cmake 会自动为每个项目（包括生成可执行文件的项目和生成链接库的项目）创建对应的文件夹，存储编译中间文件。

  但是这个项目名称还是很必要的，在 cmake 中有两个预定义的变量：`<projectname>_BINARY_DIR` 和 `<projectname>_SOURCE_DIR` ，即一旦使用了 project 指明了一个项目名称，则同时隐式定义了这两个预定义的变量。在内部编译情况下，这两个变量的含义相同，而在外部编译下，两者指代的内容会有所不同。要理解这两个变量的含义，我们首先需要了解什么是外部构建（out of source build），下面会介绍什么叫外部构建。

  同时 cmake 还预定义了`PROJECT_BINARY_DIR`和`PROJECT_SOURCE_DIR`变量，其中`PROJECT_BINARY_DIR`就等同于`<projectname>_BINARY_DIR,PROJECT_SOURCE_DIR`等同于`<projectname>_SOURCE_DIR`。因此在实际应用中，强烈推荐使用`PROJECT_BINARY_DIR`和`PROJECT_SOURCE_DIR`变量，这样即使项目名称发生变化也不会影响 CMakeLists.txt 文件。

- 外部构建的含义：外部构建和内部构建最大的不同在于执行 cmake 的工作路径不同，在我们使用 cmake 构建项目时，首先要求指定 **“Where is the source code"**，然后要求指定"Where to build the binaries"。

  此时如果这两个路径相同的话，cmake 生成的所有中间文件和可执行文件都会存放在项目目录（即所谓的根目录）中；而如果我们在项目目录下先新建一个 build 文件夹，然后将"Where to build the binaries"指定为该 build 文件夹的路径，此时中间文件和可执行文件都会存放在`build`目录下。第二种方法的优点显而易见，它最大限度的保持了代码目录的整洁，同时由于第二种方法的生成、编译和安装时发生在不同于项目目录的其他目录中，所以第二种方法就叫做 **“外部构建”**。
  回到 project 命令下的问题，在外部构建的情况下，`PROJECT_SOURCE_DIR`指向 的目录同内部构建一样，仍然是项目目录，而`PROJECT_BINARY_DIR`则有所不同，指向的是`<项目目录>/build` 目录。cmake 强烈推荐使用外部构建的方法。

- **ADD_SUBDIRECTORY 命令**
  **语法：** `ADD_SUBDIRECTORY(source_dir [binary_dir] [EXCLUDE_FROM_ALL])`
  该命令告诉 CMake 去**子目录**中查看可用的 CMakeLists.txt 文件
  指令用于向当前工程添加存放源文件的子目录,并可以指定中间二进制和目标二进制存放的位置。`EXCLUDE_FROM_ALL` 参数的含义是将这个目录从编译过程中排除。比如,工程的 example,可能就需要工程构建完成后,再进入 example 目录单独进行构建。
  在我们的项目中，我们添加了 src 目录到项目中，而把对应于 src 目录生成的中间文件和目标文件存放到 bin 目录下，在上一节举例中“外部构建”的情况下，中间文件和目标文件将存放在 build/srcobj 目录下
- **ADD_EXECUTABLE 命令**
  告诉工程生成一个**可执行文件**。该命令定义了工程最终生成的可执行文件的文件名以及参与编译的头文件和 cpp 文件。
  如果想指定生成的可执行文件的存放路径，可以设置 cmake 中预定义变量 EXECUTABLE_OUTPUT_PATH 的值。例如，将生成的可执行文件放置在 cmake 编译路径的 bin 文件夹下可用：`SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)`
- **ADD_LIBRARY 命令**
  **语法：**`ADD_LIBRARY(libname [SHARED|STATIC]`
  告诉工程生成一个库文件
- **FIND_LIBRARY 命令**
  查找库所在目录，语法如下：

```
find_library (
          <VAR>
          name | NAMES name1 [name2 ...] [NAMES_PER_DIR]
          [HINTS path1 [path2 ... ENV var]]
          [PATHS path1 [path2 ... ENV var]]
          [PATH_SUFFIXES suffix1 [suffix2 ...]]
          [DOC "cache documentation string"]
          [NO_DEFAULT_PATH]
          [NO_CMAKE_ENVIRONMENT_PATH]
          [NO_CMAKE_PATH]
          [NO_SYSTEM_ENVIRONMENT_PATH]
          [NO_CMAKE_SYSTEM_PATH]
          [CMAKE_FIND_ROOT_PATH_BOTH |
           ONLY_CMAKE_FIND_ROOT_PATH |
           NO_CMAKE_FIND_ROOT_PATH]
         )

```

举例：

```
FIND_LIBRARY(RUNTIME_LIB rt /usr/lib  /usr/local/lib NO_DEFAULT_PATH)
```

cmake 会在目录中查找，如果所有目录中都没有，值`RUNTIME_LIB`就会被赋为`NO_DEFAULT_PATH`

- **SET 命令——用于设置变量，相当于为变量取别名**
  **SET(CMAKE_BUILE_TYPE DEBUG)** 设置编译类型 debug 或者 release。debug 版会生成相关调试信息，可以使用 GDB 进行调试；release 不会生成调试信息。当无法进行调试时查看此处是否设置为 debug.
  SET(CMAKE_C_FLAGS_DEBUG “-g -Wall”) 设置编译器的类型
  CMAKE_C_FLAGS_DEBUG ---- C 编译器
  CMAKE_CXX_FLAGS_DEBUG ---- C++ 编译器
- **CMAKE_MODULE_PATH 变量——定义 cmake 模块所在路径**
  如果你的工程比较复杂,有可能会自己编写一些 **cmake 模块**（所谓 cmake 模块指的是在该文件内部定义了一些变量，这些变量指明了一些头文件路径和库文件路径等有用信息）。这些 cmake 模块是随你的工程发布的,为了让 cmake 在处理 CMakeLists.txt 时找到这些模块,你需要通过 SET 指令,将自己的 cmake 模块路径设置一下。
  比如
  `SET(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)`
  这时候你就可以通过 INCLUDE 指令来调用自己的模块了
- **EXECUTABLE_OUTPUT_PATH 和 LIBRARY_OUTPUT_PATH 变量**
  我们可以通过 `SET` 指令重新定义 `EXECUTABLE_OUTPUT_PATH` 和 `LIBRARY_OUTPUT_PATH` 变量来指定最终的目标二进制的位置(指最终生成的 CRNode 可执行文件或者最终的共享库，而不包含编译生成的中间文件)。
  命令如下：
  `SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)`
  `SET(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)`
  需要注意的是，在哪里 `ADD_EXECUTABLE` 或 `ADD_LIBRARY`,如果需要改变目标存放路径,就在哪里加入上述的定义。

## 从 VS 项目配置过程理解 CMakeLists 内容

由于在之前学习 C++编程一直是在 VS 中进行，当时就涉及到了众多有关头文件、链接库的配置过程，还有很多编译选项的设置，而 CMake 则相当于用自己的语法规则配置了整个项目，控制编译流程，因此可以从 VS 的配置过程来对照学习 CMake 中一些基本语法到底在做什么事。

采用**外部构建**项目时候编写的 CMakeLists.txt：
**1、在项目文件夹下新建一个 CMakeLists.txt**
同时新建一个文件夹 build 在此文件夹中执行 cmake …即可进行项目构建；（前提是项目文件夹下源文件已经准备好）
`set(TARGET_NAME rimeserver)`；设置项目的变量名字；
`PROJECT(${TARGET_NAME})`；
`cmake_minimum_required(VERSION 2.8.12)`
**2、CMakeLists.txt 文件中依赖库及文件的设置**

1. `include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)`
   包含 CMakeLists.txt 文件同级目录的 include 文件夹，相当于在 VS 中设置**包含文件**；
   2）`add_executable(${TARGET_NAME} ${SOURCES})`
   `${TARGET_NAME}`代表你设置的项目名称变量；
   `${SOURCES}`代表你的源文件变量；
   `add_executable`代表项目生成一个可执行文件；
   相当于在 VS 中新建一个项目时一步步填写的**项目名称**等。
   3）`add_dependencies(${TARGET_NAME} deploy)`
   deploy 代表你项目依赖的项目，保证 deploy 目标在其他的目标之前被构建；
   4）`target_link_libraries(${TARGET_NAME}`
   指定项目依赖的 lib 文件，相当于在 VS 中设置链接器下面的**链接库**；
   5）`set_target_properties(${TARGET_NAME} PROPERTIES LINK_FLAGS "/SUBSYSTEM:WINDOWS")`
   设置项目属性，相当于 VS 中项目属性设置页中的各种条目。

## CMake 中常用变量汇总

CMake 变量查询网站：https://gitlab.kitware.com/cmake/community/wikis/doc/cmake/Useful-Variables

**说明：**

- 引用变量内容需要用`${变量名}`格式

- 可以使用 message 命令查看变量具体内容

  ```shell
  message(STATUS "The CMAKE_VERSION  is ${CMAKE_VERSION}")
  message(STATUS "The CMAKE_SOURCE_DIR  is ${CMAKE_SOURCE_DIR}")
  12
  ```

- 可以在 CMakeLists.txt 文件中使用 set 命令设置某些变量值
  如`set(CMAKE_BUILD_TYPE "Release")`

- 可以通过 cmake**命令行参数**设置变量值
  如`cmake -DCMAKE_BUILD_TYPE=Release`，这种方式会被 CMakeLists.txt 中 set 命令设置的变量值覆盖

**常用变量：**

| 变量名                        | 含义                                                                  |
| ----------------------------- | --------------------------------------------------------------------- |
| **PROJECT_NAME**              | `project`命令中写的项目名                                             |
| **CMAKE_VERSION**             | 当前使用 CMake 的版本                                                 |
| **CMAKE_SOURCE_DIR**          | 工程顶层目录，即入口 CMakeLists 文件所在路径                          |
| **PROJECT_SOURCE_DIR**        | 同**CMAKE_SOURCE_DIR**                                                |
| **CMAKE_BINARY_DIR**          | 工程编译发生的目录，即执行 cmake 命令进行项目配置的目录，一般为 build |
| **PROJECT_BINARY_DIR**        | 同**CMAKE_BINARY_DIR**                                                |
| **CMAKE_CURRENT_SOURCE_DIR**  | 当前处理的 CMakeLists.txt 所在的路径                                  |
| **CMAKE_CURRRENT_BINARY_DIR** | 当前处理的 CMakeLists.txt 中生成目标文件所在编译目录                  |
| **CMAKE_CURRENT_LIST_FILE**   | 输出调用这个变量的 CMakeLists.txt 文件的完整路径                      |
| **CMAKE_CURRENT_LIST_DIR**    | 当前处理的 CMakeLists.txt 文件所在目录的路径                          |
| **CMAKE_INSTALL_PREFIX**      | 指定`make install`命令执行时包安装路径                                |
| **CMAKE_MODULE_PATH**         | `find_package`命令搜索包路径**之一**，默认为空                        |

**编译配置相关变量：**

| 变量名                 | 含义                                                                              |
| ---------------------- | --------------------------------------------------------------------------------- |
| **CMAKE_BUILD_TYPE**   | 编译选项，Release 或者 Debug，如`set(CMAKE_BUILD_TYPE "Release")`                 |
| **CMAKE_CXX_FLAGS**    | 编译标志，设置 C++11 编译，`set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")` |
| **CMAKE_CXX_STANDARD** | 也可以设置 C++11 编译，`set(CMAKE_CXX_STANDARD 11)`                               |

## 常用 CMakeLists 文件模板

### 基础模板

```shell
## This is a standard CMakeLists.txt template, including
## how to enable C++11 compiler option and how to set 'Release'
## target compile option

cmake_minimum_required(VERSION 2.8)
project(cmake_template)

############# Use message function to print information ############
message(STATUS "CMake version: ${CMAKE_VERSION}")

############ enable C++11 compiler###############
## Method 1
include(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)
CHECK_CXX_COMPILER_FLAG("-std=c++0x" COMPILER_SUPPORTS_CXX0X)
if(COMPILER_SUPPORTS_CXX11)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
elseif(COMPILER_SUPPORTS_CXX0X)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
else()
    message(WARNING "The compiler ${CMAKE_CXX_COMPILER} has no C++11 support.")
    message(STATUS "Please use a different C++ compiler.")
endif()
## Method 2
set(CMAKE_CXX_STANDARD 11)


############ #enable warnings##################
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC -Wall")


#############Set cmake compiler options##############
## Method 1
set(CMAKE_BUILD_TYPE "Release")

## Method 2
IF(NOT CMAKE_BUILD_TYPE)
  SET(CMAKE_BUILD_TYPE RelWithDebInfo CACHE STRING
      "Choose the type of build, options are: None Debug Release RelWithDebInfo MinSizeRel."
      FORCE)
ENDIF(NOT CMAKE_BUILD_TYPE)

################## Some useful arguments ##########################
set(CMAKE_INCLUDE_CURRENT_DIR ON)

##########
##Build###
##########
add_library(lib hello.cpp)

add_executable(main main.cpp)
target_link_libraries(main ${LINK_LIBRARIES})
12345678910111213141516171819202122232425262728293031323334353637383940414243444546474849505152
```

### 使用 OpenCV 库 CMakeLists 文件模板

```shell
cmake_minimum_required(VERSION 2.8.12)
project(OpenCV_test)

#find packages
find_package(OpenCV REQUIRED)

#include headers
include_directories(${OpenCV_INCLUDE_DIRS})

#target generation
add_executable(OpenCV_test main.cpp)

#link libraries
target_link_libraries(OpenCV_test ${OpenCV_LIBS})
1234567891011121314
```

### 使用 PCL 库 CMakeLists 文件模板

```shell
cmake_minimum_required(VERSION 2.8.12)
project(PCL_test)

#find packages
find_package(PCL 1.7 REQUIRED)
add_definitions(${PCL_DEFINITIONS})

#include headers
include_directories(${PCL_INCLUDE_DIRS})

#target generation
add_executable(PCL_test main.cpp)

#link libraries
target_link_libraries(PCL_test ${PCL_LIBRARIES})
123456789101112131415
```

### 使用 Eigen 库 CMakeLists 文件模板

```shell
cmake_minimum_required(VERSION 2.8.12)
project(Eigen_test)

#find packages
find_package(Eigen3 REQUIRED)

#include headers
include_directories(${EIGEN3_INCLUDE_DIR})

#target generation
add_executable(Eigen_test main.cpp)
```
