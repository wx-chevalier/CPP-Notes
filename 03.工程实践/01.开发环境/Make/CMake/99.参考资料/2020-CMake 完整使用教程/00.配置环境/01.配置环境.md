> [原文地址](https://www.ljjyy.com/archives/2021/03/100651.html)

# 配置环境

学习 CMake 之前，需要对系统进行设置，这样才能运行所有示例。

本章的主要内容有：

- 如何获取代码
- 如何在 GNU/Linux、macOS 和 Windows 上安装运行示例所需的所有工具
- 自动化测试如何工作
- 如何报告问题，并提出改进建议

我们会尽可能让初学者看懂本书的内容。不过，这本书并非完全适合零基础人士。我们假设，您对构建目标平台上可用的软件，及本地工具有基本的了解。有 Git 版本控制的经验，可与源码库进行“互动”(不是必需)。

# 0.1 获取代码

本书的源代码可以在 GitHub 上找到，网址是 https://github.com/dev-cafe/cmake-cookbook 。开源代码遵循 MIT 许可：只要原始版权和许可声明包含在软件/源代码的任何副本中，可以以任何方式重用和重新混合代码。许可的全文可以在 https://opensource.org/licenses/MIT 中看到。

为了测试源码，需要使用 Git 获取代码：

- 主要的 GNU/Linux 发行版都可以通过包管理器安装 Git。也可以从 Git 项目网站 [https://git-scm.com](https://git-scm.com/) 下载二进制发行版，进行安装。
- MacOS 上，可以使用自制或 MacPorts 安装 Git。
- Windows 上，可以从 Git 项目网站( [https://git-scm.com](https://git-scm.com/) )下载 Git 可执行安装文件。

可以通过 GitHub 桌面客户端访问这些示例，网址为 [https://desktop.github.com](https://desktop.github.com/) 。

另一种选择是从 https://github.com/dev-cafe/cmake-cookbook 下载 zip 文件。

安装 Git 后，可以将远程库克隆到本地计算机，如下所示：

```
$ git clone https://github.com/dev-cafe/cmake-cookbook.git
```

这将创建一个名为`cmake-cookbook`的文件夹。本书内容与源码的章节对应，书中章节的编号和源码的顺序相同。

在 GNU/Linux、MacOS 和 Windows 上，使用最新的持续集成进行测试。我们会在之后讨论测试的设置。

我们用标签 v1.0 标记了与本书中打印的示例相对应的版本。为了与书中内容对应，可以如下获取此特定版本：

```
$ git clone --single-branch -b v1.0 https://github.com/dev-cafe/cmake-cookbook.git
```

我们希望收到 Bug 修复，并且 GitHub 库将继续发展。要获取更新，可以选择库的 master 分支。

# 0.2 Docker 镜像

在 Docker 中进行环境搭建，无疑是非常方便的(依赖项都已经安装好了)。我们的 Docker 镜像是基于 Ubuntu 18.04 的镜像制作，您可以按照官方文档[https://docs.docker.com](https://docs.docker.com/) 在您的操作系统上安装 Docker。

Docker 安装好后，您可以下载并运行我们的镜像，然后可以对本书示例进行测试:

```
$ docker run -it devcafe/cmake-cookbook_ubuntu-18.04
$ git clone https://github.com/dev-cafe/cmake-cookbook.git
$ cd cmake-cookbook
$ pipenv install --three
$ pipenv run python testing/collect_tests.py 'chapter-*/recipe-*'
```

# 0.3 安装必要的软件

与在 Docker 中使用不同，另一种选择是直接在主机操作系统上安装依赖项。为此，我们概括了一个工具栈，可以作为示例的基础。您必须安装以下组件：

1. CMake
2. 编译器
3. 自动化构建工具
4. Python

我们还会详细介绍，如何安装所需的某些依赖项。

## 0.3.1 获取 CMake

本书要使用的 CMake 最低需要为 3.5。只有少数示例，演示了 3.5 版之后引入的新功能。每个示例都有提示，指出示例代码在哪里可用，以及所需的 CMake 的最低版本。提示信息如下:

**NOTE**:_这个示例的代码可以在 codes/chapter-03/recipe10 中找到，其中包括一个 C 示例。该示例在 CMake 3.5 版(或更高版本)中是有效的，并且已经在 GNU/Linux、macOS 和 Windows 上进行了测试。_

有些(如果不是大多数)示例仍然适用于较低版本的 CMake。但是，我们没有测试过这个。我们认为 CMake 3.5 是大多数系统和发行版的默认软件，而且升级 CMake 也没什么难度。

CMake 可以以多种方式安装。下载并提取由 Kitware 维护的二进制发行版，可以在所有平台上运行，下载页面位于 https://cmake.org/download/ 。

大多数 GNU/Linux 发行版都在包管理器中提供了 CMake。然而，在一些发行版中，版本可能比较旧，因此下载由 Kitware 提供的二进制文件当然是首选。下面的命令将从 CMake 打包的版本中下载并安装在`$HOME/Deps/CMake`(根据您的偏好调整此路径)下的 CMake 3.5.2：

```
$ cmake_version="3.5.2"
$ target_path=$HOME/Deps/cmake/${cmake_version}
$ cmake_url="https://cmake.org/files/v${cmake_version%.*}/cmake-${cmake_version}-Linux-x86_64.tar.gz"
$ mkdir -p "${target_path}"
$ curl -Ls "${cmake_url}" | tar -xz -C "${target_path}" --strip-components=1
$ export PATH=$HOME/Deps/cmake/${cmake_version}/bin${PATH:+:$PATH}
$ cmake --version
```

macOS 获取最新版本的 CMake：

```
$ brew upgrade cmake
```

Windows 上，可以使用 Visual Studio 2017，它提供了 CMake 支持。Visual Studio 2017 的安装记录在第 13 章，_可选生成器和交叉编译_，示例技巧 1，_使用 Visual Studio 2017 构建 CMake 项目_。

或者，可以从 [https://www.msys2.org](https://www.msys2.org/) 下载 MSYS2 安装程序，按照其中给出的说明更新包列表，然后使用包管理器`pacman`安装 CMake。下面的代码正在构建 64 位版本：

```
$ pacman -S mingw64/mingw-w64-x86_64-cmake
```

对于 32 位版本，请使用以下代码(为了简单起见，我们以后只会提到 64 位版本)：

```
$ pacman -S mingw64/mingw-w64-i686-cmake
```

MSYS2 的另一个特性是在 Windows 上提供了一个终端，比较像 Unix 操作系统上的终端，提供可用的开发环境。

## 0.3.2 编译器

我们将需要 C++、C 和 Fortran 的编译器。编译器的版本需要比较新，因为我们需要在大多数示例中支持最新的语言标准。CMake 为来自商业和非商业供应商的许多编译器，提供了非常好的支持。为了让示例始终能够跨平台，并尽可能独立于操作系统，我们使用了开源编译器:

- GNU/Linux 上，GNU 编译器集合(GCC)是直接的选择。它是免费的，适用于所有发行版。例如，在 Ubuntu 上，可以安装以下编译器：

  ```
  $ sudo apt-get install g++ gcc gfortran
  ```

- 在 LLVM 家族中，Clang 也是 C++和 C 编译器的一个很好的选择：

  ```
  $ sudo apt-get install clang clang++ gfortran
  ```

- macOS 上，XCode 附带的 LLVM 编译器适用于 C++和 C。我们在 macOS 测试中使用了 GCC 的 Fortran 编译器。GCC 编译器必须使用包管理器单独安装：

  ```
  $ brew install gcc
  ```

- Windows 上，可以使用 Visual Studio 测试 C++和 C 示例。或者，可以使用 MSYS2 安装程序，MSYS2 环境中(对于 64 位版本)使用以下单个命令安装整个工具链，包括 C++、C 和 Fortran 编译器：

  ```
  $ pacman -S mingw64/mingw-w64-x86_64-toolchain
  ```

## 0.3.3 自动化构建工具

自动化构建工具为示例中的项目提供构建和链接的基础设施，最终会安装和使用什么，很大程度上取决于操作系统：

- GNU/Linux 上，GNU Make(很可能)在安装编译器时自动安装。
- macOS 上，XCode 将提供 GNU Make。
- Windows 上，Visual Studio 提供了完整的基础设施。MSYS2 环境中，GNU Make 作为 mingw64/mingw-w64-x86_64 工具链包的一部分，进行安装。

为了获得最大的可移植性，我们尽可能使示例不受这些系统相关细节的影响。这种方法的优点是配置、构建和链接，是每个编译器的*固有特性*。

Ninja 是一个不错的自动化构建工具，适用于 GNU/Linux、macOS 和 Windows。Ninja 注重速度，特别是增量重构。为 GNU/Linux、macOS 和 Windows 预先打包的二进制文件可以在 GitHub 库中找到，网址是 https://github.com/ninja-build/ninja/releases 。

Fortran 项目中使用 CMake 和 Ninja 需要注意。使用 CMake 3.7.2 或更高版本是必要的，Kitware 还有维护 Ninja，相关包可以在 https://github.com/Kitware/ninja/releases 上找到。

在 GNU/Linux 上，可以使用以下一系列命令安装 Ninja：

```
$ mkdir -p ninja
$ ninja_url="https://github.com/Kitware/ninja/releases/download/v1.8.2.g3bbbe.kitware.dyndep-1.jobserver-1/ninja-1.8.2.g3bbbe.kitware.dyndep-1.jobserver-1_x86_64-linux-gnu.tar.gz"
$ curl -Ls ${ninja_url} | tar -xz -C ninja --strip-components=1
$ export PATH=$HOME/Deps/ninja${PATH:+:$PATH}
```

Windows 上，使用 MSYS2 环境(假设是 64 位版本)执行以下命令：

```
$ pacman -S mingw64/mingw-w64-x86_64-ninja
```

**NOTE**:_我们建议阅读这篇文章 http://www.aosabook.org/en/posa/ninja.html ，里面是对 NInja 编译器的历史和设计的选择，进行启发性的讨论。_

## 0.3.4 Python

本书主要关于 CMake，但是其中的一些方法，需要使用 Python。因此，也需要对 Python 进行安装：解释器、头文件和库。Python 2.7 的生命周期结束于 2020 年，因此我们将使用 Python 3.5。

在 Ubuntu 14.04 LTS 上(这是 Travis CI 使用的环境，我们后面会讨论)，Python 3.5 可以安装如下：

```
sudo apt-get install python3.5-dev
```

Windows 可使用 MSYS2 环境，Python 安装方法如下(假设是 64 位版本):

```
$ pacman -S mingw64/mingw-w64-x86_64-python3
$ pacman -S mingw64/mingw-w64-x86_64-python3-pip
$ python3 -m pip install pipenv
```

为了运行已经写好的测试机制，还需要一些特定的 Python 模块。可以使用包管理器在系统范围内安装这些包，也可以在隔离的环境中安装。建议采用后一种方法：

- 可以在不影响系统环境的情况下，将安装包进行清理/安装。
- 可以在没有管理员权限的情况下安装包。
- 可以降低软件版本和依赖项冲突的风险。
- 为了复现性，可以更好地控制包的依赖性。

为此，我们准备了一个`Pipfile`。结合`pipfile.lock`，可以使用`Pipenv`( [http://pipenv.readthedocs](http://pipenv.readthedocs/) )。创建一个独立的环境，并安装所有包。要为示例库创建此环境，可在库的顶层目录中运行以下命令：

```
$ pip install --user pip pipenv --upgrade
$ pipenv install --python python3.5
```

执行`pipenv shell`命令会进入一个命令行环境，其中包含特定版本的 Python 和可用的包。执行`exit`将退出当前环境。当然，还可以使用`pipenv run`在隔离的环境中直接执行命令。

或者，可以将库中的`requirements.txt`文件与`Virtualenv`( http://docs.pythonguide.org/en/latest/dev/virtualenvs/ )和`pip`结合使用，以达到相同的效果：

```
$ virtualenv --python=python3.5 venv
$ source venv/bin/activate
$ pip install -r requirements.txt
```

可以使用`deactivate`命令退出虚拟环境。

另一种选择是使用`Conda`环境，我们建议安装`Miniconda`。将把最新的`Miniconda`安装到 GNU/Linux 的`$HOME/Deps/conda`目录(从 https://repo.continuum.io/miniconda/miniconda3-latestlinux-x86_64.sh 下载)或 macOS(从 https://repo.continuum.io/miniconda/miniconda3-latestmacosx-x86_64.sh 下载)：

```
$ curl -Ls https://repo.continuum.io/miniconda/Miniconda3-latest-Linux-x86_64.sh > miniconda.sh
$ bash miniconda.sh -b -p "$HOME"/Deps/conda &> /dev/null
$ touch "$HOME"/Deps/conda/conda-meta/pinned
$ export PATH=$HOME/Deps/conda/bin${PATH:+:$PATH}
$ conda config --set show_channel_urls True
$ conda config --set changeps1 no
$ conda update --all
$ conda clean -tipy
```

Windows 上，可以从 https://repo.continuum.io/miniconda/Miniconda3-latest-Windows-x86_64.exe 下载最新的`Miniconda`。该软件包可以使用`PowerShell`安装，如下:

```
$basedir = $pwd.Path + "\"
$filepath = $basedir + "Miniconda3-latest-Windows-x86_64.exe"
$Anaconda_loc = "C:\Deps\conda"
$args = "/InstallationType=JustMe /AddToPath=0 /RegisterPython=0 /S /D=$Anaconda_loc"
Start-Process -FilePath $filepath -ArgumentList $args -Wait -Passthru
$conda_path = $Anaconda_loc + "\Scripts\conda.exe"
$args = "config --set show_channel_urls True"
Start-Process -FilePath "$conda_path" -ArgumentList $args -Wait -Passthru
$args = "config --set changeps1 no"
Start-Process -FilePath "$conda_path" -ArgumentList $args -Wait -Passthru
$args = "update --all"
Start-Process -FilePath "$conda_path" -ArgumentList $args -Wait -Passthru
$args = "clean -tipy"
Start-Process -FilePath "$conda_path" -ArgumentList $args -Wait -Passthru
```

安装了`Conda`后, Python 模块可以按如下方式安装:

```
$ conda create -n cmake-cookbook python=3.5
$ conda activate cmake-cookbook
$ conda install --file requirements.txt
```

执行`conda deactivate`将退出`conda`的环境。

## 0.3.5 依赖软件

有些示例需要额外的依赖，这些软件将在这里介绍。

### 0.3.5.1 BLAS 和 LAPACK

大多数 Linux 发行版都为 BLAS 和 LAPACK 提供包。例如，在 Ubuntu 14.04 LTS 上，您可以运行以下命令：

```
$ sudo apt-get install libatlas-dev liblapack-dev liblapacke-dev
```

macOS 上，XCode 附带的加速库可以满足我们的需要。

Windows 使用 MSYS2 环境，可以按如下方式安装这些库(假设是 64 位版本)：

```
$ pacman -S mingw64/mingw-w64-x86_64-openblas
```

或者，可以从 GitHub ( https://github.com/referlapack/lapack )下载 BLAS 和 LAPACK 的参考实现，并从源代码编译库。商业供应商为平台提供安装程序，安装包中有 BLAS 和 LAPACK 相关的 API。

### 0.3.5.2 消息传递接口(MPI)

MPI 有许多商业和非商业实现。这里，安装免费的非商业实现就足够了。在 Ubuntu 14.04 LTS 上，我们推荐`OpenMPI`。可使用以下命令安装：

```
$ sudo apt-get install openmpi-bin libopenmpi-dev
```

在 macOS 上，`Homebrew`发布了`MPICH`：

```
$ brew install mpich
```

还可以从 https://www.open-mpi.org/software/ 上获取源代码，编译`OpenMPI`。对于 Windows，Microsoft MPI 可以通过 https://msdn.microsoft.com/en-us/library/bb524831(v=vs.85).aspx 下载安装。

### 0.3.5.3 线性代数模板库

一些示例需要线性代数模板库，版本为 3.3 或更高。如果包管理器不提供`Eigen`，可以使用在线打包源([http://eigen.tuxfamily.org](http://eigen.tuxfamily.org/) )安装它。例如，在 GNU/Linux 和 macOS 上，可以将`Eigen`安装到`$HOME/Deps/Eigen`目录:

```
$ eigen_version="3.3.4"
$ mkdir -p eigen
$ curl -Ls http://bitbucket.org/eigen/eigen/get/${eigen_version}.tar.gz | tar -xz -C eigen --strip-components=1
$ cd eigen
$ cmake -H. -Bbuild_eigen -
DCMAKE_INSTALL_PREFIX="$HOME/Deps/eigen" &> /dev/null
$ cmake --build build_eigen -- install &> /dev/null
```

### 0.3.5.4 Boost 库

`Boost`库适用于各种操作系统，大多数 Linux 发行版都通过它们的包管理器提供该库的安装。例如，在 Ubuntu 14.04 LTS 上，`Boost`文件系统库、`Boost Python`库和`Boost`测试库可以通过以下命令安装：

```
$ sudo apt-get install libboost-filesystem-dev libboost-python-dev libboost-test-dev
```

对于 macOS, `MacPorts`和自制程序都为最新版本的`Boost`提供了安装包。我们在 macOS 上的测试设置安装`Boost`如下：

```
$ brew cask uninstall --force oclint
$ brew uninstall --force --ignore-dependencies boost
$ brew install boost
$ brew install boost-python3
```

Windows 的二进制发行版也可以从`Boost`网站 [http://www.boost.org](http://www.boost.org/) 下载。或者，可以从 [https://www.boost.org](https://www.boost.org/) 下载源代码，并自己编译`Boost`库。

### 0.3.5.5 交叉编译器

在类 Debian/Ubuntu 系统上，可以使用以下命令安装交叉编译器：

```
$ sudo apt-get install gcc-mingw-w64 g++-mingw-w64 gfortran-mingw-w64
```

在 macOS 上，使用`Brew`，可以安装以下交叉编译器：

```
$ brew install mingw-w64
```

其他包管理器提供相应的包。使用打包的跨编译器的另一种方法，是使用 M 交叉环境( [https://mxe.cc](https://mxe.cc/) )，并从源代码对其进行构建。

### 0.3.5.6 ZeroMQ, pkg-config, UUID 和 Doxygen

Ubuntu 14.04 LTS 上，这些包可以安装如下：

```
$ sudo apt-get install pkg-config libzmq3-dev doxygen graphviz-dev uuid-dev
```

macOS 上，我们建议使用`Brew`安装：

```
$ brew install ossp-uuid pkg-config zeromq doxygen
```

`pkg-config`程序和`UUID`库只在类 Unix 系统上可用。Windows 上使用 MSYS2 环境，可以按如下方式安装这些依赖项(假设是 64 位版本)：

```
$ pacman -S mingw64/mingw-w64-x86_64-zeromq
$ pacman -S mingw64/mingw-w64-x86_64-pkg-config
$ pacman -S mingw64/mingw-w64-x86_64-doxygen
$ pacman -S mingw64/mingw-w64-x86_64-graphviz
```

### 0.3.5.7 Conda 的构建和部署

想要使用`Conda`打包的示例的话，需要`Miniconda`和`Conda`构建和部署工具。`Miniconda`的安装说明之前已经给出。要在 GNU/Linux 和 macOS 上安装`Conda`构建和部署工具，请运行以下命令:

```
$ conda install --yes --quiet conda-build anaconda-client jinja2 setuptools
$ conda clean -tipsy
$ conda info -a
```

这些工具也可以安装在 Windows 上:

```
$conda_path = "C:\Deps\conda\Scripts\conda.exe"
$args = "install --yes --quiet conda-build anaconda-client jinja2 setuptools"
Start-Process -FilePath "$conda_path" -ArgumentList $args -Wait -Passthru
$args = "clean -tipsy"
Start-Process -FilePath "$conda_path" -ArgumentList $args -Wait -Passthru
$args = "info -a"
Start-Process -FilePath "$conda_path" -ArgumentList $args -Wait -Passthru
```

# 0.4 测试环境

示例在下列持续集成(CI)上进行过测试：

- Travis( [https://travis-ci.org](https://travis-ci.org/) )用于 GNU/Linux 和 macOS
- Appveyor( [https://www.appveyor.com](https://www.appveyor.com/) )用于 Windows
- CircleCI ( [https://circleci.com](https://circleci.com/) )用于附加的 GNU/Linux 测试和商业编译器

CI 服务的配置文件可以在示例库中找到( https://github.com/dev-cafe/cmake-cookbook/ ):

- Travis 的配置文件为`travis.yml`
- Appveyor 的配置文件为`.appveyor.yml`
- CircleCI 的配置文件为`.circleci/config.yml`
- Travis 和 Appveyor 的其他安装脚本，可以在`testing/dependencies`文件夹中找到。

**NOTE**:_GNU/Linux 系统上，Travis 使用 CMake 3.5.2 和 CMake 3.12.1 对实例进行测试。macOS 系统上用 CMake 3.12.1 进行测试。Appveyor 使用 CMake 3.11.3 进行测试。Circle 使用 CMake 3.12.1 进行测试。_

测试机制是一组 Python 脚本，包含在`testing`文件夹中。脚本`collect_tests.py`将运行测试并报告它们的状态。示例也可以单独测试，也可以批量测试；`collect_tests.py`接受正则表达式作为命令行输入，例如:

```
$ pipenv run python testing/collect_tests.py 'chapter-0[1,7]/recipe-0[1,2,5]'
```

该命令将对第 1 章和第 7 章的示例 1、2 和 5 进行测试。

要获得更详细的输出，可以设置环境变量`VERBOSE_OUTPUT=ON`：

```
$ env VERBOSE_OUTPUT=ON pipenv run python testing/collect_tests.py 'chapter-*/recipe-*'
```
