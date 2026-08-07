<!-- 本文件由生成打印稿.py 生成，请勿直接编辑正文。 -->
<style>
@page {
  size: A4;
  margin: 17mm 14mm 15mm;
  @top-center {
    content: "林月的 XCPC 算法模板";
    color: #555;
    font: 7.8pt "Noto Sans CJK SC", "Microsoft YaHei", sans-serif;
  }
  @bottom-center {
    content: counter(page);
    color: #555;
    font: 7.8pt "Noto Sans CJK SC", "Microsoft YaHei", sans-serif;
  }
}
@page cover {
  margin: 0;
  @top-center { content: none; }
  @bottom-center { content: none; }
}
html, body {
  margin: 0;
  padding: 0;
  color: #222;
  font-family: "Noto Sans CJK SC", "Microsoft YaHei", "SimSun", sans-serif;
  font-size: 9.4pt;
  line-height: 1.45;
}
a { color: #17468c; text-decoration: none; }
p { margin: 1.8mm 0; }
ul, ol { margin: 1.8mm 0; padding-left: 6mm; }
li { margin: .7mm 0; }
h1, h2, h3, h4 { color: #222; break-after: avoid-page; }
h1 { font-size: 18pt; }
h2 { margin: 5mm 0 2mm; font-size: 13.5pt; line-height: 1.3; }
h3 { margin: 4mm 0 1.5mm; font-size: 11.5pt; }
h4 { margin: 3mm 0 1mm; font-size: 10.2pt; }
table { width: 100%; border-collapse: collapse; font-size: 8.3pt; }
th, td { border: 0.2mm solid #bbb; padding: .9mm 1.4mm; vertical-align: top; }
th { background: #eee; font-weight: 700; }
blockquote { margin: 2mm 0; padding: .8mm 2.5mm; border-left: 1mm solid #999; color: #555; }
.katex {
  font-size: 1.03em;
}
.katex-display {
  margin: 2.2mm 0 2.8mm;
  text-align: center;
  break-inside: avoid;
}
.katex-display > .katex {
  max-width: 100%;
  font-size: 1em;
  text-align: center;
}
.katex-display > .katex > .katex-html {
  max-width: 100%;
}
.math-error {
  border: .2mm solid #a00;
  color: #a00;
}
code {
  font-family: "JetBrains Mono", "Cascadia Mono", "Consolas", monospace;
  font-size: .92em;
}
pre.code-block {
  margin: 2mm 0 3mm;
  padding: 1.2mm 0;
  border: .2mm solid #ddd;
  border-radius: .8mm;
  background: #f7f7f7;
  font-size: 7.15pt;
  line-height: 1.27;
  tab-size: 4;
  white-space: normal;
  break-inside: auto;
}
pre.code-block code { font-size: inherit; }
.code-line {
  display: grid;
  grid-template-columns: 7mm minmax(0, 1fr);
  min-height: 1.27em;
  break-inside: avoid;
}
.code-tail {
  display: block;
  break-inside: avoid-page;
}
.line-no {
  padding-right: 1.4mm;
  border-right: .2mm solid #ddd;
  color: #aaa;
  text-align: right;
  user-select: none;
}
.line-src {
  padding: 0 1.8mm;
  white-space: pre-wrap;
  overflow-wrap: anywhere;
}
.cover {
  page: cover;
  page-break-after: always;
  break-after: page;
  box-sizing: border-box;
  min-height: 297mm;
  padding: 34mm 25mm 22mm;
  color: #111;
  background: white;
  display: flex;
  flex-direction: column;
  justify-content: space-between;
  text-align: center;
}
.cover-main { margin-top: 30mm; }
.cover-mark {
  width: 19mm;
  height: 19mm;
  margin: 0 auto 14mm;
  border: .7mm solid #111;
  display: grid;
  place-items: center;
  font: 700 15pt "Times New Roman", serif;
  letter-spacing: .08em;
}
.cover-rule { width: 84mm; height: .8mm; margin: 0 auto 10mm; background: #111; }
.cover h1 { margin: 0; color: #111; font: 700 31pt/1.2 "Noto Serif CJK SC", "SimSun", serif; border: 0; }
.cover-en { margin: 4mm 0 0; color: #333; font: 700 18pt/1.25 "Times New Roman", serif; }
.cover-desc { margin-top: 12mm; color: #555; font-size: 10pt; letter-spacing: .08em; }
.cover-meta { margin: 0; color: #222; font: 700 11pt/1.65 "Times New Roman", "SimSun", serif; }
.toc {
  page-break-after: always;
  break-after: page;
}
.toc > h1 {
  margin: 0 0 2mm;
  border: 0;
  text-align: center;
  font: 700 22pt "Noto Serif CJK SC", "SimSun", serif;
  letter-spacing: .22em;
}
.toc-meta { margin: 0 0 5mm; color: #666; text-align: center; font-size: 7.8pt; }
.toc-columns { column-count: 2; column-gap: 7mm; column-rule: .2mm solid #999; }
.toc-group { margin: 0 0 1.5mm; }
.toc-group h2 {
  display: flex;
  gap: 2mm;
  margin: 1mm 0 .5mm;
  padding: .35mm 1mm;
  border: 0;
  background: #e1e1e1;
  color: #111;
  font-size: 8pt;
  line-height: 1.2;
}
.toc-group h2 a { color: #111; }
.toc-group-page { margin-left: auto; }
.toc-entry {
  display: flex;
  align-items: flex-end;
  gap: .8mm;
  margin: .25mm 0;
  font-size: 7.4pt;
  line-height: 1.22;
  break-inside: avoid;
}
.toc-entry a { min-width: 0; color: #222; }
.toc-dots { flex: 1 1 5mm; min-width: 3mm; margin-bottom: .75mm; border-bottom: .2mm dotted #777; }
.toc-page { flex: none; color: #222; font-family: "Cascadia Mono", "Consolas", monospace; font-variant-numeric: tabular-nums; }
.part-cover {
  page-break-before: always;
  break-before: page;
  margin: 0 0 5mm;
  padding: 0 0 3mm;
  border-bottom: .6mm solid #333;
}
.part-no { color: #666; font: 700 8pt "Cascadia Mono", monospace; letter-spacing: .1em; }
.part-cover h1 { string-set: chapter content(); margin: 2mm 0 1mm; font-size: 18pt; border: 0; }
.part-meta { margin: 0; color: #666; font-size: 7.8pt; }
.template-title { margin-top: 5mm; font-size: 13.5pt; border: 0; }
.template-title code, .doc-title code { color: #222; font: inherit; }
.doc-title { margin-top: 5mm; font-size: 13pt; }
.page-key { color: white; font: 1pt/0 "Arial", sans-serif; }
.toc-detail { table-layout: fixed; }
.toc-detail th:first-child, .toc-detail td:first-child { width: 13mm; }
.toc-detail th:last-child, .toc-detail td:last-child { width: 12mm; text-align: right; white-space: nowrap; }
tr { break-inside: avoid; }
.page-break { display: none; }
</style>

<section class="part-cover" id="part-10b">
  <div class="part-no">PART 10B<span class="page-key">LYPAGE:part-10b:END</span></div>
  <h1>动态规划：优化与最长公共子序列</h1>
  <p class="part-meta">4 个代码模板 · 林月 · v1.0.0 · 2026.8.1</p>
</section>

## 公共粘贴环境

开局只保留完整版与极简版：完整版内联常用宏与函数，并在本地包含 `linyue.hpp`；极简版直接展开以下环境。除文件首部另有依赖说明外，C++ 片段默认接在其中一个版本后；多个备选实现定义同名类型时只选一个。

```cpp
#include <bits/stdc++.h>
using namespace std;
#define int long long
#define pii array<int, 2>
#define endl "\n"
```

普通整数、下标、状态、权值和答案默认都使用宏展开后的 64 位 `int`。只有实际超时、超空间，或需要自然溢出、确定位宽和 128 位中间量时，才局部声明并使用 `i32 / i64 / u32 / u64 / i128 / u128`；固定宽度长类型名只出现在别名声明右侧。

开局头文件不预置短类型别名；`ld` 及上述整型别名均由实际使用它们的模板按需声明。代码不对 `.size()` 结果强转为 `int`；`size_t` 只保留在哈希返回值等标准库接口签名中。可能受 64 位下标影响的模板只在注释中提示卡常改法。

命名约定：模板/类型不超过 10 字符，函数不超过 7 字符，变量、字段、参数和模板参数不超过 3 字符；一般查询用 `ask`，定向查询用 `askMax / askMin / askGCD`，区间更新用 `update`，单点更新用 `modify`。同族模板统一类型和接口名。

## 本册目录

<table class="toc-detail">
<thead><tr><th>类型</th><th>条目</th><th>页码</th></tr></thead>
<tbody>
<tr><td>代码</td><td><a href="#part-10b-template-001"><code>05A - 分治DP优化.cpp</code></a></td><td><span class="toc-page">291</span></td></tr>
<tr><td>代码</td><td><a href="#part-10b-template-002"><code>05B - Knuth优化.cpp</code></a></td><td><span class="toc-page">291</span></td></tr>
<tr><td>代码</td><td><a href="#part-10b-template-003"><code>06A - 最长公共子序列（常规）.cpp</code></a></td><td><span class="toc-page">291</span></td></tr>
<tr><td>代码</td><td><a href="#part-10b-template-004"><code>06B - 最长公共子序列（Bitset）.cpp</code></a></td><td><span class="toc-page">292</span></td></tr>
</tbody>
</table>

<h2 id="part-10b-template-001" class="template-title"><code>05A - 分治DP优化.cpp</code><span class="page-key">LYPAGE:part-10b-template-001:END</span></h2>

```cpp
/*
用途：计算 g[j] = min_{0<=k<j}(f[k] + cost(k+1,j))。
前提：最优决策点随 j 单调；单调性不成立时结果错误。
调用：divide(1,n,0,n-1,f,g,p,cst,inf)。
复杂度：每一层 DP O(n log n) 次转移计算，常见进一步可证为 O(n log n)。
性能：默认使用宏展开后的 int；若大规模数据因此超时或超空间，可将纯下标、状态或计数局部改为 i32。
*/
template <class T, class C>
void divide(int l, int r, int ql, int qr,
              const vector<T>& f, vector<T>& g, vector<int>& p,
              C cst, T inf) {
    if (l > r) return;
    int m = (l + r) >> 1;
    pair<T, int> z{inf, -1};
    int up = min(qr, m - 1);
    for (int k = ql; k <= up; ++k) {
        if (f[k] == inf) continue;
        z = min(z, pair<T, int>{f[k] + cst(k + 1, m), k});
    }
    g[m] = z.first;
    p[m] = z.second;
    if (l == r) return;
    int q = z.second == -1 ? ql : z.second;
    divide(l, m - 1, ql, q, f, g, p, cst, inf);
    divide(m + 1, r, q, qr, f, g, p, cst, inf);
}
```

<h2 id="part-10b-template-002" class="template-title"><code>05B - Knuth优化.cpp</code><span class="page-key">LYPAGE:part-10b-template-002:END</span></h2>

```cpp
/*
用途：区间 DP：f[l][r]=min(f[l][k]+f[k+1][r])+cost(l,r)。
前提：cost 满足四边形不等式且区间包含单调，因而 opt[l][r-1]<=opt[l][r]<=opt[l+1][r]。
下标：0..n-1。复杂度 O(n^2) 时间、O(n^2) 空间。
性能：默认使用宏展开后的 int；若大规模数据因此超时或超空间，可将纯下标、状态或计数局部改为 i32。
*/
template <class T, class C>
pair<vector<vector<T>>, vector<vector<int>>> knuthDP(int n, C cst) {
    assert(n >= 0);
    vector f(n, vector<T>(n));
    vector p(n, vector<int>(n));
    for (int i = 0; i < n; ++i) p[i][i] = i;
    for (int d = 1; d < n; ++d) {
        for (int l = 0, r = d; r < n; ++l, ++r) {
            int a = p[l][r - 1];
            int b = min<int>(r - 1, p[l + 1][r]);
            f[l][r] = numeric_limits<T>::max() / 4;
            for (int k = a; k <= b; ++k) {
                T x = f[l][k] + f[k + 1][r] + cst(l, r);
                if (x < f[l][r]) f[l][r] = x, p[l][r] = k;
            }
        }
    }
    return {move(f), move(p)};
}
```

<h2 id="part-10b-template-003" class="template-title"><code>06A - 最长公共子序列（常规）.cpp</code><span class="page-key">LYPAGE:part-10b-template-003:END</span></h2>

```cpp
/*
用途：任意可比较序列的 LCS；返回一组最长公共子序列。
复杂度：O(nm) 时间、O(nm) 空间。只求字符串长度且 m 很大时使用 06B。
性能：默认使用宏展开后的 int；若大规模数据因此超时或超空间，可将纯下标、状态或计数局部改为 i32。
*/
template <class Seq>
vector<typename Seq::value_type> lcs(const Seq& a, const Seq& b) {
    int n = a.size(), m = b.size();
    vector f(n + 1, vector<int>(m + 1));
    for (int i = 1; i <= n; ++i) {
        for (int j = 1; j <= m; ++j) {
            if (a[i - 1] == b[j - 1]) f[i][j] = f[i - 1][j - 1] + 1;
            else f[i][j] = max(f[i - 1][j], f[i][j - 1]);
        }
    }
    vector<typename Seq::value_type> r;
    for (int i = n, j = m; i && j;) {
        if (a[i - 1] == b[j - 1]) r.push_back(a[--i]), --j;
        else if (f[i - 1][j] >= f[i][j - 1]) --i;
        else --j;
    }
    reverse(r.begin(), r.end());
    return r;
}
```

<h2 id="part-10b-template-004" class="template-title"><code>06B - 最长公共子序列（Bitset）.cpp</code><span class="page-key">LYPAGE:part-10b-template-004:END</span></h2>

```cpp
/*
用途：只求两个字节串的 LCS 长度；利用 64 位并行优化。
复杂度：O(|a||b|/64 + |b|)，空间 O(256|b|/64)。
大位集必须使用 u64；若被 #define int long long 替换会显著增大内存和降低速度。
性能：默认使用宏展开后的 int；若大规模数据因此超时或超空间，可将纯下标、状态或计数局部改为 i32。
*/

using u64 = uint64_t;

int lcs(const string& a, const string& b) {
    int n = b.size(), z = (n + 63) >> 6;
    vector<vector<u64>> p(256, vector<u64>(z));
    for (int i = 0; i < n; ++i) p[(unsigned char)b[i]][i >> 6] |= u64(1) << (i & 63);
    vector<u64> s(z), x(z), y(z), d(z);
    for (unsigned char c : a) {
        u64 q = 1;
        for (int i = 0; i < z; ++i) {
            x[i] = p[c][i] | s[i];
            u64 nq = s[i] >> 63;
            y[i] = (s[i] << 1) | q;
            q = nq;
        }
        u64 br = 0;
        for (int i = 0; i < z; ++i) {
            u64 t = y[i] + br;
            u64 ov = t < y[i];
            d[i] = x[i] - t;
            br = ov || x[i] < t;
            s[i] = x[i] & ~d[i];
        }
    }
    int r = 0;
    for (u64 x : s) r += __builtin_popcountll(x);
    return r;
}
```
