> [原文地址](https://blog.csdn.net/zhanghm1995/article/details/105466372)

# find_package 用法详解

## 1、find_packakge 命令基本介绍

在我们实际开发过程中，经常不可避免会使用到第三方开源库，这些开源库可能是通过`apt-get install`命令自动安装到系统目录中，也可能是由我们自己下载库的源码然后通过编译安装到指令目录下的。

不管哪种方式安装的库文件，如果我们需要自己的项目中使用这些库，首先面临的第一个问题就是如何找到这些库。所谓“找到”这些库，其实是根据我们的需要找到指定版本的库**头文件包含路径、链接库路径**等，从而能够满足我们开发项目的编译链接需要。

在没有 CMake 的时代，这种库查找链接的工作都需要借助`MakeFile`中的各种命令来完成，非常的繁琐，而且不方便移植，到了 CMake 时代，CMake 给我们提供了`find_package()`命令用来查找依赖包，理想情况下，一句 find_package()命令就能把一整个依赖包的头文件包含路径、库路径、库名字、版本号等情况都获取到，后续只管用就好了。但实际使用过程可能会出现这样那样的问题，因此需要我们对`find_package`这个强大的命令有个大概的理解。

本篇文章先从一个 find_package 的例子出发，然后逐渐引出对`find_package`命令搜包过程的介绍，然后深入理解`find_package`的具体操作原理，帮助大家理解和消化。

## 2、一个使用 find_package 命令的例子

为了能够帮助大家理解`find_package`命令的用法，此处首先用`OpenCV`库举例子，示范如何通过`find_pakcage`命令找到`OpenCV`库并配置，从而能够在我们自己的项目中调用 OpenCV 库，实现特定的功能。

下面的代码主要实现了利用`OpenCV`载入一张图片并显示的简单功能：

**注：** 此处假设你已经安装了 OpenCV 库， 并对 OpenCV 有稍许的了解即可。

具体代码项目下载地址：[Github：find_package_learning](https://github.com/zhanghming/programming-learning-examples/tree/master/cmake_learning/learn_cmake_easily)

`opencv_test.cpp`：

```cpp
#include <cstdio>
#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;

int main() {
  Mat image;
  image = imread("../opencv_test.jpg");

  if (!image.data) {
    printf("No image data\n");
    return -1;
  }

  namedWindow("Display Image", CV_WINDOW_AUTOSIZE);
  imshow("Display Image", image);
  waitKey(0);
  return 0;
}

```

`CMakeLists.txt`：

```cmake
cmake_minimum_required(VERSION 2.8)
project(find_package_learning)
find_package(OpenCV 3 REQUIRED)

message(STATUS "OpenCV_DIR = ${OpenCV_DIR}")
message(STATUS "OpenCV_INCLUDE_DIRS = ${OpenCV_INCLUDE_DIRS}")
message(STATUS "OpenCV_LIBS = ${OpenCV_LIBS}")

include_directories(${OPENCV_INCLUDE_DIRS})
add_executable(opencv_test opencv_test.cpp)
target_link_libraries(opencv_test ${OpenCV_LIBS})
1234567891011
```

### 2.1 编译运行

在源码路径打开终端，执行：

```shell
mkdir build
cd build
cmake ..
make -j4
```

在**源码路径下**放置一张图片并命名为`opencv_test.jpg`，运行：

```shell
./opencv_test
```

就可以看到显示的图片了。

### 2.2 编译输出与分析

我的**Ubuntu18.04**系统在`usr/local`路径下安装了`OpencCV3.4.4`，在执行上述`cmake ..`命令时输出为：

```shell
-- The C compiler identification is GNU 7.5.0
-- The CXX compiler identification is GNU 7.5.0
-- Check for working C compiler: /usr/bin/cc
-- Check for working C compiler: /usr/bin/cc -- works
-- Detecting C compiler ABI info
-- Detecting C compiler ABI info - done
-- Detecting C compile features
-- Detecting C compile features - done
-- Check for working CXX compiler: /usr/bin/c++
-- Check for working CXX compiler: /usr/bin/c++ -- works
-- Detecting CXX compiler ABI info
-- Detecting CXX compiler ABI info - done
-- Detecting CXX compile features
-- Detecting CXX compile features - done
-- Found OpenCV: /usr/local (found suitable version "3.4.4", minimum required is "3")
-- OpenCV_DIR = /usr/local/share/OpenCV
-- OpenCV_INCLUDE_DIRS = /usr/local/include;/usr/local/include/opencv
-- OpenCV_LIBS = opencv_calib3d;opencv_core;opencv_dnn;opencv_features2d;opencv_flann;opencv_highgui;opencv_imgcodecs;opencv_imgproc;opencv_ml;opencv_objdetect;opencv_photo;opencv_shape;opencv_stitching;opencv_superres;opencv_video;opencv_videoio;opencv_videostab;opencv_viz;opencv_aruco;opencv_bgsegm;opencv_bioinspired;opencv_ccalib;opencv_cvv;opencv_datasets;opencv_dnn_objdetect;opencv_dpm;opencv_face;opencv_freetype;opencv_fuzzy;opencv_hdf;opencv_hfs;opencv_img_hash;opencv_line_descriptor;opencv_optflow;opencv_phase_unwrapping;opencv_plot;opencv_reg;opencv_rgbd;opencv_saliency;opencv_stereo;opencv_structured_light;opencv_surface_matching;opencv_text;opencv_tracking;opencv_xfeatures2d;opencv_ximgproc;opencv_xobjdetect;opencv_xphoto
-- Configuring done
-- Generating done
-- Build files have been written to: /home/zhanghm/Programming/programming-learning-examples/cmake_learning/learn_cmake_easily/find_package_learning/build

```

重点看下其中`OpenCV_DIR`、`OpenCV_INCLUDE_DIRS`和`OpenCV_LIBS`打印的结果，这是我在`CMakeLists.txt`中用`message`命令输出这三个变量的值的结果。

可以看到在执行`find_package(OpenCV 3 REQUIRED)`命令后，CMake 找到了我们安装的位于`/usr/local`下的 OpenCV 库，并设置了 CMake 变量`OpenCV_DIR`为 OpenCV 库的配置文件所在路径，正是通过载入这个路径下的`OpenCVConfig.cmake`配置文件才能配置好 OpenCV 库，然后在`OpenCVConfig.cmake`配置文件中定义了变量`OpenCV_INCLUDE_DIRS`为 OpenCV 库头文件包含路径，这样我们才能才在代码中使用`#include <opencv2/opencv.hpp>`而不会出现**编译错误**，同时定义了变量`OpenCV_LIBS`为 OpenCV 链接库路径，这样我们才能正确链接到 OpenCV 中的库文件，而不会出现类似**未定义的引用**这样的**链接错误**。

所以整个过程总结为：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200412142256509.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3poYW5naG0xOTk1,size_16,color_FFFFFF,t_70#pic_center)
通过这个例子就可以看出`find_package`本质上就是一个**搜包的命令**，通过一些特定的规则找到`<package_name>Config.cmake`包配置文件，通过执行该配置文件，从而定义了一系列的变量，通过这些变量就可以准确定位到**OpenCV 库的头文件和库文件**，完成编译。

那么关键的问题来了，`find_package`命令是怎么能够定位并载入指定库的配置文件的呢？这就需要梳理一下`find_package`命令的搜包过程。

## 3、find_package 命令搜包过程

首先我们需要明确一点，CMake 本身不提供任何搜索库的便捷方法，所有搜索库并给变量赋值的操作必须由 CMake 代码完成，也就是上述中的`XXXConfig.cmake`以及下面将要提到的`FindXXX.cmake`配置文件。只不过，库的作者通常会提供这两个文件，以方便使用者调用。

### 3.1 find_package 工作模式

`find_package`命令有两种工作模式，这两种工作模式的不同决定了其搜包路径的不同：

- **Module 模式**
  `find_package`命令基础工作模式(Basic Signature)，也是默认工作模式。
- **Config 模式**
  `find_package`命令高级工作模式(Full Signature)。 只有在`find_package()`中指定**CONFIG**、**NO_MODULE**等关键字，或者**Module**模式查找失败后才会进入到**Config**模式。

因此 find_package 工作模式流程图为：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200412150800892.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3poYW5naG0xOTk1,size_16,color_FFFFFF,t_70#pic_center)

### 3.2 Module 模式用法

**Module**模式的参数为：

```shell
find_package(<package> [version] [EXACT] [QUIET] [MODULE]
             [REQUIRED] [[COMPONENTS] [components...]]
             [OPTIONAL_COMPONENTS components...]
             [NO_POLICY_SCOPE])

```

**参数解释：**
**package**：必填参数。需要查找的包名，注意大小写。

**version 和 EXACT**：可选参数，version 指定的是版本，如果指定就必须检查找到的包的版本是否和**version**兼容。如果指定**EXACT**则表示必须完全匹配的版本而不是兼容版本就可以。

**QUIET**：可选参数，表示如果查找失败，不会在屏幕进行输出（但是如果指定了**REQUIRED**字段，则**QUIET**无效，仍然会输出查找失败提示语）。

**MODULE**：可选字段。前面提到说“如果**Module**模式查找失败则回退到**Config**模式进行查找”，但是假如加入了**MODULE**选项，那么就只在**Module**模式查找，如果**Module**模式下查找失败并不切换到**Config**模式查找。

**REQUIRED**：可选字段。表示一定要找到包，找不到的话就立即停掉整个 CMake。而如果不指定**REQUIRED**则 CMake 会继续执行。

**COMPONENTS，components**：可选字段，表示查找的包中必须要找到的组件(components），如果有任何一个找不到就算失败，类似于**REQUIRED**，导致 CMake 停止执行。

#### Module 模式查找顺序

**Module**模式下是要查找到名为`Find<PackageName>.cmake`的配置文件。

Module 模式只有两个查找路径：**CMAKE_MODULE_PATH**和**CMake**安装路径下的**Modules**目录，
**搜包路径依次为：**

```shell
CMAKE_MODULE_PATH
CMAKE_ROOT
```

先在**CMAKE_MODULE_PATH**变量对应的路径中查找。如果路径为空，或者路径中查找失败，则在 CMake 安装目录（即**CMAKE_ROOT**变量）下的**Modules**目录下（通常为`/usr/share/cmake-3.10/Modules`，3.10 是我的 CMake 版本）查找。这两个变量可以在`CMakeLists.txt`文件中打印查看具体内容：

```cmake
message(STATUS "CMAKE_MODULE_PATH = ${CMAKE_MODULE_PATH}")
message(STATUS "CMAKE_ROOT = ${CMAKE_ROOT}")
```

其中**CMAKE_MODULE_PATH**默认为空，可以利用`set`命令赋值。

在安装 CMake 时，CMake 为我们提供了很多开发库的`FindXXX.cmake`模块文件，可以通过命令查询：

```shell
cmake --help-module-list | grep -E ^Find
```

### 3.3 Config 模式用法

**Config**模式的完整命令参数为：

```shell
find_package(<package> [version] [EXACT] [QUIET]
             [REQUIRED] [[COMPONENTS] [components...]]
             [CONFIG|NO_MODULE]
             [NO_POLICY_SCOPE]
             [NAMES name1 [name2 ...]]
             [CONFIGS config1 [config2 ...]]
             [HINTS path1 [path2 ... ]]
             [PATHS path1 [path2 ... ]]
             [PATH_SUFFIXES suffix1 [suffix2 ...]]
             [NO_DEFAULT_PATH]
             [NO_CMAKE_ENVIRONMENT_PATH]
             [NO_CMAKE_PATH]
             [NO_SYSTEM_ENVIRONMENT_PATH]
             [NO_CMAKE_PACKAGE_REGISTRY]
             [NO_CMAKE_BUILDS_PATH] # Deprecated; does nothing.
             [NO_CMAKE_SYSTEM_PATH]
             [NO_CMAKE_SYSTEM_PACKAGE_REGISTRY]
             [CMAKE_FIND_ROOT_PATH_BOTH |
              ONLY_CMAKE_FIND_ROOT_PATH |
              NO_CMAKE_FIND_ROOT_PATH])
```

相比于**Module**模式，**Config**模式的参数更多，也更复杂，但实际在使用过程中我们并不会用到所有参数，大部分参数都是可选的，我们只需要掌握基本的参数用法即可。

其中具体查找库并给`XXX_INCLUDE_DIRS`和`XXX_LIBRARIES`两个变量赋值的操作由 XXXConfig.cmake 模块完成。

两种模式看起来似乎差不多，不过 CMake 默认采取 Module 模式，如果 Module 模式未找到库，才会采取 Config 模式。如果 XXX_DIR 路径下找不到 XXXConfig.cmake 文件，则会找/usr/local/lib/cmake/XXX/中的 XXXConfig.cmake 文件。总之，Config 模式是一个备选策略。通常，库安装时会拷贝一份 XXXConfig.cmake 到系统目录中，因此在没有显式指定搜索路径时也可以顺利找到。

#### Config 模式查找顺序

**Config**模式下是要查找名为`<PackageName>Config.cmake`或`<lower-case-package-name>-config.cmake`的模块文件。

**搜包路径依次为：**
与**Module**模式不同，**Config**模式需要查找的路径非常多，也要匹配很多的可能性，因此有些路径是首先作为**根目录**，然后进行子目录的匹配，我会进行说明。

具体查找顺序为：
1、名为`<PackageName>_DIR`的 CMake 变量或环境变量路径
默认为空。
**这个路径是非根目录路径，需要指定到`<PackageName>Config.cmake`或`<lower-case-package-name>-config.cmake`文件所在目录才能找到。**
2、名为`CMAKE_PREFIX_PATH`、`CMAKE_FRAMEWORK_PATH`、`CMAKE_APPBUNDLE_PATH`的 CMake 变量或**环境变量**路径
**根目录**，默认都为空。
注意如果你电脑中安装了**ROS**并配置好之后，你在终端执行`echo $CMAKE_PREFIX_PATH`会发现 ROS 会将 CMAKE_PREFIX_PATH 这个变量设置为 ROS 中的库的路径，意思是会首先查找 ROS 安装的库，如果恰好你在 ROS 中安装了 OpenCV 库，就会发现首先找到的是 ROS 中的 OpenCV，而不是你自己安装到系统中的 OpenCV。
3、`PATH`环境变量路径
**根目录**，默认为系统环境`PATH`环境变量值。
其实这个路径才是**Config**模式大部分情况下能够查找到安装到系统中各种库的原因。
这个路径的查找规则为：
遍历`PATH`环境变量中的各路径，如果该路径如果以 bin 或 sbin 结尾，则**自动回退到上一级目录**得到根目录。例如我的`PATH`路径包括：

```shell
$ echo $PATH
/home/zhanghm/.local/bin:/usr/local/cuda-10.1/bin:/opt/ros/melodic/bin:/home/zhanghm/.local/bin:/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin:/usr/games:/usr/local/games:/snap/bin
```

在上述指明的是**根目录**路径时，CMake 会首先检查这些**根目录**路径下是否有名为`<PackageName>Config.cmake`或`<lower-case-package-name>-config.cmake`的模块文件，如果没有，CMake 会继续**检查或匹配**这些根目录下的以下路径（`<PackageName>_DIR`路径不是根目录路径）：

```shell
<prefix>/(lib/<arch>|lib|share)/cmake/<name>*/
<prefix>/(lib/<arch>|lib|share)/<name>*/
<prefix>/(lib/<arch>|lib|share)/<name>*/(cmake|CMake)/
```

其中为系统架构名，如 Ubuntu 下一般为：`/usr/lib/x86_64-linux-gnu`，整个`(lib/<arch>|lib|share)`为可选路径，例如 OpenCV 库而言会**检查或匹配**`<prefix>/OpenCV/`、`<prefix>/lib/x86_64-linux-gnu/OpenCV/`、`<prefix>/lib/share/OpenCV/`、`<prefix>/share/OpenCV/`等路径；name 为包名，不区分大小写`<name>*`意思是包名后接一些版本后等字符也是合法的，如`pcl-1.9`也会被找到。

### 3.4 查找指定包建议

上面的查找规则整体看起来好像很复杂，但其实我们在安装库的时候都会自动配置安装到对的位置，一般都不会出现问题。如果我们需要指定特定的库，我们也只需要设置**优先级最高**的几个变量名即可。包括下面两种情况：

1、如果你明确知道想要查找的库`<PackageName>Config.cmake`或`<lower-case-package-name>-config.cmake`文件所在路径，为了能够准确定位到这个包，可以直接设置变量`<PackageName>_DIR`为具体路径，如：

```shell
set(OpenCV_DIR "/home/zhanghm/Softwares/enviroment_config/opencv3_4_4/opencv/build")
```

就可以明确需要查找的 OpenCV 包的路径了。

2、如果你有多个包的配置文件需要查找，可以将这些配置文件都统一放在一个命名为`cmake`的文件夹下，然后设置变量`CMAKE_PREFIX_PATH`变量指向这个`cmake`文件夹路径，**需要注意根据上述的匹配规则，此时每个包的配置文件需要单独放置在命名为包名的文件夹下（文件夹名不区分大小写）**，否则会提示找不到。

## 4、总结

通过前面的描述，我相信大家已经能够基本掌握`find_package`命令的各种用法了，也能够在出现各种问题时自己进行问题定位。但还有一个我们需要注意的点是我们能够在自己的项目中使用`find_package`命令便捷进行依赖包配置的前提是这个包的开发者也是用 CMake 配置好了这个包，并提供了`<PackageName>Config.cmake`或`Find<PackageName>.cmake`的配置文件。

那如果这个依赖包是我们自己开发的，应该如何配置这个包生成这些配置文件，从而能够被其他项目利用`find_package`命令来配置我们自定义的包呢？
