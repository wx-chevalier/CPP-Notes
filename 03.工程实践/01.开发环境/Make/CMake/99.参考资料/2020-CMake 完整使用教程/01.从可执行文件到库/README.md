# 从可执行文件到库

本章的主要内容有：

- 将单个源码文件编译为可执行文件
- 切换生成器
- 构建和连接静态库与动态库
- 用条件语句控制编译
- 向用户显示选项
- 指定编译器
- 切换构建类型
- 设置编译器选项
- 为语言设定标准
- 使用控制流进行构造

本章的示例将指导您完成构建代码所需的基本任务：编译可执行文件、编译库、根据用户输入执行构建操作等等。CMake 是一个构建系统生成器，特别适合于独立平台和编译器。除非另有说明，否则所有配置都独立于操作系统，它们可以在 GNU/Linux、macOS 和 Windows 的系统下运行。

本书的示例主要为 C++项目设计，并使用 C++示例进行了演示，但 CMake 也可以用于其他语言的项目，包括 C 和 Fortran。我们会尝试一些有意思的配置，其中包含了一些 C++、C 和 Fortran 语言示例。您可以根据自己喜好，选择性了解。有些示例是定制的，以突出在选择特定语言时需要面临的挑战。

# 1.2 切换生成器

**NOTE**:_此示例代码可以在 codes/chapter-01/recipe-02 中找到，其中有一个 C++、C 和 Fortran 示例。该配置在 CMake 3.5 版(或更高版本)下测试没问题，并且已经在 GNU/Linux、macOS 和 Windows 上进行了测试。_

CMake 是一个构建系统生成器，可以使用单个 CMakeLists.txt 为不同平台上的不同工具集配置项目。您可以在 CMakeLists.txt 中描述构建系统必须运行的操作，以配置并编译代码。基于这些指令，CMake 将为所选的构建系统(Unix Makefile、Ninja、Visual Studio 等等)生成相应的指令。我们将在第 13 章中重新讨论生成器。

## 准备工作

CMake 针对不同平台支持本地构建工具列表。同时支持命令行工具(如 Unix Makefile 和 Ninja)和集成开发环境(IDE)工具。用以下命令，可在平台上找到生成器名单，以及已安装的 CMake 版本：

```
$ cmake --help
```

这个命令的输出，将列出 CMake 命令行界面上所有的选项，您会找到可用生成器的列表。例如，安装了 CMake 3.11.2 的 GNU/Linux 机器上的输出：

```
Generators
The following generators are available on this platform:
Unix Makefiles = Generates standard UNIX makefiles.
Ninja = Generates build.ninja files.
Watcom WMake = Generates Watcom WMake makefiles.
CodeBlocks - Ninja = Generates CodeBlocks project files.
CodeBlocks - Unix Makefiles = Generates CodeBlocks project files.
CodeLite - Ninja = Generates CodeLite project files.
CodeLite - Unix Makefiles = Generates CodeLite project files.
Sublime Text 2 - Ninja = Generates Sublime Text 2 project files.
Sublime Text 2 - Unix Makefiles = Generates Sublime Text 2 project files.
Kate - Ninja = Generates Kate project files.
Kate - Unix Makefiles = Generates Kate project files.
Eclipse CDT4 - Ninja = Generates Eclipse CDT 4.0 project files.
Eclipse CDT4 - Unix Makefiles= Generates Eclipse CDT 4.0 project files.
```

使用此示例，我们将展示为项目切换生成器是多么**EASY**。

## 具体实施

我们将重用前一节示例中的`hello-world.cpp`和`CMakeLists.txt`。惟一的区别在使用 CMake 时，因为现在必须显式地使用命令行方式，用`-G`切换生成器。

1. 首先，使用以下步骤配置项目:

   ```
   $ mkdir -p build
   $ cd build
   $ cmake -G Ninja ..

   -- The CXX compiler identification is GNU 8.1.0
   -- Check for working CXX compiler: /usr/bin/c++
   -- Check for working CXX compiler: /usr/bin/c++ -- works
   -- Detecting CXX compiler ABI info
   -- Detecting CXX compiler ABI info - done
   -- Detecting CXX compile features
   -- Detecting CXX compile features - done
   -- Configuring done
   -- Generating done
   -- Build files have been written to: /home/user/cmake-cookbook/chapter-01/recipe-02/cxx-exampl
   ```

2. 第二步，构建项目：

   ```
   $ cmake --build .

   [2/2] Linking CXX executable hello-world
   ```

## 如何工作

与前一个配置相比，每一步的输出没什么变化。每个生成器都有自己的文件集，所以编译步骤的输出和构建目录的内容是不同的：

- `build.ninja`和`rules.ninja`：包含 Ninja 的所有的构建语句和构建规则。
- `CMakeCache.txt`：CMake 会在这个文件中进行缓存，与生成器无关。
- `CMakeFiles`：包含由 CMake 在配置期间生成的临时文件。
- `cmake_install.cmake`：CMake 脚本处理安装规则，并在安装时使用。

`cmake --build .`将`ninja`命令封装在一个跨平台的接口中。

## 更多信息

我们将在第 13 章中讨论可选生成器和交叉编译。

要了解关于生成器的更多信息，CMake 官方文档是一个很好的选择: https://cmake.org/cmake/help/latest/manual/cmake-generators.7.html

# 1.3 构建和链接静态库和动态库

**NOTE**: _这个示例代码可以在 codes/chapter-01/recipe-03 找到，其中有 C++和 Fortran 示例。该配置在 CMake 3.5 版(或更高版本)测试有效的，并且已经在 GNU/Linux、macOS 和 Windows 上进行了测试。_

项目中会有单个源文件构建的多个可执行文件的可能。项目中有多个源文件，通常分布在不同子目录中。这种实践有助于项目的源代码结构，而且支持模块化、代码重用和关注点分离。同时，这种分离可以简化并加速项目的重新编译。本示例中，我们将展示如何将源代码编译到库中，以及如何链接这些库。

## 准备工作

回看第一个例子，这里并不再为可执行文件提供单个源文件，我们现在将引入一个类，用来包装要打印到屏幕上的消息。更新一下的`hello-world.cpp`:

```cpp
#include "Message.hpp"

#include <cstdlib>
#include <iostream>

int main() {
  Message say_hello("Hello, CMake World!");
  std::cout << say_hello << std::endl;

  Message say_goodbye("Goodbye, CMake World");
  std::cout << say_goodbye << std::endl;

  return EXIT_SUCCESS;
}
```

`Message`类包装了一个字符串，并提供重载过的`<<`操作，并且包括两个源码文件：`Message.hpp`头文件与`Message.cpp`源文件。`Message.hpp`中的接口包含以下内容：

```cpp
#pragma once

#include <iosfwd>
#include <string>

class Message {
public:
  Message(const std::string &m) : message_(m) {}
  friend std::ostream &operator<<(std::ostream &os, Message &obj) {
    return obj.printObject(os);
  }
private:
  std::string message_;
  std::ostream &printObject(std::ostream &os);
};
```

`Message.cpp`实现如下：

```cpp
#include "Message.hpp"

#include <iostream>
#include <string>

std::ostream &Message::printObject(std::ostream &os) {
  os << "This is my very nice message: " << std::endl;
  os << message_;
  return os;
}
```

## 具体实施

这里有两个文件需要编译，所以`CMakeLists.txt`必须进行修改。本例中，先把它们编译成一个库，而不是直接编译成可执行文件:

1. 创建目标——静态库。库的名称和源码文件名相同，具体代码如下：

   ```
   add_library(message
     STATIC
       Message.hpp
       Message.cpp
     )
   ```

2. 创建`hello-world`可执行文件的目标部分不需要修改：

   ```
   add_executable(hello-world hello-world.cpp)
   ```

3. 最后，将目标库链接到可执行目标：

   ```
   target_link_libraries(hello-world message)
   ```

4. 对项目进行配置和构建。库编译完成后，将连接到`hello-world`可执行文件中：

   ```
   $ mkdir -p build
   $ cd build
   $ cmake ..
   $ cmake --build .

   Scanning dependencies of target message
   [ 25%] Building CXX object CMakeFiles/message.dir/Message.cpp.o
   [ 50%] Linking CXX static library libmessage.a
   [ 50%] Built target message
   Scanning dependencies of target hello-world
   [ 75%] Building CXX object CMakeFiles/hello-world.dir/hello-world.cpp.o
   [100%] Linking CXX executable hello-world
   [100%] Built target hello-world
   ```

   ```
   $ ./hello-world

   This is my very nice message:
   Hello, CMake World!
   This is my very nice message:
   Goodbye, CMake World
   ```

## 工作原理

本节引入了两个新命令：

- `add_library(message STATIC Message.hpp Message.cpp)`：生成必要的构建指令，将指定的源码编译到库中。`add_library`的第一个参数是目标名。整个`CMakeLists.txt`中，可使用相同的名称来引用库。生成的库的实际名称将由 CMake 通过在前面添加前缀`lib`和适当的扩展名作为后缀来形成。生成库是根据第二个参数(`STATIC`或`SHARED`)和操作系统确定的。
- `target_link_libraries(hello-world message)`: 将库链接到可执行文件。此命令还确保`hello-world`可执行文件可以正确地依赖于消息库。因此，在消息库链接到`hello-world`可执行文件之前，需要完成消息库的构建。

编译成功后，构建目录包含`libmessage.a`一个静态库(在 GNU/Linux 上)和`hello-world`可执行文件。

CMake 接受其他值作为`add_library`的第二个参数的有效值，我们来看下本书会用到的值：

- **STATIC**：用于创建静态库，即编译文件的打包存档，以便在链接其他目标时使用，例如：可执行文件。
- **SHARED**：用于创建动态库，即可以动态链接，并在运行时加载的库。可以在`CMakeLists.txt`中使用`add_library(message SHARED Message.hpp Message.cpp)`从静态库切换到动态共享对象(DSO)。
- **OBJECT**：可将给定`add_library`的列表中的源码编译到目标文件，不将它们归档到静态库中，也不能将它们链接到共享对象中。如果需要一次性创建静态库和动态库，那么使用对象库尤其有用。我们将在本示例中演示。
- **MODULE**：又为 DSO 组。与`SHARED`库不同，它们不链接到项目中的任何目标，不过可以进行动态加载。该参数可以用于构建运行时插件。

CMake 还能够生成特殊类型的库，这不会在构建系统中产生输出，但是对于组织目标之间的依赖关系，和构建需求非常有用：

- **IMPORTED**：此类库目标表示位于项目外部的库。此类库的主要用途是，对现有依赖项进行构建。因此，`IMPORTED`库将被视为不可变的。我们将在本书的其他章节演示使用`IMPORTED`库的示例。参见: https://cmake.org/cmake/help/latest/manual/cmakebuildsystem.7.html#imported-targets
- **INTERFACE**：与`IMPORTED`库类似。不过，该类型库可变，没有位置信息。它主要用于项目之外的目标构建使用。我们将在本章第 5 节中演示`INTERFACE`库的示例。参见: https://cmake.org/cmake/help/latest/manual/cmake-buildsystem.7.html#interface-libraries
- **ALIAS**：顾名思义，这种库为项目中已存在的库目标定义别名。不过，不能为`IMPORTED`库选择别名。参见: https://cmake.org/cmake/help/latest/manual/cmake-buildsystem.7.html#alias-libraries

本例中，我们使用`add_library`直接集合了源代码。后面的章节中，我们将使用`target_sources`汇集源码，特别是在第 7 章。请参见 Craig Scott 的这篇精彩博文: https://crascit.com/2016/01/31/enhanced-source-file-handling-with-target_sources/ ，其中有对`target_sources`命令的具体使用。

## 更多信息

现在展示`OBJECT`库的使用，修改`CMakeLists.txt`，如下：

```sh
cmake_minimum_required(VERSION 3.5 FATAL_ERROR)
project(recipe-03 LANGUAGES CXX)

add_library(message-objs
	OBJECT
		Message.hpp
		Message.cpp
	)

# this is only needed for older compilers
# but doesn't hurt either to have it
set_target_properties(message-objs
	PROPERTIES
		POSITION_INDEPENDENT_CODE 1
	)

add_library(message-shared
	SHARED
		$<TARGET_OBJECTS:message-objs>
	)

add_library(message-static
	STATIC
		$<TARGET_OBJECTS:message-objs>
	)

add_executable(hello-world hello-world.cpp)

target_link_libraries(hello-world message-static)
```

首先，`add_library`改为`add_library(Message-objs OBJECT Message.hpp Message.cpp)`。此外，需要保证编译的目标文件与生成位置无关。可以通过使用`set_target_properties`命令，设置`message-objs`目标的相应属性来实现。

**NOTE**: _可能在某些平台和/或使用较老的编译器上，需要显式地为目标设置`POSITION_INDEPENDENT_CODE`属性。_

现在，可以使用这个对象库来获取静态库(`message-static`)和动态库(`message-shared`)。要注意引用对象库的生成器表达式语法:`$<TARGET_OBJECTS:message-objs>`。生成器表达式是 CMake 在生成时(即配置之后)构造，用于生成特定于配置的构建输出。参见: https://cmake.org/cmake/help/latest/manual/cmake-generator-expressions.7.html 。我们将在第 5 章中深入研究生成器表达式。最后，将`hello-world`可执行文件链接到消息库的静态版本。

是否可以让 CMake 生成同名的两个库？换句话说，它们都可以被称为`message`，而不是`message-static`和`message-share` 吗？我们需要修改这两个目标的属性：

```sh
add_library(message-shared
  SHARED
    $<TARGET_OBJECTS:message-objs>
	)

set_target_properties(message-shared
	PROPERTIES
		OUTPUT_NAME "message"
	)

add_library(message-static
	STATIC
		$<TARGET_OBJECTS:message-objs>
	)

set_target_properties(message-static
	PROPERTIES
		OUTPUT_NAME "message"
	)
```

我们可以链接到 DSO 吗？这取决于操作系统和编译器：

1. GNU/Linux 和 macOS 上，不管选择什么编译器，它都可以工作。
2. Windows 上，不能与 Visual Studio 兼容，但可以与 MinGW 和 MSYS2 兼容。

这是为什么呢？生成好的 DSO 组需要程序员限制符号的可见性。需要在编译器的帮助下实现，但不同的操作系统和编译器上，约定不同。CMake 有一个机制来处理这个问题，我们将在第 10 章中解释它如何工作。

# 1.4 用条件句控制编译

**NOTE**:_这个示例代码可以在 codes/chapter-01/recipe-04 找到，其中有一个 C++示例。该配置在 CMake 3.5 版(或更高版本)测试有效的，并且已经在 GNU/Linux、macOS 和 Windows 上进行了测试。_

目前为止，看到的示例比较简单，CMake 执行流是线性的：从一组源文件到单个可执行文件，也可以生成静态库或动态库。为了确保完全控制构建项目、配置、编译和链接所涉及的所有步骤的执行流，CMake 提供了自己的语言。本节中，我们将探索条件结构`if-else- else-endif`的使用。

**NOTE**: _CMake 语言相当庞杂，由基本的控制结构、特定于 CMake 的命令和使用新函数模块化扩展语言的基础设施组成。完整的概览可以在这里找到: https://cmake.org/cmake/help/latest/manual/cmake-language.7.html_

## 具体实施

从与上一个示例的的源代码开始，我们希望能够在不同的两种行为之间进行切换：

1. 将 `Message.hpp` 和 `Message.cpp` 构建成一个库(静态或动态)，然后将生成库链接到`hello-world`可执行文件中。
2. 将 `Message.hpp`，`Message.cpp`和`hello-world.cpp`构建成一个可执行文件，但不生成任何一个库。

让我们来看看如何使用`CMakeLists.txt`来实现：

1. 首先，定义最低 CMake 版本、项目名称和支持的语言：

   ```
   cmake_minimum_required(VERSION 3.5 FATAL_ERROR)
   project(recipe-04 LANGUAGES CXX)
   ```

2. 我们引入了一个新变量`USE_LIBRARY`，这是一个逻辑变量，值为`OFF`。我们还打印了它的值：

   ```
   set(USE_LIBRARY OFF)

   message(STATUS "Compile sources into a library? ${USE_LIBRARY}")
   ```

3. CMake 中定义`BUILD_SHARED_LIBS`全局变量，并设置为`OFF`。调用`add_library`并省略第二个参数，将构建一个静态库：

   ```
   set(BUILD_SHARED_LIBS OFF)
   ```

4. 然后，引入一个变量`_sources`，包括`Message.hpp`和`Message.cpp`：

   ```
   list(APPEND _sources Message.hpp Message.cpp)
   ```

5. 然后，引入一个基于`USE_LIBRARY`值的`if-else`语句。如果逻辑为真，则`Message.hpp`和`Message.cpp`将打包成一个库：

   ```sh
   if(USE_LIBRARY)
   	# add_library will create a static library
   	# since BUILD_SHARED_LIBS is OFF
   	add_library(message ${_sources})
   	add_executable(hello-world hello-world.cpp)
   	target_link_libraries(hello-world message)
   else()
   	add_executable(hello-world hello-world.cpp ${_sources})
   endif()
   ```

6. 我们可以再次使用相同的命令集进行构建。由于`USE_LIBRARY`为`OFF`, `hello-world`可执行文件将使用所有源文件来编译。可以通过在 GNU/Linux 上，运行`objdump -x`命令进行验证。

## 工作原理

我们介绍了两个变量：`USE_LIBRARY`和`BUILD_SHARED_LIBS`。这两个变量都设置为`OFF`。如 CMake 语言文档中描述，逻辑真或假可以用多种方式表示：

- 如果将逻辑变量设置为以下任意一种：`1`、`ON`、`YES`、`true`、`Y`或非零数，则逻辑变量为`true`。
- 如果将逻辑变量设置为以下任意一种：`0`、`OFF`、`NO`、`false`、`N`、`IGNORE、NOTFOUND`、空字符串，或者以`-NOTFOUND`为后缀，则逻辑变量为`false`。

`USE_LIBRARY`变量将在第一个和第二个行为之间切换。`BUILD_SHARED_LIBS`是 CMake 的一个全局标志。因为 CMake 内部要查询`BUILD_SHARED_LIBS`全局变量，所以`add_library`命令可以在不传递`STATIC/SHARED/OBJECT`参数的情况下调用；如果为`false`或未定义，将生成一个静态库。

这个例子说明，可以引入条件来控制 CMake 中的执行流。但是，当前的设置不允许从外部切换，不需要手动修改`CMakeLists.txt`。原则上，我们希望能够向用户开放所有设置，这样就可以在不修改构建代码的情况下调整配置，稍后将展示如何做到这一点。

**NOTE**:_`else()`和`endif()`中的`()`，可能会让刚开始学习 CMake 代码的同学感到惊讶。其历史原因是，因为其能够指出指令的作用范围。例如，可以使用`if(USE_LIBRARY)…else(USE_LIBRARY)…endif(USE_LIBIRAY)`。这个格式并不唯一，可以根据个人喜好来决定使用哪种格式。_

**TIPS**:_`_sources`变量是一个局部变量，不应该在当前范围之外使用，可以在名称前加下划线。_

# 1.5 向用户显示选项

**NOTE**: _这个示例代码可以在 codes/chapter-01/recipe-05 找到，其中有一个 C++示例。该配置在 CMake 3.5 版(或更高版本)测试有效的，并且已经在 GNU/Linux、macOS 和 Windows 上进行了测试。_

前面的配置中，我们引入了条件句：通过硬编码的方式给定逻辑变量值。不过，这会影响用户修改这些变量。CMake 代码没有向读者传达，该值可以从外部进行修改。推荐在`CMakeLists.txt`中使用`option()`命令，以选项的形式显示逻辑开关，用于外部设置，从而切换构建系统的生成行为。本节的示例将向您展示，如何使用这个命令。

## 具体实施

看一下前面示例中的静态/动态库示例。与其硬编码`USE_LIBRARY`为`ON`或`OFF`，现在为其设置一个默认值，同时也可以从外部进行更改：

1. 用一个选项替换上一个示例的`set(USE_LIBRARY OFF)`命令。该选项将修改`USE_LIBRARY`的值，并设置其默认值为`OFF`：

   ```
   option(USE_LIBRARY "Compile sources into a library" OFF)
   ```

2. 现在，可以通过 CMake 的`-D`CLI 选项，将信息传递给 CMake 来切换库的行为：

   ```
   $ mkdir -p build
   $ cd build
   $ cmake -D USE_LIBRARY=ON ..

   -- ...
   -- Compile sources into a library? ON
   -- ...

   $ cmake --build .

   Scanning dependencies of target message
   [ 25%] Building CXX object CMakeFiles/message.dir/Message.cpp.o
   [ 50%] Linking CXX static library libmessage.a
   [ 50%] Built target message
   Scanning dependencies of target hello-world
   [ 75%] Building CXX object CMakeFiles/hello-world.dir/hello-world.cpp.o
   [100%] Linking CXX executable hello-world
   [100%] Built target hello-world
   ```

`-D`开关用于为 CMake 设置任何类型的变量：逻辑变量、路径等等。

## 工作原理

`option`可接受三个参数：

```
option(<option_variable> "help string" [initial value])
```

- `<option_variable>`表示该选项的变量的名称。
- `"help string"`记录选项的字符串，在 CMake 的终端或图形用户界面中可见。
- `[initial value]`选项的默认值，可以是`ON`或`OFF`。

## 更多信息

有时选项之间会有依赖的情况。示例中，我们提供生成静态库或动态库的选项。但是，如果没有将`USE_LIBRARY`逻辑设置为`ON`，则此选项没有任何意义。CMake 提供`cmake_dependent_option()`命令用来定义依赖于其他选项的选项：

```
include(CMakeDependentOption)

# second option depends on the value of the first
cmake_dependent_option(
	MAKE_STATIC_LIBRARY "Compile sources into a static library" OFF
	"USE_LIBRARY" ON
	)

# third option depends on the value of the first
cmake_dependent_option(
	MAKE_SHARED_LIBRARY "Compile sources into a shared library" ON
	"USE_LIBRARY" ON
	)
```

如果`USE_LIBRARY`为`ON`，`MAKE_STATIC_LIBRARY`默认值为`OFF`，否则`MAKE_SHARED_LIBRARY`默认值为`ON`。可以这样运行：

```
$ cmake -D USE_LIBRARY=OFF -D MAKE_SHARED_LIBRARY=ON ..
```

这仍然不会构建库，因为`USE_LIBRARY`仍然为`OFF`。

CMake 有适当的机制，通过包含模块来扩展其语法和功能，这些模块要么是 CMake 自带的，要么是定制的。本例中，包含了一个名为`CMakeDependentOption`的模块。如果没有`include`这个模块，`cmake_dependent_option()`命令将不可用。参见 https://cmake.org/cmake/help/latest/module/CMakeDependentOption.html

**TIPS**:_手册中的任何模块都可以以命令行的方式使用`cmake --help-module <name-of-module>`。例如，`cmake --help-module CMakeDependentOption`将打印刚才讨论的模块的手册页(帮助页面)。_

# 1.6 指定编译器

**NOTE**:_此示例代码可以在 codes/chapter-01/recipe-06 中找到，其中有一个 C++/C 示例。该配置在 CMake 3.5 版(或更高版本)下测试没问题，并且已经在 GNU/Linux、macOS 和 Windows 上进行了测试。_

目前为止，我们还没有过多考虑如何选择编译器。CMake 可以根据平台和生成器选择编译器，还能将编译器标志设置为默认值。然而，我们通常控制编译器的选择。在后面的示例中，我们还将考虑构建类型的选择，并展示如何控制编译器标志。

## 具体实施

如何选择一个特定的编译器？例如，如果想使用 Intel 或 Portland Group 编译器怎么办？CMake 将语言的编译器存储在`CMAKE_<LANG>_COMPILER`变量中，其中`<LANG>`是受支持的任何一种语言，对于我们的目的是`CXX`、`C`或`Fortran`。用户可以通过以下两种方式之一设置此变量：

1. 使用 CLI 中的`-D`选项，例如：

   ```
   $ cmake -D CMAKE_CXX_COMPILER=clang++ ..
   ```

2. 通过导出环境变量`CXX`(C++编译器)、`CC`(C 编译器)和`FC`(Fortran 编译器)。例如，使用这个命令使用`clang++`作为`C++`编译器：

   ```
   $ env CXX=clang++ cmake ..
   ```

到目前为止讨论的示例，都可以通过传递适当的选项，配置合适的编译器。

**NOTE**:_CMake 了解运行环境，可以通过其 CLI 的`-D`开关或环境变量设置许多选项。前一种机制覆盖后一种机制，但是我们建议使用`-D`显式设置选项。显式优于隐式，因为环境变量可能被设置为不适合(当前项目)的值。_

我们在这里假设，其他编译器在标准路径中可用，CMake 在标准路径中执行查找编译器。如果不是这样，用户将需要将完整的编译器可执行文件或包装器路径传递给 CMake。

**TIPS**:_我们建议使用`-D CMAKE_<LANG>_COMPILER`CLI 选项设置编译器，而不是导出`CXX`、`CC`和`FC`。这是确保跨平台并与非 POSIX 兼容的唯一方法。为了避免变量污染环境，这些变量可能会影响与项目一起构建的外部库环境。_

## 工作原理

配置时，CMake 会进行一系列平台测试，以确定哪些编译器可用，以及它们是否适合当前的项目。一个合适的编译器不仅取决于我们所使用的平台，还取决于我们想要使用的生成器。CMake 执行的第一个测试基于项目语言的编译器的名称。例如，`cc`是一个工作的`C`编译器，那么它将用作`C`项目的默认编译器。GNU/Linux 上，使用 Unix Makefile 或 Ninja 时, GCC 家族中的编译器很可能是`C++`、`C`和`Fortran`的默认选择。Microsoft Windows 上，将选择 Visual Studio 中的`C++`和`C`编译器(前提是 Visual Studio 是生成器)。如果选择 MinGW 或 MSYS Makefile 作为生成器，则默认使用 MinGW 编译器。

## 更多信息

我们的平台上的 CMake，在哪里可以找到可用的编译器和编译器标志？CMake 提供`--system-information`标志，它将把关于系统的所有信息转储到屏幕或文件中。要查看这个信息，请尝试以下操作：

```
$ cmake --system-information information.txt
```

文件中(本例中是`information.txt`)可以看到`CMAKE_CXX_COMPILER`、`CMAKE_C_COMPILER`和`CMAKE_Fortran_COMPILER`的默认值，以及默认标志。我们将在下一个示例中看到相关的标志。

CMake 提供了额外的变量来与编译器交互：

- `CMAKE_<LANG>_COMPILER_LOADED`:如果为项目启用了语言`<LANG>`，则将设置为`TRUE`。
- `CMAKE_<LANG>_COMPILER_ID`:编译器标识字符串，编译器供应商所特有。例如，`GCC`用于 GNU 编译器集合，`AppleClang`用于 macOS 上的 Clang, `MSVC`用于 Microsoft Visual Studio 编译器。注意，不能保证为所有编译器或语言定义此变量。
- `CMAKE_COMPILER_IS_GNU<LANG>`:如果语言`<LANG>`是 GNU 编译器集合的一部分，则将此逻辑变量设置为`TRUE`。注意变量名的`<LANG>`部分遵循 GNU 约定：C 语言为`CC`, C++语言为`CXX`, Fortran 语言为`G77`。
- `CMAKE_<LANG>_COMPILER_VERSION`:此变量包含一个字符串，该字符串给定语言的编译器版本。版本信息在`major[.minor[.patch[.tweak]]]`中给出。但是，对于`CMAKE_<LANG>_COMPILER_ID`，不能保证所有编译器或语言都定义了此变量。

我们可以尝试使用不同的编译器，配置下面的示例`CMakeLists.txt`。这个例子中，我们将使用 CMake 变量来探索已使用的编译器(及版本)：

```
cmake_minimum_required(VERSION 3.5 FATAL_ERROR)
project(recipe-06 LANGUAGES C CXX)

message(STATUS "Is the C++ compiler loaded? ${CMAKE_CXX_COMPILER_LOADED}")
if(CMAKE_CXX_COMPILER_LOADED)
	message(STATUS "The C++ compiler ID is: ${CMAKE_CXX_COMPILER_ID}")
	message(STATUS "Is the C++ from GNU? ${CMAKE_COMPILER_IS_GNUCXX}")
	message(STATUS "The C++ compiler version is: ${CMAKE_CXX_COMPILER_VERSION}")
endif()

message(STATUS "Is the C compiler loaded? ${CMAKE_C_COMPILER_LOADED}")
if(CMAKE_C_COMPILER_LOADED)
	message(STATUS "The C compiler ID is: ${CMAKE_C_COMPILER_ID}")
	message(STATUS "Is the C from GNU? ${CMAKE_COMPILER_IS_GNUCC}")
	message(STATUS "The C compiler version is: ${CMAKE_C_COMPILER_VERSION}")
endif()
```

注意，这个例子不包含任何目标，没有要构建的东西，我们只关注配置步骤:

```
$ mkdir -p build
$ cd build
$ cmake ..

...
-- Is the C++ compiler loaded? 1
-- The C++ compiler ID is: GNU
-- Is the C++ from GNU? 1
-- The C++ compiler version is: 8.1.0
-- Is the C compiler loaded? 1
-- The C compiler ID is: GNU
-- Is the C from GNU? 1
-- The C compiler version is: 8.1.0
...
```

当然，输出将取决于可用和已选择的编译器(及版本)。

# 1.7 切换构建类型

**NOTE**:_此示例代码可以在 codes/chapter-01/recipe-07 中找到，包含一个 C++/C 示例。该示例在 CMake 3.5 版(或更高版本)中是有效的，并且已经在 GNU/Linux、macOS 和 Windows 上进行过测试。_

CMake 可以配置构建类型，例如：Debug、Release 等。配置时，可以为 Debug 或 Release 构建设置相关的选项或属性，例如：编译器和链接器标志。控制生成构建系统使用的配置变量是`CMAKE_BUILD_TYPE`。该变量默认为空，CMake 识别的值为:

1. **Debug**：用于在没有优化的情况下，使用带有调试符号构建库或可执行文件。
2. **Release**：用于构建的优化的库或可执行文件，不包含调试符号。
3. **RelWithDebInfo**：用于构建较少的优化库或可执行文件，包含调试符号。
4. **MinSizeRel**：用于不增加目标代码大小的优化方式，来构建库或可执行文件。

## 具体实施

示例中，我们将展示如何为项目设置构建类型：

1. 首先，定义最低 CMake 版本、项目名称和支持的语言：

   ```
   cmake_minimum_required(VERSION 3.5 FATAL_ERROR)
   project(recipe-07 LANGUAGES C CXX)
   ```

2. 然后，设置一个默认的构建类型(本例中是 Release)，并打印一条消息。要注意的是，该变量被设置为缓存变量，可以通过缓存进行编辑：

   ```
   if(NOT CMAKE_BUILD_TYPE)
   	set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
   endif()
   message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
   ```

3. 最后，打印出 CMake 设置的相应编译标志：

   ```
   message(STATUS "C flags, Debug configuration: ${CMAKE_C_FLAGS_DEBUG}")
   message(STATUS "C flags, Release configuration: ${CMAKE_C_FLAGS_RELEASE}")
   message(STATUS "C flags, Release configuration with Debug info: ${CMAKE_C_FLAGS_RELWITHDEBINFO}")
   message(STATUS "C flags, minimal Release configuration: ${CMAKE_C_FLAGS_MINSIZEREL}")
   message(STATUS "C++ flags, Debug configuration: ${CMAKE_CXX_FLAGS_DEBUG}")
   message(STATUS "C++ flags, Release configuration: ${CMAKE_CXX_FLAGS_RELEASE}")
   message(STATUS "C++ flags, Release configuration with Debug info: ${CMAKE_CXX_FLAGS_RELWITHDEBINFO}")
   message(STATUS "C++ flags, minimal Release configuration: ${CMAKE_CXX_FLAGS_MINSIZEREL}")
   ```

4. 验证配置的输出:

   ```
   $ mkdir -p build
   $ cd build
   $ cmake ..

   ...
   -- Build type: Release
   -- C flags, Debug configuration: -g
   -- C flags, Release configuration: -O3 -DNDEBUG
   -- C flags, Release configuration with Debug info: -O2 -g -DNDEBUG
   -- C flags, minimal Release configuration: -Os -DNDEBUG
   -- C++ flags, Debug configuration: -g
   -- C++ flags, Release configuration: -O3 -DNDEBUG
   -- C++ flags, Release configuration with Debug info: -O2 -g -DNDEBUG
   -- C++ flags, minimal Release configuration: -Os -DNDEBUG
   ```

5. 切换构建类型:

   ```
   $ cmake -D CMAKE_BUILD_TYPE=Debug ..

   -- Build type: Debug
   -- C flags, Debug configuration: -g
   -- C flags, Release configuration: -O3 -DNDEBUG
   -- C flags, Release configuration with Debug info: -O2 -g -DNDEBUG
   -- C flags, minimal Release configuration: -Os -DNDEBUG
   -- C++ flags, Debug configuration: -g
   -- C++ flags, Release configuration: -O3 -DNDEBUG
   -- C++ flags, Release configuration with Debug info: -O2 -g -DNDEBUG
   -- C++ flags, minimal Release configuration: -Os -DNDEBUG
   ```

## 工作原理

我们演示了如何设置默认构建类型，以及如何(从命令行)覆盖它。这样，就可以控制项目，是使用优化，还是关闭优化启用调试。我们还看到了不同配置使用了哪些标志，这主要取决于选择的编译器。需要在运行 CMake 时显式地打印标志，也可以仔细阅读运行`CMake --system-information`的输出，以了解当前平台、默认编译器和语言的默认组合是什么。下一个示例中，我们将讨论如何为不同的编译器和不同的构建类型，扩展或调整编译器标志。

## 更多信息

我们展示了变量`CMAKE_BUILD_TYPE`，如何切换生成构建系统的配置(这个链接中有说明: https://cmake.org/cmake/help/v3.5/variable/CMAKE_BUILD_TYPE.html )。Release 和 Debug 配置中构建项目通常很有用，例如：评估编译器优化级别的效果。对于单配置生成器，如 Unix Makefile、MSYS Makefile 或 Ninja，因为要对项目重新配置，这里需要运行 CMake 两次。不过，CMake 也支持复合配置生成器。这些通常是集成开发环境提供的项目文件，最显著的是 Visual Studio 和 Xcode，它们可以同时处理多个配置。可以使用`CMAKE_CONFIGURATION_TYPES`变量可以对这些生成器的可用配置类型进行调整，该变量将接受一个值列表(可从这个链接获得文档:https://cmake.org/cmake/help/v3.5/variable/CMAKE_CONFIGURATION_TYPES.html)。

下面是对 Visual Studio 的 CMake 调用:

```
$ mkdir -p build
$ cd build
$ cmake .. -G"Visual Studio 12 2017 Win64" -D CMAKE_CONFIGURATION_TYPES="Release;Debug"
```

将为 Release 和 Debug 配置生成一个构建树。然后，您可以使`--config`标志来决定构建这两个中的哪一个:

```
$ cmake --build . --config Release
```

**NOTE**:_当使用单配置生成器开发代码时，为 Release 版和 Debug 创建单独的构建目录，两者使用相同的源代码。这样，就可以在两者之间切换，而不用重新配置和编译。_

# 1.8 设置编译器选项

**NOTE**:_此示例代码可以在 codes/chapter-01/recipe-08 中找到，有一个 C++示例。该示例在 CMake 3.5 版(或更高版本)中是有效的，并且已经在 GNU/Linux、macOS 和 Windows 上进行过测试。_

前面的示例展示了如何探测 CMake，从而获得关于编译器的信息，以及如何切换项目中的编译器。后一个任务是控制项目的编译器标志。CMake 为调整或扩展编译器标志提供了很大的灵活性，您可以选择下面两种方法:

- CMake 将编译选项视为目标属性。因此，可以根据每个目标设置编译选项，而不需要覆盖 CMake 默认值。
- 可以使用`-D`CLI 标志直接修改`CMAKE_<LANG>_FLAGS_<CONFIG>`变量。这将影响项目中的所有目标，并覆盖或扩展 CMake 默认值。

本示例中，我们将展示这两种方法。

## 准备工作

编写一个示例程序，计算不同几何形状的面积，`computer_area.cpp`：

```
#include "geometry_circle.hpp"
#include "geometry_polygon.hpp"
#include "geometry_rhombus.hpp"
#include "geometry_square.hpp"

#include <cstdlib>
#include <iostream>

int main() {
  using namespace geometry;

  double radius = 2.5293;
  double A_circle = area::circle(radius);
  std::cout << "A circle of radius " << radius << " has an area of " << A_circle
            << std::endl;

  int nSides = 19;
  double side = 1.29312;
  double A_polygon = area::polygon(nSides, side);
  std::cout << "A regular polygon of " << nSides << " sides of length " << side
            << " has an area of " << A_polygon << std::endl;

  double d1 = 5.0;
  double d2 = 7.8912;
  double A_rhombus = area::rhombus(d1, d2);
  std::cout << "A rhombus of major diagonal " << d1 << " and minor diagonal " << d2
            << " has an area of " << A_rhombus << std::endl;

  double l = 10.0;
  double A_square = area::square(l);
  std::cout << "A square of side " << l << " has an area of " << A_square
  << std::endl;

  return EXIT_SUCCESS;
}
```

函数的各种实现分布在不同的文件中，每个几何形状都有一个头文件和源文件。总共有 4 个头文件和 5 个源文件要编译：

```
.
├─ CMakeLists.txt
├─ compute-areas.cpp
├─ geometry_circle.cpp
├─ geometry_circle.hpp
├─ geometry_polygon.cpp
├─ geometry_polygon.hpp
├─ geometry_rhombus.cpp
├─ geometry_rhombus.hpp
├─ geometry_square.cpp
└─ geometry_square.hpp
```

我们不会为所有文件提供清单，读者可以参考 codes/chapter-01/recipe-08 。

## 具体实施

现在已经有了源代码，我们的目标是配置项目，并使用编译器标示进行实验:

1. 设置 CMake 的最低版本:

   ```
   cmake_minimum_required(VERSION 3.5 FATAL_ERROR)
   ```

2. 声明项目名称和语言:

   ```
   project(recipe-08 LANGUAGES CXX)
   ```

3. 然后，打印当前编译器标志。CMake 将对所有 C++目标使用这些:

   ```
   message("C++ compiler flags: ${CMAKE_CXX_FLAGS}")
   ```

4. 为目标准备了标志列表，其中一些将无法在 Windows 上使用:

   ```
   list(APPEND flags "-fPIC" "-Wall")
   if(NOT WIN32)
     list(APPEND flags "-Wextra" "-Wpedantic")
   endif()
   ```

5. 添加了一个新的目标——`geometry`库，并列出它的源依赖关系:

   ```
   add_library(geometry
     STATIC
       geometry_circle.cpp
       geometry_circle.hpp
       geometry_polygon.cpp
       geometry_polygon.hpp
       geometry_rhombus.cpp
       geometry_rhombus.hpp
       geometry_square.cpp
       geometry_square.hpp
     )
   ```

6. 为这个库目标设置了编译选项:

   ```
   target_compile_options(geometry
     PRIVATE
       ${flags}
     )
   ```

7. 然后，将生成`compute-areas`可执行文件作为一个目标:

   ```
   add_executable(compute-areas compute-areas.cpp)
   ```

8. 还为可执行目标设置了编译选项:

   ```
   target_compile_options(compute-areas
     PRIVATE
       "-fPIC"
     )
   ```

9. 最后，将可执行文件链接到 geometry 库:

   ```
   target_link_libraries(compute-areas geometry)
   ```

## 如何工作

本例中，警告标志有`-Wall`、`-Wextra`和`-Wpedantic`，将这些标示添加到`geometry`目标的编译选项中； `compute-areas`和 `geometry`目标都将使用`-fPIC`标志。编译选项可以添加三个级别的可见性：`INTERFACE`、`PUBLIC`和`PRIVATE`。

可见性的含义如下:

- **PRIVATE**，编译选项会应用于给定的目标，不会传递给与目标相关的目标。我们的示例中， 即使`compute-areas`将链接到`geometry`库，`compute-areas`也不会继承`geometry`目标上设置的编译器选项。
- **INTERFACE**，给定的编译选项将只应用于指定目标，并传递给与目标相关的目标。
- **PUBLIC**，编译选项将应用于指定目标和使用它的目标。

目标属性的可见性 CMake 的核心，我们将在本书中经常讨论这个话题。以这种方式添加编译选项，不会影响全局 CMake 变量`CMAKE_<LANG>_FLAGS_<CONFIG>`，并能更细粒度控制在哪些目标上使用哪些选项。

我们如何验证，这些标志是否按照我们的意图正确使用呢？或者换句话说，如何确定项目在 CMake 构建时，实际使用了哪些编译标志？一种方法是，使用 CMake 将额外的参数传递给本地构建工具。本例中会设置环境变量`VERBOSE=1`：

```
$ mkdir -p build
$ cd build
$ cmake ..
$ cmake --build . -- VERBOSE=1

... lots of output ...

[ 14%] Building CXX object CMakeFiles/geometry.dir/geometry_circle.cpp.o
/usr/bin/c++ -fPIC -Wall -Wextra -Wpedantic -o CMakeFiles/geometry.dir/geometry_circle.cpp.o -c /home/bast/tmp/cmake-cookbook/chapter-01/recipe-08/cxx-example/geometry_circle.cpp
[ 28%] Building CXX object CMakeFiles/geometry.dir/geometry_polygon.cpp.o
/usr/bin/c++ -fPIC -Wall -Wextra -Wpedantic -o CMakeFiles/geometry.dir/geometry_polygon.cpp.o -c /home/bast/tmp/cmake-cookbook/chapter-01/recipe-08/cxx-example/geometry_polygon.cpp
[ 42%] Building CXX object CMakeFiles/geometry.dir/geometry_rhombus.cpp.o
/usr/bin/c++ -fPIC -Wall -Wextra -Wpedantic -o CMakeFiles/geometry.dir/geometry_rhombus.cpp.o -c /home/bast/tmp/cmake-cookbook/chapter-01/recipe-08/cxx-example/geometry_rhombus.cpp
[ 57%] Building CXX object CMakeFiles/geometry.dir/geometry_square.cpp.o
/usr/bin/c++ -fPIC -Wall -Wextra -Wpedantic -o CMakeFiles/geometry.dir/geometry_square.cpp.o -c /home/bast/tmp/cmake-cookbook/chapter-01/recipe-08/cxx-example/geometry_square.cpp

... more output ...

[ 85%] Building CXX object CMakeFiles/compute-areas.dir/compute-areas.cpp.o
/usr/bin/c++ -fPIC -o CMakeFiles/compute-areas.dir/compute-areas.cpp.o -c /home/bast/tmp/cmake-cookbook/chapter-01/recipe-08/cxx-example/compute-areas.cpp

... more output ...
```

输出确认编译标志，确认指令设置正确。

控制编译器标志的第二种方法，不用对`CMakeLists.txt`进行修改。如果想在这个项目中修改`geometry`和`compute-areas`目标的编译器选项，可以使用 CMake 参数进行配置：

```
$ cmake -D CMAKE_CXX_FLAGS="-fno-exceptions -fno-rtti" ..
```

这个命令将编译项目，禁用异常和运行时类型标识(RTTI)。

也可以使用全局标志，可以使用`CMakeLists.txt`运行以下命令：

```
$ cmake -D CMAKE_CXX_FLAGS="-fno-exceptions -fno-rtti" ..
```

这将使用`-fno-rtti - fpic - wall - Wextra - wpedantic`配置`geometry`目标，同时使用`-fno exception -fno-rtti - fpic`配置`compute-areas`。

**NOTE**:_本书中，我们推荐为每个目标设置编译器标志。使用`target_compile_options()`不仅允许对编译选项进行细粒度控制，而且还可以更好地与 CMake 的更高级特性进行集成。_

## 更多信息

大多数时候，编译器有特性标示。当前的例子只适用于`GCC`和`Clang`；其他供应商的编译器不确定是否会理解(如果不是全部)这些标志。如果项目是真正跨平台，那么这个问题就必须得到解决，有三种方法可以解决这个问题。

最典型的方法是将所需编译器标志列表附加到每个配置类型 CMake 变量`CMAKE_<LANG>_FLAGS_<CONFIG>`。标志确定设置为给定编译器有效的标志，因此将包含在`if-endif`子句中，用于检查`CMAKE_<LANG>_COMPILER_ID`变量，例如：

```
if(CMAKE_CXX_COMPILER_ID MATCHES GNU)
  list(APPEND CMAKE_CXX_FLAGS "-fno-rtti" "-fno-exceptions")
  list(APPEND CMAKE_CXX_FLAGS_DEBUG "-Wsuggest-final-types" "-Wsuggest-final-methods" "-Wsuggest-override")
  list(APPEND CMAKE_CXX_FLAGS_RELEASE "-O3" "-Wno-unused")
endif()
if(CMAKE_CXX_COMPILER_ID MATCHES Clang)
  list(APPEND CMAKE_CXX_FLAGS "-fno-rtti" "-fno-exceptions" "-Qunused-arguments" "-fcolor-diagnostics")
  list(APPEND CMAKE_CXX_FLAGS_DEBUG "-Wdocumentation")
  list(APPEND CMAKE_CXX_FLAGS_RELEASE "-O3" "-Wno-unused")
endif()
```

更细粒度的方法是，不修改`CMAKE_<LANG>_FLAGS_<CONFIG>`变量，而是定义特定的标志列表：

```
set(COMPILER_FLAGS)
set(COMPILER_FLAGS_DEBUG)
set(COMPILER_FLAGS_RELEASE)

if(CMAKE_CXX_COMPILER_ID MATCHES GNU)
  list(APPEND CXX_FLAGS "-fno-rtti" "-fno-exceptions")
  list(APPEND CXX_FLAGS_DEBUG "-Wsuggest-final-types" "-Wsuggest-final-methods" "-Wsuggest-override")
  list(APPEND CXX_FLAGS_RELEASE "-O3" "-Wno-unused")
endif()

if(CMAKE_CXX_COMPILER_ID MATCHES Clang)
  list(APPEND CXX_FLAGS "-fno-rtti" "-fno-exceptions" "-Qunused-arguments" "-fcolor-diagnostics")
  list(APPEND CXX_FLAGS_DEBUG "-Wdocumentation")
  list(APPEND CXX_FLAGS_RELEASE "-O3" "-Wno-unused")
endif()
```

稍后，使用生成器表达式来设置编译器标志的基础上，为每个配置和每个目标生成构建系统:

```
target_compile_option(compute-areas
  PRIVATE
    ${CXX_FLAGS}
    "$<$<CONFIG:Debug>:${CXX_FLAGS_DEBUG}>"
    "$<$<CONFIG:Release>:${CXX_FLAGS_RELEASE}>"
  )
```

当前示例中展示了这两种方法，我们推荐后者(特定于项目的变量和`target_compile_options`)。

两种方法都有效，并在许多项目中得到广泛应用。不过，每种方式都有缺点。`CMAKE_<LANG>_COMPILER_ID`不能保证为所有编译器都定义。此外，一些标志可能会被弃用，或者在编译器的较晚版本中引入。与`CMAKE_<LANG>_COMPILER_ID`类似，`CMAKE_<LANG>_COMPILER_VERSION`变量不能保证为所有语言和供应商都提供定义。尽管检查这些变量的方式非常流行，但我们认为更健壮的替代方法是检查所需的标志集是否与给定的编译器一起工作，这样项目中实际上只使用有效的标志。结合特定于项目的变量、`target_compile_options`和生成器表达式，会让解决方案变得非常强大。我们将在第 7 章的第 3 节中展示，如何使用`check-and-set`模式。

# 1.9 为语言设定标准

**NOTE**:_此示例代码可以在 codes/chapter-01/recipe-09 中找到，包含一个 C++和 Fortran 示例。该示例在 CMake 3.5 版(或更高版本)中是有效的，并且已经在 GNU/Linux、macOS 和 Windows 上进行过测试。_

编程语言有不同的标准，即提供改进的语言版本。启用新标准是通过设置适当的编译器标志来实现的。前面的示例中，我们已经展示了如何为每个目标或全局进行配置。3.1 版本中，CMake 引入了一个独立于平台和编译器的机制，用于为`C++`和`C`设置语言标准：为目标设置`<LANG>_STANDARD`属性。

## 准备工作

对于下面的示例，需要一个符合`C++14`标准或更高版本的`C++`编译器。此示例代码定义了动物的多态，我们使用`std::unique_ptr`作为结构中的基类：

```
std::unique_ptr<Animal> cat = Cat("Simon");
std::unique_ptr<Animal> dog = Dog("Marlowe);
```

没有为各种子类型显式地使用构造函数，而是使用工厂方法的实现。工厂方法使用`C++11`的可变参数模板实现。它包含继承层次结构中每个对象的创建函数映射：

```
typedef std::function<std::unique_ptr<Animal>(const
std::string &)> CreateAnimal;
```

基于预先分配的标签来分派它们，创建对象:

```
std::unique_ptr<Animal> simon = farm.create("CAT", "Simon");
std::unique_ptr<Animal> marlowe = farm.create("DOG", "Marlowe");
```

标签和创建功能在工厂使用前已注册:

```
Factory<CreateAnimal> farm;
farm.subscribe("CAT", [](const std::string & n) { return std::make_unique<Cat>(n); });
farm.subscribe("DOG", [](const std::string & n) { return std::make_unique<Dog>(n); });
```

使用`C++11 Lambda`函数定义创建函数，使用`std::make_unique`来避免引入裸指针的操作。这个工厂函数是在`C++14`中引入。

**NOTE**:_CMake 的这一功能是在 3.1 版中添加的，并且还在更新。CMake 的后续版本为`C++`标准的后续版本和不同的编译器，提供了越来越好的支持。我们建议您在文档页面上检查您选择的编译器是否受支持: https://cmake.org/cmake/help/latest/manual/cmake-compile-features.7.html#supported-compiler_

## 具体实施

将逐步构建`CMakeLists.txt`，并展示如何设置语言标准(本例中是`C++14`):

1. 声明最低要求的 CMake 版本，项目名称和语言:

   ```
   cmake_minimum_required(VERSION 3.5 FATAL_ERROR)
   project(recipe-09 LANGUAGES CXX)
   ```

2. 要求在 Windows 上导出所有库符号:

   ```
   set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
   ```

3. 需要为库添加一个目标，这将编译源代码为一个动态库:

   ```
   add_library(animals
     SHARED
       Animal.cpp
       Animal.hpp
       Cat.cpp
       Cat.hpp
       Dog.cpp
       Dog.hpp
       Factory.hpp
     )
   ```

4. 现在，为目标设置了`CXX_STANDARD`、`CXX_EXTENSIONS`和`CXX_STANDARD_REQUIRED`属性。还设置了`position_independent ent_code`属性，以避免在使用一些编译器构建 DSO 时出现问题:

   ```
   set_target_properties(animals
     PROPERTIES
       CXX_STANDARD 14
       CXX_EXTENSIONS OFF
       CXX_STANDARD_REQUIRED ON
       POSITION_INDEPENDENT_CODE 1
   )
   ```

5. 然后，为”动物农场”的可执行文件添加一个新目标，并设置它的属性:

   ```
   add_executable(animal-farm animal-farm.cpp)
   set_target_properties(animal-farm
     PROPERTIES
       CXX_STANDARD 14
       CXX_EXTENSIONS OFF
       CXX_STANDARD_REQUIRED ON
     )
   ```

6. 最后，将可执行文件链接到库:

   ```
   target_link_libraries(animal-farm animals)
   ```

7. 现在，来看看猫和狗都说了什么:

   ```
   $ mkdir -p build
   $ cd build
   $ cmake ..
   $ cmake --build .
   $ ./animal-farm

   I'm Simon the cat!
   I'm Marlowe the dog!
   ```

## 工作原理

步骤 4 和步骤 5 中，我们为动物和动物农场目标设置了一些属性:

- **CXX_STANDARD**会设置我们想要的标准。
- **CXX_EXTENSIONS**告诉 CMake，只启用`ISO C++`标准的编译器标志，而不使用特定编译器的扩展。
- **CXX_STANDARD_REQUIRED**指定所选标准的版本。如果这个版本不可用，CMake 将停止配置并出现错误。当这个属性被设置为`OFF`时，CMake 将寻找下一个标准的最新版本，直到一个合适的标志。这意味着，首先查找`C++14`，然后是`C++11`，然后是`C++98`。（译者注：目前会从`C++20`或`C++17`开始查找）

**NOTE**:_本书发布时，还没有`Fortran_STANDARD`可用，但是可以使用`target_compile_options`设置标准，可以参见: https://github.com/devcafe/cmake-cookbook/tree/v1.0/chapter-01/recipe-09_

**TIPS**:_如果语言标准是所有目标共享的全局属性，那么可以将`CMAKE_<LANG>_STANDARD`、`CMAKE_<LANG>_EXTENSIONS`和`CMAKE_<LANG>_STANDARD_REQUIRED`变量设置为相应的值。所有目标上的对应属性都将使用这些设置。_

## 更多信息

通过引入编译特性，CMake 对语言标准提供了更精细的控制。这些是语言标准引入的特性，比如`C++11`中的可变参数模板和`Lambda`表达式，以及`C++14`中的自动返回类型推断。可以使用`target_compile_features()`命令要求为特定的目标提供特定的特性，CMake 将自动为标准设置正确的编译器标志。也可以让 CMake 为可选编译器特性，生成兼容头文件。

**TIPS**:_我们建议阅读 CMake 在线文档，全面了解`cmake-compile-features`和如何处理编译特性和语言标准: https://cmake.org/cmake/help/latest/manual/cmake-compile-features.7.html 。_

# 1.10 使用控制流

**NOTE**:_此示例代码可以在 codes/chapter-01/recipe-10 中找到，有一个 C++示例。该示例在 CMake 3.5 版(或更高版本)中是有效的，并且已经在 GNU/Linux、macOS 和 Windows 上进行过测试。_

本章前面的示例中，已经使用过`if-else-endif`。CMake 还提供了创建循环的语言工具：`foreach endforeach`和`while-endwhile`。两者都可以与`break`结合使用，以便尽早从循环中跳出。本示例将展示如何使用`foreach`，来循环源文件列表。我们将应用这样的循环，在引入新目标的前提下，来为一组源文件进行优化降级。

## 准备工作

将重用第 8 节中的几何示例，目标是通过将一些源代码汇集到一个列表中，从而微调编译器的优化。

## 具体实施

下面是`CMakeLists.txt`中要的详细步骤:

1. 与示例 8 中一样，指定了 CMake 的最低版本、项目名称和语言，并声明了几何库目标:

   ```
   cmake_minimum_required(VERSION 3.5 FATAL_ERROR)
   project(recipe-10 LANGUAGES CXX)
   add_library(geometry
     STATIC
       geometry_circle.cpp
       geometry_circle.hpp
       geometry_polygon.cpp
       geometry_polygon.hpp
       geometry_rhombus.cpp
       geometry_rhombus.hpp
       geometry_square.cpp
       geometry_square.hpp
     )
   ```

2. 使用`-O3`编译器优化级别编译库，对目标设置一个私有编译器选项:

   ```
   target_compile_options(geometry
     PRIVATE
     	-O3
     )
   ```

3. 然后，生成一个源文件列表，以较低的优化选项进行编译:

   ```
   list(
     APPEND sources_with_lower_optimization
       geometry_circle.cpp
       geometry_rhombus.cpp
     )
   ```

4. 循环这些源文件，将它们的优化级别调到`-O2`。使用它们的源文件属性完成:

   ```
   message(STATUS "Setting source properties using IN LISTS syntax:")
   foreach(_source IN LISTS sources_with_lower_optimization)
     set_source_files_properties(${_source} PROPERTIES COMPILE_FLAGS -O2)
     message(STATUS "Appending -O2 flag for ${_source}")
   endforeach()
   ```

5. 为了确保设置属性，再次循环并在打印每个源文件的`COMPILE_FLAGS`属性:

   ```
   message(STATUS "Querying sources properties using plain syntax:")
   foreach(_source ${sources_with_lower_optimization})
     get_source_file_property(_flags ${_source} COMPILE_FLAGS)
     message(STATUS "Source ${_source} has the following extra COMPILE_FLAGS: ${_flags}")
   endforeach()
   ```

6. 最后，添加`compute-areas`可执行目标，并将`geometry`库连接上去:

   ```
   add_executable(compute-areas compute-areas.cpp)
   target_link_libraries(compute-areas geometry)
   ```

7. 验证在配置步骤中正确设置了标志:

   ```
   $ mkdir -p build
   $ cd build
   $ cmake ..

   ...
   -- Setting source properties using IN LISTS syntax:
   -- Appending -O2 flag for geometry_circle.cpp
   -- Appending -O2 flag for geometry_rhombus.cpp
   -- Querying sources properties using plain syntax:
   -- Source geometry_circle.cpp has the following extra COMPILE_FLAGS: -O2
   -- Source geometry_rhombus.cpp has the following extra COMPILE_FLAGS: -O2
   ```

8. 最后，还使用`VERBOSE=1`检查构建步骤。将看到`-O2`标志添加在`-O3`标志之后，但是最后一个优化级别标志(在本例中是`-O2`)不同:

   ```
   $ cmake --build . -- VERBOSE=1
   ```

## 工作原理

`foreach-endforeach`语法可用于在变量列表上，表示重复特定任务。本示例中，使用它来操作、设置和获取项目中特定文件的编译器标志。CMake 代码片段中引入了另外两个新命令:

- `set_source_files_properties(file PROPERTIES property value)`，它将属性设置为给定文件的传递值。与目标非常相似，文件在 CMake 中也有属性，允许对构建系统进行非常细粒度的控制。源文件的可用属性列表可以在这里找到: https://cmake.org/cmake/help/v3.5/manual/cmake-properties.7.html#source-file-properties 。
- `get_source_file_property(VAR file property)`，检索给定文件所需属性的值，并将其存储在 CMake`VAR`变量中。

**NOTE**:_CMake 中，列表是用分号分隔的字符串组。列表可以由`list`或`set`命令创建。例如，`set(var a b c d e)`和`list(APPEND a b c d e)`都创建了列表`a;b;c;d;e`。_

**TIPS**:_为了对一组文件降低优化，将它们收集到一个单独的目标(库)中，并为这个目标显式地设置优化级别，而不是附加一个标志，这样可能会更简洁，不过在本示例中，我们的重点是`foreach-endforeach`。_

## 更多信息

`foreach()`的四种使用方式:

- `foreach(loop_var arg1 arg2 ...)`: 其中提供循环变量和显式项列表。当为`sources_with_lower_optimization`中的项打印编译器标志集时，使用此表单。注意，如果项目列表位于变量中，则必须显式展开它；也就是说，`${sources_with_lower_optimization}`必须作为参数传递。
- 通过指定一个范围，可以对整数进行循环，例如：`foreach(loop_var range total)`或`foreach(loop_var range start stop [step])`。
- 对列表值变量的循环，例如：`foreach(loop_var IN LISTS [list1[...]])` 。参数解释为列表，其内容就会自动展开。
- 对变量的循环，例如：`foreach(loop_var IN ITEMS [item1 [...]])`。参数的内容没有展开。
