# 检测外部库和程序

本章中主要内容有:

- 检测 Python 解释器
- 检测 Python 库
- 检测 Python 模块和包
- 检测 BLAS 和 LAPACK 数学库
- 检测 OpenMP 并行环境
- 检测 MPI 并行环境
- 检测 Eigen 库
- 检测 Boost 库
- 检测外部库:Ⅰ. 使用 pkg-config
- 检测外部库:Ⅱ. 书写 find 模块

我们的项目常常会依赖于其他项目和库。本章将演示，如何检测外部库、框架和项目，以及如何链接到这些库。CMake 有一组预打包模块，用于检测常用库和程序，例如：Python 和 Boost。可以使用`cmake --help-module-list`获得现有模块的列表。但是，不是所有的库和程序都包含在其中，有时必须自己编写检测脚本。本章将讨论相应的工具，了解 CMake 的`find`族命令:

- **find_file**：在相应路径下查找命名文件
- **find_library**：查找一个库文件
- **find_package**：从外部项目查找和加载设置
- **find_path**：查找包含指定文件的目录
- **find_program**：找到一个可执行程序

**NOTE**:_可以使用`--help-command`命令行显示 CMake 内置命令的打印文档。_

# 3.2 检测 Python 库

**NOTE**:_此示例代码可以在 https://github.com/devcafe/cmake-cookbook/tree/v1.0/chapter-03/recipe-02 中找到，有一个 C 示例。该示例在 CMake 3.5 版(或更高版本)中是有效的，并且已经在 GNU/Linux、macOS 和 Windows 上进行过测试。_

可以使用 Python 工具来分析和操作程序的输出。然而，还有更强大的方法可以将解释语言(如 Python)与编译语言(如 C 或 C++)组合在一起使用。一种是扩展 Python，通过编译成共享库的 C 或 C++模块在这些类型上提供新类型和新功能，这是第 9 章的主题。另一种是将 Python 解释器嵌入到 C 或 C++程序中。两种方法都需要下列条件:

- Python 解释器的工作版本
- Python 头文件 Python.h 的可用性
- Python 运行时库 libpython

三个组件所使用的 Python 版本必须相同。我们已经演示了如何找到 Python 解释器；本示例中，我们将展示另外两种方式。

## 准备工作

我们将一个简单的 Python 代码，嵌入到 C 程序中，可以在 Python 文档页面上找到。源文件称为`hello-embedded-python.c`:

```
#include <Python.h>

int main(int argc, char *argv[]) {
  Py_SetProgramName(argv[0]); /* optional but recommended */
  Py_Initialize();
  PyRun_SimpleString("from time import time,ctime\n"
                     "print 'Today is',ctime(time())\n");
  Py_Finalize();
  return 0;
}
```

此代码将在程序中初始化 Python 解释器的实例，并使用 Python 的`time`模块，打印日期。

**NOTE**:_嵌入代码可以在 Python 文档页面的 https://docs.python.org/2/extending/embedding.html 和 https://docs.python.org/3/extending/embedding.html 中找到。_

## 具体实施

以下是`CMakeLists.txt`中的步骤:

1. 包含 CMake 最低版本、项目名称和所需语言:

   ```
   cmake_minimum_required(VERSION 3.5 FATAL_ERROR)
   project(recipe-02 LANGUAGES C)
   ```

2. 制使用 C99 标准，这不严格要求与 Python 链接，但有时你可能需要对 Python 进行连接:

   ```
   set(CMAKE_C_STANDARD 99)
   set(CMAKE_C_EXTENSIONS OFF)
   set(CMAKE_C_STANDARD_REQUIRED ON)
   ```

3. 找到 Python 解释器。这是一个`REQUIRED`依赖:

   ```
   find_package(PythonInterp REQUIRED)
   ```

4. 找到 Python 头文件和库的模块，称为`FindPythonLibs.cmake`:

   ```
   find_package(PythonLibs ${PYTHON_VERSION_MAJOR}.${PYTHON_VERSION_MINOR} EXACT REQUIRED)
   ```

5. 使用`hello-embedded-python.c`源文件，添加一个可执行目标:

   ```
   add_executable(hello-embedded-python hello-embedded-python.c)
   ```

6. 可执行文件包含`Python.h`头文件。因此，这个目标的`include`目录必须包含 Python 的`include`目录，可以通过`PYTHON_INCLUDE_DIRS`变量进行指定:

   ```
   target_include_directories(hello-embedded-python
     PRIVATE
     	${PYTHON_INCLUDE_DIRS}
   	)
   ```

7. 最后，将可执行文件链接到 Python 库，通过`PYTHON_LIBRARIES`变量访问:

   ```
   target_link_libraries(hello-embedded-python
     PRIVATE
     	${PYTHON_LIBRARIES}
   	)
   ```

8. 现在，进行构建:

   ```
   $ mkdir -p build
   $ cd build
   $ cmake ..

   ...
   -- Found PythonInterp: /usr/bin/python (found version "3.6.5")
   -- Found PythonLibs: /usr/lib/libpython3.6m.so (found suitable exact version "3.6.5")
   ```

9. 最后，执行构建，并运行可执行文件:

   ```
   $ cmake --build .
   $ ./hello-embedded-python

   Today is Thu Jun 7 22:26:02 2018
   ```

## 工作原理

`FindPythonLibs.cmake`模块将查找 Python 头文件和库的标准位置。由于，我们的项目需要这些依赖项，如果没有找到这些依赖项，将停止配置，并报出错误。

注意，我们显式地要求 CMake 检测安装的 Python 可执行文件。这是为了确保可执行文件、头文件和库都有一个匹配的版本。这对于不同版本，可能在运行时导致崩溃。我们通过`FindPythonInterp.cmake`中定义的`PYTHON_VERSION_MAJOR`和`PYTHON_VERSION_MINOR`来实现:

```
find_package(PythonInterp REQUIRED)
find_package(PythonLibs ${PYTHON_VERSION_MAJOR}.${PYTHON_VERSION_MINOR} EXACT REQUIRED)
```

使用`EXACT`关键字，限制 CMake 检测特定的版本，在本例中是匹配的相应 Python 版本的包括文件和库。我们可以使用`PYTHON_VERSION_STRING`变量，进行更接近的匹配:

```
find_package(PythonInterp REQUIRED)
find_package(PythonLibs ${PYTHON_VERSION_STRING} EXACT REQUIRED)
```

## 更多信息

当 Python 不在标准安装目录中，我们如何确定 Python 头文件和库的位置是正确的？对于 Python 解释器，可以通过 CLI 的`-D`选项传递`PYTHON_LIBRARY`和`PYTHON_INCLUDE_DIR`选项来强制 CMake 查找特定的目录。这些选项指定了以下内容:

- **PYTHON_LIBRARY**：指向 Python 库的路径
- **PYTHON_INCLUDE_DIR**：Python.h 所在的路径

这样，就能获得所需的 Python 版本。

**TIPS**:_有时需要将`-D PYTHON_EXECUTABLE`、`-D PYTHON_LIBRARY`和`-D PYTHON_INCLUDE_DIR`传递给 CMake CLI，以便找到及定位相应的版本的组件。_

要将 Python 解释器及其开发组件匹配为完全相同的版本可能非常困难，对于那些将它们安装在非标准位置或系统上安装了多个版本的情况尤其如此。CMake 3.12 版本中增加了新的 Python 检测模块，旨在解决这个棘手的问题。我们`CMakeLists.txt`的检测部分也将简化为:

```
find_package(Python COMPONENTS Interpreter Development REQUIRED)
```

我们建议您阅读新模块的文档，地址是: https://cmake.org/cmake/help/v3.12/module/FindPython.html

# 3.3 检测 Python 模块和包

**NOTE**:_此示例代码可以在 https://github.com/devcafe/cmake-cookbook/tree/v1.0/chapter-03/recipe-03 中找到，包含一个 C++示例。该示例在 CMake 3.5 版(或更高版本)中是有效的，并且已经在 GNU/Linux、macOS 和 Windows 上进行过测试。_

前面的示例中，我们演示了如何检测 Python 解释器，以及如何编译一个简单的 C 程序(嵌入 Python 解释器)。通常，代码将依赖于特定的 Python 模块，无论是 Python 工具、嵌入 Python 的程序，还是扩展 Python 的库。例如，科学界非常流行使用 NumPy 处理矩阵问题。依赖于 Python 模块或包的项目中，确定满足对这些 Python 模块的依赖非常重要。本示例将展示如何探测用户的环境，以找到特定的 Python 模块和包。

## 准备工作

我们将尝试在 C++程序中嵌入一个稍微复杂一点的例子。这个示例再次引用[Python 在线文档](https://docs.python.org/3.5/extending/embedding.html#pureembedded)，并展示了如何通过调用编译后的 C++可执行文件，来执行用户定义的 Python 模块中的函数。

Python 3 示例代码(`Py3-pure-embedding.cpp`)包含以下源代码(请参见https://docs.python.org/2/extending/embedding.html#pure-embedded 与 Python 2 代码等效):

```
#include <Python.h>
int main(int argc, char* argv[]) {
  PyObject* pName, * pModule, * pDict, * pFunc;
  PyObject* pArgs, * pValue;
  int i;
  if (argc < 3) {
    fprintf(stderr, "Usage: pure-embedding pythonfile funcname [args]\n");
    return 1;
  }
  Py_Initialize();
  PyRun_SimpleString("import sys");
  PyRun_SimpleString("sys.path.append(\".\")");
  pName = PyUnicode_DecodeFSDefault(argv[1]);
  /* Error checking of pName left out */
  pModule = PyImport_Import(pName);
  Py_DECREF(pName);
  if (pModule != NULL) {
    pFunc = PyObject_GetAttrString(pModule, argv[2]);
    /* pFunc is a new reference */
    if (pFunc && PyCallable_Check(pFunc)) {
      pArgs = PyTuple_New(argc - 3);
      for (i = 0; i < argc - 3; ++i) {
        pValue = PyLong_FromLong(atoi(argv[i + 3]));
        if (!pValue) {
          Py_DECREF(pArgs);
          Py_DECREF(pModule);
          fprintf(stderr, "Cannot convert argument\n");
          return 1;
        }
        /* pValue reference stolen here: */
        PyTuple_SetItem(pArgs, i, pValue);
      }
      pValue = PyObject_CallObject(pFunc, pArgs);
      Py_DECREF(pArgs);
      if (pValue != NULL) {
        printf("Result of call: %ld\n", PyLong_AsLong(pValue));
        Py_DECREF(pValue);
      }
      else {
        Py_DECREF(pFunc);
        Py_DECREF(pModule);
        PyErr_Print();
        fprintf(stderr, "Call failed\n");
        return 1;
      }
    }
    else {
      if (PyErr_Occurred())
        PyErr_Print();
      fprintf(stderr, "Cannot find function \"%s\"\n", argv[2]);
    }
    Py_XDECREF(pFunc);
    Py_DECREF(pModule);
  }
  else {
    PyErr_Print();
    fprintf(stderr, "Failed to load \"%s\"\n", argv[1]);
    return 1;
  }
  Py_Finalize();
  return 0;
}
```

我们希望嵌入的 Python 代码(`use_numpy.py`)使用 NumPy 设置一个矩阵，所有矩阵元素都为 1.0:

```
import numpy as np
def print_ones(rows, cols):
  A = np.ones(shape=(rows, cols), dtype=float)
  print(A)

  # we return the number of elements to verify
  # that the C++ code is able to receive return values
  num_elements = rows*cols
  return(num_elements)
```

## 具体实施

下面的代码中，我们能够使用 CMake 检查 NumPy 是否可用。我们需要确保 Python 解释器、头文件和库在系统上是可用的。然后，将再来确认 NumPy 的可用性：

1. 首先，我们定义了最低 CMake 版本、项目名称、语言和 C++标准:

   ```
   cmake_minimum_required(VERSION 3.5 FATAL_ERROR)
   project(recipe-03 LANGUAGES CXX)
   set(CMAKE_CXX_STANDARD 11)
   set(CMAKE_CXX_EXTENSIONS OFF)
   set(CMAKE_CXX_STANDARD_REQUIRED ON)
   ```

2. 查找解释器、头文件和库的方法与前面的方法完全相同:

   ```
   find_package(PythonInterp REQUIRED)
   find_package(PythonLibs ${PYTHON_VERSION_MAJOR}.${PYTHON_VERSION_MINOR} EXACT REQUIRED)
   ```

3. 正确打包的 Python 模块，指定安装位置和版本。可以在`CMakeLists.txt`中执行 Python 脚本进行探测:

   ```
   execute_process(
     COMMAND
     	${PYTHON_EXECUTABLE} "-c" "import re, numpy; print(re.compile('/__init__.py.*').sub('',numpy.__file__))"
     RESULT_VARIABLE _numpy_status
     OUTPUT_VARIABLE _numpy_location
     ERROR_QUIET
     OUTPUT_STRIP_TRAILING_WHITESPACE
     )
   ```

4. 如果找到 NumPy，则`_numpy_status`变量为整数，否则为错误的字符串，而`_numpy_location`将包含 NumPy 模块的路径。如果找到 NumPy，则将它的位置保存到一个名为`NumPy`的新变量中。注意，新变量被缓存，这意味着 CMake 创建了一个持久性变量，用户稍后可以修改该变量:

   ```
   if(NOT _numpy_status)
   	set(NumPy ${_numpy_location} CACHE STRING "Location of NumPy")
   endif()
   ```

5. 下一步是检查模块的版本。同样，我们在`CMakeLists.txt`中施加了一些 Python 魔法，将版本保存到`_numpy_version`变量中:

   ```
   execute_process(
     COMMAND
     	${PYTHON_EXECUTABLE} "-c" "import numpy; print(numpy.__version__)"
     OUTPUT_VARIABLE _numpy_version
     ERROR_QUIET
     OUTPUT_STRIP_TRAILING_WHITESPACE
     )
   ```

6. 最后，`FindPackageHandleStandardArgs`的 CMake 包以正确的格式设置`NumPy_FOUND`变量和输出信息:

   ```
   include(FindPackageHandleStandardArgs)
   find_package_handle_standard_args(NumPy
     FOUND_VAR NumPy_FOUND
     REQUIRED_VARS NumPy
     VERSION_VAR _numpy_version
     )
   ```

7. 一旦正确的找到所有依赖项，我们就可以编译可执行文件，并将其链接到 Python 库:

   ```
   add_executable(pure-embedding "")

   target_sources(pure-embedding
     PRIVATE
     	Py${PYTHON_VERSION_MAJOR}-pure-embedding.cpp
     )

   target_include_directories(pure-embedding
     PRIVATE
     	${PYTHON_INCLUDE_DIRS}
     )

   target_link_libraries(pure-embedding
     PRIVATE
     	${PYTHON_LIBRARIES}
     )
   ```

8. 我们还必须保证`use_numpy.py`在`build`目录中可用:

   ```
   add_custom_command(
     OUTPUT
     	${CMAKE_CURRENT_BINARY_DIR}/use_numpy.py
     COMMAND
     	${CMAKE_COMMAND} -E copy_if_different ${CMAKE_CURRENT_SOURCE_DIR}/use_numpy.py
    	 ${CMAKE_CURRENT_BINARY_DIR}/use_numpy.py
     DEPENDS
     	${CMAKE_CURRENT_SOURCE_DIR}/use_numpy.py
     )

   # make sure building pure-embedding triggers the above custom command
   target_sources(pure-embedding
     PRIVATE
     	${CMAKE_CURRENT_BINARY_DIR}/use_numpy.py
     )
   ```

9. 现在，我们可以测试嵌入的代码:

   ```
   $ mkdir -p build
   $ cd build
   $ cmake ..

   -- ...
   -- Found PythonInterp: /usr/bin/python (found version "3.6.5")
   -- Found PythonLibs: /usr/lib/libpython3.6m.so (found suitable exact version "3.6.5")
   -- Found NumPy: /usr/lib/python3.6/site-packages/numpy (found version "1.14.3")

   $ cmake --build .
   $ ./pure-embedding use_numpy print_ones 2 3

   [[1. 1. 1.]
   [1. 1. 1.]]
   Result of call: 6
   ```

## 工作原理

例子中有三个新的 CMake 命令，需要`include(FindPackageHandleStandardArgs)`：

- `execute_process`
- `add_custom_command`
- `find_package_handle_standard_args`

`execute_process`将作为通过子进程执行一个或多个命令。最后，子进程返回值将保存到变量作为参数，传递给`RESULT_VARIABLE`，而管道标准输出和标准错误的内容将被保存到变量作为参数传递给`OUTPUT_VARIABLE`和`ERROR_VARIABLE`。`execute_process`可以执行任何操作，并使用它们的结果来推断系统配置。本例中，用它来确保 NumPy 可用，然后获得模块版本。

`find_package_handle_standard_args`提供了，用于处理与查找相关程序和库的标准工具。引用此命令时，可以正确的处理与版本相关的选项(`REQUIRED`和`EXACT`)，而无需更多的 CMake 代码。稍后将介绍`QUIET`和`COMPONENTS`选项。本示例中，使用了以下方法:

```
include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(NumPy
  FOUND_VAR NumPy_FOUND
  REQUIRED_VARS NumPy
  VERSION_VAR _numpy_version
  )
```

所有必需的变量都设置为有效的文件路径(NumPy)后，发送到模块(`NumPy_FOUND`)。它还将版本保存在可传递的版本变量(`_numpy_version`)中并打印:

```
-- Found NumPy: /usr/lib/python3.6/site-packages/numpy (found version "1.14.3")
```

目前的示例中，没有进一步使用这些变量。如果返回`NumPy_FOUND`为`FALSE`，则停止配置。

最后，将`use_numpy.py`复制到`build`目录，对代码进行注释:

```
add_custom_command(
  OUTPUT
  	${CMAKE_CURRENT_BINARY_DIR}/use_numpy.py
  COMMAND
  	${CMAKE_COMMAND} -E copy_if_different ${CMAKE_CURRENT_SOURCE_DIR}/use_numpy.py
  	${CMAKE_CURRENT_BINARY_DIR}/use_numpy.py
  DEPENDS
  	${CMAKE_CURRENT_SOURCE_DIR}/use_numpy.py
  )

target_sources(pure-embedding
  PRIVATE
  	${CMAKE_CURRENT_BINARY_DIR}/use_numpy.py
  )
```

我们也可以使用`file(COPY…)`命令来实现复制。这里，我们选择使用`add_custom_command`，来确保文件在每次更改时都会被复制，而不仅仅是第一次运行配置时。我们将在第 5 章更详细地讨论`add_custom_command`。还要注意`target_sources`命令，它将依赖项添加到`${CMAKE_CURRENT_BINARY_DIR}/use_numpy.py`；这样做是为了确保构建目标，能够触发之前的命令。

# 3.4 检测 BLAS 和 LAPACK 数学库

**NOTE**:_此示例代码可以在 codes/chapter-03/recipe-04 中找到，有一个 C++示例。该示例在 CMake 3.5 版(或更高版本)中是有效的，并且已经在 GNU/Linux、macOS 和 Windows 上进行过测试。_

许多数据算法严重依赖于矩阵和向量运算。例如：矩阵-向量和矩阵-矩阵乘法，求线性方程组的解，特征值和特征向量的计算或奇异值分解。这些操作在代码库中非常普遍，因为操作的数据量比较大，因此高效的实现有绝对的必要。幸运的是，有专家库可用：基本线性代数子程序(BLAS)和线性代数包(LAPACK)，为许多线性代数操作提供了标准 API。供应商有不同的实现，但都共享 API。虽然，用于数学库底层实现，实际所用的编程语言会随着时间而变化(Fortran、C、Assembly)，但是也都是 Fortran 调用接口。考虑到调用街扩，本示例中的任务要链接到这些库，并展示如何用不同语言编写的库。

## 准备工作

为了展示数学库的检测和连接，我们编译一个 C++程序，将矩阵的维数作为命令行输入，生成一个随机的方阵**A**，一个随机向量**b**，并计算线性系统方程: **Ax = b**。另外，将对向量**b**的进行随机缩放。这里，需要使用的子程序是 BLAS 中的 DSCAL 和 LAPACK 中的 DGESV 来求线性方程组的解。示例 C++代码的清单( `linear-algebra.cpp`)：

```
#include "CxxBLAS.hpp"
#include "CxxLAPACK.hpp"

#include <iostream>
#include <random>
#include <vector>

int main(int argc, char** argv) {
  if (argc != 2) {
    std::cout << "Usage: ./linear-algebra dim" << std::endl;
    return EXIT_FAILURE;
  }

  // Generate a uniform distribution of real number between -1.0 and 1.0
  std::random_device rd;
  std::mt19937 mt(rd());
  std::uniform_real_distribution<double> dist(-1.0, 1.0);

  // Allocate matrices and right-hand side vector
  int dim = std::atoi(argv[1]);
  std::vector<double> A(dim * dim);
  std::vector<double> b(dim);
  std::vector<int> ipiv(dim);
  // Fill matrix and RHS with random numbers between -1.0 and 1.0
  for (int r = 0; r < dim; r++) {
    for (int c = 0; c < dim; c++) {
      A[r + c * dim] = dist(mt);
    }
    b[r] = dist(mt);
  }

  // Scale RHS vector by a random number between -1.0 and 1.0
  C_DSCAL(dim, dist(mt), b.data(), 1);
  std::cout << "C_DSCAL done" << std::endl;

  // Save matrix and RHS
  std::vector<double> A1(A);
  std::vector<double> b1(b);
  int info;
  info = C_DGESV(dim, 1, A.data(), dim, ipiv.data(), b.data(), dim);
  std::cout << "C_DGESV done" << std::endl;
  std::cout << "info is " << info << std::endl;

  double eps = 0.0;
  for (int i = 0; i < dim; ++i) {
    double sum = 0.0;
    for (int j = 0; j < dim; ++j)
      sum += A1[i + j * dim] * b[j];
    eps += std::abs(b1[i] - sum);
  }
  std::cout << "check is " << eps << std::endl;

  return 0;
}
```

使用 C++11 的随机库来生成-1.0 到 1.0 之间的随机分布。`C_DSCAL`和`C_DGESV`分别是到 BLAS 和 LAPACK 库的接口。为了避免名称混淆，将在下面来进一步讨论 CMake 模块：

文件`CxxBLAS.hpp`用`extern "C"`封装链接 BLAS:

```
#pragma once
#include "fc_mangle.h"
#include <cstddef>
#ifdef __cplusplus
extern "C" {
#endif
extern void DSCAL(int *n, double *alpha, double *vec, int *inc);
#ifdef __cplusplus
}
#endif
void C_DSCAL(size_t length, double alpha, double *vec, int inc);
```

对应的实现文件`CxxBLAS.cpp`:

```
#include "CxxBLAS.hpp"

#include <climits>

// see http://www.netlib.no/netlib/blas/dscal.f
void C_DSCAL(size_t length, double alpha, double *vec, int inc) {
  int big_blocks = (int)(length / INT_MAX);
  int small_size = (int)(length % INT_MAX);
  for (int block = 0; block <= big_blocks; block++) {
    double *vec_s = &vec[block * inc * (size_t)INT_MAX];
    signed int length_s = (block == big_blocks) ? small_size : INT_MAX;
    ::DSCAL(&length_s, &alpha, vec_s, &inc);
  }
}
```

`CxxLAPACK.hpp`和`CxxLAPACK.cpp`为 LAPACK 调用执行相应的转换。

## 具体实施

对应的`CMakeLists.txt`包含以下构建块:

1. 我们定义了 CMake 最低版本，项目名称和支持的语言:

   ```
   cmake_minimum_required(VERSION 3.5 FATAL_ERROR)
   project(recipe-04 LANGUAGES CXX C Fortran)
   ```

2. 使用 C++11 标准:

   ```
   set(CMAKE_CXX_STANDARD 11)
   set(CMAKE_CXX_EXTENSIONS OFF)
   set(CMAKE_CXX_STANDARD_REQUIRED ON)
   ```

3. 此外，我们验证 Fortran 和 C/C++编译器是否能协同工作，并生成头文件，这个文件可以处理名称混乱。两个功能都由`FortranCInterface`模块提供:

   ```
   include(FortranCInterface)

   FortranCInterface_VERIFY(CXX)

   FortranCInterface_HEADER(
     fc_mangle.h
     MACRO_NAMESPACE "FC_"
     SYMBOLS DSCAL DGESV
     )
   ```

4. 然后，找到 BLAS 和 LAPACK:

   ```
   find_package(BLAS REQUIRED)
   find_package(LAPACK REQUIRED)
   ```

5. 接下来，添加一个库，其中包含 BLAS 和 LAPACK 包装器的源代码，并链接到`LAPACK_LIBRARIES`，其中也包含`BLAS_LIBRARIES`:

   ```
   add_library(math "")

   target_sources(math
     PRIVATE
       CxxBLAS.cpp
       CxxLAPACK.cpp
     )

   target_include_directories(math
     PUBLIC
       ${CMAKE_CURRENT_SOURCE_DIR}
       ${CMAKE_CURRENT_BINARY_DIR}
     )

   target_link_libraries(math
     PUBLIC
     	${LAPACK_LIBRARIES}
     )
   ```

6. 注意，目标的包含目录和链接库声明为`PUBLIC`，因此任何依赖于数学库的附加目标也将在其包含目录中。

7. 最后，我们添加一个可执行目标并链接`math`：

   ```
   add_executable(linear-algebra "")

   target_sources(linear-algebra
     PRIVATE
     	linear-algebra.cpp
     )

   target_link_libraries(linear-algebra
     PRIVATE
     	math
     )
   ```

8. 配置时，我们可以关注相关的打印输出:

   ```
   $ mkdir -p build
   $ cd build
   $ cmake ..

   ...
   -- Detecting Fortran/C Interface
   -- Detecting Fortran/C Interface - Found GLOBAL and MODULE mangling
   -- Verifying Fortran/C Compiler Compatibility
   -- Verifying Fortran/C Compiler Compatibility - Success
   ...
   -- Found BLAS: /usr/lib/libblas.so
   ...
   -- A library with LAPACK API found.
   ...
   ```

9. 最后，构建并测试可执行文件:

   ```
   $ cmake --build .
   $ ./linear-algebra 1000

   C_DSCAL done
   C_DGESV done
   info is 0
   check is 1.54284e-10
   ```

## 工作原理

`FindBLAS.cmake`和`FindLAPACK.cmake`将在标准位置查找 BLAS 和 LAPACK 库。对于前者，该模块有`SGEMM`函数的 Fortran 实现，一般用于单精度矩阵乘积。对于后者，该模块有`CHEEV`函数的 Fortran 实现，用于计算复杂厄米矩阵的特征值和特征向量。查找在 CMake 内部，通过编译一个小程序来完成，该程序调用这些函数，并尝试链接到候选库。如果失败，则表示相应库不存于系统上。

生成机器码时，每个编译器都会处理符号混淆，不幸的是，这种操作并不通用，而与编译器相关。为了解决这个问题，我们使用`FortranCInterface`模块( https://cmake.org/cmake/help/v3.5/module/FortranCInterface.html )验证 Fortran 和 C/C++能否混合编译，然后生成一个 Fortran-C 接口头文件`fc_mangle.h`，这个文件用来解决编译器性的问题。然后，必须将生成的`fc_mann .h`包含在接口头文件`CxxBLAS.hpp`和`CxxLAPACK.hpp`中。为了使用`FortranCInterface`，我们需要在`LANGUAGES`列表中添加 C 和 Fortran 支持。当然，也可以定义自己的预处理器定义，但是可移植性会差很多。

我们将在第 9 章中更详细地讨论 Fortran 和 C 的互操作性。

**NOTE**:_目前，BLAS 和 LAPACK 的许多实现已经在 Fortran 外附带了一层 C 包装。这些包装器多年来已经标准化，称为 CBLAS 和 LAPACKE。_

## 更多信息

许多算法代码比较依赖于矩阵代数运算，使用 BLAS 和 LAPACK API 的高性能实现就非常重要了。供应商为不同的体系结构和并行环境提供不同的库，`FindBLAS.cmake`和`FindLAPACK.cmake`可能的无法定位到当前库。如果发生这种情况，可以通过`-D`选项显式地从 CLI 对库进行设置。

# 3.5 检测 OpenMP 的并行环境

**NOTE**:_此示例代码可以在 codes/chapter-03/recipe-05 中找到，有一个 C++和一个 Fortran 示例。该示例在 CMake 3.5 版(或更高版本)中是有效的，并且已经在 GNU/Linux、macOS 和 Windows 上进行过测试。codes/chapter-03/recipe-05 中也有一个适用于 CMake 3.5 的示例。_

目前，市面上的计算机几乎都是多核机器，对于性能敏感的程序，我们必须关注这些多核处理器，并在编程模型中使用并发。OpenMP 是多核处理器上并行性的标准之一。为了从 OpenMP 并行化中获得性能收益，通常不需要修改或重写现有程序。一旦确定了代码中的性能关键部分，例如：使用分析工具，程序员就可以通过预处理器指令，指示编译器为这些区域生成可并行的代码。

本示例中，我们将展示如何编译一个包含 OpenMP 指令的程序(前提是使用一个支持 OpenMP 的编译器)。有许多支持 OpenMP 的 Fortran、C 和 C++编译器。对于相对较新的 CMake 版本，为 OpenMP 提供了非常好的支持。本示例将展示如何在使用 CMake 3.9 或更高版本时，使用简单 C++和 Fortran 程序来链接到 OpenMP。

**NOTE**:_根据 Linux 发行版的不同，Clang 编译器的默认版本可能不支持 OpenMP。使用或非苹果版本的 Clang(例如，Conda 提供的)或 GNU 编译器,除非单独安装 libomp 库(https://iscinumpy.gitlab.io/post/omp-on-high-sierra/ )，否则本节示例将无法在 macOS 上工作。_

## 准备工作

C 和 C++程序可以通过包含`omp.h`头文件和链接到正确的库，来使用 OpenMP 功能。编译器将在性能关键部分之前添加预处理指令，并生成并行代码。在本示例中，我们将构建以下示例源代码(`example.cpp`)。这段代码从 1 到 N 求和，其中 N 作为命令行参数:

```
#include <iostream>
#include <omp.h>
#include <string>

int main(int argc, char *argv[])
{
  std::cout << "number of available processors: " << omp_get_num_procs()
            << std::endl;
  std::cout << "number of threads: " << omp_get_max_threads() << std::endl;
  auto n = std::stol(argv[1]);
  std::cout << "we will form sum of numbers from 1 to " << n << std::endl;
  // start timer
  auto t0 = omp_get_wtime();
  auto s = 0LL;
#pragma omp parallel for reduction(+ : s)
  for (auto i = 1; i <= n; i++)
  {
    s += i;
  }
  // stop timer
  auto t1 = omp_get_wtime();

  std::cout << "sum: " << s << std::endl;
  std::cout << "elapsed wall clock time: " << t1 - t0 << " seconds" << std::endl;

  return 0;
}
```

在 Fortran 语言中，需要使用`omp_lib`模块并链接到库。在性能关键部分之前的代码注释中，可以再次使用并行指令。例如：`F90`需要包含以下内容:

```
program example

  use omp_lib

  implicit none

  integer(8) :: i, n, s
  character(len=32) :: arg
  real(8) :: t0, t1

  print *, "number of available processors:", omp_get_num_procs()
  print *, "number of threads:", omp_get_max_threads()

  call get_command_argument(1, arg)
  read(arg , *) n

  print *, "we will form sum of numbers from 1 to", n

  ! start timer
  t0 = omp_get_wtime()

  s = 0
!$omp parallel do reduction(+:s)
  do i = 1, n
  s = s + i
  end do

  ! stop timer
  t1 = omp_get_wtime()

  print *, "sum:", s
  print *, "elapsed wall clock time (seconds):", t1 - t0

end program
```

## 具体实施

对于 C++和 Fortran 的例子，`CMakeLists.txt`将遵循一个模板，该模板在这两种语言上很相似：

1. 两者都定义了 CMake 最低版本、项目名称和语言(CXX 或 Fortran；我们将展示 C++版本):

   ```
   cmake_minimum_required(VERSION 3.9 FATAL_ERROR)
   project(recipe-05 LANGUAGES CXX)
   ```

2. 使用 C++11 标准:

   ```
   set(CMAKE_CXX_STANDARD 11)
   set(CMAKE_CXX_EXTENSIONS OFF)
   set(CMAKE_CXX_STANDARD_REQUIRED ON)
   ```

3. 调用 find_package 来搜索 OpenMP:

   ```
   find_package(OpenMP REQUIRED)
   ```

4. 最后，我们定义可执行目标，并链接到 FindOpenMP 模块提供的导入目标(在 Fortran 的情况下，我们链接到`OpenMP::OpenMP_Fortran`):

   ```
   add_executable(example example.cpp)
   target_link_libraries(example
     PUBLIC
     	OpenMP::OpenMP_CXX
     )
   ```

5. 现在，可以配置和构建代码了:

   ```
   $ mkdir -p build
   $ cd build
   $ cmake ..
   $ cmake --build .
   ```

6. 并行测试(在本例中使用了 4 个内核):

   ```
   $ ./example 1000000000

   number of available processors: 4
   number of threads: 4
   we will form sum of numbers from 1 to 1000000000
   sum: 500000000500000000
   elapsed wall clock time: 1.08343 seconds
   ```

7. 为了比较，我们可以重新运行这个例子，并将 OpenMP 线程的数量设置为 1:

   ```
   $ env OMP_NUM_THREADS=1 ./example 1000000000

   number of available processors: 4
   number of threads: 1
   we will form sum of numbers from 1 to 1000000000
   sum: 500000000500000000
   elapsed wall clock time: 2.96427 seconds
   ```

## 工作原理

我们的示例很简单：编译代码，并运行在多个内核上时，我们会看到加速效果。加速效果并不是`OMP_NUM_THREADS`的倍数，不过本示例中并不关心，因为我们更关注的是如何使用 CMake 配置需要使用 OpenMP 的项目。我们发现链接到 OpenMP 非常简单，这要感谢`FindOpenMP`模块:

```
target_link_libraries(example
	PUBLIC
		OpenMP::OpenMP_CXX
	)
```

我们不关心编译标志或包含目录——这些设置和依赖项是在`OpenMP::OpenMP_CXX`中定义的(`IMPORTED`类型)。如第 1 章第 3 节中提到的，`IMPORTED`库是伪目标，它完全是我们自己项目的外部依赖项。要使用 OpenMP，需要设置一些编译器标志，包括目录和链接库。所有这些都包含在`OpenMP::OpenMP_CXX`的属性上，并通过使用`target_link_libraries`命令传递给`example`。这使得在 CMake 中，使用库变得非常容易。我们可以使用`cmake_print_properties`命令打印接口的属性，该命令由`CMakePrintHelpers.CMake`模块提供:

```
include(CMakePrintHelpers)
cmake_print_properties(
	TARGETS
		OpenMP::OpenMP_CXX
	PROPERTIES
		INTERFACE_COMPILE_OPTIONS
		INTERFACE_INCLUDE_DIRECTORIES
		INTERFACE_LINK_LIBRARIES
	)
```

所有属性都有`INTERFACE_`前缀，因为这些属性对所需目标，需要以接口形式提供，并且目标以接口的方式使用 OpenMP。

对于低于 3.9 的 CMake 版本:

```
add_executable(example example.cpp)

target_compile_options(example
  PUBLIC
  	${OpenMP_CXX_FLAGS}
  )

set_target_properties(example
  PROPERTIES
  	LINK_FLAGS ${OpenMP_CXX_FLAGS}
  )
```

对于低于 3.5 的 CMake 版本，我们需要为 Fortran 项目显式定义编译标志。

在这个示例中，我们讨论了 C++和 Fortran。相同的参数和方法对于 C 项目也有效。

# 3.6 检测 MPI 的并行环境

**NOTE**:_此示例代码可以在 codes/chapter-03/recipe-06 中找到，包含一个 C++和一个 C 的示例。该示例在 CMake 3.9 版(或更高版本)中是有效的，并且已经在 GNU/Linux、macOS 和 Windows 上进行过测试。codes/chapter-03/recipe-06 中也有一个适用于 CMake 3.5 的 C 示例。_

消息传递接口(Message Passing Interface, MPI)，可以作为 OpenMP(共享内存并行方式)的补充，它也是分布式系统上并行程序的实际标准。尽管，最新的 MPI 实现也允许共享内存并行，但高性能计算中的一种典型方法就是，在计算节点上 OpenMP 与 MPI 结合使用。MPI 标准的实施包括:

1. 运行时库
2. 头文件和 Fortran 90 模块
3. 编译器的包装器，用来调用编译器，使用额外的参数来构建 MPI 库，以处理目录和库。通常，包装器`mpic++/mpiCC/mpicxx`用于 C++，`mpicc`用于 C，`mpifort`用于 Fortran。
4. 启动 MPI：应该启动程序，以编译代码的并行执行。它的名称依赖于实现，可以使用这几个命令启动：`mpirun`、`mpiexec`或`orterun`。

本示例，将展示如何在系统上找到合适的 MPI 实现，从而编译一个简单的“Hello, World”MPI 例程。

## 准备工作

示例代码(`hello-mpi.cpp`，可从[http://www.mpitutorial.com](http://www.mpitutorial.com/) 下载)将在本示例中进行编译，它将初始化 MPI 库，让每个进程打印其名称:

```
#include <iostream>

#include <mpi.h>

int main(int argc, char **argv)
{
  // Initialize the MPI environment. The two arguments to MPI Init are not
  // currently used by MPI implementations, but are there in case future
  // implementations might need the arguments.
  MPI_Init(NULL, NULL);

  // Get the number of processes
  int world_size;
  MPI_Comm_size(MPI_COMM_WORLD, &world_size);

  // Get the rank of the process
  int world_rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

  // Get the name of the processor
  char processor_name[MPI_MAX_PROCESSOR_NAME];
  int name_len;
  MPI_Get_processor_name(processor_name, &name_len);

  // Print off a hello world message
  std::cout << "Hello world from processor " << processor_name << ", rank "
            << world_rank << " out of " << world_size << " processors" << std::endl;

  // Finalize the MPI environment. No more MPI calls can be made after this
  MPI_Finalize();
}
```

## 具体实施

这个示例中，我们先查找 MPI 实现：库、头文件、编译器包装器和启动器。为此，我们将用到`FindMPI.cmake`标准 CMake 模块:

1. 首先，定义了 CMake 最低版本、项目名称、支持的语言和语言标准:

   ```
   cmake_minimum_required(VERSION 3.9 FATAL_ERROR)

   project(recipe-06 LANGUAGES CXX)

   set(CMAKE_CXX_STANDARD 11)
   set(CMAKE_CXX_EXTENSIONS OFF)
   set(CMAKE_CXX_STANDARD_REQUIRED ON)
   ```

2. 然后，调用`find_package`来定位 MPI:

   ```
   find_package(MPI REQUIRED)
   ```

3. 与前面的配置类似，定义了可执行文件的的名称和相关源码，并链接到目标:

   ```
   add_executable(hello-mpi hello-mpi.cpp)

   target_link_libraries(hello-mpi
     PUBLIC
    	  MPI::MPI_CXX
     )
   ```

4. 配置和构建可执行文件:

   ```
   $ mkdir -p build
   $ cd build
   $ cmake .. # -D CMAKE_CXX_COMPILER=mpicxx C++例子中可加，加与不加对于构建结果没有影响╭(╯^╰)╮

   -- ...
   -- Found MPI_CXX: /usr/lib/openmpi/libmpi_cxx.so (found version "3.1")
   -- Found MPI: TRUE (found version "3.1")
   -- ...

   $ cmake --build .
   ```

5. 为了并行执行这个程序，我们使用`mpirun`启动器(本例中，启动了两个任务):

   ```
   $ mpirun -np 2 ./hello-mpi

   Hello world from processor larry, rank 1 out of 2 processors
   Hello world from processor larry, rank 0 out of 2 processors
   ```

## 工作原理

请记住，编译包装器是对 MPI 库编译器的封装。底层实现中，将会调用相同的编译器，并使用额外的参数(如成功构建并行程序所需的头文件包含路径和库)来扩充它。

编译和链接源文件时，包装器用了哪些标志？我们可以使用`--showme`选项来查看。要找出编译器的标志，我们可以这样使用:

```
$ mpicxx --showme:compile

-pthread
```

为了找出链接器标志，我们可以这样:

```
$ mpicxx --showme:link

-pthread -Wl,-rpath -Wl,/usr/lib/openmpi -Wl,--enable-new-dtags -L/usr/lib/openmpi -lmpi_cxx -lmpi
```

与之前的 OpenMP 配置类似，我们发现到 MPI 的链接非常简单，这要归功于`FindMPI`模块提供的目标:

正如在前面的配方中所讨论的，对于 CMake 版本低于 3.9，需要更多的工作量:

```
add_executable(hello-mpi hello-mpi.c)

target_compile_options(hello-mpi
  PUBLIC
  	${MPI_CXX_COMPILE_FLAGS}
  )

target_include_directories(hello-mpi
  PUBLIC
  	${MPI_CXX_INCLUDE_PATH}
  )

target_link_libraries(hello-mpi
  PUBLIC
  	${MPI_CXX_LIBRARIES}
  )
```

本示例中，我们讨论了 C++项目。其中的参数和方法对于 C 或 Fortran 项目同样有效。

# 3.7 检测 Eigen 库

**NOTE**:_此示例代码可以在 codes/chapter-03/recipe-07 中找到，包含一个 C++的示例。该示例在 CMake 3.9 版(或更高版本)中是有效的，并且已经在 GNU/Linux、macOS 和 Windows 上进行过测试。codes/chapter-03/recipe-06 中也有一个适用于 CMake 3.5 的 C++示例。_

BLAS 库为矩阵和向量操作提供了标准化接口。不过，这个接口用 Fortran 语言书写。虽然已经展示了如何使用 C++直接使用这些库，但在现代 C++程序中，希望有更高级的接口。

纯头文件实现的 Eigen 库，使用模板编程来提供接口。矩阵和向量的计算，会在编译时进行数据类型检查，以确保兼容所有维度的矩阵。密集和稀疏矩阵的运算，也可使用表达式模板高效的进行实现，如：矩阵-矩阵乘积，线性系统求解器和特征值问题。从 3.3 版开始，Eigen 可以链接到 BLAS 和 LAPACK 库中，这可以将某些操作实现进行卸载，使库的实现更加灵活，从而获得更多的性能收益。

本示例将展示如何查找 Eigen 库，使用 OpenMP 并行化，并将部分工作转移到 BLAS 库。

本示例中会实现，矩阵-向量乘法和 LU 分解，可以选择卸载 BLAS 和 LAPACK 库中的一些实现。这个示例中，只考虑将在 BLAS 库中卸载。

## 准备工作

本例中，我们编译一个程序，该程序会从命令行获取的随机方阵和维向量。然后我们将用 LU 分解来解线性方程组**Ax=b**。以下是源代码(`linear-algebra.cpp`):

```
#include <chrono>
#include <cmath>
#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <vector>

#include <Eigen/Dense>

int main(int argc, char **argv)
{
  if (argc != 2)
  {
    std::cout << "Usage: ./linear-algebra dim" << std::endl;
    return EXIT_FAILURE;
  }
  std::chrono::time_point<std::chrono::system_clock> start, end;
  std::chrono::duration<double> elapsed_seconds;
  std::time_t end_time;
  std::cout << "Number of threads used by Eigen: " << Eigen::nbThreads()
            << std::endl;

  // Allocate matrices and right-hand side vector
  start = std::chrono::system_clock::now();
  int dim = std::atoi(argv[1]);
  Eigen::MatrixXd A = Eigen::MatrixXd::Random(dim, dim);
  Eigen::VectorXd b = Eigen::VectorXd::Random(dim);
  end = std::chrono::system_clock::now();

  // Report times
  elapsed_seconds = end - start;
  end_time = std::chrono::system_clock::to_time_t(end);
  std::cout << "matrices allocated and initialized "
            << std::put_time(std::localtime(&end_time), "%a %b %d %Y
  %r\n")
            << "elapsed time: " << elapsed_seconds.count() << "s\n";

  start = std::chrono::system_clock::now();
  // Save matrix and RHS
  Eigen::MatrixXd A1 = A;
  Eigen::VectorXd b1 = b;
  end = std::chrono::system_clock::now();
  end_time = std::chrono::system_clock::to_time_t(end);
  std::cout << "Scaling done, A and b saved "
            << std::put_time(std::localtime(&end_time), "%a %b %d %Y %r\n")
            << "elapsed time: " << elapsed_seconds.count() << "s\n";
  start = std::chrono::system_clock::now();
  Eigen::VectorXd x = A.lu().solve(b);
  end = std::chrono::system_clock::now();

  // Report times
  elapsed_seconds = end - start;
  end_time = std::chrono::system_clock::to_time_t(end);
  double relative_error = (A * x - b).norm() / b.norm();
  std::cout << "Linear system solver done "
            << std::put_time(std::localtime(&end_time), "%a %b %d %Y %r\n")
            << "elapsed time: " << elapsed_seconds.count() << "s\n";
  std::cout << "relative error is " << relative_error << std::endl;

  return 0;
}
```

矩阵-向量乘法和 LU 分解是在 Eigen 库中实现的，但是可以选择 BLAS 和 LAPACK 库中的实现。在这个示例中，我们只考虑 BLAS 库中的实现。

## 具体实施

这个示例中，我们将用到 Eigen 和 BLAS 库，以及 OpenMP。使用 OpenMP 将 Eigen 并行化，并从 BLAS 库中卸载部分线性代数实现:

1. 首先声明 CMake 最低版本、项目名称和使用 C++11 语言标准:

   ```
   cmake_minimum_required(VERSION 3.9 FATAL_ERROR)

   project(recipe-07 LANGUAGES CXX)

   set(CMAKE_CXX_STANDARD 11)
   set(CMAKE_CXX_EXTENSIONS OFF)
   set(CMAKE_CXX_STANDARD_REQUIRED ON)
   ```

2. 因为 Eigen 可以使用共享内存的方式，所以可以使用 OpenMP 并行处理计算密集型操作:

   ```
   find_package(OpenMP REQUIRED)
   ```

3. 调用`find_package`来搜索 Eigen(将在下一小节中讨论):

   ```
   find_package(Eigen3 3.3 REQUIRED CONFIG)
   ```

4. 如果找到 Eigen，我们将打印状态信息。注意，使用的是`Eigen3::Eigen`，这是一个`IMPORT`目标，可通过提供的 CMake 脚本找到这个目标:

   ```
   if(TARGET Eigen3::Eigen)
     message(STATUS "Eigen3 v${EIGEN3_VERSION_STRING} found in ${EIGEN3_INCLUDE_DIR}")
   endif()
   ```

5. 接下来，将源文件声明为可执行目标:

   ```
   add_executable(linear-algebra linear-algebra.cpp)
   ```

6. 然后，找到 BLAS。注意，现在不需要依赖项:

   ```
   find_package(BLAS)
   ```

7. 如果找到 BLAS，我们可为可执行目标，设置相应的宏定义和链接库:

   ```
   if(BLAS_FOUND)
     message(STATUS "Eigen will use some subroutines from BLAS.")
     message(STATUS "See: http://eigen.tuxfamily.org/dox-devel/TopicUsingBlasLapack.html")
     target_compile_definitions(linear-algebra
       PRIVATE
       	EIGEN_USE_BLAS
       )
     target_link_libraries(linear-algebra
       PUBLIC
       	${BLAS_LIBRARIES}
       )
   else()
   	message(STATUS "BLAS not found. Using Eigen own functions")
   endif()
   ```

8. 最后，我们链接到`Eigen3::Eigen`和`OpenMP::OpenMP_CXX`目标。这就可以设置所有必要的编译标示和链接标志:

   ```
   target_link_libraries(linear-algebra
     PUBLIC
       Eigen3::Eigen
       OpenMP::OpenMP_CXX
     )
   ```

9. 开始配置:

   ```
   $ mkdir -p build
   $ cd build
   $ cmake ..

   -- ...
   -- Found OpenMP_CXX: -fopenmp (found version "4.5")
   -- Found OpenMP: TRUE (found version "4.5")
   -- Eigen3 v3.3.4 found in /usr/include/eigen3
   -- ...
   -- Found BLAS: /usr/lib/libblas.so
   -- Eigen will use some subroutines from BLAS.
   -- See: http://eigen.tuxfamily.org/dox-devel/TopicUsingBlasLapack.html
   ```

10. 最后，编译并测试代码。注意，可执行文件使用四个线程运行:

```
$ cmake --build .
$ ./linear-algebra 1000

Number of threads used by Eigen: 4
matrices allocated and initialized Sun Jun 17 2018 11:04:20 AM
elapsed time: 0.0492328s
Scaling done, A and b saved Sun Jun 17 2018 11:04:20 AM
elapsed time: 0.0492328s
Linear system solver done Sun Jun 17 2018 11:04:20 AM
elapsed time: 0.483142s
relative error is 4.21946e-13
```

## 工作原理

Eigen 支持 CMake 查找，这样配置项目就会变得很容易。从 3.3 版开始，Eigen 提供了 CMake 模块，这些模块将导出相应的目标`Eigen3::Eigen`。

`find_package`可以通过选项传递，届时 CMake 将不会使用`FindEigen3.cmake`模块，而是通过特定的`Eigen3Config.cmake`，`Eigen3ConfigVersion.cmake`和`Eigen3Targets.cmake`提供 Eigen3 安装的标准位置(`<installation-prefix>/share/eigen3/cmake`)。这种包定位模式称为“Config”模式，比`Find<package>.cmake`方式更加通用。有关“模块”模式和“配置”模式的更多信息，可参考官方文档 https://cmake.org/cmake/help/v3.5/command/find_package.html 。

虽然 Eigen3、BLAS 和 OpenMP 声明为`PUBLIC`依赖项，但`EIGEN_USE_BLAS`编译定义声明为`PRIVATE`。可以在单独的库目标中汇集库依赖项，而不是直接链接可执行文件。使用`PUBLIC/PRIVATE`关键字，可以根据库目标的依赖关系调整相应标志和定义。

## 更多信息

CMake 将在预定义的位置层次结构中查找配置模块。首先是`CMAKE_PREFIX_PATH`，`<package>_DIR`是接下来的搜索路径。因此，如果 Eigen3 安装在非标准位置，可以使用这两个选项来告诉 CMake 在哪里查找它:

1. 通过将 Eigen3 的安装前缀传递给`CMAKE_PREFIX_PATH`:

   ```
   $ cmake -D CMAKE_PREFIX_PATH=<installation-prefix> ..
   ```

2. 通过传递配置文件的位置作为`Eigen3_DIR`:

   ```
   $ cmake -D Eigen3_DIR=<installation-prefix>/share/eigen3/cmake ..
   ```

# 3.9 检测外部库:Ⅰ. 使用 pkg-config

**NOTE**:_此示例代码可以在 codes/chapter-03/recipe-09 中找到，包含一个 C 的示例。该示例在 CMake 3.6 版(或更高版本)中是有效的，并且已经在 GNU/Linux、macOS 和 Windows 上进行过测试。codes/chapter-03/recipe-09 中也有一个适用于 CMake 3.5 的示例。_

目前为止，我们已经讨论了两种检测外部依赖关系的方法:

- 使用 CMake 自带的`find-module`，但并不是所有的包在 CMake 的`find`模块都找得到。
- 使用`<package>Config.cmake`, `<package>ConfigVersion.cmake`和`<package>Targets.cmake`，这些文件由软件包供应商提供，并与软件包一起安装在标准位置的 cmake 文件夹下。

如果某个依赖项既不提供查找模块，也不提供供应商打包的 CMake 文件，该怎么办?在这种情况下，我们只有两个选择:

- 依赖`pkg-config`程序，来找到系统上的包。这依赖于包供应商在`.pc`配置文件中，其中有关于发行包的元数据。
- 为依赖项编写自己的`find-package`模块。

本示例中，将展示如何利用 CMake 中的`pkg-config`来定位 ZeroMQ 消息库。下一个示例中，将编写一个 find 模块，展示如何为 ZeroMQ 编写属于自己`find`模块。

## 准备工作

我们构建的代码来自 ZeroMQ 手册 http://zguide.zeromq.org/page:all 的示例。由两个源文件`hwserver.c`和`hwclient.c`组成，这两个源文件将构建为两个独立的可执行文件。执行时，它们将打印“Hello, World”。

## 具体实施

这是一个 C 项目，我们将使用 C99 标准，逐步构建`CMakeLists.txt`文件:

1. 声明一个 C 项目，并要求符合 C99 标准:

   ```
   cmake_minimum_required(VERSION 3.6 FATAL_ERROR)

   project(recipe-09 LANGUAGES C)

   set(CMAKE_C_STANDARD 99)
   set(CMAKE_C_EXTENSIONS OFF)
   set(CMAKE_C_STANDARD_REQUIRED ON)
   ```

2. 使用 CMake 附带的 find-module，查找`pkg-config`。这里在`find_package`中传递了`QUIET`参数。只有在没有找到`pkg-config`时，CMake 才会报错:

   ```
   find_package(PkgConfig REQUIRED QUIET)
   ```

3. 找到`pkg-config`时，我们将使用`pkg_search_module`函数，以搜索任何附带包配置`.pc`文件的库或程序。该示例中，我们查找 ZeroMQ 库:

   ```
   pkg_search_module(
     ZeroMQ
     REQUIRED
     	libzeromq libzmq lib0mq
     IMPORTED_TARGET
     )
   ```

4. 如果找到 ZeroMQ 库，则打印状态消息:

   ```
   if(TARGET PkgConfig::ZeroMQ)
   	message(STATUS "Found ZeroMQ")
   endif()
   ```

5. 然后，添加两个可执行目标，并链接到 ZeroMQ。这将自动设置包括目录和链接库:

   ```
   add_executable(hwserver hwserver.c)
   target_link_libraries(hwserver PkgConfig::ZeroMQ)
   add_executable(hwclient hwclient.c)
   target_link_libraries(hwclient PkgConfig::ZeroMQ)
   ```

6. 现在，我们可以配置和构建示例:

   ```
   $ mkdir -p build
   $ cd build
   $ cmake ..
   $ cmake --build .
   ```

7. 在终端中，启动服务器，启动时会输出类似于本例的消息:

   ```
   Current 0MQ version is 4.2.2
   ```

8. 然后，在另一个终端启动客户端，它将打印如下内容:

   ```
   Connecting to hello world server…
   Sending Hello 0…
   Received World 0
   Sending Hello 1…
   Received World 1
   Sending Hello 2…
   ...
   ```

## 工作

当找到`pkg-config`时, CMake 需要提供两个函数，来封装这个程序提供的功能:

- `pkg_check_modules`，查找传递列表中的所有模块(库和/或程序)
- `pkg_search_module`，要在传递的列表中找到第一个工作模块

与`find_package`一样，这些函数接受`REQUIRED`和`QUIET`参数。更详细地说，我们对`pkg_search_module`的调用如下:

```
pkg_search_module(
  ZeroMQ
  REQUIRED
  	libzeromq libzmq lib0mq
  IMPORTED_TARGET
  )
```

这里，第一个参数是前缀，它将用于命名存储搜索 ZeroMQ 库结果的目标：`PkgConfig::ZeroMQ`。注意，我们需要为系统上的库名传递不同的选项：`libzeromq`、`libzmq`和`lib0mq`。这是因为不同的操作系统和包管理器，可为同一个包选择不同的名称。

**NOTE**:_`pkg_check_modules`和`pkg_search_module`函数添加了`IMPORTED_TARGET`选项，并在 CMake 3.6 中定义导入目标的功能。3.6 之前的版本，只定义了变量`ZeroMQ_INCLUDE_DIRS`(用于 include 目录)和`ZeroMQ_LIBRARIES`(用于链接库)，供后续使用。_

# 3.10 检测外部库:Ⅱ. 自定义 find 模块

**NOTE**:_此示例代码可以在 codes/chapter-03/recipe-10 中找到，包含一个 C 的示例。该示例在 CMake 3.5 版(或更高版本)中是有效的，并且已经在 GNU/Linux、macOS 和 Windows 上进行过测试。_

此示例补充了上一节的示例，我们将展示如何编写一个`find`模块来定位系统上的 ZeroMQ 消息库，以便能够在非 Unix 操作系统上检测该库。我们重用服务器-客户端示例代码。

## 如何实施

这是一个 C 项目，使用 C99 标准，并逐步构建 CMakeLists.txt 文件:

1. 声明一个 C 项目，并要求符合 C99 标准:

   ```
   cmake_minimum_required(VERSION 3.5 FATAL_ERROR)

   project(recipe-10 LANGUAGES C)

   set(CMAKE_C_STANDARD 99)
   set(CMAKE_C_EXTENSIONS OFF)
   set(CMAKE_C_STANDARD_REQUIRED ON)
   ```

2. 将当前源目录`CMAKE_CURRENT_SOURCE_DIR`，添加到 CMake 将查找模块的路径列表`CMAKE_MODULE_PATH`中。这样 CMake 就可以找到，我们自定义的`FindZeroMQ.cmake`模块:

   ```
   list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR})
   ```

3. 现在`FindZeroMQ.cmake`模块是可用的，可以通过这个模块来搜索项目所需的依赖项。由于我们没有使用`QUIET`选项来查找`find_package`，所以当找到库时，状态消息将自动打印:

   ```
   find_package(ZeroMQ REQUIRED)
   ```

4. 我们继续添加`hwserver`可执行目标。头文件包含目录和链接库是使用`find_package`命令成功后，使用`ZeroMQ_INCLUDE_DIRS`和`ZeroMQ_LIBRARIES`变量进行指定的:

   ```
   add_executable(hwserver hwserver.c)
   target_include_directories(hwserver
     PRIVATE
     	${ZeroMQ_INCLUDE_DIRS}
     )
   target_link_libraries(hwserver
     PRIVATE
     	${ZeroMQ_LIBRARIES}
     )
   ```

5. 最后，我们对`hwclient`可执行目标执行相同的操作:

   ```
   add_executable(hwclient hwclient.c)
   target_include_directories(hwclient
     PRIVATE
     	${ZeroMQ_INCLUDE_DIRS}
     )
   target_link_libraries(hwclient
     PRIVATE
     	${ZeroMQ_LIBRARIES}
     )
   ```

此示例的主`CMakeLists.txt`在使用`FindZeroMQ.cmake`时，与前一个示例中使用的`CMakeLists.txt`不同。这个模块使用`find_path`和`find_library` CMake 内置命令，搜索 ZeroMQ 头文件和库，并使用`find_package_handle_standard_args`设置相关变量，就像我们在第 3 节中做的那样。

1. `FindZeroMQ.cmake`中，检查了`ZeroMQ_ROOT`变量是否设置。此变量可用于 ZeroMQ 库的检测，并引导到自定义安装目录。用户可能设置了`ZeroMQ_ROOT`作为环境变量，我们也会进行检查了:

   ```
   if(NOT ZeroMQ_ROOT)
   	set(ZeroMQ_ROOT "$ENV{ZeroMQ_ROOT}")
   endif()
   ```

2. 然后，搜索系统上`zmq.h`头文件的位置。这是基于`_ZeroMQ_ROOT`变量和`find_path`命令进行的:

   ```
   if(NOT ZeroMQ_ROOT)
   	find_path(_ZeroMQ_ROOT NAMES include/zmq.h)
   else()
   	set(_ZeroMQ_ROOT "${ZeroMQ_ROOT}")
   endif()

   find_path(ZeroMQ_INCLUDE_DIRS NAMES zmq.h HINTS ${_ZeroMQ_ROOT}/include)
   ```

3. 如果成功找到头文件，则将`ZeroMQ_INCLUDE_DIRS`设置为其位置。我们继续通过使用字符串操作和正则表达式，寻找相应版本的 ZeroMQ 库:

   ```
   set(_ZeroMQ_H ${ZeroMQ_INCLUDE_DIRS}/zmq.h)

   function(_zmqver_EXTRACT _ZeroMQ_VER_COMPONENT _ZeroMQ_VER_OUTPUT)
   set(CMAKE_MATCH_1 "0")
   set(_ZeroMQ_expr "^[ \\t]*#define[ \\t]+${_ZeroMQ_VER_COMPONENT}[ \\t]+([0-9]+)$")
   file(STRINGS "${_ZeroMQ_H}" _ZeroMQ_ver REGEX "${_ZeroMQ_expr}")
   string(REGEX MATCH "${_ZeroMQ_expr}" ZeroMQ_ver "${_ZeroMQ_ver}")
   set(${_ZeroMQ_VER_OUTPUT} "${CMAKE_MATCH_1}" PARENT_SCOPE)
   endfunction()

   _zmqver_EXTRACT("ZMQ_VERSION_MAJOR" ZeroMQ_VERSION_MAJOR)
   _zmqver_EXTRACT("ZMQ_VERSION_MINOR" ZeroMQ_VERSION_MINOR)
   _zmqver_EXTRACT("ZMQ_VERSION_PATCH" ZeroMQ_VERSION_PATCH)
   ```

4. 然后，为`find_package_handle_standard_args`准备`ZeroMQ_VERSION`变量:

   ```
   if(ZeroMQ_FIND_VERSION_COUNT GREATER 2)
   	set(ZeroMQ_VERSION "${ZeroMQ_VERSION_MAJOR}.${ZeroMQ_VERSION_MINOR}.${ZeroMQ_VERSION_PATCH}")
   else()
   	set(ZeroMQ_VERSION "${ZeroMQ_VERSION_MAJOR}.${ZeroMQ_VERSION_MINOR}")
   endif()
   ```

5. 使用`find_library`命令搜索 ZeroMQ 库。因为库的命名有所不同，这里我们需要区分 Unix 的平台和 Windows 平台:

   ```
   if(NOT ${CMAKE_C_PLATFORM_ID} STREQUAL "Windows")
     find_library(ZeroMQ_LIBRARIES
       NAMES
       	zmq
       HINTS
         ${_ZeroMQ_ROOT}/lib
         ${_ZeroMQ_ROOT}/lib/x86_64-linux-gnu
       )
   else()
     find_library(ZeroMQ_LIBRARIES
       NAMES
       	libzmq
         "libzmq-mt-${ZeroMQ_VERSION_MAJOR}_${ZeroMQ_VERSION_MINOR}_${ZeroMQ_VERSION_PATCH}"
         "libzmq-${CMAKE_VS_PLATFORM_TOOLSET}-mt-${ZeroMQ_VERSION_MAJOR}_${ZeroMQ_VERSION_MINOR}_${ZeroMQ_VERSION_PATCH}"
         libzmq_d
         "libzmq-mt-gd-${ZeroMQ_VERSION_MAJOR}_${ZeroMQ_VERSION_MINOR}_${ZeroMQ_VERSION_PATCH}"
         "libzmq-${CMAKE_VS_PLATFORM_TOOLSET}-mt-gd-${ZeroMQ_VERSION_MAJOR}_${ZeroMQ_VERSION_MINOR}_${ZeroMQ_VERSION_PATCH}"
       HINTS
       	${_ZeroMQ_ROOT}/lib
       )
   endif()
   ```

6. 最后，包含了标准`FindPackageHandleStandardArgs.cmake`，并调用相应的 CMake 命令。如果找到所有需要的变量，并且版本匹配，则将`ZeroMQ_FOUND`变量设置为`TRUE`:

   ```
   include(FindPackageHandleStandardArgs)

   find_package_handle_standard_args(ZeroMQ
     FOUND_VAR
     	ZeroMQ_FOUND
     REQUIRED_VARS
     ZeroMQ_INCLUDE_DIRS
     ZeroMQ_LIBRARIES
     VERSION_VAR
     ZeroMQ_VERSION
     )
   ```

**NOTE**:_刚才描述的`FindZeroMQ.cmake`模块已经在 https://github.com/zeromq/azmq/blob/master/config/FindZeroMQ.cmake 上进行了修改。_

## 工作原理

`find-module`通常遵循特定的模式:

1. 检查用户是否为所需的包提供了自定义位置。

2. 使用`find_`家族中的命令搜索所需包的必需组件，即头文件、库、可执行程序等等。我们使用`find_path`查找头文件的完整路径，并使用`find_library`查找库。CMake 还提供`find_file`、`find_program`和`find_package`。这些命令的签名如下:

   ```
   find_path(<VAR> NAMES name PATHS paths)
   ```

3. 如果搜索成功，`<VAR>`将保存搜索结果；如果搜索失败，则会设置为`<VAR>-NOTFOUND`。`NAMES`和`PATHS`分别是 CMake 应该查找的文件的名称和搜索应该指向的路径。

4. 初步搜索的结果中，可以提取版本号。示例中，ZeroMQ 头文件包含库版本，可以使用字符串操作和正则表达式提取库版本信息。

5. 最后，调用`find_package_handle_standard_args`命令。处理`find_package`命令的`REQUIRED`、`QUIET`和版本参数，并设置`ZeroMQ_FOUND`变量。

**NOTE**:_任何 CMake 命令的完整文档都可以从命令行获得。例如，`cmake --help-command find_file`将输出`find_file`命令的手册页。对于 CMake 标准模块的手册，可以在 CLI 使用`--help-module`看到。例如，`cmake --help-module FindPackageHandleStandardArgs`将输出`FindPackageHandleStandardArgs.cmake`的手册页面。_

## 更多信息

总而言之，有四种方式可用于找到依赖包:

1. 使用由包供应商提供 CMake 文件`<package>Config.cmake` ，`<package>ConfigVersion.cmake`和`<package>Targets.cmake`，通常会在包的标准安装位置查找。
2. 无论是由 CMake 还是第三方提供的模块，为所需包使用`find-module`。
3. 使用`pkg-config`，如本节的示例所示。
4. 如果这些都不可行，那么编写自己的`find`模块。

这四种可选方案按相关性进行了排序，每种方法也都有其挑战。

目前，并不是所有的包供应商都提供 CMake 的 Find 文件，不过正变得越来越普遍。因为导出 CMake 目标，使得第三方代码很容易使用它所依赖的库和/或程序附加的依赖。

从一开始，`Find-module`就一直是 CMake 中定位依赖的主流手段。但是，它们中的大多数仍然依赖于设置依赖项使用的变量，比如`Boost_INCLUDE_DIRS`、`PYTHON_INTERPRETER`等等。这种方式很难在第三方发布自己的包时，确保依赖关系被满足。

使用`pkg-config`的方法可以很好地进行适配，因为它已经成为 Unix 系统的标准。然而，也由于这个原因，它不是一个完全跨平台的方法。此外，如 CMake 文档所述，在某些情况下，用户可能会意外地覆盖检测包，并导致`pkg-config`提供不正确的信息。

最后的方法是编写自己的查找模块脚本，就像本示例中那样。这是可行的，并且依赖于`FindPackageHandleStandardArgs.cmake`。然而，编写一个全面的查找模块脚本绝非易事；有需要考虑很多可能性，我们在 Unix 和 Windows 平台上，为查找 ZeroMQ 库文件演示了一个例子。

所有软件开发人员都非常清楚这些问题和困难，正如 CMake 邮件列表上讨论所示: https://cmake.org/pipermail/cmake/2018-May/067556.html 。`pkg-config`在 Unix 包开发人员中是可以接受的，但是它不能很容易地移植到非 Unix 平台。CMake 配置文件功能强大，但并非所有软件开发人员都熟悉 CMake 语法。公共包规范项目是统一用于包查找的`pkg-config`和 CMake 配置文件方法的最新尝试。您可以在项目的网站上找到更多信息: https://mwoehlke.github.io/cps/

在第 10 章中将讨论，如何使用前面讨论中概述的第一种方法，使第三方应用程序，找到自己的包：为项目提供自己的 CMake 查找文件。
