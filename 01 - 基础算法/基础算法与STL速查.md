# 基础算法与 STL 速查

## 容器复杂度

|需求|首选|复杂度与提醒|
|---|---|---|
|尾部动态数组|`vector`|尾插均摊 `O(1)`；扩容会使迭代器、指针失效。|
|双端队列/单调队列|`deque`|两端 `O(1)`，内存不连续。|
|先进先出/后进先出|`queue` / `stack`|适合作为受限接口。|
|有序集合/映射|`set` / `map`|`O(log n)`；需要排名用 FHQ/PBDS。|
|哈希集合/映射|`unordered_*`|平均 `O(1)`、最坏 `O(n)`；对抗数据使用自定义随机哈希。|
|优先队列|`priority_queue`|默认大根堆；小根堆写 `greater<T>`。|
|固定长度元组|`array`|可比较、连续内存，常比小 `vector` 更轻。|

`map[key]` 在键不存在时会插入默认值；只查询应使用 `find/contains`。删除遍历中的元素写 `it = c.erase(it)`。`reserve` 只改容量，`resize` 才改元素个数。

### GNU PBDS

PBDS 仅可用于 GCC/libstdc++，完整可抄模板见数据结构目录：`10C/10E` 为有序多重集及唯一键 `tree`，`13A/13B` 为 `gp_hash_table/cc_hash_table`，`09B` 为带节点句柄的可并堆，`08C` 为可做 `prefix_range` 的 Patricia trie。特别注意：`tree::find_by_order(k)` 使用 0-indexed；PBDS 哈希的默认策略不能照搬 `unordered_map::reserve`；有序多重集不能用违反严格弱序的 `less_equal` 假装实现。

## 高频库算法

- `lower_bound` 找第一个 `>=x`，`upper_bound` 找第一个 `>x`；容器必须已按同一比较器排序。
- `nth_element(a.begin(),a.begin()+k,a.end())` 把第 `k` 小放到位，平均线性，不保证两侧有序。
- `partial_sort` 需要有序的前若干项；只需一个次序统计量不要全排序。
- `unique` 只去除相邻重复，通常先 `sort`，再 `erase(unique(...),end())`。
- `next_permutation` 要从有序初态开始才能遍历全部排列。
- `accumulate` 的初值决定运算类型：本库写 `int{}`，泛型模板写 `T{}`，不要让裸 `0` 把累加器固定成原生 32 位整数。
- 浮点排序比较器必须满足严格弱序；不要把 `abs(a-b)<eps` 直接当 `operator<`。

## 输入、输出与本地验证

- 混用 `getline` 与 `cin >>` 前先消费行末；未知个数的一行可 `getline` 后交给 `stringstream`。
- 浮点输出用 `fixed << setprecision(k)`；`setprecision` 在非 `fixed` 下表示有效数字数。
- `endl` 原本会刷新缓冲，本库宏把它变成换行字符串；交互题需要显式 `flush`。
- 随机造数必须使用均匀分布，不能用 `rng()%range` 验证概率性质；普通对拍小范围取模足够。
- 对拍至少保存随机种子、完整输入和两份输出；发现反例后先缩小数据，再改模板。

## 调试、编译参数与卡常定位

### 四套编译方式

以下命令以 GCC/MinGW 的 C++17 为准；可执行文件名按系统改为 `a` 或 `a.exe`。

|目的|建议参数|注意|
|---|---|---|
|日常警告|`g++ a.cpp -std=c++17 -O2 -g -Wall -Wextra -Wshadow=compatible-local -Winit-self -Wuninitialized`|先清未初始化、遮蔽和返回值问题。|
|容器边界|`g++ a.cpp -std=c++17 -O0 -g3 -D_GLIBCXX_DEBUG -D_GLIBCXX_ASSERTIONS`|会改变 STL 容器 ABI 与常数，只用于整份程序的本地调试。|
|未定义行为|`g++ a.cpp -std=c++17 -O1 -g3 -fsanitize=address,undefined -fno-omit-frame-pointer -fno-sanitize-recover=all`|不要与 `-static` 混用；评测机通常不提供 Sanitizer。|
|提交/基准|`g++ a.cpp -std=c++17 -O2 -pipe -DNDEBUG`|只在所有断言均已验证后加 `-DNDEBUG`。|

- ASan 重点检查越界、释放后使用和栈生命周期；UBSan 重点检查有符号溢出、非法移位、除零与错误对齐。
- 部分精简 MinGW 未附带 `libasan/libubsan`，链接时报 `cannot find -lasan/-lubsan` 时改用 WSL/Linux 或完整 LLVM/MinGW 工具链，不要误判为代码错误。
- `-D_GLIBCXX_DEBUG` 最适合定位失效迭代器和容器越界，但会大幅变慢；不要拿该版本做性能判断。
- Linux 递归爆栈先估算单层栈帧，再用 `ulimit -s unlimited` 验证；MinGW 可临时加 `-Wl,--stack=268435456`。最终仍优先改成显式栈。
- `-march=native`、`#pragma GCC target(...)` 依赖评测 CPU；未确认指令集时不要提交。`-Ofast` 会放宽浮点语义，可能破坏 NaN、无穷与精度相关算法。
- `#pragma GCC optimize("Ofast", "unroll-loops")` 不是通用加速开关。只有基准证明瓶颈在纯整数热点且题目允许时才启用，并保留不带 pragma 的正确版本。

### 定位

1. 固定随机种子并保存首个失败输入；先把规模、值域、边数和操作数缩到仍会失败的最小附近。
2. 在状态变化后检查不变量，不要只在最终答案处打印。二分插入检查点，先确定第一轮出现分歧的位置。
3. 小数据与朴素算法逐项比较；没有朴素解时使用交换输入、整体平移、加零项、重复求解等变形关系验证。
4. WA 优先核对边界、清空、重复值和溢出；RE 跑 ASan/UBSan；只在结果正确后再做计时与卡常。
5. 对拍输出必须写入 `cerr` 或独立日志；提交前搜索 `_(`、`toc(`、`freopen`、随机种子和临时断言。

常用算法不变量：

- 网络流：每条边满足 `0 <= f <= c`，除源汇外流量守恒，最终 `maxflow == mincut`。
- NTT/FFT：先验证 `intt(ntt(a)) == a`，再对拍朴素卷积；另检查 `sum(c) == sum(a)sum(b)`（同模意义）。任意模极限数据优先三模 NTT。
- 高精度：与 Python `int` 对拍正负号和前导零；除法同时检查 `a == q*b+r`、`|r|<|b|` 且余数符号约定一致。
- 数据结构：每次修改后用小数组重算全部查询；可持久化结构额外检查旧版本未变化。

### 本地计时与基准

基于 `linyue.hpp`：

```cpp
auto t = tic();
// 被测代码
toc(t, "ntt"); // 输出到 cerr；ONLINE_JUDGE 下提交前删除
```

- 使用 `steady_clock`，不要用可能受系统时间调整影响的时钟。先预热一次，再测 3–5 次并看中位数。
- 算法基准与读写基准分开；计时区间内禁止调试输出、随机设备初始化和重复申请无关大内存。
- 比较卡常模板时使用同一编译器、参数、输入、CPU 与进程位数。一次快几毫秒不足以证明常数更低。
- 性能异常先看复杂度与内存访问：连续数组、窄下标、减少 `%`、减少整轮变换通常比手动 `inline` 或循环展开更可靠。

### 5.4 GDB 最小命令集

`g++ -O0 -g3` 编译后执行 `gdb ./a`，常用命令为 `run < data.in`、`bt`、`frame n`、`print x`、`display x`、`break file.cpp:line`、`watch x`、`continue`。优化后的变量可能被消除；先在 `-O0` 下定位正确性，再用 `-O2 -g` 复现只在优化时出现的问题。

## 6. 基础恒等式与边界

- 前缀和查询闭区间 `[l,r]` 为 `s[r+1]-s[l]`；二维前缀和用四项容斥。
- 差分做闭区间加：`d[l]+=x,d[r+1]-=x`，必须给 `r+1` 留哨兵。
- 单调栈统计重复值贡献时，一侧用严格比较、另一侧用非严格比较。
- 二分只依赖单调谓词；先写出“不存在答案”时返回什么，再确定开闭边界。
- 整数中点用 `l+(r-l)/2`；带负数的数学 `floor/ceil` 除法不能直接套 C++ 向零截断。
