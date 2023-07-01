# 打包项目

本章的主要内容有：

- 生成源代码和二进制包
- 使用 CMake/pybind11 构建的 C++/Python 项目，通过 PyPI 发布
- 使用 CMake/CFFI 构建 C/Fortran/Python 项目，通过 PyPI 发布
- 以 Conda 包的形式发布一个简单的项目
- 将 Conda 包作为依赖项发布给项目

目前为止，已经从源代码编译并安装了软件包——这意味着可以通过 Git 获取项目，并手动执行配置、构建、测试和安装。然而，在实际中，软件包通常是使用管理器来安装的，比如 Apt、DNF、Pacman、pip 和 Conda。我们需要以各种格式发布我们的代码项目——作为源文件或二进制安装程序。

本章中，我们将探讨不同的打包策略。首先，讨论使用 CMake 中的工具 CPack 进行打包，还提供打包和上传 CMake 项目到 Python 包索引(PyPI, [https://pypi.org)和 Anaconda 云(https://anaconda.org](https://pypi.xn--org/)anaconda(https-oi98au39f//anaconda.org) )的方法，这些都是通过包管理器 pip 和 Conda (https://conda.io/docs/ )分发包的平台。对于 PyPI，我们将演示如何打包和分发混合 C++/Python 或 C/Fortran/Python 的项目。对于 Conda，我们将展示如何对依赖于其他库的 C++项目进行打包。

# 11.1 生成源代码和二进制包

**NOTE**:_此示例代码可以在 codes/chapter-11/recipe-01 中找到。该示例在 CMake 3.6 版(或更高版本)中是有效的，并且已经在 GNU/Linux、macOS 和 Windows 上进行过测试。_

如果代码是开源的，用户将能够下载项目的源代码，并使用完全定制的 CMake 脚本自行构建。当然，打包操作也可以使用脚本完成，但是 CPack 提供了更简单和可移植的替代方案。本示例将指导您创建一些包:

- **源代码包**：可以将源代码直接压缩成归档文件，进行发送。用户将不必为特定的版本控制系统操心。

- **二进制包**：工具将新构建的目标以打包的方式到归档文件中。这个功能非常有用，但可能不够健壮，无法发布库和可执行程序。

- 平台原生的二进制安装

  ：CPack 能够以许多不同的格式生成二进制安装程序，因此可以将软件发布到不同的平台。我们将展示如何生成安装程序:

  - 基于 Debian 的 GNU/Linux 发行版的`.deb`格式： https://manpages.debian.org/unstable/dpkg-dev/deb.5.en.html
  - 基于 Red Hat 的 GNU/Linux 发行版的`.rpm`格式： http://rpm.org/
  - macOS 包的`.dmg`格式: https://developer.apple.com/library/archive/documentation/CoreFoundation/Conceptual/CFBundles/BundleTypes/BundleTypes.html
  - Windows 的 NSIS 格式: http://nsis.sourceforge.net/Main_Page

## 准备工作

我们将使用第 10 章第 3 节的示例，项目树由以下目录和文件组成:

```
.
├── cmake
│    ├── coffee.icns
│    ├── Info.plist.in
│    └── messageConfig.cmake.in
├── CMakeCPack.cmake
├── CMakeLists.txt
├── INSTALL.md
├── LICENSE
├── src
│    ├── CMakeLists.txt
│    ├── hello-world.cpp
│    ├── Message.cpp
│    └── Message.hpp
└── tests
    ├── CMakeLists.txt
    └── use_target
        ├── CMakeLists.txt
        └── use_message.cpp
```

由于本示例的重点是使用 CPack，所以不会讨论源码。我们只会在`CMakeCPack.cmake`中添加打包指令。此外，还添加了`INSTALL.md`和`LICENSE`文件：打包要求需要包含安装说明和项目许可信息。

## 具体实施

让我们看看需要添加到这个项目中的打包指令。我们将在`CMakeCPack.cmake`中收集它们，并在在`CMakeLists.txt`的末尾包含这个模块`include(cmakecpackage.cmake)`:

1. 我们声明包的名称，与项目的名称相同，因此我们使用`PROJECT_NAME`的 CMake 变量:

   ```
   set(CPACK_PACKAGE_NAME "${PROJECT_NAME}")
   ```

2. 声明包的供应商：

   ```
   set(CPACK_PACKAGE_VENDOR "CMake Cookbook")
   ```

3. 打包的源代码将包括一个描述文件。这是带有安装说明的纯文本文件:

   ```
   set(CPACK_PACKAGE_DESCRIPTION_FILE "${PROJECT_SOURCE_DIR}/INSTALL.md")
   ```

4. 还添加了一个包的描述:

   ```
   set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "message: a small messaging library")
   ```

5. 许可证文件也将包括在包中:

   ```
   set(CPACK_RESOURCE_FILE_LICENSE "${PROJECT_SOURCE_DIR}/LICENSE")
   ```

6. 从发布包中安装时，文件将放在`/opt/recipe-01`目录下:

   ```
   set(CPACK_PACKAGING_INSTALL_PREFIX "/opt/${PROJECT_NAME}")
   ```

7. CPack 所需的主要、次要和补丁版本:

   ```
   set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
   set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
   set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")
   ```

8. 设置了在包装的时候需要忽略的文件列表和目录:

   ```
   set(CPACK_SOURCE_IGNORE_FILES "${PROJECT_BINARY_DIR};/.git/;.gitignore")
   ```

9. 列出了源代码归档的打包生成器——在我们的例子中是`ZIP`，用于生成`.ZIP`归档，`TGZ`用于`.tar.gz`归档:

   ```
   set(CPACK_SOURCE_GENERATOR "ZIP;TGZ")
   ```

10. 我们还列出了二进制存档生成器:

    ```
    set(CPACK_GENERATOR "ZIP;TGZ")
    ```

11. 现在也可声明平台原生二进制安装程序，从 DEB 和 RPM 包生成器开始，不过只适用于 GNU/Linux:

    ```
    if(UNIX)
      if(CMAKE_SYSTEM_NAME MATCHES Linux)
        list(APPEND CPACK_GENERATOR "DEB")
        set(CPACK_DEBIAN_PACKAGE_MAINTAINER "robertodr")
        set(CPACK_DEBIAN_PACKAGE_SECTION "devel")
        set(CPACK_DEBIAN_PACKAGE_DEPENDS "uuid-dev")

        list(APPEND CPACK_GENERATOR "RPM")
        set(CPACK_RPM_PACKAGE_RELEASE "1")
        set(CPACK_RPM_PACKAGE_LICENSE "MIT")
        set(CPACK_RPM_PACKAGE_REQUIRES "uuid-devel")
      endif()
    endif()
    ```

12. 如果我们在 Windows 上，我们会想要生成一个 NSIS 安装程序:

    ```
    if(WIN32 OR MINGW)
      list(APPEND CPACK_GENERATOR "NSIS")
      set(CPACK_NSIS_PACKAGE_NAME "message")
      set(CPACK_NSIS_CONTACT "robertdr")
      set(CPACK_NSIS_ENABLE_UNINSTALL_BEFORE_INSTALL ON)
    endif()
    ```

13. 另一方面，在 macOS 上，bundle 包是我们的安装程序的选择:

    ```
    if(APPLE)
      list(APPEND CPACK_GENERATOR "Bundle")
      set(CPACK_BUNDLE_NAME "message")
      configure_file(${PROJECT_SOURCE_DIR}/cmake/Info.plist.in Info.plist @ONLY)
      set(CPACK_BUNDLE_PLIST ${CMAKE_CURRENT_BINARY_DIR}/Info.plist)
      set(CPACK_BUNDLE_ICON ${PROJECT_SOURCE_DIR}/cmake/coffee.icns)
    endif()
    ```

14. 我们在现有系统的包装生成器上，向用户打印一条信息:

    ```
    message(STATUS "CPack generators: ${CPACK_GENERATOR}")
    ```

15. 最后，我们包括了`CPack.cmake`标准模块。这将向构建系统添加一个包和一个`package_source`目标:

    ```
    include(CPack)
    ```

现在来配置这个项目：

```
$ mkdir -p build
$ cd build
$ cmake ..
```

使用下面的命令，我们可以列出可用的目标(示例输出是在使用 Unix Makefile 作为生成器的 GNU/Linux 系统上获得的):

```
$ cmake --build . --target help

The following are some of the valid targets for this Makefile:
... all (the default if no target is provided)
... clean
... depend
... install/strip
... install
... package_source
... package
... install/local
... test
... list_install_components
... edit_cache
... rebuild_cache
... hello- world
... message
```

我们可以看到`package`和`package_source`目标是可用的。可以使用以下命令生成源包:

```
$ cmake --build . --target package_source

Run CPack packaging tool for source...
CPack: Create package using ZIP
CPack: Install projects
CPack: - Install directory: /home/user/cmake-cookbook/chapter-11/recipe-01/cxx-example
CPack: Create package
CPack: - package: /home/user/cmake-cookbook/chapter- 11/recipe-01/cxx-example/build/recipe-01-1.0.0-Source.zip generated.
CPack: Create package using TGZ
CPack: Install projects
CPack: - Install directory: /home/user/cmake-cookbook/chapter- 11/recipe-01/cxx-example
CPack: Create package
CPack: - package: /home/user/cmake-cookbook/chapter-11/recipe-01/cxx-example/build/recipe-01- 1.0.0-Source.tar.gz generated.
```

同样，也可以构建二进制包:

```
$ cmake --build . --target package message-1.0.0-Linux.deb
```

例子中，最后得到了以下二进制包:

```
message-1.0.0-Linux.rpm
message-1.0.0-Linux.tar.gz
message-1.0.0-Linux.zip
```

## 工作原理

CPack 可用于生成用于分发的包。生成构建系统时，我们在`CMakeCPack.cmake`中列出了 CPack 指令，用于在构建目录下生成`CPackConfig.cmake`。当运行以`package`或`package_source`目标的 CMake 命令时，CPack 会自动调用，参数是自动生成的配置文件。实际上，这两个新目标是对 CPack 简单规则的使用。与 CMake 一样，CPack 也有生成器的概念。CMake 上下文中的生成器是用于生成本地构建脚本的工具，例如 Unix Makefile 或 Visual Studio 项目文件，而 CPack 上下文中的生成器是用于打包的工具。我们列出了这些变量，并对不同的平台进行了特别的关注，为源包和二进制包定义了`CPACK_SOURCE_GENERATOR`和`CPACK_GENERATOR`变量。因此，`DEB`包生成器将调用`Debian`打包实用程序，而`TGZ`生成器将调用给定平台上的归档工具。我们可以直接在`build`目录中调用 CPack，并选择要与`-G`命令行选项一起使用的生成器。`RPM`包可以通过以下步骤生成:

```
$ cd build
$ cpack -G RPM

CPack: Create package using RPM
CPack: Install projects
CPack: - Run preinstall target for: recipe-01
CPack: - Install project: recipe-01
CPack: Create package
CPackRPM: Will use GENERATED spec file: /home/user/cmake-cookbook/chapter-11/recipe-01/cxx-example/build/_CPack_Packages/Linux/RPM/SPECS/recipe-01.spec
CPack: - package: /home/user/cmake-cookbook/chapter-11/recipe-01/cxx-example/build/recipe-01-1.0.0-Linux.rpm generated.
```

对于任何发行版，无论是源代码还是二进制文件，我们只需要打包用户需要的内容，因此整个构建目录和其他与版本控制相关的文件，都必须从要打包的文件列表中排除。我们的例子中，排除列表使用下面的命令声明：

```
set(CPACK_SOURCE_IGNORE_FILES "${PROJECT_BINARY_DIR};/.git/;.gitignore")
```

我们还需要指定包的基本信息，例如：名称、简短描述和版本。这个信息是通过 CMake 变量设置的，当包含相应的模块时，CMake 变量被传递给 CPack。

**NOTE**:_由于 CMake 3.9 中的`project()`命令接受`DESCRIPTION`字段，该字段带有一个描述项目的短字符串。CMake 将设置一个`PROJECT_DESCRIPTION`，可以用它来重置`CPACK_PACKAGE_DESCRIPTION_SUMMARY`。_

让我们详细看看，可以为示例项目生成的不同类型包的说明。

### 打包源码

我们的示例中，决定对源存档使用`TGZ`和`ZIP`生成器。这些文件将分别生成`.tar.gz`和`.zip`压缩文件。我们可以检查生成的`.tar.gz`文件的内容:

```
$ tar tzf recipe-01-1.0.0-Source.tar.gz

recipe-01-1.0.0-Source/opt/
recipe-01-1.0.0-Source/opt/recipe-01/
recipe-01-1.0.0-Source/opt/recipe-01/cmake/
recipe-01-1.0.0-Source/opt/recipe-01/cmake/coffee.icns
recipe-01-1.0.0-Source/opt/recipe-01/cmake/Info.plist.in
recipe-01-1.0.0-Source/opt/recipe-01/cmake/messageConfig.cmake.in
recipe-01-1.0.0-Source/opt/recipe-01/CMakeLists.txt
recipe-01-1.0.0-Source/opt/recipe-01/src/
recipe-01-1.0.0-Source/opt/recipe-01/src/Message.hpp
recipe-01-1.0.0-Source/opt/recipe-01/src/CMakeLists.txt
recipe-01-1.0.0-Source/opt/recipe-01/src/Message.cpp
recipe-01-1.0.0-Source/opt/recipe-01/src/hello-world.cpp
recipe-01-1.0.0-Source/opt/recipe-01/LICENSE
recipe-01-1.0.0-Source/opt/recipe-01/tests/
recipe-01-1.0.0-Source/opt/recipe-01/tests/CMakeLists.txt
recipe-01-1.0.0-Source/opt/recipe-01/tests/use_target/
recipe-01-1.0.0-Source/opt/recipe-01/tests/use_target/CMakeLists.txt
recipe-01-1.0.0-Source/opt/recipe-01/tests/use_target/use_message.cpp
recipe-01-1.0.0-Source/opt/recipe-01/INSTALL.md
```

与预期相同，只包含源码树的内容。注意`INSTALL.md`和`LICENSE`文件也包括在内，可以通过`CPACK_PACKAGE_DESCRIPTION_FILE`和`CPACK_RESOURCE_FILE_LICENSE`变量指定。

**NOTE**:_Visual Studio 生成器无法解析`package_source`目标:https://gitlab.kitware.com/cmake/cmake/issues/13058。_

### 二进制包

创建二进制存档时，CPack 将打包`CMakeCPack.cmake`中描述的目标的内容。因此，在我们的示例中，hello-world 可执行文件、消息动态库以及相应的头文件都将以`.tar.gz`和`.zip`的格式打包。此外，还将打包 CMake 配置文件。这对于需要链接到我们的库的其他项目非常有用。包中使用的安装目录可能与从构建树中安装项目时使用的前缀不同，可以使用`CPACK_PACKAGING_INSTALL_PREFIX`变量来实现这一点。我们的示例中，我们将它设置为系统上的特定位置:`/opt/recipe-01`。

```
$ tar tzf recipe-01-1.0.0-Linux.tar.gz

recipe-01- 1.0.0-Linux/opt/
recipe-01-1.0.0-Linux/opt/recipe-01/
recipe-01-1.0.0- Linux/opt/recipe-01/bin/
recipe-01-1.0.0-Linux/opt/recipe-01/bin/hello- world
recipe-01-1.0.0-Linux/opt/recipe-01/share/
recipe-01-1.0.0- Linux/opt/recipe-01/share/cmake/
recipe-01-1.0.0-Linux/opt/recipe- 01/share/cmake/recipe-01/
recipe-01-1.0.0-Linux/opt/recipe- 01/share/cmake/recipe-01/messageConfig.cmake
recipe-01-1.0.0- Linux/opt/recipe-01/share/cmake/recipe-01/messageTargets-hello- world.cmake
recipe-01-1.0.0-Linux/opt/recipe-01/share/cmake/recipe- 01/messageConfigVersion.cmake
recipe-01-1.0.0-Linux/opt/recipe- 01/share/cmake/recipe-01/messageTargets-hello-world- release.cmake
recipe-01-1.0.0-Linux/opt/recipe-01/share/cmake/recipe- 01/messageTargets-release.cmake
recipe-01-1.0.0-Linux/opt/recipe- 01/share/cmake/recipe-01/messageTargets.cmake
recipe-01-1.0.0- Linux/opt/recipe-01/include/
recipe-01-1.0.0-Linux/opt/recipe- 01/include/message/
recipe-01-1.0.0-Linux/opt/recipe- 01/include/message/Message.hpp
recipe-01-1.0.0-Linux/opt/recipe- 01/include/message/messageExport.h
recipe-01-1.0.0-Linux/opt/recipe- 01/lib64/
recipe-01-1.0.0-Linux/opt/recipe- 01/lib64/libmessage.so
recipe-01-1.0.0-Linux/opt/recipe- 01/lib64/libmessage.so.1`
```

### 平台原生的二进制安装

我们希望每个平台原生二进制安装程序的配置略有不同。可以在单个`CMakeCPack.cmake`中使用 CPack 管理这些差异，就像例子中做的那样。

对于 GNU/Linux 系统，配置了`DEB`和`RPM`生成器:

```
if(UNIX)
  if(CMAKE_SYSTEM_NAME MATCHES Linux)
    list(APPEND CPACK_GENERATOR "DEB")
    set(CPACK_DEBIAN_PACKAGE_MAINTAINER "robertodr")
    set(CPACK_DEBIAN_PACKAGE_SECTION "devel")
    set(CPACK_DEBIAN_PACKAGE_DEPENDS "uuid-dev")

    list(APPEND CPACK_GENERATOR "RPM")
    set(CPACK_RPM_PACKAGE_RELEASE "1")
    set(CPACK_RPM_PACKAGE_LICENSE "MIT")
    set(CPACK_RPM_PACKAGE_REQUIRES "uuid-devel")
  endif()
endif()
```

我们的示例依赖于 UUID 库，`CPACK_DEBIAN_PACKAGE_DEPENDS`和`cpack_rpm_package_require`选项允许指定，包和数据库中对其他包的依赖关系。可以使用 dpkg 和 rpm 程序分别分析生成的`.deb`和`.rpm`包的内容。

注意，`CPACK_PACKAGING_INSTALL_PREFIX`也会影响这些包生成器：我们的包将安装到`/opt/recipe-01`。

CMake 真正提供了跨平台和可移植构建系统的支持。下面将使用 Nullsoft 脚本安装系统(NSIS)创建一个安装程序:

```
if(WIN32 OR MINGW)
  list(APPEND CPACK_GENERATOR "NSIS")
  set(CPACK_NSIS_PACKAGE_NAME "message")
  set(CPACK_NSIS_CONTACT "robertdr")
  set(CPACK_NSIS_ENABLE_UNINSTALL_BEFORE_INSTALL ON)
endif()
```

如果在 macOS 上构建项目，将启用`Bundle packager`:

```
if(APPLE)
  list(APPEND CPACK_GENERATOR "Bundle")
  set(CPACK_BUNDLE_NAME "message")
  configure_file(${PROJECT_SOURCE_DIR}/cmake/Info.plist.in Info.plist @ONLY)
  set(CPACK_BUNDLE_PLIST ${CMAKE_CURRENT_BINARY_DIR}/Info.plist)
  set(CPACK_BUNDLE_ICON ${PROJECT_SOURCE_DIR}/cmake/coffee.icns)
endif()
```

macOS 的示例中，需要为包配置属性列表文件，这是通过`configure_file`实现的。`Info.plist`的位置和包的图标，这些都可以通过 CPack 的变量进行设置。

**NOTE**:_可以在这里阅读，关于属性列表格式的更多信息:https://en.wikipedia.org/wiki/Property_list_

## 更多信息

对`CMakeCPack.cmake`进行设置，要比列出 CPack 的配置选项简单的多，我们可以将`CPACK_*`变量的每个生成器设置放在单独的文件中，比如`CMakeCPackOptions.cmake`，并将这些设置包含到`CMakeCPack.cmake`使用`set(CPACK_PROJECT_CONFIG_FILE "${PROJECT_SOUsRCE_DIR}/CMakeCPackOptions.cmake")`将设置包含入`CMakeCPack.cmake`中。还可以在 CMake 时配置该文件，然后在 CPack 时包含该文件，这为配置多格式包生成器提供了一种简洁的方法(参见https://cmake.org/cmake/help/v3.6/module/CPack.html )。

与 CMake 中的所有工具一样，CPack 功能强大、功能多样，并且提供了更多的灵活性和选项。感兴趣的读者应该看官方文档的命令行界面 CPack (https://cmake.org/cmake/help/v3.6/manual/cpack.1.html )手册页，如何使用 CPack 生成器打包相关项目的更多细节(https://cmake.org/cmake/help/v3.6/module/CPack.html )。

# 11.2 通过 PyPI 发布使用 CMake/pybind11 构建的 C++/Python 项目

**NOTE**:_此示例代码可以在 codes/chapter-11/recipe-02 中找到。该示例在 CMake 3.11 版(或更高版本)中是有效的，并且已经在 GNU/Linux、macOS 和 Windows 上进行过测试。_

本示例中，我们将以第 9 章第 5 节的代码的 pybind11 为例，为其添加相关的安装目标和 pip 打包信息，并将项目上传到 PyPI。我们要实现一个可以使用 pip 安装，并运行 CMake 从而获取底层 pybind11 依赖项的项目。

## 准备工作

要通过 PyPI 分发包的话，需要一个[https://pypi.org](https://pypi.org/) 帐户。当然，也可以先从本地路径进行安装练习。

**TIPS**:_建议使用 Pipenv ([https://docs.pipenv.org](https://docs.pipenv.org/) )或虚拟环境([https://virtualenv.pypa](https://virtualenv.pypa/) )安装这个包和其他的 Python 包。_

我们基于第 9 章第 5 节的项目，它包含一个主`CMakeLists.txt`文件和一个`account/CMakeLists.txt`文件，配置帐户示例目标时，使用如下的项目树：

```
.
├── account
│    ├── account.cpp
│    ├── account.hpp
│    ├── CMakeLists.txt
│    s└── test.py
└── CMakeLists.txt
```

示例中，`account.cpp`,`account.hpp`和`test.py`没有任何变化。修改`account/CMakeLists.txt`，并为 pip 添加几个文件，以便能够构建安装包。为此，需要根目录中的另外三个文件：`README.rst`，`MANIFEST.in`和`setup.py`。

`README.rst`中包含关于项目的 s 文档：

```
Example project
===============

Project description in here ...
```

`MANIFEST.in`列出了需要安装的 Python 模块：

```
include README.rst CMakeLists.txt
recursive-include account *.cpp *.hpp CMakeLists.txt
```

最后，`setup.py`包含构建指令和安装项目的说明：

```
import distutils.command.build as _build
import os
import sys
from distutils import spawn
from distutils.sysconfig import get_python_lib
from setuptools import setup


def extend_build():
  class build(_build.build):
    def run(self):
      cwd = os.getcwd()
      if spawn.find_executable('cmake') is None:
        sys.stderr.write("CMake is required to build this package.\n")
        sys.exit(-1)
        _source_dir = os.path.split(__file__)[0]
        _build_dir = os.path.join(_source_dir, 'build_setup_py')
        _prefix = get_python_lib()
        try:
          cmake_configure_command = [
              'cmake',
              '-H{0}'.format(_source_dir),
              '-B{0}'.format(_build_dir),
              '-DCMAKE_INSTALL_PREFIX={0}'.format(_prefix),
          ]
          _generator = os.getenv('CMAKE_GENERATOR')
          if _generator is not None:
            cmake_configure_command.append('-
                                          G{0}'.format(_generator))
          spawn.spawn(cmake_configure_command)
          spawn.spawn(
                ['cmake', '--build', _build_dir, '--target', 'install'])
          os.chdir(cwd)
        except spawn.DistutilsExecError:
          sys.stderr.write("Error while building with CMake\n")
          sys.exit(-1)
          _build.build.run(self)
  return build

_here = os.path.abspath(os.path.dirname(__file__))

if sys.version_info[0] < 3:
  with open(os.path.join(_here, 'README.rst')) as f:
    long_description = f.read()
else:
  with open(os.path.join(_here, 'README.rst'), encoding='utf-8') as f:
    long_description = f.read()

_this_package = 'account'

version = {}
with open(os.path.join(_here, _this_package, 'version.py')) as f:
  exec(f.read(), version)

setup(
    name=_this_package,
    version=version['__version__'],
    description='Description in here.',
    long_description=long_description,
    author='Bruce Wayne',
    author_email='bruce.wayne@example.com',
    url='http://example.com',
    license='MIT',
    packages=[_this_package],
    include_package_data=True,
    classifiers=[
        'Development Status :: 3 - Alpha',
        'Intended Audience :: Science/Research',
        'Programming Language :: Python :: 2.7',
        'Programming Language :: Python :: 3.6'
    ],
    cmdclass={'build': extend_build()})
```

`account`子目录中放置一个`__init__.py`脚本：

```
from .version import __version__
from .account import Account
__all__ = [
  '__version__',
  'Account',
]
```

再放一个`version.py`脚本：

```
__version__ = '0.0.0'
```

项目的文件结构如下：

```
.
├── account
│    ├── account.cpp
│    ├── account.hpp
│    ├── CMakeLists.txt
│    ├── __init__.py
│    ├── test.py
│    └── version.py
├── CMakeLists.txt
├── MANIFEST.in
├── README.rst
└── setup.py
```

## 具体实施

本示例基于第 9 章第 5 节项目的基础上。

首先，修改`account/CMakeLists.txt`，添加安装目标：

```
install(
  TARGETS
  	account
  LIBRARY
  	DESTINATION account
  )
```

安装目标时，`README.rst`, `MANIFEST.in`，`setup.py`、`__init__.py`和`version.py`将放置在对应的位置上，我们准备使用 pybind11 测试安装过程：

1. 为此，在某处创建一个新目录，我们将在那里测试安装。

2. 在创建的目录中，从本地路径运行`pipenv install`。调整本地路径，指向`setup.py`的目录：

   ```
   $ pipenv install /path/to/cxx-example
   ```

3. 在 Pipenv 环境中打开一个 Python shell：

   ```
   $ pipenv run python
   ```

4. Python shell 中，可以测试我们的 CMake 包：

   ```
   >>> from account import Account
   >>> account1 = Account()
   >>> account1.deposit(100.0)
   >>> account1.deposit(100.0)
   >>> account1.withdraw(50.0)
   >>> print(account1.get_balance())
   150.0
   ```

## 工作原理

`${CMAKE_CURRENT_BINARY_DIR}`目录包含编译后的`account.cpython-36m-x86_64-linux-gnu.so`，这个动态库就是使用 pybind11 构建 Python 模块。但是请注意，它的名称取决于操作系统(本例中是 64 位 Linux)和 Python 环境(本例中是 Python 3.6)。`setup.py`s 脚本将运行 CMake，并根据所选的 Python 环境(系统 Python，Pipenv 或虚拟环境)将 Python 模块安装到正确的路径下。

不过，在安装模块时面临两个挑战：

- 名称可变
- CMake 外部设置路径

可以使用下面的安装目标来解决这个问题，将在 setup.py 中定义安装目标位置：

```
install(
  TARGETS
  	account
  LIBRARY
  	DESTINATION account
  )
```

指示 CMake 将编译好的 Python 模块文件安装到相对于安装目标位置的`account`子目录中(第 10 章中详细讨论了如何设置目标位置)。`setup.py`将通过设置`CMAKE_INSTALL_PREFIX`来设置安装位置，并根据 Python 环境指向正确的路径。

让我们看看`setup.py`如何实现的。自下而上来看一下脚本：

```
setup(
  name=_this_package,
  version=version['__version__'],
  description='Description in here.',
  long_description=long_description,
  author='Bruce Wayne',
  author_email='bruce.wayne@example.com',
  url='http://example.com',
  license='MIT',
  packages=[_this_package],
  include_package_data=True,
  classifiers=[
    'Development Status :: 3 - Alpha',
    'Intended Audience :: Science/Research',
    'Programming Language :: Python :: 2.7',
    'Programming Language :: Python :: 3.6'
  ],
  cmdclass={'build': extend_build()})
```

该脚本包含许多占位符，还包含一些自解释的语句。这里我们将重点介绍最后一个指令`cmdclass`。这个指令中，通过自定义`extend_build`函数扩展默认的构建步骤。这个默认的构建步骤如下：

```
def extend_build():
  class build(_build.build):
    def run(self):
      cwd = os.getcwd()
      if spawn.find_executable('cmake') is None:
        sys.stderr.write("CMake is required to build this package.\n")
        sys.exit(-1)
        _source_dir = os.path.split(__file__)[0]
        _build_dir = os.path.join(_source_dir, 'build_setup_py')
        _prefix = get_python_lib()
        try:
          cmake_configure_command = [
              'cmake',
              '-H{0}'.format(_source_dir),
              '-B{0}'.format(_build_dir),
              '-DCMAKE_INSTALL_PREFIX={0}'.format(_prefix),
          ]
          _generator = os.getenv('CMAKE_GENERATOR')
          if _generator is not None:
            cmake_configure_command.append('-
                                          G{0}'.format(_generator))
          spawn.spawn(cmake_configure_command)
          spawn.spawn(
                ['cmake', '--build', _build_dir, '--target', 'install'])
          os.chdir(cwd)
        except spawn.DistutilsExecError:
          sys.stderr.write("Error while building with CMake\n")
          sys.exit(-1)
          _build.build.run(self)
  return build
```

首先，检查 CMake 是否可用。函数执行了两个 CMake 命令：

```
cmake_configure_command = [
    'cmake',
    '-H{0}'.format(_source_dir),
    '-B{0}'.format(_build_dir),
    '-DCMAKE_INSTALL_PREFIX={0}'.format(_prefix),
]
_generator = os.getenv('CMAKE_GENERATOR')
if _generator is not None:
  cmake_configure_command.append('-
                                G{0}'.format(_generator))
spawn.spawn(cmake_configure_command)
spawn.spawn(
      ['cmake', '--build', _build_dir, '--target', 'install'])
```

我们可以设置`CMAKE_GENERATOR`环境变量来修改生成器。安装目录如下方式设置：

```
_prefix = get_python_lib()
```

从安装目录的根目录下，通过`distutils.sysconfig`导入`get_python_lib`函数。`cmake --build _build_dir --target install`命令以一种可移植的方式，构建和安装我们的项目。使用`_build_dir`而不使用`build`的原因是，在测试本地安装时，项目可能已经包含了一个`build`目录，这将与新安装过程发生冲突。对于已经上传到 PyPI 的包，构建目录的名称并不会带来什么影响。

## 更多信息

现在我们已经测试了本地安装，准备将包上传到 PyPI。在此之前，请确保`setup.py`中的元数据(例如：项目名称、联系方式和许可协议信息)是合理的，并且项目名称没有与 PyPI 已存在项目重名。在上传到[https://pypi.org](https://pypi.org/) 之前，先测试 PyPI([https://test.pypi.org](https://test.pypi.org/) )上，进行上载和下载的尝试。

上传之前，我们需要在主目录中创建一个名为`.pypirc`的文件，其中包含(替换成自己的`yourusername`和`yourpassword`)：

```
[distutils]account
index-servers=
  pypi
  pypitest

[pypi]
username = yourusername
password = yourpassword

[pypitest]
repository = https://test.pypi.org/legacy/
username = yourusername
password = yourpassword
```

我们将分两步进行。首先，我们在本地创建 Release 包：

```
$ python setup.py sdist
```

第二步中，使用 Twine 上传生成的分布数据(我们将 Twine 安装到本地的 Pipenv 中):

```
$ pipenv run twine upload dist/* -r pypitest

Uploading distributions to https://test.pypi.org/legacy/
Uploading yourpackage-0.0.0.tar.gz
```

下一步，从测试实例到，将包安装到一个隔离的环境中：

```
$ pipenv shell
$ pip install --index-url https://test.pypi.org/simple/ yourpackage
```

当一切正常，就将我们的包上传到了 PyPI：

```
$ pipenv run twine upload dist/* -r pypi
```

# 11.3 通过 PyPI 发布使用 CMake/CFFI 构建 C/Fortran/Python 项目

**NOTE**:_此示例代码可以在 codes/chapter-11/recipe-03 中找到，其中有一个 C++和 Fortran 示例。该示例在 CMake 3.5 版(或更高版本)中是有效的，并且已经在 GNU/Linux、macOS 和 Windows 上进行过测试。_

基于第 9 章第 6 节的示例，我们将重用前一个示例中的构建块，不过这次使用 Python CFFI 来提供 Python 接口，而不是 pybind11。这个示例中，我们通过 PyPI 共享一个 Fortran 项目，这个项目可以是 C 或 C++项目，也可以是任何公开 C 接口的语言，非 Fortran 就可以。

## 准备工作

项目将使用如下的目录结构：

```
.
├── account
│    ├── account.h
│    ├── CMakeLists.txt
│    ├── implementation
│    │    └── fortran_implementation.f90
│    ├── __init__.py
│    ├── interface_file_names.cfg.in
│    ├── test.py
│    └── version.py
├── CMakeLists.txt
├── MANIFEST.in
├── README.rst
└── setup.py
```

主`CMakeLists.txt`文件和`account`下面的所有源文件(`account/CMakeLists.txt`除外)与第 9 章中的使用方式相同。`README.rst`文件与前面的示例相同。`setup.py`脚本比上一个示例多了一行(包含`install_require =['cffi']`的那一行):

```
# ... up to this line the script is unchanged
setup(
  name=_this_package,
  version=version['__version__'],
  description='Description in here.',
  long_description=long_description,
  author='Bruce Wayne',
  author_email='bruce.wayne@example.com',
  url='http://example.com',
  license='MIT',
  packages=[_this_package],
  install_requires=['cffi'],
  include_package_data=True,
  classifiers=[
    'Development Status :: 3 - Alpha',
    'Intended Audience :: Science/Research',
    'Programming Language :: Python :: 2.7',
    'Programming Language :: Python :: 3.6'
  ],
  cmdclass={'build': extend_build()})
```

`MANIFEST.in`应该与 Python 模块和包一起安装，并包含以下内容:

```
include README.rst CMakeLists.txt
recursive-include account *.h *.f90 CMakeLists.txt
```

`account`子目录下，我们看到两个新文件。一个`version.py`文件，其为`setup.py`保存项目的版本信息：

```
__version__ = '0.0.0'
```

子目录还包含`interface_file_names.cfg.in`文件:

```
[configuration]
header_file_name = account.h
library_file_name = $<TARGET_FILE_NAME:account>
```

## 具体实施

讨论一下实现打包的步骤：

1. 示例基于第 9 章第 6 节，使用 Python CFFI 扩展了`account/CMakeLists.txt`，增加以下指令:

   ```
   file(
     GENERATE OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/interface_file_names.cfg
     INPUT ${CMAKE_CURRENT_SOURCE_DIR}/interface_file_names.cfg.in
     )

   set_target_properties(account
     PROPERTIES
       PUBLIC_HEADER "account.h;${CMAKE_CURRENT_BINARY_DIR}/account_export.h"
       RESOURCE "${CMAKE_CURRENT_BINARY_DIR}/interface_file_names.cfg"
     )

   install(
     TARGETS
       account
     LIBRARY
       DESTINATION account/lib
     RUNTIME
       DESTINATION account/lib
     PUBLIC_HEADER
       DESTINATION account/include
     RESOURCE
       DESTINATION account
     )
   ```

   安装目标和附加文件准备好之后，就可以测试安装了。为此，会在某处创建一个新目录，我们将在那里测试安装。

2. 新创建的目录中，我们从本地路径运行 pipenv install。调整本地路径，指向`setup.py`脚本保存的目录:

   ```
   $ pipenv install /path/to/fortran-example
   ```

3. 现在在 Pipenv 环境中生成一个 Python shell:

   ```
   $ pipenv run python
   ```

4. Python shell 中，可以测试 CMake 包:

   ```
   >>> import account
   >>> account1 = account.new()
   >>> account.deposit(account1, 100.0)
   >>> account.deposit(account1, 100.0)
   >>> account.withdraw(account1, 50.0)
   >>> print(account.get_balance(account1))

   150.0
   ```

## 工作原理

使用 Python CFFI 和 CMake 安装混合语言项目的扩展与第 9 章第 6 节的例子相对比，和使用 Python CFFI 的 Python 包多了两个额外的步骤:

1. 需要`setup.py`s
2. 安装目标时，CFFI 所需的头文件和动态库文件，需要安装在正确的路径中，具体路径取决于所选择的 Python 环境

`setup.py`的结构与前面的示例几乎一致，唯一的修改是包含`install_require =['cffi']`，以确保安装示例包时，也获取并安装了所需的 Python CFFI。`setup.py`脚本会自动安装`__init__.py`和`version.py`。`MANIFEST.in`中的改变不仅有`README.rst`和 CMake 文件，还有头文件和 Fortran 源文件:

```
include README.rst CMakeLists.txt
recursive-include account *.h *.f90 CMakeLists.txt
```

这个示例中，使用 Python CFFI 和`setup.py`打包 CMake 项目时，我们会面临三个挑战:

- 需要将`account.h`和`account_export.h`头文件，以及动态库复制到系统环境中 Python 模块的位置。
- 需要告诉`__init__.py`，在哪里可以找到这些头文件和库。第 9 章第 6 节中，我们使用环境变量解决了这些问题，不过使用 Python 模块时，不可能每次去都设置这些变量。
- Python 方面，我们不知道动态库文件的确切名称(后缀)，因为这取决于操作系统。

让我们从最后一点开始说起：不知道确切的名称，但在 CMake 生成构建系统时是知道的，因此我们在`interface_file_names.cfg,in`中使用生成器表达式，对占位符进行展开：

```
[configuration]
header_file_name = account.h
library_file_name = $<TARGET_FILE_NAME:account>
```

输入文件用来生成`${CMAKE_CURRENT_BINARY_DIR}/interface_file_names.cfg`：

```
file(
  GENERATE OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/interface_file_names.cfg
  INPUT ${CMAKE_CURRENT_SOURCE_DIR}/interface_file_names.cfg.in
  )
```

然后，将两个头文件定义为`PUBLIC_HEADER`(参见第 10 章)，配置文件定义为`RESOURCE`：

```
set_target_properties(account
  PROPERTIES
  	PUBLIC_HEADER "account.h;${CMAKE_CURRENT_BINARY_DIR}/account_export.h"
  	RESOURCE "${CMAKE_CURRENT_BINARY_DIR}/interface_file_names.cfg"
)
```

最后，将库、头文件和配置文件安装到`setup.py`定义的安装路径中:

```
install(
  TARGETS
  	account
  LIBRARY
  	DESTINATION account/lib
  RUNTIME
  	DESTINATION account/lib
  PUBLIC_HEADER
  	DESTINATION account/include
  RESOURCE
  	DESTINATION account
  )
```

注意，我们为库和运行时都设置了指向`account/lib`的目标。这对于 Windows 很重要，因为动态库具有可执行入口点，因此我们必须同时指定这两个入口点。

Python 包将能够找到这些文件，要使用`account/__init__.py`来完成：

```
# this interface requires the header file and library file
# and these can be either provided by interface_file_names.cfg
# in the same path as this file
# or if this is not found then using environment variables
_this_path = Path(os.path.dirname(os.path.realpath(__file__)))
_cfg_file = _this_path / 'interface_file_names.cfg'
if _cfg_file.exists():
  config = ConfigParser()
  config.read(_cfg_file)
  header_file_name = config.get('configuration', 'header_file_name')
  _header_file = _this_path / 'include' / header_file_name
  _header_file = str(_header_file)
  library_file_name = config.get('configuration', 'library_file_name')
  _library_file = _this_path / 'lib' / library_file_name
  _library_file = str(_library_file)
else:
  _header_file = os.getenv('ACCOUNT_HEADER_FILE')
  assert _header_file is not None
  _library_file = os.getenv('ACCOUNT_LIBRARY_FILE')
  assert _library_file is not None
```

本例中，将找到`_cfg_file`并进行解析，`setup.py`将找到`include`下的头文件和`lib`下的库，并将它们传递给 CFFI，从而构造库对象。这也是为什么，使用`lib`作为安装目标`DESTINATION`，而不使用`CMAKE_INSTALL_LIBDIR`的原因(否则可能会让`account/__init__.py`混淆)。

## 更多信息

将包放到 PyPI 测试和生产实例中的后续步骤，因为有些步骤是类似的，所以可以直接参考前面的示例。

# 11.4 以 Conda 包的形式发布一个简单的项目

**NOTE**:_此示例代码可以在 codes/chapter-11/recipe-04 中找到。该示例在 CMake 3.5 版(或更高版本)中是有效的，并且已经在 GNU/Linux、macOS 和 Windows 上进行过测试。_

虽然 PyPI 是发布 Python 包的标准平台，但 Anaconda ([https://anaconda.org](https://anaconda.org/) )更为可能更为流行，因为它不仅允许使用 Python 接口发布 Python 或混合项目，还允许对非 Python 项目进行打包和依赖关系管理。这个示例中，我们将为一个非常简单的 C++示例项目准备一个 Conda 包，该项目使用 CMake 配置和构建，除了 C++之外没有依赖关系。下一个示例中，我们将会来看看一个更复杂的 Conda 包。

## 准备工作

我们的目标是打包以下示例代码(`example.cpp`)：

```
#include <iostream>
int main() {
	std::cout << "hello from your conda package!" << std::endl;
	return 0;
}
```

## 具体实施

1. `CMakeLists.txt`文件给出了最低版本要求、项目名称和支持的语言:

```
cmake_minimum_required(VERSION 3.5 FATAL_ERROR)
project(recipe-04 LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
```

1. 使用`example.cpp`构建`hello-conda`可执行目标：

   ```
   add_executable(hello-conda "")
   target_sources(hello-conda
     PRIVATE
     	example.cpp
     )
   ```

2. 使用`CMakeLists.txt`定义安装目标：

   ```
   nstall(
     TARGETS
     	hello-conda
     DESTINATION
     	bin
     )
   ```

3. 将在一个名为`meta.yaml`的文件中，对 Conda 包进行描述。我们将把它放在`conda-recipe`目录下，文件结构如下：

   ```
   .
   ├── CMakeLists.txt
   ├── conda-recipe
   │    └── meta.yaml
   └── example.cpp
   ```

4. `meta.yaml`包含如下内容：

   ```
   package:
     name: conda-example-simple
     version: "0.0.0"

   source:
     path: .. /  # this can be changed to git-url

   build:
     number: 0
     binary_relocation: true
     script:
       - cmake -H. -Bbuild_conda -G "${CMAKE_GENERATOR}" -DCMAKE_INSTALL_PREFIX=${PREFIX} # [not win]
       - cmake -H. -Bbuild_conda -G "%CMAKE_GENERATOR%" -DCMAKE_INSTALL_PREFIX="%LIBRARY_PREFIX%" # [win]
       - cmake - -build build_conda - -target install

   requirements:
     build:
       - cmake >=3.5
       - { { compiler('cxx') } }

   about:
     home: http://www.example.com
     license: MIT
     summary: "Summary in here ..."
   ```

5. 现在来构建包：

   ```
   $ conda build conda-recipe
   ```

6. 过程中屏幕上看到大量输出，但是一旦构建完成，就可以对包进行安装。首先，在本地进行测试：

   ```
   $ conda install --use-local conda-example-simple
   ```

7. 现在准备测试安装包，打开一个新的终端(假设 Anaconda 处于激活状态)，并输入以下内容：

   ```
   $ hello-conda

   hello from your conda package!
   ```

8. 测试成功后，再移除包装：

   ```
   $ conda remove conda-example-simple
   ```

## 工作原理

`CMakeLists.txt`中，安装目标是这个示例的一个基本组件:

```
install(
  TARGETS
  	hello-conda
  DESTINATION
  	bin
  )
```

目标的二进制文件会安装到`${CMAKE_INSTALL_PREFIX}/bin`中。变量由 Conda 定义，并且构建步骤中定义在`meta.yaml`：

```
build:
  number: 0
  binary_relocation: true
  script:
    - cmake -H. -Bbuild_conda -G "${CMAKE_GENERATOR}" -DCMAKE_INSTALL_PREFIX=${PREFIX} # [not win]
    - cmake -H. -Bbuild_conda -G "%CMAKE_GENERATOR%" -DCMAKE_INSTALL_PREFIX="%LIBRARY_PREFIX%" # [win]
    - cmake - -build build_conda - -target install
```

将安装目录设置为`${prefix}` (Conda 的内部变量)，然后构建并安装项目。调用构建目录命名为`build_conda`的动机与前面的示例类似：特定的构建目录名可能已经命名为`build`。

## 更多信息

配置文件`meta.yaml`可为任何项目指定构建、测试和安装步骤。详情请参考官方文档：https://conda.io/docs/user-guide/tasks/build-packages/define-metadata.html

要将 Conda 包上传到 Anaconda 云，请遵循官方的 Anaconda 文档: https://docs.anaconda.com/anaconda-cloud/user-guide/

此外，也可以考虑将 Miniconda，作为 Anaconda 的轻量级替代品：https://conda.io/miniconda.html

# 11.5 将 Conda 包作为依赖项发布给项目

**NOTE**:_此示例代码可以在 codes/chapter-11/recipe-05 中找到。该示例在 CMake 3.5 版(或更高版本)中是有效的，并且已经在 GNU/Linux、macOS 和 Windows 上进行过测试。_

这个示例中，我们将基于之前示例的结果，并且为 CMake 项目准备一个更真实和复杂的 Conda 包，这将取决于 DGEMM 的函数实现，对于矩阵与矩阵的乘法，可以使用 Intel 的 MKL 库进行。Intel 的 MKL 库可以以 Conda 包的形式提供。此示例将为我们提供一个工具集，用于准备和共享具有依赖关系的 Conda 包。

## 准备工作

对于这个示例，我们将使用与前一个示例中的 Conda 配置，和相同的文件命名和目录结构：

```
.
├── CMakeLists.txt
├── conda-recipe
│    └── meta.yaml
└── example.cpp
```

示例文件(`example.cpp`)将执行矩阵-矩阵乘法，并将 MKL 库返回的结果与“noddy”实现进行比较:

```
#include "mkl.h"

#include <cassert>
#include <cmath>
#include <iostream>
#include <random>

int main() {
  // generate a uniform distribution of real number between -1.0 and 1.0
  std::random_device rd;
  std::mt19937 mt(rd());
  std:: uniform_real_distribution < double > dist(-1.0, 1.0);

  int m = 500;
  int k = 1000;
  int n = 2000;

  double *A = (double *)mkl_malloc(m * k * sizeof(double), 64);
  double *B = (double *)mkl_malloc(k * n * sizeof(double), 64);
  double *C = (double *)mkl_malloc(m * n * sizeof(double), 64);
  double * D = new double[m * n];

  for (int i = 0; i < (m * k); i++) {
    A[i] = dist(mt);
  }

  for (int i = 0; i < (k * n); i++) {
    B[i] = dist(mt);
  }

  for (int i = 0; i < (m * n); i++) {
    C[i] = 0.0;
  }

  double alpha = 1.0;
  double beta = 0.0;
  cblas_dgemm(CblasRowMajor,
              CblasNoTrans,
              CblasNoTrans,
              m,
              n,
              k,
              alpha,
              A,
              k,
              B,
              n,
              beta,
              C,
              n);

  // D_mn = A_mk B_kn
  for (int r = 0; r < m; r++) {
    for (int c = 0; c < n; c++) {
      D[r * n + c] = 0.0;
      for (int i = 0; i < k; i++) {
        D[r * n + c] += A[r * k + i] * B[i * n + c];
      }
    }
  }

  // compare the two matrices
  double r = 0.0;
  for (int i = 0; i < (m * n); i++) {
    r += std::pow(C[i] - D[i], 2.0);
  }
  assert (r < 1.0e-12 & & "ERROR: matrices C and D do not match");

  mkl_free(A);
  mkl_free(B);
  mkl_free(C);
  delete[] D;

  std:: cout << "MKL DGEMM example worked!" << std:: endl;

  return 0;`
}
```

我们还需要修改`meta.yaml`。然而，与上一个示例相比，唯一的变化是在依赖项中加入了`mkl-devel`：

```
package:
  name: conda-example-dgemm
  version: "0.0.0"

source:
  path: ../ # this can be changed to git-url

build:
  number: 0
  script:
  - cmake -H. -Bbuild_conda -G "${CMAKE_GENERATOR}"
  -DCMAKE_INSTALL_PREFIX=${PREFIX} # [not win]
  - cmake -H. -Bbuild_conda -G "%CMAKE_GENERATOR%"
  -DCMAKE_INSTALL_PREFIX="%LIBRARY_PREFIX%" # [win]
  - cmake - -build build_conda - -target install

requirements:
  build:
    - cmake >=3.5
    - {{ compiler('cxx') }}
  host:
    - mkl - devel 2018

about:
  home: http://www.example.com
  license: MIT
  summary: "Summary in here ..."
```

## 具体实施

1. `CMakeLists.txt`文件声明了最低版本、项目名称和支持的语言：

```
cmake_minimum_required(VERSION 3.5 FATAL_ERROR)
project(recipe-05 LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
```

1. 使用`example.cpp`构建`dgem-example`可执行目标：

   ```
   add_executable(dgemm-example "")
   target_sources(dgemm-example
     PRIVATE
     	example.cpp
     )
   ```

2. 然后，需要找到通过`MKL-devel`安装的 MKL 库。我们准备了一个名为`IntelMKL`的`INTERFACE`库，该库可以用于其他目标，并将为依赖的目标设置包括目录、编译器选项和链接库。根据 Intel 的建议(https://software.intel.com/en-us/articles/intel-mml-link-line-advisor/ )进行设置。首先，设置编译器选项：

   ```
   add_library(IntelMKL INTERFACE)

   target_compile_options(IntelMKL
     INTERFACE
     	$<$<OR:$<CXX_COMPILER_ID:GNU>,$<CXX_COMPILER_ID:AppleClang>>:-m64>
     )
   ```

3. 接下来，查找`mkl.h`头文件，并为`IntelMKL`目标设置`include`目录：

   ```
   find_path(_mkl_h
     NAMES
     	mkl.h
     HINTS
     	${CMAKE_INSTALL_PREFIX}/include
     )

   target_include_directories(IntelMKL
     INTERFACE
     	${_mkl_h}
     )

   message(STATUS "MKL header file FOUND: ${_mkl_h}")
   ```

4. 最后，为`IntelMKL`目标设置链接库:

   ```
   find_library(_mkl_libs
     NAMES
       mkl_rt
     HINTS
       ${CMAKE_INSTALL_PREFIX}/lib
     )
   message(STATUS "MKL single dynamic library FOUND: ${_mkl_libs}")

   find_package(Threads QUIET)
   target_link_libraries(IntelMKL
     INTERFACE
       ${_mkl_libs}
       $<$<OR:$<CXX_COMPILER_ID:GNU>,$<CXX_COMPILER_ID:AppleClang>>:Threads::Threads>
       $<$<OR:$<CXX_COMPILER_ID:GNU>,$<CXX_COMPILER_ID:AppleClang>>:m>
     )
   ```

5. 使用`cmake_print_properties`函数，打印`IntelMKL`目标的信息：

   ```
   include(CMakePrintHelpers)
   cmake_print_properties(
     TARGETS
     	IntelMKL
     PROPERTIES
       INTERFACE_COMPILE_OPTIONS
       INTERFACE_INCLUDE_DIRECTORIES
       INTERFACE_LINK_LIBRARIES
     )
   ```

6. 将这些库连接到`dgem-example`:

   ```
   target_link_libraries(dgemm-example
     PRIVATE
     	IntelMKL
     )
   ```

7. `CMakeLists.txt`中定义了安装目标:

   ```
   install(
     TARGETS
     	dgemm-example
     DESTINATION
     	bin
     )
   ```

8. 尝试构建包：

   ```
   $ conda build conda-recipe
   ```

9. 过程中屏幕上将看到大量输出，但是一旦构建完成，就可以对包进行安装包。首先，在本地进行安装测试：

   ```
   $ conda install --use-local conda-example-dgemm
   ```

10. 现在测试安装，打开一个新的终端(假设 Anaconda 处于激活状态)，并输入：

    ```
    $ dgemm-example

    MKL DGEMM example worked!
    ```

11. 安装成功之后，再进行卸载：

    ```
    $ conda remove conda-example-dgemm
    ```

## 工作原理

`meta.yaml`中的变化就是`mml-devel`依赖项。从 CMake 的角度来看，这里的挑战是定位 Anaconda 安装的 MKL 库。幸运的是，我们知道它位于`${CMAKE_INSTALL_PREFIX}`中。可以使用在线的`Intel MKL link line advisor`(https://software.intel.com/en-us/articles/intel-mml-link-line-advisor/) 查看如何根据选择的平台和编译器，将 MKL 链接到我们的项目中，我们会将此信息封装到`INTERFACE`库中。这个解决方案非常适合类 MKL 的情况：库不是由我们的项目或任何子项目创建的目标，但是它仍然需要以一种方式进行处理；也就是：设置编译器标志，包括目录和链接库。`INTERFACE`库是构建系统中的目标，但不创建任何构建输出(至少不会直接创建)。但由于它们是目标，我们可对它们的属性进行设置。这样与“实际”目标一样，可以安装、导出和导入。

首先，我们用`INTERFACE`属性声明一个名为`IntelMKL`的新库。然后，根据需要设置属性，并使用`INTERFACE`属性在目标上调用适当的 CMake 命令：

- target_compile_options：用于设置`INTERFACE_COMPILE_OPTIONS`。示例中，设置了`-m64`，不过这个标志只有 GNU 和 AppleClange 编译器能够识别。并且，我们使用生成器表达式来实现。
- target_include_directories：用于设置`INTERFACE_INCLUDE_DIRECTORIES`。使用`find_path`，可以在找到系统上的`mkl.h`头文件后设置这些参数。
- target_link_libraries：用于设置`INTERFACE_LINK_LIBRARIES`。我们决定链接动态库`libmkl_rt.so`，并用`find_library`搜索它。GNU 或 AppleClang 编译器还需要将可执行文件链接到线程和数学库。同样，这些情况可以使用生成器表达式优雅地进行处理。

在`IntelMKL`目标上设置的属性后，可以通过`cmake_print_properties`命令将属性进行打印。最后，链接到`IntelMKL`目标，这将设置编译器标志，包括目录和链接库：

```
target_link_libraries(dgemm-example
  PRIVATE
  	IntelMKL
  )
```

## 更多信息

Anaconda 云上包含大量包。使用上述方法，可以为 CMake 项目构建依赖于其他 Conda 包的 Conda 包。这样，就可以探索软件功能的各种可能性，并与他人分享您的软件包!
