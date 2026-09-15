# STL

## 01 - STL 与 GNU 扩展

### 01A - PBDS前缀字典树（GNU环境）
用 GNU PBDS Patricia trie 维护唯一字符串并枚举给定前缀。
基本操作为 $\mathcal O(\log n)$ 期望，前缀枚举另加输出规模。
仅 GCC/libstdc++；键唯一，`prefix_range(s)` 返回以 `s` 为前缀的半开范围。只需插入、查询或计数时使用数据结构目录的 `08A`。

```cpp
#ifdef int
#pragma push_macro("int")
#undef int
#define LYE_RESTORE_INT
#endif
#ifdef endl
#pragma push_macro("endl")
#undef endl
#define LYE_RESTORE_ENDL
#endif
#include <ext/pb_ds/assoc_container.hpp>
#include <ext/pb_ds/trie_policy.hpp>
#ifdef LYE_RESTORE_ENDL
#pragma pop_macro("endl")
#undef LYE_RESTORE_ENDL
#endif
#ifdef LYE_RESTORE_INT
#pragma pop_macro("int")
#undef LYE_RESTORE_INT
#endif

using PTrie = __gnu_pbds::trie<
    string, __gnu_pbds::null_type,
    __gnu_pbds::trie_string_access_traits<>,
    __gnu_pbds::pat_trie_tag,
    __gnu_pbds::trie_prefix_search_node_update>;
```

### 01B - PBDS可并堆（GNU环境）
用 GNU PBDS pairing heap 支持优先队列、任意节点修改/删除和破坏性合并。
push/join 均摊 $\mathcal O(1)$，pop 均摊 $\mathcal O(\log n)$。
仅 GCC/libstdc++；默认 `Cmp=less<T>` 为大根堆，`greater<T>` 为小根堆。`push` 返回句柄供 `modify/erase` 使用，`join` 后右堆清空，句柄操作复杂度依赖修改方向。

```cpp
#ifdef int
#pragma push_macro("int")
#undef int
#define LYE_RESTORE_INT
#endif
#ifdef endl
#pragma push_macro("endl")
#undef endl
#define LYE_RESTORE_ENDL
#endif
#include <ext/pb_ds/priority_queue.hpp>
#ifdef LYE_RESTORE_ENDL
#pragma pop_macro("endl")
#undef LYE_RESTORE_ENDL
#endif
#ifdef LYE_RESTORE_INT
#pragma pop_macro("int")
#undef LYE_RESTORE_INT
#endif

template <class T, class Cmp = less<T>> struct Heap {
    using H = __gnu_pbds::priority_queue<T, Cmp, __gnu_pbds::pairing_heap_tag>;
    using It = typename H::point_iterator;
    H h;

    It push(const T &x) { return h.push(x); }
    void pop() { h.pop(); }
    const T &ask() const { return h.top(); }
    void modify(It p, const T &x) { h.modify(p, x); }
    void erase(It p) { h.erase(p); }
    void merge(Heap &o) { h.join(o.h); }
    bool askEmp() const { return h.empty(); }
    int askSz() const { return h.size(); }
    void clear() { h.clear(); }
};
```

### 01C - PBDS有序多重集（GNU环境）
用 GNU PBDS 有序树维护多重集，并支持排名与第 $k$ 小。
插入、删除、排名和第 $k$ 小均 $\mathcal O(\log n)$。
仅 GCC/libstdc++；重复键用唯一编号区分，排名和 `kth` 为 $1$-indexed；与可移植的 `11A` 是同一问题的不同实现。

```cpp
#ifdef int
#pragma push_macro("int")
#undef int
#define LYE_RESTORE_INT
#endif
#ifdef endl
#pragma push_macro("endl")
#undef endl
#define LYE_RESTORE_ENDL
#endif
#include <ext/pb_ds/assoc_container.hpp>
#include <ext/pb_ds/tree_policy.hpp>
#ifdef LYE_RESTORE_ENDL
#pragma pop_macro("endl")
#undef LYE_RESTORE_ENDL
#endif
#ifdef LYE_RESTORE_INT
#pragma pop_macro("int")
#undef LYE_RESTORE_INT
#endif

template <class Key> struct OSet {
    using P = pair<Key, int>;
    using Tree = __gnu_pbds::tree<P, __gnu_pbds::null_type, less<P>,
                                  __gnu_pbds::rb_tree_tag,
                                  __gnu_pbds::tree_order_statistics_node_update>;
    Tree t;
    int id = 0;

    int insert(const Key& x) {
        int k = ++id;
        t.insert({x, k});
        return k;
    }

    bool erase(const Key& x, int k) { return t.erase({x, k}); }

    bool erase(const Key& x) {
        auto it = t.lower_bound({x, 0});
        if (it == t.end() || it->first != x) return false;
        t.erase(it);
        return true;
    }

    int askSz() const { return t.size(); }
    bool askEmp() const { return t.empty(); }
    void clear() { t.clear(), id = 0; }
    int askCnt(const Key& x) const {
        return t.order_of_key({x, numeric_limits<int>::max()}) - t.order_of_key({x, 0});
    }
    int askRank(const Key& x) const { return t.order_of_key({x, 0}) + 1; }

    optional<Key> askKth(int k) const {
        if (k < 1 || k > askSz()) return nullopt;
        return t.find_by_order(k - 1)->first;
    }

    optional<Key> askPre(const Key& x) const {
        auto it = t.lower_bound({x, 0});
        if (it == t.begin()) return nullopt;
        return (--it)->first;
    }

    optional<Key> askNxt(const Key& x) const {
        auto it = t.upper_bound({x, numeric_limits<int>::max()});
        return it == t.end() ? nullopt : optional<Key>(it->first);
    }

    optional<Key> askGE(const Key& x) const {
        auto it = t.lower_bound({x, 0});
        return it == t.end() ? nullopt : optional<Key>(it->first);
    }
};
```

### 01D - PBDS有序集合与映射（GNU环境）
用 GNU PBDS 有序树维护唯一键集合/映射，并支持排名与第 $k$ 小。
常规操作、排名、分裂和合并均为 $\mathcal O(\log n)$ 期望/摊销复杂度。
仅 GCC/libstdc++；`OST` 的 `order_of_key`、`find_by_order` 为 $0$-indexed，`split` 后原树保留不大于 $x$ 的键，`join` 要求键域严格分离；重复键使用 `01C`。

```cpp
#ifdef int
#pragma push_macro("int")
#undef int
#define LYE_RESTORE_INT
#endif
#ifdef endl
#pragma push_macro("endl")
#undef endl
#define LYE_RESTORE_ENDL
#endif
#include <ext/pb_ds/assoc_container.hpp>
#include <ext/pb_ds/tree_policy.hpp>
#ifdef LYE_RESTORE_ENDL
#pragma pop_macro("endl")
#undef LYE_RESTORE_ENDL
#endif
#ifdef LYE_RESTORE_INT
#pragma pop_macro("int")
#undef LYE_RESTORE_INT
#endif

template <class K, class Cmp = less<K>, class Tag = __gnu_pbds::rb_tree_tag>
using OST = __gnu_pbds::tree<K, __gnu_pbds::null_type, Cmp, Tag,
                            __gnu_pbds::tree_order_statistics_node_update>;

template <class K, class V, class Cmp = less<K>, class Tag = __gnu_pbds::rb_tree_tag>
using OMT = __gnu_pbds::tree<K, V, Cmp, Tag,
                            __gnu_pbds::tree_order_statistics_node_update>;
```

### 01E - PBDS平衡树（GNU环境，基础）

GNU PBDS 的基础有序平衡树，维护唯一键并支持排名、第 $k$ 小、前驱后继。
各操作 $\mathcal O(\log n)$。
仅 GCC/libstdc++；提供 `add/del/askRank/askKth/askPre/askNxt/askSz/askEmp`，排名和第 $k$ 小统一为 $1$-indexed；重复键使用 `01C`，可移植实现使用数据结构目录的 `11A`。

```cpp
#ifdef int
#pragma push_macro("int")
#undef int
#define LYE_RESTORE_INT
#endif
#ifdef endl
#pragma push_macro("endl")
#undef endl
#define LYE_RESTORE_ENDL
#endif
#include <ext/pb_ds/assoc_container.hpp>
#include <ext/pb_ds/tree_policy.hpp>
#ifdef LYE_RESTORE_ENDL
#pragma pop_macro("endl")
#undef LYE_RESTORE_ENDL
#endif
#ifdef LYE_RESTORE_INT
#pragma pop_macro("int")
#undef LYE_RESTORE_INT
#endif

template <class T = int, class C = less<T>, class Tag = __gnu_pbds::rb_tree_tag>
struct PBTree {
    using Tree = __gnu_pbds::tree<T, __gnu_pbds::null_type, C, Tag,
                                  __gnu_pbds::tree_order_statistics_node_update>;
    Tree t;

    bool add(const T& x) { return t.insert(x).second; }
    bool del(const T& x) { return t.erase(x); }
    bool ask(const T& x) const { return t.find(x) != t.end(); }
    int askSz() const { return t.size(); }
    bool askEmp() const { return t.empty(); }
    void clear() { t.clear(); }

    int askRank(const T& x) const { return t.order_of_key(x) + 1; }

    optional<T> askKth(int k) const {
        if (k < 1 || k > askSz()) return nullopt;
        return *t.find_by_order(k - 1);
    }

    optional<T> askPre(const T& x) const {
        auto it = t.lower_bound(x);
        if (it == t.begin()) return nullopt;
        return *--it;
    }

    optional<T> askNxt(const T& x) const {
        auto it = t.upper_bound(x);
        return it == t.end() ? nullopt : optional<T>(*it);
    }
};
```

### 01F - PBDS开放寻址哈希表（gp_hash_table，GNU环境）
用 GNU PBDS gp_hash_table 维护大量唯一键的映射/集合，采用开放寻址以减小常数，GCC/libstdc++ 的 `gp_hash_table` 与随机盐哈希。
平均插入、查找、删除 $\mathcal O(1)$，最坏不保证。
仅 GCC/libstdc++；开放寻址常数小但增删可能使迭代器失效，`HMap<K,V>`/`HSet<K>` 提供常用映射和集合接口，哈希含运行时盐。

```cpp

using u64 = uint64_t;

#ifdef int
#pragma push_macro("int")
#undef int
#define LYE_RESTORE_INT
#endif
#ifdef endl
#pragma push_macro("endl")
#undef endl
#define LYE_RESTORE_ENDL
#endif
#include <ext/pb_ds/assoc_container.hpp>
#ifdef LYE_RESTORE_ENDL
#pragma pop_macro("endl")
#undef LYE_RESTORE_ENDL
#endif
#ifdef LYE_RESTORE_INT
#pragma pop_macro("int")
#undef LYE_RESTORE_INT
#endif

struct Hash {
    inline static const u64 r =
        chrono::steady_clock::now().time_since_epoch().count();

    static u64 mix(u64 x) {
        x += 0x9e3779b97f4a7c15ULL;
        x = (x ^ (x >> 30)) * 0xbf58476d1ce4e5b9ULL;
        x = (x ^ (x >> 27)) * 0x94d049bb133111ebULL;
        return x ^ (x >> 31);
    }

    template <class T> size_t operator()(const T &x) const {
        return mix(u64(hash<T>{}(x)) + r);
    }

    template <class A, class B> size_t operator()(const pair<A, B> &x) const {
        u64 a = (*this)(x.first), b = (*this)(x.second);
        return mix(a ^ (b + 0x9e3779b97f4a7c15ULL + (a << 6) + (a >> 2)));
    }
};

template <class K, class V, class H = Hash, class Eq = equal_to<K>>
using HMap = __gnu_pbds::gp_hash_table<K, V, H, Eq>;

template <class K, class H = Hash, class Eq = equal_to<K>>
using HSet = __gnu_pbds::gp_hash_table<K, __gnu_pbds::null_type, H, Eq>;
```

### 01G - PBDS拉链哈希表（cc_hash_table，GNU环境）
用 GNU PBDS cc_hash_table 维护哈希映射/集合，适合避免开放寻址聚簇或存放较大值，GCC/libstdc++ 的 `cc_hash_table` 与随机盐哈希。
平均各操作 $\mathcal O(1)$，空间与节点数成正比。
仅 GCC/libstdc++；拉链法节点开销较大但可降低开放寻址聚簇风险，接口同 `01F`；与 `01F` 是同一哈希字典问题的两种实现，通常只选一份。

```cpp

using u64 = uint64_t;

#ifdef int
#pragma push_macro("int")
#undef int
#define LYE_RESTORE_INT
#endif
#ifdef endl
#pragma push_macro("endl")
#undef endl
#define LYE_RESTORE_ENDL
#endif
#include <ext/pb_ds/assoc_container.hpp>
#ifdef LYE_RESTORE_ENDL
#pragma pop_macro("endl")
#undef LYE_RESTORE_ENDL
#endif
#ifdef LYE_RESTORE_INT
#pragma pop_macro("int")
#undef LYE_RESTORE_INT
#endif

struct Hash {
    inline static const u64 r =
        chrono::steady_clock::now().time_since_epoch().count();

    static u64 mix(u64 x) {
        x += 0x9e3779b97f4a7c15ULL;
        x = (x ^ (x >> 30)) * 0xbf58476d1ce4e5b9ULL;
        x = (x ^ (x >> 27)) * 0x94d049bb133111ebULL;
        return x ^ (x >> 31);
    }

    template <class T> size_t operator()(const T &x) const {
        return mix(u64(hash<T>{}(x)) + r);
    }

    template <class A, class B> size_t operator()(const pair<A, B> &x) const {
        u64 a = (*this)(x.first), b = (*this)(x.second);
        return mix(a ^ (b + 0x9e3779b97f4a7c15ULL + (a << 6) + (a >> 2)));
    }
};

template <class K, class V, class H = Hash, class Eq = equal_to<K>>
using HMap = __gnu_pbds::cc_hash_table<K, V, H, Eq>;

template <class K, class H = Hash, class Eq = equal_to<K>>
using HSet = __gnu_pbds::cc_hash_table<K, __gnu_pbds::null_type, H, Eq>;
```

### 库函数

#### pb_ds 库

其中 `gp_hash_table` 使用的最多，其等价于 `unordered_map` ，内部是无序的。

```c++
#include <bits/extc++.h>
#include <ext/pb_ds/assoc_container.hpp>
template<class S, class T> using omap = __gnu_pbds::gp_hash_table<S, T, myhash>;
```

#### 查找后继 lower\_bound、upper\_bound

`lower` 表示 $\ge$ ，`upper` 表示 $>$ 。使用前记得**先进行排序**。

返回区间中第一个不小于给定值的元素位置。

```c++
cout << lower_bound(a + start, a + end, x);

cout << lower_bound(a, a + n, x) - a; //在a数组中查找第一个>=x的元素下标
upper_bound(a, a + n, k) - lower_bound(a, a + n, k) //查找k在a中出现了几次
```

#### 数组打乱 shuffle

```c++
mt19937 rnd(chrono::steady_clock::now().time_since_epoch().count());
shuffle(ver.begin(), ver.end(), rnd);
```

#### 二分搜索 binary\_search

用于查找某一元素是否在容器中，相当于 find 函数。在使用前需要**先进行排序**。

判断区间内是否存在给定值。

```c++
cout << binary_search(a + start, a + end, x);
```

#### 批量递增赋值函数 iota

对容器递增初始化。

用连续递增值填充区间。

```c++
iota(a + start, a + end, x);
```

#### 数组去重函数 unique

在使用前需要**先进行排序**。

其作用是，对于区间 `[开始位置, 结束位置)` ，**不停的把后面不重复的元素移到前面来**，也可以说是**用不重复的元素占领重复元素的位置**。并且返回**去重后容器中不重复序列的最后一个元素的下一个元素**。所以在进行操作后，数组、容器的大小并**没有发生改变**。

对已排序区间去重并返回新的逻辑结尾。

```c++
unique(a + start, a + end);

//与earse函数结合，达到去重+删除的目的
a.erase(unique(ALL(a)), a.end());
```

#### bit 库与位运算函数 \__builtin\_

```c++
__builtin_popcount(x) // 返回x二进制下含1的数量，例如x=15=(1111)时答案为4

__builtin_ffs(x) // 返回x右数第一个1的位置(1-idx)，1(1) 返回 1，8(1000) 返回 4，26(11010) 返回 2

__builtin_ctz(x) // 返回x二进制下后导0的个数，1(1) 返回 0，8(1000) 返回 3

bit_width(x) // 返回x二进制下的位数，9(1001) 返回 4，26(11010) 返回 5
```

注：以上函数的 `long long` 版本只需要在函数后面加上 `ll` 即可（例如 `__builtin_popcountll(x)` )， `unsigned long long` 加上 `ull` 。

#### 数字转字符串函数

`itoa` 虽然能将整数转换成任意进制的字符串，但是其不是标准的`C`函数，且为Windows独有，且不支持 `long long` ，建议手写。

将数值转换为字符串。

```c++
double val = 12.12;
cout << to_string(val);
```

将整数转换到指定进制的字符数组（不建议使用）。

```c++
char ans[10] = {};
itoa(12, ans, 2);
cout << ans << endl; /*1100*/

// 长整型函数名ltoa，最高支持到int型上限2^31。ultoa同理。
```

#### 字符串转数字

用 stoi 将字符串转换为整数。

```c++
cout << stoi("12") << endl;

// 【不建议使用】stoi转换进制，参数为待转换字符串、起始位置、进制。
// int stoi(string value, int st, int radix);
cout << stoi("1010", 0, 2) << endl; /*10*/
cout << stoi("c", 0, 16) << endl; /*12*/
cout << stoi("0x3f3f3f3f", 0, 0) << endl; /*1061109567*/

// 长整型函数名stoll，最高支持到long long型上限2^63。stoull、stod、stold同理。
```

用 atoi 将 C 字符串转换为整数。

```c++
cout << atoi("12") << endl;
cout << atoi("   12") << endl; /*12*/
cout << atoi("-12abc") << endl; /*-12*/
cout << atoi("abc12") << endl; /*0*/

// 长整型函数名atoll，最高支持到long long型上限2^63。
```

#### 全排列算法 next\_permutation、prev\_permutation

在提及这个函数时，我们先需要补充几点字典序相关的知识。

> 对于三个字符所组成的序列`{a,b,c}`，其按照字典序的6种排列分别为：
`{abc}`，`{acb}`，`{bac}`，`{bca}`，`{cab}`，`{cba}`
其排序原理是：先固定 `a` (序列内最小元素)，再对之后的元素排列。而 `b` < `c` ，所以 `abc` < `acb` 。同理，先固定 `b` (序列内次小元素)，再对之后的元素排列。即可得出以上序列。

`next_permutation` 算法，即是按照**字典序顺序**输出的全排列；相对应的， `prev_permutation` 则是按照**逆字典序顺序**输出的全排列。可以是数字，亦可以是其他类型元素。其直接在序列上进行更新，故直接输出序列即可。

```c++
int n;
cin >> n;
vector<int> a(n);
// iota(a.begin(), a.end(), 1);
for (auto &it : a) cin >> it;
sort(a.begin(), a.end());

do {
    for (auto it : a) cout << it << " ";
    cout << endl;
} while (next_permutation(a.begin(), a.end()));
```

#### 字符串转换为数值函数 sto

可以快捷的将**一串字符串**转换为**指定进制的数字**。

使用方法

- `stoi(字符串, 0, x进制)` ：将一串 `x` 进制的字符串转换为 `int` 型数字。

![](https://img2020.cnblogs.com/blog/2491503/202201/2491503-20220117162754548-696368550.png)

- `stoll(字符串, 0, x进制)` ：将一串 `x` 进制的字符串转换为 `long long` 型数字。
- `stoull`，`stod`，`stold` 同理。

#### 数值转换为字符串函数 to\_string

允许将**各种数值类型**转换为字符串类型。

将数值转换为字符串。

```c++
string s = to_string(num);
```

#### 判断非递减 is\_sorted

判断区间是否非递减。

```c++
cout << is_sorted(a + start, a + end);
```

#### 累加 accumulate

累加区间元素并输出结果。

```c++
cout << accumulate(a + start, a + end, x);
```

#### 迭代器 iterator

构造容器的正向迭代器。

```c++
UUU::iterator it;

vector<int>::iterator it; //创建一个正向迭代器，++ 操作时指向下一个
vector<int>::reverse_iterator it; //创建一个反向迭代器，++ 操作时指向上一个
```

#### 其他函数

`exp2(x)` ：返回 $2^x$ 

`log2(x)` ：返回 $\log_2(x)$

`gcd(x, y) / lcm(x, y)` ：以 $\log$ 的复杂度返回 $\gcd(|x|, |y|)$ 与 $\textrm{lcm}(|x|, |y|)$ ，且返回值符号也为正数。

### 容器与成员函数

#### 元组 tuple

按索引获取 tuple 或 pair 元素。

```c++
tuple<string, int, int> Student = {"Wida", 23, 45000);
cout << get<0>(Student) << endl; //获取Student对象中的第一个元素，这里的输出结果应为“Wida”
```

#### 数组 array

```c++
array<int, 3> x; // 建立一个包含三个元素的数组x

[] // 跟正常数组一样，可以使用随机访问
cout << x[0]; // 获取数组重的第一个元素
```

#### 变长数组 vector

```c++
resize(n) // 重设容器大小，但是不改变已有元素的值
assign(n, 0) // 重设容器大小为n，且替换容器内的内容为0

// 尽量不要使用[]的形式声明多维变长数组，而是使用嵌套的方式替代
vector<int> ver[n + 1]; // 不好的声明方式
vector<vector<int>> ver(n + 1);

// 嵌套时只需要在最后一个注明变量类型
vector dis(n + 1, vector<int>(m + 1));
vector dis(m + 1, vector(n + 1, vector<int>(n + 1)));
```

#### 栈  stack

栈顶入，栈顶出。先进后出。

该容器没有 clear 成员函数。

```c++
size() / empty()
push(x) //向栈顶插入x
top() //获取栈顶元素
pop() //弹出栈顶元素
```

#### 队列 queue

队尾进，队头出。先进先出。

该容器没有 clear 成员函数。

```c++
size() / empty()
push(x) //向队尾插入x
front() / back() //获取队头、队尾元素
pop() //弹出队头元素
```

该容器没有 clear 成员函数。

```c++
queue<int> q;
q = queue<int>();
```

#### 双向队列 deque

```c++
size() / empty() / clear()
push_front(x) / push_back(x)
pop_front(x) / pop_back(x)
front() / back()
begin() / end()
[]
```

#### 优先队列 priority\_queue

默认升序（大根堆），自定义排序需要重载 `<` 。

priority_queue<int, vector<int>, greater<int> > p; //重定义为降序（小根堆）
该容器没有 clear 成员函数。

```c++
priority_queue<int, vector<int>, greater<int> > p; //重定义为降序（小根堆）
push(x); //向栈顶插入x
top(); //获取栈顶元素
pop(); //弹出栈顶元素
```

注意该容器重载运算符的方向。

```c++
struct Node {
    int x; string s;
    friend bool operator < (const Node &a, const Node &b) {
        if (a.x != b.x) return a.x > b.x;
        return a.s > b.s;
    }
};
```


#### 字符串 string

```c++
size() / empty() / clear()
```

截取字符串的指定子串。

```c++
cout << S.substr(1, 12);

find(x) / rfind(x); //顺序、逆序查找x，返回下标，没找到时返回一个极大值【！建议与 size() 比较，而不要和 -1 比较，后者可能出错】
//注意，没有count函数
```

#### 有序、多重有序集合 set、multiset

默认升序（大根堆），`set` 去重，`multiset` 不去重，$\mathcal O(\log N)$ 。

```c++
set<int, greater<> > s; //重定义为降序（小根堆）
size() / empty() / clear()
begin() / end()
++ / -- //返回前驱、后继

insert(x); //插入x
find(x) / rfind(x); //顺序、逆序查找x，返回迭代器【迭代器！！！】，没找到时返回end()
count(x); //返回x的个数
lower_cound(x); //返回第一个>=x的迭代器【迭代器！！！】
upper_cound(x); //返回第一个>x的迭代器【迭代器！！！】
```

特殊函数 `next` 和 `prev` 详解：

```c++
auto it = s.find(x); // 建立一个迭代器
prev(it) / next(it); // 默认返回迭代器it的前/后一个迭代器
prev(it, 2) / next(it, 2); // 可选参数可以控制返回前/后任意个迭代器

/* 以下是一些应用 */
auto pre = prev(s.lower_bound(x)); // 返回第一个<x的迭代器
int ed = *prev(S.end(), 1); // 返回最后一个元素
```

`erase(x);` 有两种删除方式：

- 当$x$为某一元素时，删除**所有**这个数，复杂度为 $\mathcal O (\textrm{num}_x+\log N)$ ；
- 当$x$为迭代器时，删除这个迭代器。

连续删除容器头部元素。

```c++
set<int> S = {0, 9, 98, 1087, 894, 34, 756};
auto it = S.begin();
int len = S.size();
for (int i = 0; i < len; ++ i) {
    if (*it >= 500) continue;
    it = S.erase(it); //删除所有小于500的元素
}
//错误用法如下【千万不能这样用！！！】
//for (auto it : S) {
//    if (it >= 500) continue;
//    S.erase(it); //删除所有小于500的元素
//}
```

#### map、multimap

默认升序（大根堆），`map` 去重，`mulitmap` 不去重，$\mathcal O(\log S)$ ，其中 $S$ 为元素数量。

```c++
map<int, int, greater<> > mp; //重定义为降序（小根堆）
size() / empty() / clear()
begin() / end()
++ / -- //返回前驱、后继

insert({x, y}); //插入二元组
[] //随机访问，multimap不支持
count(x); //返回x为下标的个数
lower_cound(x); //返回第一个下标>=x的迭代器
upper_cound(x); //返回第一个下标>x的迭代器
```

`erase(x);` 有两种删除方式：

- 当$x$为某一元素时，删除所有**以这个元素为下标的二元组**，复杂度为 $\mathcal O (\textrm{num}_x+\log N)$ ；
- 当$x$为迭代器时，删除这个迭代器。

**慎用随机访问！**--当不确定某次查询是否存在于容器中时，不要直接使用下标查询，而是先使用 `count()` 或者 `find()` 方法检查`key`值，防止不必要的零值二元组被构造。

```c++
int q = 0;
if (mp.count(i)) q = mp[i];
```

慎用自带的 pair、tuple 作为`key`值类型！使用自定义结构体！

```c++
struct fff { 
    LL x, y;
    friend bool operator < (const fff &a, const fff &b) {
        if (a.x != b.x) return a.x < b.x;
        return a.y < b.y;
    }
};
map<fff, int> mp;
```

#### bitset

将数据转换为二进制，从高位到低位排序，以 $0$ 为最低位。当位数相同时支持全部的位运算。

输入为 01 字符串时可直接使用位流读取。

```c++
bitset<10> s;
cin >> s;

//使用只含01的字符串构造--bitset<容器长度>B (字符串)
string S; cin >> S;
bitset<32> B (S);

//使用整数构造（两种方式）
int x; cin >> x;
bitset<32> B1 (x);
bitset<32> B2 = x;

// 构造时，尖括号里的数字不能是变量
int x; cin >> x;
bitset<x> ans; // 错误构造

[] //随机访问
set(x) //将第x位置1，x省略时默认全部位置1
reset(x) //将第x位置0，x省略时默认全部位置0
flip(x) //将第x位取反，x省略时默认全部位取反
to_ullong() //重转换为ULL类型
to_string() //重转换为ULL类型
count() //返回1的个数
any() //判断是否至少有一个1
none() //判断是否全为0

_Find_fisrt() // 找到从低位到高位第一个1的位置
_Find_next(x) // 找到当前位置x的下一个1的位置，复杂度 O(n/w + count)

bitset<23> B1("11101001"), B2("11101000");
cout << (B1 ^ B2) << "\n";  //按位异或
cout << (B1 | B2) << "\n";  //按位或
cout << (B1 & B2) << "\n";  //按位与
cout << (B1 == B2) << "\n"; //比较是否相等
cout << B1 << " " << B2 << "\n"; //你可以直接使用cout输出
```

#### 哈希系列 unordered

通常指代 unordered_map、unordered_set、unordered_multimap、unordered_multiset，与原版相比不进行排序。

如果将不支持哈希的类型作为 `key` 值代入，编译器就无法正常运行，这时需要我们为其手写哈希函数。而我们写的这个哈希函数的正确性其实并不是特别重要（但是不可以没有），当发生冲突时编译器会调用 `key` 的 `operator ==` 函数进行进一步判断。[参考](https://finixlei.blog.csdn.net/article/details/110267430?spm=1001.2101.3001.6650.3&utm_medium=distribute.pc_relevant.none-task-blog-2%7Edefault%7EBlogCommendFromBaidu%7ERate-3-110267430-blog-101406104.topnsimilarv1&depth_1-utm_source=distribute.pc_relevant.none-task-blog-2%7Edefault%7EBlogCommendFromBaidu%7ERate-3-110267430-blog-101406104.topnsimilarv1&utm_relevant_index=4)

#### 对 pair、tuple 定义哈希

```c++
struct hash_pair { 
    template <class T1, class T2> 
    size_t operator()(const pair<T1, T2> &p) const { 
        return hash<T1>()(p.fi) ^ hash<T2>()(p.se); 
    } 
};
unordered_set<pair<int, int>, int, hash_pair> S;
unordered_map<tuple<int, int, int>, int, hash_pair> M;
```

#### 对结构体定义哈希

需要两个条件，一个是在结构体中重载等于号（区别于非哈希容器需要重载小于号，如上所述，当冲突时编译器需要根据重载的等于号判断），第二是写一个哈希函数。注意 `hash<>()` 的尖括号中的类型匹配。

```c++
struct fff { 
    string x, y;
    int z;
    friend bool operator == (const fff &a, const fff &b) {
        return a.x == b.x || a.y == b.y || a.z == b.z;
    }
};
struct hash_fff { 
    size_t operator()(const fff &p) const { 
        return hash<string>()(p.x) ^ hash<string>()(p.y) ^ hash<int>()(p.z); 
    } 
};
unordered_map<fff, int, hash_fff> mp;
```

#### 对 vector 定义哈希

以下两个方法均可。注意 `hash<>()` 的尖括号中的类型匹配。

```c++
struct hash_vector { 
    size_t operator()(const vector<int> &p) const {
        size_t seed = 0;
        for (auto it : p) {
            seed ^= hash<int>()(it);
        }
        return seed; 
    } 
};
unordered_map<vector<int>, int, hash_vector> mp;
```

```c++
namespace std {
    template<> struct hash<vector<int>> {
        size_t operator()(const vector<int> &p) const {
            size_t seed = 0;
            for (int i : p) {
                seed ^= hash<int>()(i) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
            }
            return seed;
        }
    };
}
unordered_set<vector<int> > S;
```

### 程序标准化

#### 使用 Lambda 函数

- `function` 统一写法

需要注意的是，虽然 `function` 定义时已经声明了返回值类型了，但是有的时候会出错（例如，声明返回 `long long` 但是返回 `int` ，原因没去了解），所以推荐在后面使用 `->` 再行声明一遍。

```c++
function<void(int, int)> clac = [&](int x, int y) -> void {
};
clac(1, 2);

function<bool(int)> dfs = [&](int x) -> bool {
    return dfs(x + 1);
};
dfs(1);
```

- `auto` 非递归写法

不需要使用递归函数时，直接用 `auto` 替换 `function` 即可。

```c++
auto clac = [&](int x, int y) -> void {
};
```

- `auto` 递归写法

相较于 `function` 写法，需要额外引用一遍自身。

```c++
auto dfs = [&](auto self, int x) -> bool {
    return self(self, x + 1);
};
dfs(dfs, 1);
```

#### 使用构造函数

可以将一些必要的声明和预处理放在构造函数，在编译时，无论放置在程序的哪个位置，都会先于主函数进行。下方是我将输入流控制声明的过程。

```c++
int __FAST_IO__ = []() { // 函数名称可以随意修改
    ios::sync_with_stdio(0), cin.tie(0);
    cout.tie(0);
    cout << fixed << setprecision(12);
    freopen("out.txt", "r", stdin);
    freopen("in.txt", "w", stdout);
    return 0;
}();
```

## X - 结论

### STL 与 GNU 扩展选择

标准容器优先满足可移植性；需要排名或第 $k$ 小时使用 PBDS `01C/01D/01E`，需要节点句柄或可合并堆时使用 `01B`。哈希表在数据可能被构造攻击时选带随机盐的 `01F`，按内存局部性再考虑 `01G`；Patricia Trie `01A` 只在确实需要前缀枚举时使用。所有 PBDS 模板依赖 GCC/libstdc++，其余 STL 算法按标准复杂度工作。

<div style="page-break-after:always">/END/</div>
