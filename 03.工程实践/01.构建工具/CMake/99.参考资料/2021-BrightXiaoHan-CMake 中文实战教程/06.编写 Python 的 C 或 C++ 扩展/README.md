# 编写 Python 的 C++扩展类型

本章节代码的详细解释见 [使用 c/c++编写 python 扩展（三）：自定义 Python 内置类型](https://zhuanlan.zhihu.com/p/106773873)

## 代码运行方法

```bash
conda install eigin
export CMAKE_PREFIX_PATH=${CONDA_PREFIX:-"$(dirname $(which conda))/../"}
python setup.py build_ext --inplace
```

测试代码

```bash
python test_matrix.py
```
