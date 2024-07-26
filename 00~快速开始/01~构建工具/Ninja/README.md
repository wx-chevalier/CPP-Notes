## 简介

ninja 是 `Google` 的一名程序员推出的注重速度的构建工具.一般在 Unix/Linux 上的程序通过 `make/makefile` 来构建编译，而 `ninja` 通过将编译任务并行组织，大大提高了构建速度。

## 执行

```bash
ninja [-options] targets

支持参数
--version  # 打印版本信息
-v         # 显示构建中的所有命令行（这个对实际构建的命令核对非常有用）

-C DIR     # 在执行操作之前，切换到`DIR`目录
-f FILE    # 制定`FILE`为构建输入文件。默认文件为当前目录下的`build.ninja`。如 ./ninja -f demo.ninja

-j N       # 并行执行 N 个作业。默认N=3（需要对应的CPU支持）。如 ./ninja -j 2 all
-k N       # 持续构建直到N个作业失败为止。默认N=1
-l N       # 如果平均负载大于N，不启动新的作业
-n         # 排练（dry run）(不执行命令，视其成功执行。如 ./ninja -n -t clean)

-d MODE    # 开启调试模式 (用 -d list 罗列所有的模式)
-t TOOL    # 执行一个子工具(用 -t list 罗列所有子命令工具)。如 ./ninja -t query all
-w FLAG    # 控制告警级别
```

## 特点

1. 可以通过其他高级的编译系统生成其输入文件；
2. 它的设计就是为了更快的编译；

## 通过其他高级的编译系统生成其输入文件

### Ninja 与 Android

#### 安卓编译系统演进历史

![Ninja1.png](https://p1-juejin.byteimg.com/tos-cn-i-k3u1fbpfcp/ffabb0ee79de467ba7e9746dae4e16cd~tplv-k3u1fbpfcp-zoom-in-crop-mark:4536:0:0:0.awebp?) `Google` 在 `Android 7.0`之前都是使用的`makefile`进行编译，7.0 开始引入了`Soong构建系统`，旨在取代`make`，它利用 `Kati GNU Make 克隆工具`和 `ninja 构建系统组件`来**加速** `Android` 的构建。

##### 生成 ninja 文件

- 工具链关系

```bash
bash
Android.bp --> Blueprint --> Soong --> ninja
Makefile or Android.mk --> kati --> ninja
(Android.mk --> Soong --> Blueprint --> Android.bp)
```

![Ninja2.png](https://p3-juejin.byteimg.com/tos-cn-i-k3u1fbpfcp/371558f5c24d455a923693fb2e456bed~tplv-k3u1fbpfcp-zoom-in-crop-mark:4536:0:0:0.awebp?)

在编译过程中，`Android.bp`会被收集到`out/soong/build.ninja.d`,`blueprint`以此为基础，生成`out/soong/build.ninja` `Android.mk`会由`kati`/`ckati`生成为`out/build-aosp_arm.ninja` 两个 ninja 文件会被整合进入`out/combined-$product_$arch.ninja`

- combined-product_arch.ninja

```bash
bash
$ source build/envsetup.sh
$ lunch pixel3_mainline-userdebug
$ make nothing
$ cat out/combined-pixel3_mainline.ninja

builddir = out
pool highmem_pool
 depth = 2
subninja out/build-pixel3_mainline.ninja
subninja out/build-pixel3_mainline-package.ninja
subninja out/soong/build.ninja
```

- 执行

```bash
bash
prebuilts/build-tools/linux-x86/bin/ninja \
    -f  out/combined-pixel3_mainline.ninja
```

### ninja 本身

ninja 本身就是通过 ninja 编译出来的

#### 源码获取及编译

```bash
bash
git clone https://android.googlesource.com/platform/external/ninja

python3 configure.py --bootstrap
```

#### 编译过程

1. 生成一个 build.ninja
2. 执行 python3 configure.py --bootstrap 之后编译源码,生成一个 a.out
3. 根据这个 build.ninja 重新编译生成可执行文件 ninja
4. 在 ninja 根据 ninja.build 来编译时会自动创建一个 build 目录用于存放编译过程中的临时文件

#### [ninja_syntax.py](https://link.juejin.cn/?target=https%3A%2F%2Fgithub.com%2Fninja-build%2Fninja%2Fblob%2F84986%2Fmisc%2Fninja_syntax.py)

Ninja 提供了一个简单的生成脚本,它实际上是一个 python 模块`misc/ninja_syntax.py`,通过它我们可以较方便的生成`build.ninja`文件

```python
python
from ninja_syntax import Writer

with open("build.ninja", "w") as buildfile:
    n = Writer(buildfile)

    if platform.is_msvc():
        n.rule("link",
                command="$cxx $in $libs /nologo /link $ldflags /out:$out",
                description="LINK $out")
    else:
        n.rule("link",
                command="$cxx $ldflags -o $out $in $libs",
                description="LINK $out")
```

## 为了更快的编译

[启动全过程](https://juejin.cn/post/7121639752591212557/)

### 与[makefile](https://juejin.cn/post/7121636711997636638)的对比

```bash
bash
$ time ninja
ninja  39.24s user 2.16s system 1021% cpu 4.053 total
3.79s

$ time make
make  22.29s user 1.59s system 101% cpu 23.543 total
23.13s
```

### [语法](https://juejin.cn/post/7121638397466132516/)及概念

- **edge**(边):build 语句，可以指定目标(target)输出(output)、规则(rule)与输入(input)
- **target**(目标):编译过程需要生成的目标，由 build 语句指定
- **output**(输出):build 语句的前半段，是 target 的另一种称呼
- **input**(输入):build 语句的后半段，用于产生 output 的文件或目标，另一种称呼是依赖
- **rule**(规则):通过指定 command 与一些内置变量，决定如何从输入产生输出
- **pool**(池)：一组 rule 或 edge，通过指定其 depth，可以控制并行上限
- **scope**(作用域)：变量的作用范围，有 rule 与 build 语句的块级，也有文件级别。

```makefile
Gcc = gcc # 全局变量

# rule
rule name # name是rule名
    command = ${Gcc} ${in} > ${out}  # 执行命令
    var = str  # 局部变量
# Edge
# output0 output1 显示输出
# output2 output3 隐式输出
# rule_name 规则名称
build output0 output1 | output2 output3: rule_name $
        input0 input1 $      # 显示依赖
        | input2 input3 $    # 隐式依赖
        || input4 input5     # order-only依赖 可有可无
    var0 = str0
    var1 = str1
```

![Ninja3.jpg](https://p1-juejin.byteimg.com/tos-cn-i-k3u1fbpfcp/a148933ab967454f826142cd734d6107~tplv-k3u1fbpfcp-zoom-in-crop-mark:4536:0:0:0.awebp?)

这张图中`src/browse.py` `src/inline.sh`就是`input`也就是`依赖`,`inline`就是`rule`,`build/browse_py.h`就是`output`也就是`目标(target)`,上述组合起来的就是一个`edge`
而在其他的`edge`中,`build/browse_py.h`又会作为`input`
再比如`src/util.h`会同时作为`build/build.o`和`build/log.o`的`input`
整个图就是一个`scope`

### 底层的数据结构

![Ninja3.jpg](https://p1-juejin.byteimg.com/tos-cn-i-k3u1fbpfcp/a148933ab967454f826142cd734d6107~tplv-k3u1fbpfcp-zoom-in-crop-mark:4536:0:0:0.awebp?)

回到这张图,我们来看 Ninja 的底层的如何处理的(以下数据结构只保留到最简的部分)

#### State

`State`保存单次运行的全局状态

```cpp
struct State {
  //内置pool和Rule使用这个虚拟的内置scope来初始化它们的关系位置字段。这个范围内没有任何东西。
  static Scope kBuiltinScope;
  static Pool kDefaultPool;
  static Pool kConsolePool;
  static Rule kPhonyRule;

  // 内置的hashmap 保存所有的Node
  typedef ConcurrentHashMap<HashedStrView, Node*> Paths;
  Paths paths_;
  // 保存所有的Pool
  std::unordered_map<HashedStrView, Pool*> pools_;
  // 保存所有的edge
  vector<Edge*> edges_;
  // 根作用域
  Scope root_scope_ { ScopePosition {} };
  vector<Node*> defaults_;  // 默认目标

private:
  /// Position 0 is used for built-in decls (e.g. pools).
  DeclIndex dfs_location_ = 1;
};
```

#### Scope

`Scope`作用域：变量的作用范围，有 rule 与 build 语句的块级，也有文件级别。包含 Rule，同时保存了父 Scope 的位置

```cpp
struct Scope {
  Scope(ScopePosition parent) : parent_(parent) {}


private:

  ScopePosition parent_; // 父位置

  DeclIndex pos_ = 0; // 自己的哈希位置
  // 变量
  std::unordered_map<HashedStrView, std::vector<Binding*>> bindings_;
  // Rule
  std::unordered_map<HashedStrView, Rule*> rules_;
};
```

#### Rule

`Rule`文件的构建规则，存在局部变量

```cpp
struct Rule {
  Rule() {}

  struct {
    // 该规则在其源文件中的位置。
    size_t rule_name_diag_pos = 0;
  } parse_state_;

  RelativePosition pos_;  // 偏移值
  HashedStr name_;  // 规则名
  std::vector<std::pair<HashedStr, std::string>> bindings_;//保存局部变量
};
```

#### Binding & DefaultTarget

`Binding`以键值对的形式存在用来变量 `DefaultTarget` 保存默认的输出的 target

```cpp
struct Binding {

  RelativePosition pos_;  // 偏移位置
  HashedStr name_;  //变量名
  StringPiece parsed_value_; // 变量值
};

struct DefaultTarget {
  RelativePosition pos_; // 偏移值
  LexedPath parsed_path_; // StringPiece
  size_t diag_pos_ = 0;
};
```

#### Node

```
Node`是最边界的数据结构,ninja语法中的`input`,`output`,`target`,`default`的底层保存都是`Node
cpp
struct Node {
  Node(const HashedStrView& path, uint64_t initial_slash_bits)
      : path_(path),
        first_reference_({ kLastDeclIndex, initial_slash_bits }) {}
  ~Node();
private:
  // 路径值
  const HashedStr path_;
  std::atomic<NodeFirstReference> first_reference_;
  // 作为output所在的Edge位置
  Edge* in_edge_ = nullptr;
  // 使用此Node作为输入的所有Edge.列表顺序不确定,每次访问都是对其重新排序
  struct EdgeList {
    EdgeList(Edge* edge=nullptr, EdgeList* next=nullptr)
        : edge(edge), next(next) {}
    Edge* edge = nullptr;
    EdgeList* next = nullptr;
  };

  std::atomic<EdgeList*> out_edges_ { nullptr };
  std::atomic<EdgeList*> validation_out_edges_ { nullptr };
  std::vector<Edge*> dep_scan_out_edges_;
};
```

#### Edge

`Edge`是最核心的数据结构,会将`Node` `Rule` `Binding`等数据结构组合起来

```cpp
struct Edge {

  // 固定的属性值 在Rule下进行配置
  struct DepScanInfo {
    bool valid = false;
    bool restat = false;
    bool generator = false;
    bool deps = false;
    bool depfile = false;
    bool phony_output = false;
    uint64_t command_hash = 0;
  };

public:
  struct {
    StringPiece rule_name;  // 保存rule_name
    size_t rule_name_diag_pos = 0;
    size_t final_diag_pos = 0;
  } parse_state_;

  const Rule* rule_ = nullptr; // 使用的rule
  Pool* pool_ = nullptr; // 所在的pool
  // 在一个edge中的input，output
  vector<Node*> inputs_;
  vector<Node*> outputs_;

  std::vector<std::pair<HashedStr, std::string>> unevaled_bindings_; // 存储局部变量值

  int explicit_deps_ = 0;  // 显式输入
  int implicit_deps_ = 0;  // 隐式输入
  int order_only_deps_ = 0;  // 隐式order-only依赖

  int explicit_outs_ = 0;  // 显示输出
  int implicit_outs_ = 0; // 隐式输出

};
```

如何区分显隐式,input 和 output 会按照按照 显式 -> 隐式 -> order-only(仅依赖) 的顺序进行 push_back()
根据当前的值的位置与显隐式的数量做对比就可以知道

```cpp
edge->outputs_.reserve(edge->explicit_outs_ + edge->implicit_outs_);
edge->inputs_.reserve(edge->explicit_deps_ + edge->implicit_deps_ +
                      edge->order_only_deps_);
```

### 启动过程

#### 入口函数

```cpp
ninja.cc main() -> real_mian()
```

#### 1 处理参数

- **-f** 选择文件
- **-C** 工作路径
- **-t** 选择内置工具

```cpp
NORETURN void real_main(int argc, char** argv) {
  BuildConfig config;
  Options options = {};
  options.input_file = "build.ninja";
  options.dupe_edges_should_err = true;
  // 处理参数
  int exit_code = ReadFlags(&argc, &argv, &options, &config);  // return 1 exit
    ...
}
cpp
struct Options {
  // 文件 -f
  const char* input_file;
  // 工作路径 -C
  const char* working_dir;
  // 工具 -t
  const Tool* tool;
  // 针对一个目标的重复规则是否应该发出警告或打印错误
  bool dupe_edges_should_err;
  // 假周期是否应该警告或打印一个错误。
  bool phony_cycle_should_err;
  // 在不同的行上有多个目标的删除文件是否应该警告或打印错误。
  bool depfile_distinct_target_lines_should_err;
  // 是否保持持久
  bool persistent;
};
```

#### 2 读取 ninja 文件并构建图

![Ninja4.jpg](https://p1-juejin.byteimg.com/tos-cn-i-k3u1fbpfcp/9093d28ece644ab4bab0d322d479ce90~tplv-k3u1fbpfcp-zoom-in-crop-mark:4536:0:0:0.awebp?)

```cpp
static std::vector<ParserItem> ParseManifestChunks(const LoadedFile& file,
                                                   ThreadPool* thread_pool) {
  ...
  for (std::vector<ParserItem>& chunk_items :
      ParallelMap(thread_pool, chunk_views, [&file](StringPiece view) {
    std::vector<ParserItem> chunk_items;
    manifest_chunk::ParseChunk(file, view, &chunk_items);  // 解析build.ninja
    return chunk_items;
  })) {
    std::move(chunk_items.begin(), chunk_items.end(),
              std::back_inserter(result));
  }
  ...
}
```

再执行 ParseFileInclude

```cpp
class ChunkParser{
    const LoadedFile& file_;
    Lexer lexer_;
    const char* chunk_end_ = nullptr;
    std::vector<ParserItem>* out_ = nullptr; // 保存include和subninja的文件及Clump
    Clump* current_clump_ = nullptr;    // 读取文件并分析保存文件中的内容
};

class Clump{
    std::vector<Binding*> bindings_; // 保存全局变量
    std::vector<Rule*> rules_; // rule
    std::vector<Pool*> pools_; // pool
    std::vector<Edge*> edges_; // Edge
    std::vector<DefaultTarget*> default_targets_; // default
};

struct ParserItem {
  enum Kind {
    kError, kRequiredVersion, kInclude, kClump
  };
  Kind kind;
  union {
    Error* error;
    RequiredVersion* required_version;
    Include* include;
    Clump* clump;
  } u;
  ParserItem(Error* val)            : kind(kError)            { u.error             = val; }
  ParserItem(RequiredVersion* val)  : kind(kRequiredVersion)  { u.required_version  = val; }
  ParserItem(Include* val)          : kind(kInclude)          { u.include           = val; }
  ParserItem(Clump* val)            : kind(kClump)            { u.clump             = val; }
};
```

##### ChunkParser::ParseChunk()

此函数为读取文件进行`初步分析`的主要位置,按行,循环执行`lexer_.ReadToken()`;读取 `build.ninja` 的内容并根据内容返回枚举属性值,判断属性值并执行对应的函数

```cpp
bool ChunkParser::ParseChunk() {
  while (true) {
    if (lexer_.GetPos() >= chunk_end_) {
      assert(lexer_.GetPos() == chunk_end_ &&
             "lexer scanned beyond the end of a manifest chunk");
      return true;
    }

    Lexer::Token token = lexer_.ReadToken();
    bool success = true;
    switch (token) {
    case Lexer::INCLUDE:  success = ParseFileInclude(false); break;
    case Lexer::SUBNINJA: success = ParseFileInclude(true); break;
    case Lexer::POOL:     success = ParsePool(); break;
    case Lexer::DEFAULT:  success = ParseDefault(); break;  // 读取默认
    case Lexer::IDENT:    success = ParseBinding(); break; // 读取全局变量并保存
    case Lexer::RULE:     success = ParseRule(); break;  // 读取rule , rule保存在clump->rule_中, 在遇到rule内变量时,会保存到rule->bending_ 以键值对的形式顺序保存
    case Lexer::BUILD:    success = ParseEdge(); break;  // 获取Edge,一个build就是一个Edge
    case Lexer::NEWLINE:  break;
    case Lexer::ERROR:    return LexerError(lexer_.DescribeLastError());
    case Lexer::TNUL:     return LexerError("unexpected NUL byte");
    case Lexer::TEOF:
      assert(false && "EOF should have been detected before reading a token");
      break;
    default:
      return LexerError(std::string("unexpected ") + Lexer::TokenName(token));
    }
    if (!success) return false;
  }
  return false;  // not reached
}
```

- **ParseFileInclude(false)** : 处理`include`,保存文件到`ChunkParser::out_`
- **ParseFileInclude(true)** : 处理`subninja`,逻辑同上,区别在于作用域不同
- **ParsePool()** : 保存`pool`到`Clump::pools_`
- **ParseDefault()** : 保存`default`到`Clump::default_targets_`
- **ParseBinding()** : 保存`全局变量`到`Clump::bindings_`
- **ParseRule()** : 保存`Rule`到`Clump::rule_`
- **ParseEdge()** : 保存`Edge`到`Clump::redges_`

#### 3 构建 Edge 图

在初步加载分析后,会执行`ManifestLoader::FinishLoading(std::vector<Clump*>&,std::string*)`在再次分析得到准确的`Edge`和`Node`,将其保存到`State`,分为 5 部分

```cpp
bool ManifestLoader::FinishLoading(const std::vector<Clump*>& clumps,
                                   std::string* err) {
  // 构造输入/输出节点的初始图。
  // 选择一个可能保持碰撞次数较低的初始大小。
  // Edge的非隐式输出的数量对于最终的节点的数量是一个足够好的代理。
  {
    METRIC_RECORD(".ninja load : edge setup");
    size_t output_count = 0;
    // 计算edge的数量
    for (Clump* clump : clumps)
      output_count += clump->edge_output_count_;
    // 重新计算Node的容器大小,默认算Edge的三倍
    state_->paths_.reserve(state_->paths_.size() + output_count * 3);

    if (!PropagateError(err, ParallelMap(thread_pool_, clumps,
        [this](Clump* clump) {
      std::string err;
      // 抽出Clump中的Edge,Node,Pool等数据,初步构建Edge图
      FinishAddingClumpToGraph(clump, &err);
      return err;
    }))) {
      return false;
    }
  }
  // 记录由一条边构建的每个节点的内边。检测到重复的Edge。
  // 使用 dupbuild=warn(默认直到1.9.0),当两条Edge生成同一Node时，从后面的Edge的输出列表中删除重复的Node。
  // 如果删除了一条Edge的所有输出，请从graph中删除该Edge。
  // 简单的说就是会遍历Edge和其中的output的Node,查看是否有重复值如果有就会删除掉
  {
    METRIC_RECORD(".ninja load : link edge outputs");
    for (Clump* clump : clumps) {
      for (size_t edge_idx = 0; edge_idx < clump->edges_.size(); ) {
        Edge* edge = clump->edges_[edge_idx];
        for (size_t i = 0; i < edge->outputs_.size(); ) {
          Node* output = edge->outputs_[i];
          if (output->in_edge() == nullptr) {
            output->set_in_edge(edge);
            ++i;
            continue;
          }
          // 存在两个Edge输出同一节点
          if (options_.dupe_edge_action_ == kDupeEdgeActionError) {
            return DecorateError(clump->file_,
                                 edge->parse_state_.final_diag_pos,
                                 "multiple rules generate " + output->path() +
                                 " [-w dupbuild=err]", err);
          } else {
            if (!quiet_) {
              Warning("multiple rules generate %s. "
                      "builds involving this target will not be correct; "
                      "continuing anyway [-w dupbuild=warn]",
                      output->path().c_str());
            }
            if (edge->is_implicit_out(i))
              --edge->implicit_outs_;
            else
              --edge->explicit_outs_;
            edge->outputs_.erase(edge->outputs_.begin() + i);
          }
        }
        if (edge->outputs_.empty()) {
          clump->edges_.erase(clump->edges_.begin() + edge_idx);
          continue;
        }
        ++edge_idx;
      }
    }
  }
  // 此时所有的重复Edge已经被剔除掉了,现在开始给input添加需要自己的edge
  {
    METRIC_RECORD(".ninja load : link edge inputs");
    ParallelMap(thread_pool_, clumps, [](Clump* clump) {
      for (Edge* edge : clump->edges_) {
        for (Node* input : edge->inputs_) {
          input->AddOutEdge(edge);
        }
        for (Node* validation : edge->validations_) {
          validation->AddValidationOutEdge(edge);
        }
      }
    });
  }
  // 添加默认的target
  {
    METRIC_RECORD(".ninja load : default targets");
    for (Clump* clump : clumps) {
      // 从Clump->default_targets_中获取没有 DefaultTarget
      for (DefaultTarget* target : clump->default_targets_) {
        std::string path;
        EvaluatePathInScope(&path, target->parsed_path_,
                            target->pos_.scope_pos());
        uint64_t slash_bits;  // Unused because this only does lookup.
        std::string path_err;
        if (!CanonicalizePath(&path, &slash_bits, &path_err))
          return DecorateError(clump->file_, target->diag_pos_, path_err, err);

        Node* node = state_->LookupNodeAtPos(path, target->pos_.dfs_location());
        if (node == nullptr) {
          return DecorateError(clump->file_, target->diag_pos_,
                               "unknown target '" + path + "'", err);
        }
        state_->AddDefault(node);
      }
    }
  }
  // 将所有的Edge添加到全局的Edge vector容器中(*State->edges_),并对其分配id
  {
    METRIC_RECORD(".ninja load : build edge table");

    size_t old_size = state_->edges_.size();
    size_t new_size = old_size;
    for (Clump* clump : clumps) {
      new_size += clump->edges_.size();
    }
    state_->edges_.reserve(new_size);
    for (Clump* clump : clumps) {
      std::copy(clump->edges_.begin(), clump->edges_.end(),
                std::back_inserter(state_->edges_));
    }
    // Assign edge IDs.
    ParallelMap(thread_pool_, IntegralRange<size_t>(old_size, new_size),
        [this](size_t idx) {
      state_->edges_[idx]->id_ = idx;
    });
  }

  return true;
}
```

##### (1) edge setup

构造输入/输出节点的初始图。选择一个可能保持碰撞次数较低的初始大小。Edge 的非隐式输出的数量对于最终的节点的数量是一个足够好的代理。
执行`FinishAddingClumpToGraph() -> AddEdgeToGraph()`按照顺序,`查询Rule` -> `判断Pool`-> `重置容器容量` -> `循环构建Node`
构建完成,根据配置进行一些设置,此部分的内容就完成了

##### (2) link edge outputs

记录由一条边构建的每个节点的内边。检测到重复的 Edge。使用 dupbuild=warn(默认直到 1.9.0),当两条 Edge 生成同一 Node 时，从后面的 Edge 的输出列表中删除重复的 Node。如果删除了一条 Edge 的所有输出，请从 graph 中删除该 Edge。
简单的说就是,会遍历 Edge 和其中 output 的 Node,查看是否有重复值如果有就会删除掉

##### (3) link edge inputs

此时所有的重复 Edge 已经被剔除掉了,现在开始给 input 添加需要自己的 edge

##### (4) default targets

添加默认的 target

##### (5) build edge table

将所有的 Edge 添加到全局的 Edge vector 容器中(\*State->edges\_),并对其分配 id

#### 4 加载日志文件

会加载两个日志文件命名分别为`.ninja_log`和`.ninja_deps` `.ninja_log`保存 ninja 运行期间的所有日志 `.ninja_deps`保存了 ninja 的构建图,在此过程将节点添加到构建图中，查找每个节点的最后记录记录输出，并计算开发记录的总数.(使用`ninja -t deps`读取)

#### 5 执行编译

![Ninja5.jpg](https://p3-juejin.byteimg.com/tos-cn-i-k3u1fbpfcp/eb265109c53d4d759253bb834f596e68~tplv-k3u1fbpfcp-zoom-in-crop-mark:4536:0:0:0.awebp?)

在`subproc.Start()`中使用`posix_spawn()`创建一个新的进程,使用`/bin/sh -c "command"`来执行编译指令

## 总结

### 两个特点:

1. 由其他高级编译系统生成其输入文件
   1. ninja 自带的 ninja_syntax.py
   2. 安卓 soong 编译系统中的工具
   3. cmake -Gninja 生成 ninja 文件
2. 更快的编译
   1. 编译前会构建一张图
   2. 根据图找到依赖关系链
   3. 根据依赖关系链,展开编译命令,构建子进程进行编译以提高编译速度

## 使用拓展

### 自带工具集

ninja 自身集成了 graphviz 等一些对开发有用的工具,可以使用 `ninja -t list` 查看

```python
python
ninja subtools:

browse        # 在浏览器中浏览依赖关系图。（默认会在 8080 端口启动一个基于python的http服务）
clean         # 清除构建生成的文件
commands      # 罗列重新构建制定目标所需的所有命令
deps          # 显示存储在deps日志中的依赖关系
graph         # 为指定目标生成 graphviz dot 文件。
                 如 ninja -t graph all |dot -Tpng -o graph.png
inputs        # 显示目标的所有（递归）输入
path          # 查找两个目标之间的依赖关系路径
paths         # 查找两个目标之间的所有依赖项路径
query         # 显示一个路径的inputs/outputs
targets       # 通过DAG中rule或depth罗列target
compdb        # dump JSON兼容的数据库到标准输出
recompact     # 重新紧凑化ninja内部数据结构
```

![Ninja7.png](https://p9-juejin.byteimg.com/tos-cn-i-k3u1fbpfcp/b4b7abd7bcd84b398e3ac54a6d9973e0~tplv-k3u1fbpfcp-zoom-in-crop-mark:4536:0:0:0.awebp?)

```shell
shell
ninja -t graph ninja | dot -Tpng -o ninja.png
```

### 使用 ninja 提高编译速度

```bash
bash
source build/envsetup.sh
lunch xxx
make
```

从 Android O 开始，soong 已经是 google 的入口。从 soong 入口后，会经 soong_ui,soong,kati,blueprint 几个阶段，把 mk，bp 转换成 ninja 文件后，然后执行 ninja 命令解析 ninja 文件进行编译。

![Ninja6.png](https://p1-juejin.byteimg.com/tos-cn-i-k3u1fbpfcp/aa79a604cada42558dae4669e35ea8c8~tplv-k3u1fbpfcp-zoom-in-crop-mark:4536:0:0:0.awebp?) 从图中来看,准备过程十分冗长,每次编译都是需要重新收集相关文件,重新编译成 build.ninja,再合并为 combined-xxx.ninja 文件
如果我们舍弃掉准备的过程那么就可以直接指向 ninja 以提高速率
添加函数到 envsetup.sh
修改 miui/build/envsetup.sh,新增一个 quickbuild 函数

```shell
shell
function quickbuild() {
    # 备份当前目录
    local current=$PWD

    local out_dir="$ANDROID_BUILD_TOP/out"
    local file=$out_dir/combined-$TARGET_PRODUCT.ninja

    if [ ! -f $file ]; then
        file=$out_dir/combined-$TARGET_PRODUCT-target-files-package.ninja
    fi

    if [ -f $file ]; then
        echo "ninja: $file" $*
    else
        echo "ninja: $file not exist"
        return
    fi

    croot && prebuilts/build-tools/linux-x86/bin/ninja -f $file $*

    cd $current
}
```

**注意 : 此方式只适用于修改 c,cpp,java 文件等,如果添加文件或修改配置文件,需要重新 make 生成 ninja 文件**
