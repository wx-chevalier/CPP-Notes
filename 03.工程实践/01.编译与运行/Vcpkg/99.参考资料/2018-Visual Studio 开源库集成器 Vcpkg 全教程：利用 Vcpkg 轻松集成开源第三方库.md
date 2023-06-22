> [原文地址](https://blog.csdn.net/cjmqas/article/details/79282847)

## 1. 为什么要用 Vcpkg

### 1.1. 传统使用开源库的方法

Windows 下开发 C/C++程序，少不了编译开源的第三方库。比如用于网络连接的高性能库 libcurl、用于压缩解压的 zlib 等等。使用这些库开发极大的方便了程序员，使得我们不必重复造轮子。但是使用这些库必须要处理以下问题。

#### 1.1.1. 编译工具

由于这些开源库绝大部分都来源于 Linux 系统，导致其工程文件、编译系统都使用 gnu 系列工具，使得将其移植到 Windows 的 VC 开发环境下一直是难点。尽管最近几年很多开源库都支持了跨平台的 cmake，但是编译过程仍然复杂和多样化。

常见的编译方式有：

| 编译方式        | 特点                                 | 举例           |
| --------------- | ------------------------------------ | -------------- |
| configure、make | 需要 msys 这样的 unix 环境才可以编译 | ffmpeg         |
| 自定义编译工具  | 需要学习特定的编译命令和工具         | openssl、boost |
| cmake           | 相对简单轻松                         | libcurl        |
| VC 工程文件     | 这种最简单，直接拿来即可编译         |                |

#### 1.1.2. 编译类型

当了解了这些还不够，我们还需要考虑预先编译出哪种类型的开源库程序。比如：Debug 还是 Release、动态库还是静态库、MD 还是 MT、32 位还是 64 位。光是这三种组合就有 16 种可能性。如果像 libcurl 这种还要考虑是否引用其他开源库的功能，那么编译类型的组合会更多。管理起来很麻烦。

#### 1.1.3. 工程目录设定

由于多样的编译类型，工程目录也必须仔细设定才能保证自己的软件项目能够正常编译。

### 1.2. Vcpkg 的优点

- 自动下载开源库源代码
- 源码包的缓存管理和版本管理，可以升级版本
- 轻松编译
- 依赖关系检查（比如编译 libcurl，会自动下载 zlib、openssl 进行编译）
- 无缝集成 Visual Studio，不需要设置库文件、头文件的所在目录，自动集成。
- Visual Studio 全平台支持，不仅支持 Debug/Release、x86/x64 编译，还支持 UWP、ARM 平台的编译。

## 2. 获取 Vcpkg

### 2.1. 下载 Vcpkg

Vcpkg 的官方源码站点为：

> https://github.com/microsoft/vcpkg

一般地，你可以使用 git 命令克隆一个当前版本下来，或者直接下载压缩包。

```shell
git clone https://github.com/microsoft/vcpkg
1
```

### 2.2. 编译 Vcpkg

**注意：**

> Vcpkg 大量使用的 psl 脚本，所以官方强烈推荐使用 PowerShell 而不时 CMD 命令行来执行各种操作。尽管在使用的时候兼容 CMD，但是在编译这一步，请使用 PowerShell。

编译很简单，使用 PowerShell 执行 Vcpkg 工程目录下的“bootstrap-vcpkg.bat”命令，即可编译。编译好以后会在同级目录下生成 vcpkg.exe 文件。编译期间，脚本会自动下载 vswhere 组件。

## 3. 使用 Vcpkg

### 3.1. 查看 Vcpkg 支持的开源库列表

执行命令

```shell
.\vcpkg.exe search
1
```

### 3.2. 安装一个开源库

这里的“安装”其实是指下载和编译。

比如我们需要安装常用的 jsoncpp 库，那么执行命令

```shell
.\vcpkg.exe install jsoncpp
1
```

输出：

```shell
The following packages will be built and installed:
    jsoncpp[core]:x86-windows
Starting package 1/1: jsoncpp:x86-windows
Building package jsoncpp[core]:x86-windows...
-- CURRENT_INSTALLED_DIR=H:/Repos/vcpkg/installed/x86-windows
-- DOWNLOADS=H:/Repos/vcpkg/downloads
-- CURRENT_PACKAGES_DIR=H:/Repos/vcpkg/packages/jsoncpp_x86-windows
-- CURRENT_BUILDTREES_DIR=H:/Repos/vcpkg/buildtrees/jsoncpp
-- CURRENT_PORT_DIR=H:/Repos/vcpkg/ports/jsoncpp/.
-- Downloading https://github.com/open-source-parsers/jsoncpp/archive/1.8.1.tar.gz...
-- Downloading https://github.com/open-source-parsers/jsoncpp/archive/1.8.1.tar.gz... OK
-- Testing integrity of downloaded file...
-- Testing integrity of downloaded file... OK
-- Extracting source H:/Repos/vcpkg/downloads/open-source-parsers-jsoncpp-1.8.1.tar.gz
-- Extracting done
-- Configuring x86-windows-rel
-- Configuring x86-windows-rel done
-- Configuring x86-windows-dbg
-- Configuring x86-windows-dbg done
-- Build x86-windows-rel
-- Build x86-windows-rel done
-- Build x86-windows-dbg
-- Build x86-windows-dbg done
-- Performing post-build validation
-- Performing post-build validation done
Building package jsoncpp[core]:x86-windows... done
Installing package jsoncpp[core]:x86-windows...
Installing package jsoncpp[core]:x86-windows... done
Elapsed time for package jsoncpp:x86-windows: 47.81 s

Total elapsed time: 47.81 s

The package jsoncpp:x86-windows provides CMake targets:

    find_package(jsoncpp REQUIRED)
    target_link_libraries(main PRIVATE jsoncpp_lib)
123456789101112131415161718192021222324252627282930313233343536
```

我们大致可以了解到 install 会经历这几个过程：

1. 环境初始化
2. 下载源代码（如果已经在 cache 中，则不下载）
3. 校验文件有效性
4. 解压缩源代码
5. 利用配套工具配置源码工程，在这里是使用的是 cmake（如果是 ffmpeg，则用 msys2）
6. 编译源码。一般会同时编译 Release 和 Debug 版本。
7. 把编译好的文件拷贝到相关目录中去（一般是 installed 目录）

**注意点：**

如果电脑中没有安装 cmake，vcpkg 会自动下载 portable 版本的 cmake。但是由于各种原因，下载的网速很慢，所以建议先自行下载安装 msi 版本的 cmake。最好是下载最新版本的 cmake。

### 3.3. 指定编译某种架构的程序库

如果不指定安装的架构，vcpkg 默认把开源库编译成 x86 的 Windows 版本的库。那 vcpkg 总共支持多少种架构呢？我们可以使用如下命令便知：

```shell
.\vcpkg.exe help triplet
1
```

我们可以看到会列出如下清单：

- arm-uwp
- arm-windows
- arm64-uwp
- arm64-windows
- x64-uwp
- x64-windows-static
- x64-windows
- x86-uwp
- x86-windows-static
- x86-windows

这个清单以后随着版本的迭代还会再增加。vcpkg 不仅支持 x86 架构，还支持 arm 架构。注意：这里的 arm 架构特指类似于 surface 这种运行在 arm 处理器上的 Win10 平台，而并非我们传统意义上的 Linux 或 android 的 ARM 平台。

那如果要安装编译某一个架构的开源库，我们该怎么写呢？我们只需要在需要安装的包后面指定相应的 triplet 即可。例如我们需要编译 64 位版本的 jsoncpp，那么执行如下命令即可。

```shell
.\vcpkg.exe install jsoncpp:x64-windows
1
```

### 3.4. 移除一个已经安装（编译）的开源库

如果移除一个已经安装的开源库，那么执行 remove 指令即可。比如我们要移除 jsoncpp，那么执行命令：

```shell
.\vcpkg.exe remove jsoncpp
1
```

注意：

- 这个时候只是移除了默认的 x86-winodws 版本的文件，如果有其他平台的版本需要移除，需要制定相应的 triplet。
- 移除也只是移除了二进制程序库而已，源码包和解压缩的源码并没有删除。

如果想要一键移除“过时”的包，执行命令：

```shell
.\vcpkg.exe remove --outdated
1
```

### 3.5. 列出已经安装的开源库

执行 list 指令即可，例如：

```shell
.\vcpkg.exe list
1
```

### 3.6. 更新已经安装的开源库

一般有两种更新方式。一个是 update 指令，可以显示可以升级的开源库的列表。另一个是 upgrade 的指令，会重新编译所有需要更新的包。

### 3.7. 导出已经安装的开源库

有的时候，一个项目组中有很多人，不需要每个人都参与编译。一个人编译好所有开源库后到处给别人即可。有的时候也是出于备份的目的，也会导出已经安装的开源库。导出可以执行 export 指令。例如，我要导出 jsoncpp 库，那么执行：

```shell
.\vcpkg.exe export jsoncpp --7zip
1
```

注意，导出时必须指定导出的包格式。vcpkg 支持 5 种导出包格式，有：

| 参数   | 格式                   |
| ------ | ---------------------- |
| –raw   | 以不打包的目录格式导出 |
| –nuget | 以 nuget 包形式导出    |
| –ifw   | 我也不知道这是啥格式   |
| –zip   | 以 zip 压缩包形式导出  |
| –7zip  | 以 7z 压缩包形式导出   |

一般地，导出包的格式为：vcpkg-export-<日期>-<时间>

默认情况下只会导出 x86-windows 的包，如果要导出所有包，那需要制定相应的 triplet。比如，如果同时导出 x86 和 x64 版本的 jsoncpp，那执行命令：

```shell
.\vcpkg.exe export jsoncpp jsoncpp:x64-windows --7zip
1
```

这个命令等价于：

```shell
.\vcpkg.exe export jsoncpp:x86-windows jsoncpp:x64-windows --7zip
1
```

如果要指定输出目录和特定文件名，需使用"–output="参数

### 3.8. 导入备份的开源库

导入比较简单，执行 import 指令即可。例如：

```shell
.\vcpkg.exe import xxx.7z
1
```

## 4. Vcpkg 和 Visual Studio 的集成

### 4.1. 什么是集成？

上面我们已经安装了一些第三方库，那如何使用呢？常规情况下，我们需要设置 include 目录、lib 目录等，会有很多工作量。Vcpkg 提供了一套机制，可以全自动的适配目录，而开发者不需要关心已安装的库的目录在哪里，也不需要设置。这是 Vcpkg 的一大优势。

### 4.2. 集成到全局

“集成到全局”适用于 Visual Studio 开发环境和 msbuild 命令行。执行命令：

```shell
 .\vcpkg integrate install
1
```

当出现“Applied user-wide integration for this vcpkg root.”字样的时候，说明已经集成成功。这时候可以在任意的工程中使用安装好的第三方库。

### 4.3. 移除全局集成

移除全局集成只要执行下列命令即可：

```shell
 .\vcpkg integrate remove
1
```

### 4.4. 集成到工程

上面已经可以集成到全局，为什么还要“集成到工程”呢？因为在大部分情况下，我们不希望集成到全局，毕竟有很多第三方库我们希望自定义处理一下，或者干脆不想集成第三方库。那么集成到工程是最灵活的处理方式。也是工程级项目推荐的处理方式。

“集成到工程”是整个 vcpkg 中最复杂的一项，它需要利用 Visual Studio 中的 nuget 插件来实现。我们接下来一步一步来说。

#### 4.4.1. 生成配置

执行命令

```shell
 .\vcpkg integrate project
1
```

这时候会在“<vcpkg_dir>\scripts\buildsystems”目录下，生成 nuget 配置文件.

其中<vcpkg_dir>是指 vcpkg 实际所在目录。

#### 4.4.2. 基本配置

打开 Visual Studio，点击菜单“工具->NuGet 包管理器->程序包管理器设置”，进入设置界面，点击“程序包源”。

![img](https://imgconvert.csdnimg.cn/aHR0cDovL2ltZy5ibG9nLmNzZG4ubmV0LzIwMTgwMjA3MTc0NzQ4Mjc3?x-oss-process=image/format,png)
点击“加号”增加一个源。修改源的名字为 vcpkg。在“源”的选项中点击右侧的"…"选择 vcpkg 目录下的“scripts\buildsystems”目录，然后点击右侧的“更新按钮”。

点击“确定”，关闭设置对话框。

到此，全局性的设置已经完成，以后不必再重复设置了。

#### 4.4.3. 工程配置

用 Visual Studio 打开一个工程或解决方案。右键点击需要设置的工程，选择“管理 NuGet 程序包”。在右上角的“程序包源”中选择刚刚设置的“vcpkg”。这样在“浏览”选项卡中就可以看到“vcpkg.H.Repos.vcpkg”。点击最右侧的“安装”。这样就可以集成到某个工程了。

![img](https://imgconvert.csdnimg.cn/aHR0cDovL2ltZy5ibG9nLmNzZG4ubmV0LzIwMTgwMjA3MTc0ODE2MDQ4?x-oss-process=image/format,png)

![img](https://imgconvert.csdnimg.cn/aHR0cDovL2ltZy5ibG9nLmNzZG4ubmV0LzIwMTgwMjA3MTc0ODIzMzIw?x-oss-process=image/format,png)

### 4.5. 集成到 CMake

最新的 Visual Studio 2015 和 2017 大力支持 CMake 工程，所以对 cmake 的支持当然不能少。在 cmake 中集成只要在 cmake 文件中加入下面这句话即可。

> -DCMAKE_TOOLCHAIN_FILE=<vcpkg_dir>/scripts/buildsystems/vcpkg.cmake"

其中<vcpkg_dir>是指 vcpkg 实际所在目录。

### 4.6. 集成静态库

Vcpkg 默认编译链接的是动态库，如果要链接静态库，目前还没有简便的方法。需要做如下操作

1. 用文本方式打开 vcxproj 工程文件
2. 在 xml 的段里面增加如下两句话即可

```xml
<VcpkgTriplet>x86-windows-static</VcpkgTriplet>
<VcpkgEnabled>true</VcpkgEnabled>
12
```

在 CMake 中集成静态库，需要额外指令

```shell
cmake .. -DCMAKE_TOOLCHAIN_FILE=.../vcpkg.cmake -DVCPKG_TARGET_TRIPLET=x86-windows-static
1
```

## 5. 使用 Vcpkg 时的注意点

- Vcpkg 仅支持 Visual Studio 2015 update 3 及以上版本（包括 Visual Studio 2017），究其原因，很可能和 c++11 的支持度以及集成原理有关系。
- 目前 Vcpkg 编译静态库，默认只支持 MT 模式。
