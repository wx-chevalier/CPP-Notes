# 检测环境

- - 作系统
  - 处理与平台相关的源码
  - 处理与编译器相关的源码
  - 检测处理器体系结构
  - 检测处理器指令集
  - 为 Eigen 库使能向量化

  尽管 CMake 跨平台，但有时源代码并不是完全可移植(例如：当使用依赖于供应商的扩展时)，我们努力使源代码能够跨平台、操作系统和编译器。这个过程中会发现，有必要根据平台不同的方式配置和/或构建代码。这对于历史代码或交叉编译尤其重要，我们将在第 13 章中讨论这个主题。了解处理器指令集也有助于优化特定目标平台的性能。本章会介绍，检测环境的方法，并给出建议。

# 2.1 检测操作系统

**NOTE**:_此示例代码可以在 codes/chapter-02/recipe-01 中找到。该示例在 CMake 3.5 版(或更高版本)中是有效的，并且已经在 GNU/Linux、macOS 和 Windows 上进行过测试。_

CMake 是一组跨平台工具。不过，了解操作系统(OS)上执行配置或构建步骤也很重要。从而与操作系统相关的 CMake 代码，会根据操作系统启用条件编译，或者在可用或必要时使用特定于编译器的扩展。本示例中，我们将通过一个不需要编译任何源代码的示例，演示如何使用 CMake 检测操作系统。为了简单起见，我们只考虑配置过程。

## 具体实施

我们将用一个非常简单的`CMakeLists.txt`进行演示:

1. 首先，定义 CMake 最低版本和项目名称。请注意，语言是`NONE`:

   ```
   cmake_minimum_required(VERSION 3.5 FATAL_ERROR)
   project(recipe-01 LANGUAGES NONE)
   ```

2. 然后，根据检测到的操作系统信息打印消息:

   ```
   if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
   	message(STATUS "Configuring on/for Linux")
   elseif(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
   	message(STATUS "Configuring on/for macOS")
   elseif(CMAKE_SYSTEM_NAME STREQUAL "Windows")
   	message(STATUS "Configuring on/for Windows")
   elseif(CMAKE_SYSTEM_NAME STREQUAL "AIX")
   	message(STATUS "Configuring on/for IBM AIX")
   else()
   	message(STATUS "Configuring on/for ${CMAKE_SYSTEM_NAME}")
   endif()
   ```

   测试之前，检查前面的代码块，并考虑相应系统上的具体行为。

3. 现在，测试配置项目:

   ```
   $ mkdir -p build
   $ cd build
   $ cmake ..
   ```

4. 关于 CMake 输出，这里有一行很有趣——在 Linux 系统上(在其他系统上，输出会不同):

   ```
   -- Configuring on/for Linux
   ```

## 工作原理

CMake 为目标操作系统定义了`CMAKE_SYSTEM_NAME`，因此不需要使用定制命令、工具或脚本来查询此信息。然后，可以使用此变量的值实现特定于操作系统的条件和解决方案。在具有`uname`命令的系统上，将此变量设置为`uname -s`的输出。该变量在 macOS 上设置为“Darwin”。在 Linux 和 Windows 上，它分别计算为“Linux”和“Windows”。我们了解了如何在特定的操作系统上执行特定的 CMake 代码。当然，应该尽量减少这种定制化行为，以便简化迁移到新平台的过程。

**NOTE**:_为了最小化从一个平台转移到另一个平台时的成本，应该避免直接使用 Shell 命令，还应该避免显式的路径分隔符(Linux 和 macOS 上的前斜杠和 Windows 上的后斜杠)。CMake 代码中只使用前斜杠作为路径分隔符，CMake 将自动将它们转换为所涉及的操作系统环境。_

# 2.2 处理与平台相关的源代码

**NOTE**:_此示例代码可以在 codes/chapter-02/recipe-02 中找到，包含一个 C++示例。该示例在 CMake 3.5 版(或更高版本)中是有效的，并且已经在 GNU/Linux、macOS 和 Windows 上进行过测试。_

理想情况下，应该避免依赖于平台的源代码，但是有时我们没有选择，特别是当要求配置和编译不是自己编写的代码时。本示例中，将演示如何使用 CMake 根据操作系统编译源代码。

## 准备工作

修改`hello-world.cpp`示例代码，将第 1 章第 1 节的例子进行修改:

```cpp
#include <cstdlib>
#include <iostream>
#include <string>

std::string say_hello() {
#ifdef IS_WINDOWS
  return std::string("Hello from Windows!");
#elif IS_LINUX
  return std::string("Hello from Linux!");
#elif IS_MACOS
  return std::string("Hello from macOS!");
#else
  return std::string("Hello from an unknown system!");
#endif
}

int main() {
  std::cout << say_hello() << std::endl;
  return EXIT_SUCCESS;
}
```

## 具体实施

完成一个`CMakeLists.txt`实例，使我们能够基于目标操作系统有条件地编译源代码：

1. 首先，设置了 CMake 最低版本、项目名称和支持的语言:

   ```
   cmake_minimum_required(VERSION 3.5 FATAL_ERROR)
   project(recipe-02 LANGUAGES CXX)
   ```

2. 然后，定义可执行文件及其对应的源文件:

   ```
   add_executable(hello-world hello-world.cpp)
   ```

3. 通过定义以下目标编译定义，让预处理器知道系统名称:

   ```sh
   if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
     target_compile_definitions(hello-world PUBLIC "IS_LINUX")
   endif()
   if(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
     target_compile_definitions(hello-world PUBLIC "IS_MACOS")
   endif()
   if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
     target_compile_definitions(hello-world PUBLIC "IS_WINDOWS")
   endif()
   ```

   继续之前，先检查前面的表达式，并考虑在不同系统上有哪些行为。

4. 现在，准备测试它，并配置项目:

   ```
   $ mkdir -p build
   $ cd build
   $ cmake ..
   $ cmake --build .
   $ ./hello-world

   Hello from Linux!
   ```

Windows 系统上，将看到来自 Windows 的 Hello。其他操作系统将产生不同的输出。

## 工作原理

`hello-world.cpp`示例中，有趣的部分是基于预处理器定义`IS_WINDOWS`、`IS_LINUX`或`IS_MACOS`的条件编译:

```
std::string say_hello() {
#ifdef IS_WINDOWS
  return std::string("Hello from Windows!");
#elif IS_LINUX
  return std::string("Hello from Linux!");
#elif IS_MACOS
  return std::string("Hello from macOS!");
#else
  return std::string("Hello from an unknown system!");
#endif
}
```

这些定义在 CMakeLists.txt 中配置时定义，通过使用`target_compile_definition`在预处理阶段使用。可以不重复`if-endif`语句，以更紧凑的表达式实现，我们将在下一个示例中演示这种重构方式。也可以把`if-endif`语句加入到一个`if-else-else-endif`语句中。这个阶段，可以使用`add_definitions(-DIS_LINUX)`来设置定义(当然，可以根据平台调整定义)，而不是使用`target_compile_definition`。使用`add_definitions`的缺点是，会修改编译整个项目的定义，而`target_compile_definitions`给我们机会，将定义限制于一个特定的目标，以及通过`PRIVATE|PUBLIC|INTERFACE`限定符，限制这些定义可见性。第 1 章的第 8 节，对这些限定符有详细的说明:

- **PRIVATE**，编译定义将只应用于给定的目标，而不应用于相关的其他目标。
- **INTERFACE**，对给定目标的编译定义将只应用于使用它的目标。
- **PUBLIC**，编译定义将应用于给定的目标和使用它的所有其他目标。

**NOTE**:_将项目中的源代码与平台相关性最小化，可使移植更加容易。_

# 2.3 处理与编译器相关的源代码

**NOTE**:_此示例代码可以在 codes/chapter-02/recipe-03 中找到，包含一个 C++和 Fortran 示例。该示例在 CMake 3.5 版(或更高版本)中是有效的，并且已经在 GNU/Linux、macOS 和 Windows 上进行过测试。_

这个方法与前面的方法类似，我们将使用 CMake 来编译依赖于环境的条件源代码：本例将依赖于编译器。为了可移植性，我们尽量避免去编写新代码，但遇到有依赖的情况我们也要去解决，特别是当使用历史代码或处理编译器依赖工具，如[sanitizers](https://github.com/google/sanitizers)。从这一章和前一章的示例中，我们已经掌握了实现这一目标的所有方法。尽管如此，讨论与编译器相关的源代码的处理问题还是很有用的，这样我们将有机会从另一方面了解 CMake。

## 准备工作

本示例中，我们将从`C++`中的一个示例开始，稍后我们将演示一个`Fortran`示例，并尝试重构和简化 CMake 代码。

看一下`hello-world.cpp`源代码:

```cpp
#include <cstdlib>
#include <iostream>
#include <string>

std::string say_hello() {
#ifdef IS_INTEL_CXX_COMPILER
  // only compiled when Intel compiler is selected
  // such compiler will not compile the other branches
  return std::string("Hello Intel compiler!");
#elif IS_GNU_CXX_COMPILER
  // only compiled when GNU compiler is selected
  // such compiler will not compile the other branches
  return std::string("Hello GNU compiler!");
#elif IS_PGI_CXX_COMPILER
  // etc.
  return std::string("Hello PGI compiler!");
#elif IS_XL_CXX_COMPILER
  return std::string("Hello XL compiler!");
#else
  return std::string("Hello unknown compiler - have we met before?");
#endif
}

int main() {
  std::cout << say_hello() << std::endl;
  std::cout << "compiler name is " COMPILER_NAME << std::endl;
  return EXIT_SUCCESS;
}
```

`Fortran`示例(`hello-world.F90`):

```
program hello

  implicit none
#ifdef IS_Intel_FORTRAN_COMPILER
  print *, 'Hello Intel compiler!'
#elif IS_GNU_FORTRAN_COMPILER
  print *, 'Hello GNU compiler!'
#elif IS_PGI_FORTRAN_COMPILER
  print *, 'Hello PGI compiler!'
#elif IS_XL_FORTRAN_COMPILER
  print *, 'Hello XL compiler!'
#else
  print *, 'Hello unknown compiler - have we met before?'
#endif

end program
```

## 具体实施

我们将从`C++`的例子开始，然后再看`Fortran`的例子:

1. `CMakeLists.txt`文件中，定义了 CMake 最低版本、项目名称和支持的语言:

   ```
   cmake_minimum_required(VERSION 3.5 FATAL_ERROR)
   project(recipe-03 LANGUAGES CXX)
   ```

2. 然后，定义可执行目标及其对应的源文件:

   ```
   add_executable(hello-world hello-world.cpp)
   ```

3. 通过定义以下目标编译定义，让预处理器了解编译器的名称和供应商:

   ```
   target_compile_definitions(hello-world PUBLIC "COMPILER_NAME=\"${CMAKE_CXX_COMPILER_ID}\"")

   if(CMAKE_CXX_COMPILER_ID MATCHES Intel)
     target_compile_definitions(hello-world PUBLIC "IS_INTEL_CXX_COMPILER")
   endif()
   if(CMAKE_CXX_COMPILER_ID MATCHES GNU)
     target_compile_definitions(hello-world PUBLIC "IS_GNU_CXX_COMPILER")
   endif()
   if(CMAKE_CXX_COMPILER_ID MATCHES PGI)
     target_compile_definitions(hello-world PUBLIC "IS_PGI_CXX_COMPILER")
   endif()
   if(CMAKE_CXX_COMPILER_ID MATCHES XL)
     target_compile_definitions(hello-world PUBLIC "IS_XL_CXX_COMPILER")
   endif()
   ```

现在我们已经可以预测结果了:

```
$ mkdir -p build
$ cd build
$ cmake ..
$ cmake --build .
$ ./hello-world

Hello GNU compiler!
```

使用不同的编译器，此示例代码将打印不同的问候语。

前一个示例的`CMakeLists.txt`文件中的`if`语句似乎是重复的，我们不喜欢重复的语句。能更简洁地表达吗？当然可以！为此，让我们再来看看`Fortran`示例。

`Fortran`例子的`CMakeLists.txt`文件中，我们需要做以下工作:

1. 需要使`Fortran`语言:

   ```
   project(recipe-03 LANGUAGES Fortran)
   ```

2. 然后，定义可执行文件及其对应的源文件。在本例中，使用大写`.F90`后缀:

   ```
   add_executable(hello-world hello-world.F90)
   ```

3. 我们通过定义下面的目标编译定义，让预处理器非常清楚地了解编译器:

   ```
   target_compile_definitions(hello-world
     PUBLIC "IS_${CMAKE_Fortran_COMPILER_ID}_FORTRAN_COMPILER"
     )
   ```

其余行为与`C++`示例相同。

## 工作原理

`CMakeLists.txt`会在配置时，进行预处理定义，并传递给预处理器。`Fortran`示例包含非常紧凑的表达式，我们使用`CMAKE_Fortran_COMPILER_ID`变量，通过`target_compile_definition`使用构造预处理器进行预处理定义。为了适应这种情况，我们必须将”Intel”从`IS_INTEL_CXX_COMPILER`更改为`IS_Intel_FORTRAN_COMPILER`。通过使用相应的`CMAKE_C_COMPILER_ID`和`CMAKE_CXX_COMPILER_ID`变量，我们可以在`C`或`C++`中实现相同的效果。但是，请注意，`CMAKE_<LANG>_COMPILER_ID`不能保证为所有编译器或语言都定义。

**NOTE**:_对于应该预处理的`Fortran`代码使用`.F90`后缀，对于不需要预处理的代码使用`.f90`后缀。_

# 2.4 检测处理器体系结构

**NOTE**:_此示例代码可以在 codes/chapter-02/recipe-04 中找到，包含一个 C++示例。该示例在 CMake 3.5 版(或更高版本)中是有效的，并且已经在 GNU/Linux、macOS 和 Windows 上进行过测试。_

19 世纪 70 年代，出现的 64 位整数运算和本世纪初出现的用于个人计算机的 64 位寻址，扩大了内存寻址范围，开发商投入了大量资源来移植为 32 位体系结构硬编码，以支持 64 位寻址。许多博客文章，如 https://www.viva64.com/en/a/0004/ ，致力于讨论将`C++`代码移植到 64 位平台中的典型问题和解决方案。虽然，避免显式硬编码的方式非常明智，但需要在使用 CMake 配置的代码中适应硬编码限制。本示例中，我们会来讨论检测主机处理器体系结构的选项。

## 准备工作

我们以下面的`arch-dependent.cpp`代码为例：

```
#include <cstdlib>
#include <iostream>
#include <string>

#define STRINGIFY(x) #x
#define TOSTRING(x) STRINGIFY(x)

std::string say_hello()
{
  std::string arch_info(TOSTRING(ARCHITECTURE));
  arch_info += std::string(" architecture. ");
#ifdef IS_32_BIT_ARCH
  return arch_info + std::string("Compiled on a 32 bit host processor.");
#elif IS_64_BIT_ARCH
  return arch_info + std::string("Compiled on a 64 bit host processor.");
#else
  return arch_info + std::string("Neither 32 nor 64 bit, puzzling ...");
#endif
}

int main()
{
  std::cout << say_hello() << std::endl;
  return EXIT_SUCCESS;
}
```

## 具体实施

`CMakeLists.txt`文件中，我们需要以下内容:

1. 首先，定义可执行文件及其源文件依赖关系:

   ```
   cmake_minimum_required(VERSION 3.5 FATAL_ERROR)
   project(recipe-04 LANGUAGES CXX)
   add_executable(arch-dependent arch-dependent.cpp)
   ```

2. 检查空指针类型的大小。CMake 的`CMAKE_SIZEOF_VOID_P`变量会告诉我们 CPU 是 32 位还是 64 位。我们通过状态消息让用户知道检测到的大小，并设置预处理器定义:

   ```
   if(CMAKE_SIZEOF_VOID_P EQUAL 8)
     target_compile_definitions(arch-dependent PUBLIC "IS_64_BIT_ARCH")
     message(STATUS "Target is 64 bits")
   else()
     target_compile_definitions(arch-dependent PUBLIC "IS_32_BIT_ARCH")
     message(STATUS "Target is 32 bits")
   endif()
   ```

3. 通过定义以下目标编译定义，让预处理器了解主机处理器架构，同时在配置过程中打印状态消息:

   ```
   if(CMAKE_HOST_SYSTEM_PROCESSOR MATCHES "i386")
   	message(STATUS "i386 architecture detected")
   elseif(CMAKE_HOST_SYSTEM_PROCESSOR MATCHES "i686")
   	message(STATUS "i686 architecture detected")
   elseif(CMAKE_HOST_SYSTEM_PROCESSOR MATCHES "x86_64")
   	message(STATUS "x86_64 architecture detected")
   else()
   	message(STATUS "host processor architecture is unknown")
   endif()
   target_compile_definitions(arch-dependent
     PUBLIC "ARCHITECTURE=${CMAKE_HOST_SYSTEM_PROCESSOR}"
     )
   ```

4. 配置项目，并注意状态消息(打印出的信息可能会发生变化):

   ```
   $ mkdir -p build
   $ cd build
   $ cmake ..

   ...
   -- Target is 64 bits
   -- x86_64 architecture detected
   ...
   ```

5. 最后，构建并执行代码(实际输出将取决于处理器架构):

   ```
   $ cmake --build .
   $ ./arch-dependent

   x86_64 architecture. Compiled on a 64 bit host processor.
   ```

## 工作原理

CMake 定义了`CMAKE_HOST_SYSTEM_PROCESSOR`变量，以包含当前运行的处理器的名称。可以设置为“i386”、“i686”、“x86_64”、“AMD64”等等，当然，这取决于当前的 CPU。`CMAKE_SIZEOF_VOID_P`为 void 指针的大小。我们可以在 CMake 配置时进行查询，以便修改目标或目标编译定义。可以基于检测到的主机处理器体系结构，使用预处理器定义，确定需要编译的分支源代码。正如在前面的示例中所讨论的，编写新代码时应该避免这种依赖，但在处理遗留代码或交叉编译时，这种依赖是有用的，交叉编译会在第 13 章进行讨论。

**NOTE**:_使用`CMAKE_SIZEOF_VOID_P`是检查当前 CPU 是否具有 32 位或 64 位架构的唯一“真正”可移植的方法。_

## 更多信息

除了`CMAKE_HOST_SYSTEM_PROCESSOR`, CMake 还定义了`CMAKE_SYSTEM_PROCESSOR`变量。前者包含当前运行的 CPU 在 CMake 的名称，而后者将包含当前正在为其构建的 CPU 的名称。这是一个细微的差别，在交叉编译时起着非常重要的作用。我们将在第 13 章，看到更多关于交叉编译的内容。另一种让 CMake 检测主机处理器体系结构，是使用`C`或`C++中`定义的符号，结合 CMake 的`try_run`函数，尝试构建执行的源代码(见第 5.8 节)分支的预处理符号。这将返回已定义错误码，这些错误可以在 CMake 端捕获(此策略的灵感来自 https://github.com/axr/cmake/blob/master/targetarch.cmake ):

```
#if defined(__i386) || defined(__i386__) || defined(_M_IX86)
	#error cmake_arch i386
#elif defined(__x86_64) || defined(__x86_64__) || defined(__amd64) || defined(_M_X64)
	#error cmake_arch x86_64
#endif
```

这种策略也是检测目标处理器体系结构的推荐策略，因为 CMake 似乎没有提供可移植的内在解决方案。另一种选择，将只使用 CMake，完全不使用预处理器，代价是为每种情况设置不同的源文件，然后使用`target_source`命令将其设置为可执行目标`arch-dependent`依赖的源文件:

```
add_executable(arch-dependent "")

if(CMAKE_HOST_SYSTEM_PROCESSOR MATCHES "i386")
	message(STATUS "i386 architecture detected")
	target_sources(arch-dependent
		PRIVATE
		arch-dependent-i386.cpp
	)
elseif(CMAKE_HOST_SYSTEM_PROCESSOR MATCHES "i686")
	message(STATUS "i686 architecture detected")
	target_sources(arch-dependent
		PRIVATE
			arch-dependent-i686.cpp
	)
elseif(CMAKE_HOST_SYSTEM_PROCESSOR MATCHES "x86_64")
	message(STATUS "x86_64 architecture detected")
	target_sources(arch-dependent
		PRIVATE
			arch-dependent-x86_64.cpp
	)
else()
	message(STATUS "host processor architecture is unknown")
endif()
```

这种方法，显然需要对现有项目进行更多的工作，因为源文件需要分离。此外，不同源文件之间的代码复制肯定也会成为问题。

# 2.5 检测处理器指令集

**NOTE**:_此示例代码可以在 codes/chapter-02/recipe-05 中找到，包含一个 C++示例。该示例在 CMake 3.10 版(或更高版本)中是有效的，并且已经在 GNU/Linux、macOS 和 Windows 上进行过测试。_

本示例中，我们将讨论如何在 CMake 的帮助下检测主机处理器支持的指令集。这个功能是较新版本添加到 CMake 中的，需要 CMake 3.10 或更高版本。检测到的主机系统信息，可用于设置相应的编译器标志，或实现可选的源代码编译，或根据主机系统生成源代码。本示例中，我们的目标是检测主机系统信息，使用预处理器定义将其传递给`C++`源代码，并将信息打印到输出中。

## 准备工作

我们是`C++`源码(`processor-info.cpp`)如下所示：

```
#include "config.h"

#include <cstdlib>
#include <iostream>

int main()
{
  std::cout << "Number of logical cores: "
            << NUMBER_OF_LOGICAL_CORES << std::endl;
  std::cout << "Number of physical cores: "
            << NUMBER_OF_PHYSICAL_CORES << std::endl;
  std::cout << "Total virtual memory in megabytes: "
            << TOTAL_VIRTUAL_MEMORY << std::endl;
  std::cout << "Available virtual memory in megabytes: "
            << AVAILABLE_VIRTUAL_MEMORY << std::endl;
  std::cout << "Total physical memory in megabytes: "
            << TOTAL_PHYSICAL_MEMORY << std::endl;
  std::cout << "Available physical memory in megabytes: "
            << AVAILABLE_PHYSICAL_MEMORY << std::endl;
  std::cout << "Processor is 64Bit: "
            << IS_64BIT << std::endl;
  std::cout << "Processor has floating point unit: "
            << HAS_FPU << std::endl;
  std::cout << "Processor supports MMX instructions: "
            << HAS_MMX << std::endl;
  std::cout << "Processor supports Ext. MMX instructions: "
            << HAS_MMX_PLUS << std::endl;
  std::cout << "Processor supports SSE instructions: "
            << HAS_SSE << std::endl;
  std::cout << "Processor supports SSE2 instructions: "
            << HAS_SSE2 << std::endl;
  std::cout << "Processor supports SSE FP instructions: "
            << HAS_SSE_FP << std::endl;
  std::cout << "Processor supports SSE MMX instructions: "
            << HAS_SSE_MMX << std::endl;
  std::cout << "Processor supports 3DNow instructions: "
            << HAS_AMD_3DNOW << std::endl;
  std::cout << "Processor supports 3DNow+ instructions: "
            << HAS_AMD_3DNOW_PLUS << std::endl;
  std::cout << "IA64 processor emulating x86 : "
            << HAS_IA64 << std::endl;
  std::cout << "OS name: "
            << OS_NAME << std::endl;
  std::cout << "OS sub-type: "
            << OS_RELEASE << std::endl;
  std::cout << "OS build ID: "
            << OS_VERSION << std::endl;
  std::cout << "OS platform: "
            << OS_PLATFORM << std::endl;
  return EXIT_SUCCESS;
}
```

其包含`config.h`头文件，我们将使用`config.h.in`生成这个文件。`config.h.in`如下:

```
#pragma once

#define NUMBER_OF_LOGICAL_CORES @_NUMBER_OF_LOGICAL_CORES@
#define NUMBER_OF_PHYSICAL_CORES @_NUMBER_OF_PHYSICAL_CORES@
#define TOTAL_VIRTUAL_MEMORY @_TOTAL_VIRTUAL_MEMORY@
#define AVAILABLE_VIRTUAL_MEMORY @_AVAILABLE_VIRTUAL_MEMORY@
#define TOTAL_PHYSICAL_MEMORY @_TOTAL_PHYSICAL_MEMORY@
#define AVAILABLE_PHYSICAL_MEMORY @_AVAILABLE_PHYSICAL_MEMORY@
#define IS_64BIT @_IS_64BIT@
#define HAS_FPU @_HAS_FPU@
#define HAS_MMX @_HAS_MMX@
#define HAS_MMX_PLUS @_HAS_MMX_PLUS@
#define HAS_SSE @_HAS_SSE@
#define HAS_SSE2 @_HAS_SSE2@
#define HAS_SSE_FP @_HAS_SSE_FP@
#define HAS_SSE_MMX @_HAS_SSE_MMX@
#define HAS_AMD_3DNOW @_HAS_AMD_3DNOW@
#define HAS_AMD_3DNOW_PLUS @_HAS_AMD_3DNOW_PLUS@
#define HAS_IA64 @_HAS_IA64@
#define OS_NAME "@_OS_NAME@"
#define OS_RELEASE "@_OS_RELEASE@"
#define OS_VERSION "@_OS_VERSION@"
#define OS_PLATFORM "@_OS_PLATFORM@"
```

## 如何实施

我们将使用 CMake 为平台填充`config.h`中的定义，并将示例源文件编译为可执行文件:

1. 首先，我们定义了 CMake 最低版本、项目名称和项目语言:

   ```
   cmake_minimum_required(VERSION 3.10 FATAL_ERROR)
   project(recipe-05 CXX)
   ```

2. 然后，定义目标可执行文件及其源文件，并包括目录:

   ```
   add_executable(processor-info "")

   target_sources(processor-info
     PRIVATE
     	processor-info.cpp
     )

   target_include_directories(processor-info
     PRIVATE
    	  ${PROJECT_BINARY_DIR}
     )
   ```

3. 继续查询主机系统的信息，获取一些关键字:

   ```
   foreach(key
     IN ITEMS
       NUMBER_OF_LOGICAL_CORES
       NUMBER_OF_PHYSICAL_CORES
       TOTAL_VIRTUAL_MEMORY
       AVAILABLE_VIRTUAL_MEMORY
       TOTAL_PHYSICAL_MEMORY
       AVAILABLE_PHYSICAL_MEMORY
       IS_64BIT
       HAS_FPU
       HAS_MMX
       HAS_MMX_PLUS
       HAS_SSE
       HAS_SSE2
       HAS_SSE_FP
       HAS_SSE_MMX
       HAS_AMD_3DNOW
       HAS_AMD_3DNOW_PLUS
       HAS_IA64
       OS_NAME
       OS_RELEASE
       OS_VERSION
       OS_PLATFORM
     )
     cmake_host_system_information(RESULT _${key} QUERY ${key})
   endforeach()
   ```

4. 定义了相应的变量后，配置`config.h`:

   ```
   configure_file(config.h.in config.h @ONLY)
   ```

5. 现在准备好配置、构建和测试项目:

   ```
   $ mkdir -p build
   $ cd build
   $ cmake ..
   $ cmake --build .
   $ ./processor-info

   Number of logical cores: 4
   Number of physical cores: 2
   Total virtual memory in megabytes: 15258
   Available virtual memory in megabytes: 14678
   Total physical memory in megabytes: 7858
   Available physical memory in megabytes: 4072
   Processor is 64Bit: 1
   Processor has floating point unit: 1
   Processor supports MMX instructions: 1
   Processor supports Ext. MMX instructions: 0
   Processor supports SSE instructions: 1
   Processor supports SSE2 instructions: 1
   Processor supports SSE FP instructions: 0
   Processor supports SSE MMX instructions: 0
   Processor supports 3DNow instructions: 0
   Processor supports 3DNow+ instructions: 0
   IA64 processor emulating x86 : 0
   OS name: Linux
   OS sub-type: 4.16.7-1-ARCH
   OS build ID: #1 SMP PREEMPT Wed May 2 21:12:36 UTC 2018
   OS platform: x86_64
   ```

6. 输出会随着处理器的不同而变化。

## 工作原理

`CMakeLists.txt`中的`foreach`循环会查询多个键值，并定义相应的变量。此示例的核心函数是`cmake_host_system_information`，它查询运行 CMake 的主机系统的系统信息。本例中，我们对每个键使用了一个函数调用。然后，使用这些变量来配置`config.h.in`中的占位符，输入并生成`config.h`。此配置使用`configure_file`命令完成。最后，`config.h`包含在`processor-info.cpp`中。编译后，它将把值打印到屏幕上。我们将在第 5 章(配置时和构建时操作)和第 6 章(生成源代码)中重新讨论这种方法。

## 更多信息

对于更细粒度的处理器指令集检测，请考虑以下模块: https://github.com/VcDevel/Vc/blob/master/cmake/OptimizeForArchitecture.cmake 。有时候，构建代码的主机可能与运行代码的主机不一样。在计算集群中，登录节点的体系结构可能与计算节点上的体系结构不同。解决此问题的一种方法是，将配置和编译作为计算步骤，提交并部署到相应计算节点上。

# 2.6 为 Eigen 库使能向量化

**NOTE**:_此示例代码可以在 codes/chapter-02/recipe-06 中找到，包含一个 C++示例。该示例在 CMake 3.5 版(或更高版本)中是有效的，并且已经在 GNU/Linux、macOS 和 Windows 上进行过测试。_

处理器的向量功能，可以提高代码的性能。对于某些类型的运算来说尤为甚之，例如：线性代数。本示例将展示如何使能矢量化，以便使用线性代数的 Eigen C++库加速可执行文件。

## 准备工作

我们用 Eigen C++模板库，用来进行线性代数计算，并展示如何设置编译器标志来启用向量化。这个示例的源代码`linear-algebra.cpp`文件:

```
#include <chrono>
#include <iostream>

#include <Eigen/Dense>

EIGEN_DONT_INLINE
double simple_function(Eigen::VectorXd &va, Eigen::VectorXd &vb)
{
  // this simple function computes the dot product of two vectors
  // of course it could be expressed more compactly
  double d = va.dot(vb);
  return d;
}

int main()
{
  int len = 1000000;
  int num_repetitions = 100;

  // generate two random vectors
  Eigen::VectorXd va = Eigen::VectorXd::Random(len);
  Eigen::VectorXd vb = Eigen::VectorXd::Random(len);

  double result;
  auto start = std::chrono::system_clock::now();
  for (auto i = 0; i < num_repetitions; i++)
  {
    result = simple_function(va, vb);
  }
  auto end = std::chrono::system_clock::now();
  auto elapsed_seconds = end - start;

  std::cout << "result: " << result << std::endl;
  std::cout << "elapsed seconds: " << elapsed_seconds.count() << std::endl;
}
```

我们期望向量化可以加快`simple_function`中的点积操作。

## 如何实施

根据 Eigen 库的文档，设置适当的编译器标志就足以生成向量化的代码。让我们看看`CMakeLists.txt`:

1. 声明一个`C++11`项目:

   ```
   cmake_minimum_required(VERSION 3.5 FATAL_ERROR)

   project(recipe-06 LANGUAGES CXX)

   set(CMAKE_CXX_STANDARD 11)
   set(CMAKE_CXX_EXTENSIONS OFF)
   set(CMAKE_CXX_STANDARD_REQUIRED ON)
   ```

2. 使用 Eigen 库，我们需要在系统上找到它的头文件:

   ```
   find_package(Eigen3 3.3 REQUIRED CONFIG)
   ```

3. `CheckCXXCompilerFlag.cmake`标准模块文件:

   ```
   include(CheckCXXCompilerFlag)
   ```

4. 检查`-march=native`编译器标志是否工作:

   ```
   check_cxx_compiler_flag("-march=native" _march_native_works)
   ```

5. 另一个选项`-xHost`编译器标志也开启:

   ```
   check_cxx_compiler_flag("-xHost" _xhost_works)
   ```

6. 设置了一个空变量`_CXX_FLAGS`，来保存刚才检查的两个编译器中找到的编译器标志。如果看到`_march_native_works`，我们将`_CXX_FLAGS`设置为`-march=native`。如果看到`_xhost_works`，我们将`_CXX_FLAGS`设置为`-xHost`。如果它们都不起作用，`_CXX_FLAGS`将为空，并禁用矢量化:

   ```
   set(_CXX_FLAGS)
   if(_march_native_works)
   	message(STATUS "Using processor's vector instructions (-march=native compiler flag set)")
   	set(_CXX_FLAGS "-march=native")
   elseif(_xhost_works)
   	message(STATUS "Using processor's vector instructions (-xHost compiler flag set)")
   	set(_CXX_FLAGS "-xHost")
   else()
   	message(STATUS "No suitable compiler flag found for vectorization")
   endif()
   ```

7. 为了便于比较，我们还为未优化的版本定义了一个可执行目标，不使用优化标志:

   ```
   add_executable(linear-algebra-unoptimized linear-algebra.cpp)

   target_link_libraries(linear-algebra-unoptimized
     PRIVATE
     	Eigen3::Eigen
     )
   ```

8. 此外，我们定义了一个优化版本:

   ```
   add_executable(linear-algebra linear-algebra.cpp)

   target_compile_options(linear-algebra
     PRIVATE
     	${_CXX_FLAGS}
     )

   target_link_libraries(linear-algebra
     PRIVATE
     	Eigen3::Eigen
     )
   ```

9. 让我们比较一下这两个可执行文件——首先我们配置(在本例中，`-march=native_works`):

   ```
   $ mkdir -p build
   $ cd build
   $ cmake ..

   ...
   -- Performing Test _march_native_works
   -- Performing Test _march_native_works - Success
   -- Performing Test _xhost_works
   -- Performing Test _xhost_works - Failed
   -- Using processor's vector instructions (-march=native compiler flag set)
   ...
   ```

10. 最后，让我们编译可执行文件，并比较运行时间:

    ```
    $ cmake --build .
    $ ./linear-algebra-unoptimized

    result: -261.505
    elapsed seconds: 1.97964

    $ ./linear-algebra

    result: -261.505
    elapsed seconds: 1.05048
    ```

## 工作原理

大多数处理器提供向量指令集，代码可以利用这些特性，获得更高的性能。由于线性代数运算可以从 Eigen 库中获得很好的加速，所以在使用 Eigen 库时，就要考虑向量化。我们所要做的就是，指示编译器为我们检查处理器，并为当前体系结构生成本机指令。不同的编译器供应商会使用不同的标志来实现这一点：GNU 编译器使用`-march=native`标志来实现这一点，而 Intel 编译器使用`-xHost`标志。使用`CheckCXXCompilerFlag.cmake`模块提供的`check_cxx_compiler_flag`函数进行编译器标志的检查:

```
check_cxx_compiler_flag("-march=native" _march_native_works)
```

这个函数接受两个参数:

- 第一个是要检查的编译器标志。
- 第二个是用来存储检查结果(true 或 false)的变量。如果检查为真，我们将工作标志添加到`_CXX_FLAGS`变量中，该变量将用于为可执行目标设置编译器标志。

## 更多信息

本示例可与前一示例相结合，可以使用`cmake_host_system_information`查询处理器功能。
