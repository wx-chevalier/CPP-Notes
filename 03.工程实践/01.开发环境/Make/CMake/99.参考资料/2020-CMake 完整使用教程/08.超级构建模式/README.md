# 超级构建模式

本章的主要内容如下：

- 使用超级级构建模式
- 使用超级构建管理依赖项:Ⅰ.Boost 库
- 使用超级构建管理依赖项:Ⅱ.FFTW 库
- 使用超级构建管理依赖项:Ⅲ.Google Test 框架
- 使用超级构建支持项目

每个项目都需要处理依赖关系，使用 CMake 很容易查询这些依赖关系，是否存在于配置项目中。第 3 章，展示了如何找到安装在系统上的依赖项，到目前为止我们一直使用这种模式。但是，当不满足依赖关系，我们只能使配置失败，并向用户警告失败的原因。然而，使用 CMake 可以组织我们的项目，如果在系统上找不到依赖项，就可以自动获取和构建依赖项。本章将介绍和分析`ExternalProject.cmake`和`FetchContent.cmake`标准模块，及在超级构建模式中的使用。前者允许在构建时检索项目的依赖项，后者允许我们在配置时检索依赖项(CMake 的 3.11 版本后添加)。使用超级构建模式，我们可以利用 CMake 作为包管理器：相同的项目中，将以相同的方式处理依赖项，无论依赖项在系统上是已经可用，还是需要重新构建。接下来的 5 个示例，将带您了解该模式，并展示如何使用它来获取和构建依赖关系。

**NOTE**:_这两个模块都有大量的在线文档。`ExternalProject.cmake`，可以参考https://cmake.org/cmake/help/v3.5/module/ExternalProject.html 。`FetchContent.cmake`，可以参考https://cmake.org/cmake/help/v3.11/module/FetchContent.html 。_

# 8.3 使用超级构建管理依赖项:Ⅱ.FFTW 库

**NOTE**:_此示例代码可以在 codes/chapter-8/recipe-03 中找到，其中有一个 C 示例。该示例在 CMake 3.5 版(或更高版本)中是有效的，并且已经在 GNU/Linux、macOS 和 Windows 上进行过测试。_

对于 CMake 支持的所有项目，超级构建模式可用于管理相当复杂的依赖关系。正如在前面的示例所演示的，CMake 并不需要管理各种子项目。与前一个示例相反，这个示例中的外部子项目将是一个 CMake 项目，并将展示如何使用超级构建，下载、构建和安装 FFTW 库。FFTW 是一个快速傅里叶变换库，可在[http://www.fftw.org](http://www.fftw.org/) 免费获得。

我们项目的代码`fftw_example.c`位于 src 子目录中，它将计算源代码中定义的函数的傅里叶变换。

## 准备工作

这个示例的目录布局，是超级构建中非常常见的结构:

```
.
├── CMakeLists.txt
├── external
│    └── upstream
│        ├── CMakeLists.txt
│        └── fftw3
│            └── CMakeLists.txt
└── src
    ├── CMakeLists.txt
    └── fftw_example.c
```

代码`fftw_example.c`位于`src`子目录中，它将调用傅里叶变换函数。

## 具体实施

从主`CMakeLists.txt`开始，这里将整个超级构建过程放在一起:

1. 声明一个支持 C99 的项目：

   ```
   cmake_minimum_required(VERSION 3.5 FATAL_ERROR)
   project(recipe-03 LANGUAGES C)
   set(CMAKE_C_STANDARD 99)
   set(CMAKE_C_EXTENSIONS OFF)
   set(CMAKE_C_STANDARD_REQUIRED ON)
   ```

2. 和上一个示例一样，我们设置了`EP_BASE`目录属性和阶段安装目录:

   ```
   set_property(DIRECTORY PROPERTY EP_BASE ${CMAKE_BINARY_DIR}/subprojects)
   set(STAGED_INSTALL_PREFIX ${CMAKE_BINARY_DIR}/stage)
   message(STATUS "${PROJECT_NAME} staged install: ${STAGED_INSTALL_PREFIX}")
   ```

3. 对 FFTW 的依赖关系在`external/upstream`子目录中检查，我们会将这个子目录添加到构建系统中:

   ```
   add_subdirectory(external/upstream)
   ```

4. 包含`ExternalProject.cmake`模块：

   ```
   include(ExternalProject)
   ```

5. 我们为`recipe-03_core`声明了外部项目。这个项目的源代码在`${CMAKE_CURRENT_LIST_DIR}/src`文件夹中。该项目设置为`FFTW3_DIR`选项，选择正确的 FFTW 库:

   ```
   ExternalProject_Add(${PROJECT_NAME}_core
     DEPENDS
     	fftw3_external
     SOURCE_DIR
     	${CMAKE_CURRENT_LIST_DIR}/src
     CMAKE_ARGS
       -DFFTW3_DIR=${FFTW3_DIR}
       -DCMAKE_C_STANDARD=${CMAKE_C_STANDARD}
       -DCMAKE_C_EXTENSIONS=${CMAKE_C_EXTENSIONS}
       -DCMAKE_C_STANDARD_REQUIRED=${CMAKE_C_STANDARD_REQUIRED}
     CMAKE_CACHE_ARGS
       -DCMAKE_C_FLAGS:STRING=${CMAKE_C_FLAGS}
       -DCMAKE_PREFIX_PATH:PATH=${CMAKE_PREFIX_PATH}
     BUILD_ALWAYS
     	1
     INSTALL_COMMAND
     	""
     )
   ```

`external/upstream`子目录还包含一个`CMakeLists.txt`:

这个文件中，添加`fftw3`文件夹作为构建系统中的另一个子目录:

```
add_subdirectory(fftw3)
```

`external/upstream/fftw3`中的`CMakeLists.txt`负责处理依赖关系:

1. 首先，尝试在系统上找到 FFTW3 库。注意，我们配置`find_package`使用的参数:

   ```
   find_package(FFTW3 CONFIG QUIET)
   ```

2. 如果找到了库，就可以导入目标`FFTW3::FFTW3`来链接它。我们向用户打印一条消息，显示库的位置。我们添加一个虚拟`INTERFACE`库`fftw3_external`。超级建设中，这需要正确地固定子项目之间的依赖树:

   ```
   find_package(FFTW3 CONFIG QUIET)

   if(FFTW3_FOUND)
     get_property(_loc TARGET FFTW3::fftw3 PROPERTY LOCATION)
     message(STATUS "Found FFTW3: ${_loc} (found version ${FFTW3_VERSION})")
   	add_library(fftw3_external INTERFACE) # dummy
   else()
   	# this branch will be discussed below
   endif()
   ```

3. 如果 CMake 无法找到预安装版本的 FFTW，我们将进入`else`分支。这个分支中，使用`ExternalProject_Add`下载、构建和安装它。外部项目的名称为`fftw3_external`。`fftw3_external`项目将从官方地址下载，下载完成后将使用 MD5 校验和进行文件完整性检查:

   ```
   message(STATUS "Suitable FFTW3 could not be located. Downloading and building!")

   include(ExternalProject)
   ExternalProject_Add(fftw3_external
     URL
     	http://www.fftw.org/fftw-3.3.8.tar.gz
     URL_HASH
     	MD5=8aac833c943d8e90d51b697b27d4384d
   ```

4. 禁用打印下载进程，并将更新命令定义为空:

   ```
   OWNLOAD_NO_PROGRESS
   	1
   UPDATE_COMMAND
   	""
   ```

5. 配置、构建和安装输出将被记录到一个文件中:

   ```
   LOG_CONFIGURE
   	1
   LOG_BUILD
   	1
   LOG_INSTALL
   	1
   ```

6. 将`fftw3_external`项目的安装前缀设置为之前定义的`STAGED_INSTALL_PREFIX`目录，并关闭 FFTW3 的测试套件构建:

   ```
   CMAKE_ARGS
     -DCMAKE_INSTALL_PREFIX=${STAGED_INSTALL_PREFIX}
     -DBUILD_TESTS=OFF
   ```

7. 如果在 Windows 上构建，通过生成器表达式设置`WITH_OUR_MALLOC`预处理器选项，并关闭`ExternalProject_Add`命令:

   ```
   CMAKE_CACHE_ARGS
   	-DCMAKE_C_FLAGS:STRING=$<$<BOOL:WIN32>:-DWITH_OUR_MALLOC>
   )
   ```

8. 最后，定义`FFTW3_DIR`变量并缓存它。CMake 将使用该变量作为`FFTW3::FFTW3`目标的搜索目录:

   ```
   include(GNUInstallDirs)

   set(
     FFTW3_DIR ${STAGED_INSTALL_PREFIX}/${CMAKE_INSTALL_LIBDIR}/cmake/fftw3
     CACHE PATH "Path to internally built FFTW3Config.cmake"
     FORCE
     )
   ```

`src`文件夹中的 CMakeLists.txt 相当简洁:

1. 同样在这个文件中，我们声明了一个 C 项目:

   ```
   cmake_minimum_required(VERSION 3.5 FATAL_ERROR)
   project(recipe-03_core LANGUAGES C)
   ```

2. 使用`find_package`来检测 FFTW 库，再次使用配置检测模式:

   ```
   find_package(FFTW3 CONFIG REQUIRED)
   get_property(_loc TARGET FFTW3::fftw3 PROPERTY LOCATION)
   message(STATUS "Found FFTW3: ${_loc} (found version ${FFTW3_VERSION})")
   ```

3. 将`fftw_example.c`源文件添加到可执行目标`fftw_example`:

   ```
   add_executable(fftw_example fftw_example.c)
   ```

4. 为可执行目标设置链接库:

   ```
   target_link_libraries(fftw_example
     PRIVATE
     	FFTW3::fftw3
     )
   ```

## 工作原理

本示例演示了如何下载、构建和安装由 CMake 管理其构建系统的外部项目。与前一个示例(必须使用自定义构建系统)相反，这个超级构建设置相当简洁。需要注意的是，使用`find_package`命令了配置选项；这说明 CMake 首先查找`FFTW3Config.cmake`，以定位 FFTW3 库，将库导出为第三方项目获取的目标。目标包含库的版本、配置和位置，即关于如何配置和构建目标的完整信息。如果系统上没有安装库，我们需要声明`FFTW3Config.cmake`文件的位置。这可以通过设置`FFTW3_DIR`变量来实现。这是`external/upstream/fftw3/CMakeLists.txt`文件中的最后一步。使用`GNUInstallDirs.cmake`模块，我们将`FFTW3_DIR`设置为缓存变量，以便稍后在超级构建中使用。

**TIPS**:_配置项目时将`CMAKE_DISABLE_FIND_PACKAGE_FFTW3`设置为`ON`，将跳过对 FFTW 库的检测，并始终执行超级构建。参考:https://cmake.org/cmake/help/v3.5/variable/CMAKE_DISABLE_FIND_PACKAGE_PackageName.html_

# 8.4 使用超级构建管理依赖项:Ⅲ.Google Test 框架

**NOTE**:_此示例代码可以在 codes/chapter-8/recipe-04 中找到，其中有一个 C++示例。该示例在 CMake 3.11 版(或更高版本)中是有效的，并且已经在 GNU/Linux、macOS 和 Windows 上进行过测试。在库中也有一个例子可以在 CMake 3.5 下使用。_

第 4 章第 3 节中，我们使用 Google Test 框架实现单元测试，并在配置时使用`FetchContent`模块获取 Google Test 源(自 CMake 3.11 开始可用)。本章中，我们将重新讨论这个方法，较少关注测试方面，并更深入地研究`FetchContent`。它提供了一个简单通用的模块，可以在配置时组装项目依赖项。对于 3.11 以下的 CMake，我们还将讨论如何在配置时使用`ExternalProject_Add`模拟`FetchContent`。

## 准备工作

这个示例中，我们将复用第 4 章第 3 节的源码，构建`main.cpp`、`sum_integer.cpp`和`sum_integers.hpp`和`test.cpp`。我们将在配置时使用`FetchContent`或`ExternalProject_Add`下载所有必需的 Google Test 源，在此示例中，只关注在配置时获取依赖项，而不是实际的源代码及其单元测试。

## 具体实施

这个示例中，我们只关注如何获取 Google Test 源来构建`gtest_main`，并链接到 Google Test 库。关于这个目标如何用于测试示例源的讨论，请读者参考第 4 章第 3 节:

1. 首先包括`FetchContent`模块，它将提供需要的声明、查询和填充依赖项函数:

   ```
   include(FetchContent)
   ```

2. 然后，声明内容——名称、存储库位置和要获取的精确版本:

   ```
   FetchContent_Declare(
     googletest
     GIT_REPOSITORY https://github.com/google/googletest.git
     GIT_TAG release-1.8.0
   )
   ```

3. 查询内容是否已经被获取/填充:

   ```
   FetchContent_GetProperties(googletest)
   ```

4. 前面的函数定义了`googletest_POPULATED`。如果内容还没有填充，我们获取内容并配置子项目:

   ```
   if(NOT googletest_POPULATED)
     FetchContent_Populate(googletest)

     # ...

     # adds the targets: gtest, gtest_main, gmock, gmock_main
     add_subdirectory(
       ${googletest_SOURCE_DIR}
       ${googletest_BINARY_DIR}
       )

     # ...

   endif()
   ```

5. 注意配置时获取内容的方式:

   ```
   $ mkdir -p build
   $ cd build
   $ cmake ..
   ```

6. 这将生成以下构建目录树。Google Test 源现已就绪，剩下的就交由 CMake 处理，并提供所需的目标:

   ```
   build/
   ├── ...
   ├── _deps
   │    ├── googletest-build
   │    │    ├── ...
   │    │    └── ...
   │    ├── googletest-src
   │    │    ├── ...
   │    │    └── ...
   │    └── googletest-subbuild
   │         ├── ...
   │         └── ...
   └── ...
   ```

## 工作原理

`FetchContent`模块支持在配置时填充内容。例子中，获取了一个 Git 库，其中有一个 Git 标签:

```
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG release-1.8.0
)
```

CMake 的 3.11 版本中，`FetchContent`已经成为 CMake 的标准部分。下面的代码中，将尝试在配置时使用`ExternalProject_Add`模拟`FetchContent`。这不仅适用于较老的 CMake 版本，而且可以让我们更深入地了解`FetchContent`层下面发生了什么，并为使用`ExternalProject_Add`在构建时获取项目，提供一个有趣的替代方法。我们的目标是编写一个`fetch_git_repo`宏，并将它放在`fetch_git_repo`中。这样就可以获取相应的内容了:

```
include(fetch_git_repo.cmake)

fetch_git_repo(
  googletest
  ${CMAKE_BINARY_DIR}/_deps
  https://github.com/google/googletest.git
  release-1.8.0
)

# ...

# adds the targets: gtest, gtest_main, gmock, gmock_main
add_subdirectory(
  ${googletest_SOURCE_DIR}
  ${googletest_BINARY_DIR}
  )

# ...
```

这类似于`FetchContent`的使用。在底层实现中，我们将使用`ExternalProject_Add`。现在打开模块，检查`fetch_git_repo.cmake`中定义的`fetch_git_repo`:

```
macro(fetch_git_repo _project_name _download_root _git_url _git_tag)

  set(${_project_name}_SOURCE_DIR ${_download_root}/${_project_name}-src)
  set(${_project_name}_BINARY_DIR ${_download_root}/${_project_name}-build)

  # variables used configuring fetch_git_repo_sub.cmake
  set(FETCH_PROJECT_NAME ${_project_name})
  set(FETCH_SOURCE_DIR ${${_project_name}_SOURCE_DIR})
  set(FETCH_BINARY_DIR ${${_project_name}_BINARY_DIR})
  set(FETCH_GIT_REPOSITORY ${_git_url})
  set(FETCH_GIT_TAG ${_git_tag})

  configure_file(
    ${CMAKE_CURRENT_LIST_DIR}/fetch_at_configure_step.in
    ${_download_root}/CMakeLists.txt
    @ONLY
    )

  # undefine them again
  unset(FETCH_PROJECT_NAME)
  unset(FETCH_SOURCE_DIR)
  unset(FETCH_BINARY_DIR)
  unset(FETCH_GIT_REPOSITORY)
  unset(FETCH_GIT_TAG)

  # configure sub-project
  execute_process(
    COMMAND
    "${CMAKE_COMMAND}" -G "${CMAKE_GENERATOR}" .
    WORKING_DIRECTORY
    ${_download_root}
    )

  # build sub-project which triggers ExternalProject_Add
  execute_process(
    COMMAND
    "${CMAKE_COMMAND}" --build .
    WORKING_DIRECTORY
    ${_download_root}
    )
endmacro()
```

宏接收项目名称、下载根目录、Git 存储库 URL 和一个 Git 标记。宏定义了`${_project_name}_SOURCE_DIR`和`${_project_name}_BINARY_DIR`，我们需要在`fetch_git_repo`生命周期范围内使用定义的`${_project_name}_SOURCE_DIR`和`${_project_name}_BINARY_DIR`，因为要使用它们对子目录进行配置:

```
add_subdirectory(
  ${googletest_SOURCE_DIR}
  ${googletest_BINARY_DIR}
  )
```

`fetch_git_repo`宏中，我们希望使用`ExternalProject_Add`在配置时获取外部项目，通过三个步骤实现了这一点:

1. 首先，配置`fetch_at_configure_step.in`:

   ```
   cmake_minimum_required(VERSION 3.5 FATAL_ERROR)

   project(fetch_git_repo_sub LANGUAGES NONE)

   include(ExternalProject)

   ExternalProject_Add(
     @FETCH_PROJECT_NAME@
     SOURCE_DIR "@FETCH_SOURCE_DIR@"
     BINARY_DIR "@FETCH_BINARY_DIR@"
     GIT_REPOSITORY
     @FETCH_GIT_REPOSITORY@
     GIT_TAG
     @FETCH_GIT_TAG@
     CONFIGURE_COMMAND ""
     BUILD_COMMAND ""
     INSTALL_COMMAND ""
     TEST_COMMAND ""
     )
   ```

   使用`configure_file`，可以生成一个`CMakeLists.txt`文件，前面的占位符被`fetch_git_repo.cmake`中的值替换。注意，前面的`ExternalProject_Add`命令仅用于获取，而不仅是配置、构建、安装或测试。

2. 其次，使用配置步骤在配置时触发`ExternalProject_Add`(从主项目的角度):

   ```
   # configure sub-project
   execute_process(
     COMMAND
     	"${CMAKE_COMMAND}" -G "${CMAKE_GENERATOR}" .
     WORKING_DIRECTORY
     	${_download_root}
     )
   ```

3. 最后在`fetch_git_repo.cmake`中触发配置时构建步骤:

   ```
   # build sub-project which triggers ExternalProject_Add
   execute_process(
     COMMAND
     	"${CMAKE_COMMAND}" --build .
     WORKING_DIRECTORY
     	${_download_root}
     )
   ```

这个解决方案的一个优点是，由于外部依赖项不是由`ExternalProject_Add`配置的，所以不需要通过`ExternalProject_Add`调用任何配置，将其引导至项目。我们可以使用`add_subdirectory`配置和构建模块，就像外部依赖项是项目源代码树的一部分一样。聪明的伪装!

## 更多信息

有关`FetchContent`选项的详细讨论，请参考https://cmake.org/cmake/help/v3.11/module/FetchContent.html 配置时`ExternalProject_Add`的解决方案灵感来自 Craig Scott，博客文章:https://crascit.com/2015/07/25/cgtest/

# 8.5 使用超级构建支持项目

**NOTE**:_此示例代码可以在 codes/chapter-8/recipe-05 中找到，其中有一个 C++示例。该示例在 CMake 3.5 版(或更高版本)中是有效的，并且已经在 GNU/Linux、macOS 和 Windows 上进行过测试。_

`ExternalProject`和`FetchContent`是 CMake 库中两个非常强大的工具。经过前面的示例，我们应该相信超级构建方法，在管理复杂依赖关系的项目时是多么有用。目前为止，我们已经展示了如何使用`ExternalProject`来处理以下问题:

- 存储在源树中的源
- 从在线服务器上，检索/获取可用的存档资源

前面的示例展示了，如何使用`FetchContent`处理开源 Git 存储库中可用的依赖项。本示例将展示，如何使用`ExternalProject`达到同样的效果。最后，将介绍一个示例，该示例将在第 10 章第 4 节中重用。

## 准备工作

这个超级构建的源代码树现在应该很熟悉了:

```
.
├── CMakeLists.txt
├── external
│    └── upstream
│        ├── CMakeLists.txt
│        └── message
│            └── CMakeLists.txt
└── src
    ├── CMakeLists.txt
    └── use_message.cpp
```

根目录有一个`CMakeLists.txt`，我们知道它会配合超级构建。子目录`src`和`external`中是我们自己的源代码，CMake 指令需要满足对消息库的依赖，我们将在本例中构建消息库。

## 具体实施

目前为止，建立超级构建的过程应该已经很熟悉了。让我们再次看看必要的步骤，从根目录的`CMakeLists.txt`开始:

1. 声明一个 C++11 项目，并对项目构建类型的默认值进行设置。

   ```
   cmake_minimum_required(VERSION 3.6 FATAL_ERROR)

   project(recipe-05 LANGUAGES CXX)

   set(CMAKE_CXX_STANDARD 11)
   set(CMAKE_CXX_EXTENSIONS OFF)
   set(CMAKE_CXX_STANDARD_REQUIRED ON)

   if(NOT DEFINED CMAKE_BUILD_TYPE OR "${CMAKE_BUILD_TYPE}" STREQUAL "")
   	set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
   endif()

   message(STATUS "Build type set to ${CMAKE_BUILD_TYPE}")
   ```

2. 设置`EP_BASE`目录属性。这将固定`ExternalProject`管理所有子项目的布局:

   ```
   set_property(DIRECTORY PROPERTY EP_BASE ${CMAKE_BINARY_DIR}/subprojects)
   ```

3. 我们设置了`STAGED_INSTALL_PREFIX`。与之前一样，这个位置将作为依赖项的构建树中的安装目录:

   ```
   set(STAGED_INSTALL_PREFIX ${CMAKE_BINARY_DIR}/stage)
   message(STATUS "${PROJECT_NAME} staged install: ${STAGED_INSTALL_PREFIX}")
   ```

4. 将`external/upstream`作为子目录添加：

   ```
   add_subdirectory(external/upstream)
   ```

5. 添加`ExternalProject_Add`，这样我们的项目也将由超级构建管理:

   ```
   include(ExternalProject)
   ExternalProject_Add(${PROJECT_NAME}_core
     DEPENDS
     	message_external
     SOURCE_DIR
     	${CMAKE_CURRENT_SOURCE_DIR}/src
     CMAKE_ARGS
       -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
       -DCMAKE_CXX_COMPILER=${CMAKE_CXX_COMPILER}
       -DCMAKE_CXX_STANDARD=${CMAKE_CXX_STANDARD}
       -DCMAKE_CXX_EXTENSIONS=${CMAKE_CXX_EXTENSIONS}
       -DCMAKE_CXX_STANDARD_REQUIRED=${CMAKE_CXX_STANDARD_REQUIRED}
       -Dmessage_DIR=${message_DIR}
       CMAKE_CACHE_ARGS
       -DCMAKE_CXX_FLAGS:STRING=${CMAKE_CXX_FLAGS}
       -DCMAKE_PREFIX_PATH:PATH=${CMAKE_PREFIX_PATH}
     BUILD_ALWAYS
     	1
     INSTALL_COMMAND
     	""
     )
   ```

`external/upstream`的`CMakeLists.txt`中只包含一条命令:

```
add_subdirectory(message)
```

跳转到`message`文件夹，我们会看到对消息库的依赖的常用命令:

1. 首先，调用`find_package`找到一个合适版本的库:

   ```
   find_package(message 1 CONFIG QUIET)
   ```

2. 如果找到，会通知用户，并添加一个虚拟`INTERFACE`库:

   ```
   get_property(_loc TARGET message::message-shared PROPERTY LOCATION)
   message(STATUS "Found message: ${_loc} (found version ${message_VERSION})")
   add_library(message_external INTERFACE) # dummy
   ```

3. 如果没有找到，再次通知用户并继续使用`ExternalProject_Add`:

   ```
   message(STATUS "Suitable message could not be located, Building message instead.")
   ```

4. 该项目托管在一个公共 Git 库中，使用`GIT_TAG`选项指定下载哪个分支。和之前一样，将`UPDATE_COMMAND`选项置为空:

   ```
   include(ExternalProject)
   ExternalProject_Add(message_external
     GIT_REPOSITORY
     	https://github.com/dev-cafe/message.git
     GIT_TAG
     	master
     UPDATE_COMMAND
     	""
   ```

5. 外部项目使用 CMake 配置和构建，传递必要的构建选项:

   ```
   CMAKE_ARGS
     -DCMAKE_INSTALL_PREFIX=${STAGED_INSTALL_PREFIX}
     -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
     -DCMAKE_CXX_COMPILER=${CMAKE_CXX_COMPILER}
     -DCMAKE_CXX_STANDARD=${CMAKE_CXX_STANDARD}
     -DCMAKE_CXX_EXTENSIONS=${CMAKE_CXX_EXTENSIONS}
     -DCMAKE_CXX_STANDARD_REQUIRED=${CMAKE_CXX_STANDARD_REQUIRED}
   CMAKE_CACHE_ARGS
   	-DCMAKE_CXX_FLAGS:STRING=${CMAKE_CXX_FLAGS}
   ```

6. 项目安装后进行测试:

   ```
   TEST_AFTER_INSTALL
   	1
   ```

7. 我们不希望看到下载进度，也不希望在屏幕上报告配置、构建和安装信息，所以选择关闭`ExternalProject_Add`:

   ```
     DOWNLOAD_NO_PROGRESS
     	1
     LOG_CONFIGURE
     	1
     LOG_BUILD
     	1
     LOG_INSTALL
     	1
   )
   ```

8. 为了确保子项目在超级构建的其余部分中是可见的，我们设置了`message_DIR`目录:

   ```
   if(WIN32 AND NOT CYGWIN)
   	set(DEF_message_DIR ${STAGED_INSTALL_PREFIX}/CMake)
   else()
   	set(DEF_message_DIR ${STAGED_INSTALL_PREFIX}/share/cmake/message)
   endif()

   file(TO_NATIVE_PATH "${DEF_message_DIR}" DEF_message_DIR)
   set(message_DIR ${DEF_message_DIR}
   CACHE PATH "Path to internally built messageConfig.cmake" FORCE)
   ```

最后，来看一下`src`目录上的`CMakeLists.txt`：

1. 同样，声明一个 C++11 项目:

   ```
   cmake_minimum_required(VERSION 3.6 FATAL_ERROR)
   project(recipe-05_core
   LANGUAGES CXX
   )
   set(CMAKE_CXX_STANDARD 11)
   set(CMAKE_CXX_EXTENSIONS OFF)
   set(CMAKE_CXX_STANDARD_REQUIRED ON)
   ```

2. 项目需要消息库:

   ```
   find_package(message 1 CONFIG REQUIRED)
   get_property(_loc TARGET message::message-shared PROPERTY LOCATION)
   message(STATUS "Found message: ${_loc} (found version ${message_VERSION})")
   ```

3. 声明一个可执行目标，并将其链接到消息动态库:

   ```
   add_executable(use_message use_message.cpp)

   target_link_libraries(use_message
     PUBLIC
     	message::message-shared
     )
   ```

## 工作原理

示例展示了`ExternalProject_Add`的一些新选项:

1. **GIT_REPOSITORY**:这可以用来指定包含依赖项源的存储库的 URL。CMake 还可以使用其他版本控制系统，比如 CVS (CVS_REPOSITORY)、SVN (SVN_REPOSITORY)或 Mercurial (HG_REPOSITORY)。
2. **GIT_TAG**:默认情况下，CMake 将检出给定存储库的默认分支。然而，最好依赖于一个稳定的版本。这可以通过这个选项指定，它可以接受 Git 将任何标识符识别为“版本”信息，例如：Git 提交 SHA、Git 标记或分支名称。CMake 所理解的其他版本控制系统也可以使用类似的选项。
3. **TEST_AFTER_INSTALL**:依赖项很可能有自己的测试套件，您可能希望运行测试套件，以确保在超级构建期间一切顺利。此选项将在安装步骤之后立即运行测试。

`ExternalProject_Add`可以理解的其他测试选项如下:

- **TEST_BEFORE_INSTALL**:将在安装步骤之前运行测试套件
- **TEST_EXCLUDE_FROM_MAIN**:可以从测试套件中，删除对外部项目的主要目标的依赖

这些选项都假定外部项目使用 CTest 管理测试。如果外部项目不使用 CTest 来管理测试，我们可以通过`TEST_COMMAND`选项来执行测试。

即使是为属于自己项目的模块引入超级构建模式，也需要引入额外的层，重新声明小型 CMake 项目，并通过`ExternalProject_Add`显式地传递配置设置。引入这个附加层的好处是，清晰地分离了变量和目标范围，这可以帮助管理由多个组件组成的项目中的复杂性、依赖性和名称空间，这些组件可以是内部的，也可以是外部的，并由 CMake 组合在一起。
