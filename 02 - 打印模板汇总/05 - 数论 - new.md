## 数论

### 常见数列

#### 调和级数

满足调和级数 $\mathcal O\left( \dfrac{N}{1} +\dfrac{N}{2}+\dfrac{N}{3}+\dots + \dfrac{N}{N} \right)$，可以用 $ \approx N\ln N$ 来拟合，但是会略小，误差量级在 $10\%$ 左右。本地可以在500ms内完成 $10^8$​ 量级的预处理计算。

调和数 $H_N=\sum\limits_{k=1}^{N}\frac{1}{k}=\ln N+\gamma+\frac1{2N}-\frac1{12N^2}+O(N^{-4})$，$\gamma\approx0.5772156649$ 。

调和级数 $D(N)=\sum_{i=1}^N\lfloor N/i\rfloor=\sum_{i=1}^N\tau(i)=N\ln N+(2\gamma-1)N+O(\sqrt N)$

注意 $NH_N\ne D(N)$ 。

| N的量级 |  1   |  2   |   3   |   4    |     5     |     6      |      7      |       8       |       9        |
| :-----: | :--: | :--: | :---: | :----: | :-------: | :--------: | :---------: | :-----------: | :------------: |
| 累加和  |  27  | 482  | 7’069 | 93‘668 | 1’166‘750 | 13‘970’034 | 162‘725’364 | 1‘857’511‘568 | 20’877‘697’634 |

下方示例为求解 $1$ 到 $N$ 中各个数字的因数值。

```c++
const int N = 1E5;
vector<vector<int>> dic(N + 1);
for (int i = 1; i <= N; i++) {
    for (int j = i; j <= N; j += i) {
        dic[j].push_back(i);
    }
}
```

#### 素数密度与分布

|      N的量级      |  1   |  2   |  3   |   4   |   5   |   6    |    7    |     8     |     9      |
| :---------------: | :--: | :--: | :--: | :---: | :---: | :----: | :-----: | :-------: | :--------: |
| 素数数量 $\pi(N)$ |  4   |  25  | 168  | 1‘229 | 9’592 | 78‘498 | 664’579 | 5‘761’455 | 50‘847’534 |

除此之外，对于任意两个相邻的素数 $p_1,p_2 \le 10^9$ ，有 $|p_1-p_2|<300$ 成立，更具体的说，最大的差值为 $282$ 。

#### 因数最多数字与其因数数量

|        N的量级         |  1   |  2   |  3   |     4      |      5       |                   6                    |  7   |
| :--------------------: | :--: | :--: | :--: | :--------: | :----------: | :------------------------------------: | :--: |
| 因数最多数字的因数数量 |  4   |  12  |  32  |     64     |     128      |                  240                   | 448  |
|     因数最多的数字     |  -   |  -   |  -   | 7560, 9240 | 83160, 98280 | 720720, 831600, 942480, 982800, 997920 |  -   |

### 快速幂

#### 常规

```cpp
int power(int a, int b, int m) {
    a %= m;
    int r = 1 % m;
    for (; b; b >>= 1, a = a * a % m)
        if (b & 1) r = r * a % m;
    return r;
}
```

#### 防爆

```cpp
int mul(int a, int b, int m) {
    a %= m, b %= m;
    int r = a * b - m * (int)(1.0L / m * a * b);
    return r - m * (r >= m) + m * (r < 0);
}
int mul(int a, int b, int m) {
    return (__int128_t)a * b % m;
}
int power(int a, int b, int m) {
    a %= m;
    int r = 1 % m;
    for (; b; b >>= 1, a = mul(a, a, m))
        if (b & 1) r = mul(r, a, m);
	return r;
}
```

#### 十进制大指数

逐位维护 $r\leftarrow r^{10}a^d$。

```cpp
int mul(int a, int b, int m) {
    a %= m, b %= m;
    int r = a * b - m * (int)(1.0L / m * a * b);
    return r - m * (r >= m) + m * (r < 0);
}
int power(int a, int b, int m) {
    a %= m;
    int r = 1 % m;
    for (; b; b >>= 1, a = mul(a, a, m))
        if (b & 1) r = mul(r, a, m);
	return r;
}
int power_(int a, const string &b, int m) {
    a %= m;
    int r = 1 % m;
    for (auto v : b) r = mul(power(r, 10, m), power(a, c - '0', m), m);
    return r;
}
```

### 质数判定

#### 试除法

时间 $\mathcal O(\sqrt n)$ ，==常数优化版本==可达 $\mathcal O(\frac {\sqrt n}{3})$。

```cpp
bool isprime(int n) {
    if (n < 2) return false;
    for (int p = 2; p <= n / p; ++p)
        if (n % p == 0) return false;
    return true;
}
```

#### 线性筛

最小质因子、欧拉函数、莫比乌斯函数

```cpp
constexpr int N = 2e5;

vector<int> primes, spf(N + 1), phi(N + 1), mu(N + 1);

void init() {
    phi[1] = mu[1] = 1;
    for (int i = 2; i <= N; ++i) {
        if (!spf[i]) {
            spf[i] = i, primes.push_back(i);
            phi[i] = i - 1, mu[i] = -1;
        }
        for (auto p : primes) {
            if (p > N / i) break;
            int x = p * i;
            spf[x] = p;
            if (p == spf[i]) {
                phi[x] = phi[i] * p;
                break;
            }
            phi[x] = phi[i] * (p - 1);
            mu[x] = -mu[i];
		}
    }
}
```

#### Miller–Rabin

随机化验证，非严谨计算的平均复杂度约为 $\mathcal O (3.5 \times \log X)$ 。对于某些强力质数，可能会退化至约 $\mathcal O(35 \times \log X)$ 。有==常数优化版本==可以再快五倍。

```cpp
int mul(int a, int b, int m) {
    a %= m, b %= m;
    int r = a * b - m * (int)(1.0L / m * a * b);
    return r - m * (r >= m) + m * (r < 0);
}
int power(int a, int b, int m) {
    a %= m;
    int r = 1 % m;
    for (; b; b >>= 1, a = mul(a, a, m))
        if (b & 1) r = mul(r, a, m);
	return r;
}
bool isprime(int n) {
    if (n < 2 || n % 2 == 0) return n == 2;
    int d = n - 1;
    int s = __builtin_ctzll(d);
    d >>= s;
    for (int a : {2, 325, 9375, 28178, 450775, 9780504, 1795265022}) {
        if (a % n == 0) continue;
        int x = power(a, d, n);
        if (x == 1 || x == n - 1) continue;
        for (int r = 1; r < s && x != n - 1; ++r) x = mul(x, x, n);
        if (x != n - 1) return false;
    }
    return true;
}
```

### 质因子分解与约数

#### 试除法

时间复杂度 $\mathcal O(n)$ ，注意不要忘记大质数的情况。

```cpp
vector<pii> fact(int x) {
    vector<pii> r;
    for (int p = 2; p <= x / p; ++p) {
        if (x % p) continue;
        int c = 0;
        do {
            x /= p, ++c; 
        } while (x % p == 0);
        r.push_back({p, c});
    }
    if (n > 1) f.push_back({n, 1});
    return f;
}
```

#### 筛法

需要预处理最小质因子 $\textrm{spf}$ 表，单次查询时间 $\mathcal O(\log x)$。

```cpp
vector<pii> fact(int x) {
    vector<pii> res;
    while (x > 1) {
        int p = spf[x], e = 0;
        do {
            x /= p, ++e;
        } while (x % p == 0);
        res.push_back({p, e});
    }
    return res;
}
```

#### Pollard–Rho

找一个非平凡因子的常用期望估计为 $O(n^{1/4})$ 次模运算。

```cpp
mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());

int rho(int n) {
    if (n % 2 == 0) return 2;
    if (n % 3 == 0) return 3;
    while (true) {
        int c = rng() % (n - 1) + 1, x = rng() % n, y = x, d = 1;
        auto f = [&](int v) -> int { return mul(v, v, n) + c; };
        for (int len = 1; d == 1; len = min(2 * len, 64LL)) {
            int q = 1;
            for (int i = 0; i < len; ++i) {
                x = f(x), y = f(f(y));
                q = mul(q, abs(x - y), n);
            }
            d = gcd(q, n);
        }
        if (d != n) return d;
    }
}

vector<int> fact(int n) {
    vector<int> f;
    auto dfs = [&](auto &&self, int x) -> void {
        if (x == 1) return;
        if (isprime(x)) return f.push_back(x);
        int d = rho(x);
        self(self, d), self(self, x / d);
    };
    dfs(dfs, n);
    sort(f.begin(), f.end());
    return f;
}
```

#### 枚举约数、约数个数与约数和

若 $n=\prod p_i^{e_i}$，则 $\tau(n)=\prod(e_i+1)$，$\sigma(n)=\prod(1+p_i+\cdots+p_i^{e_i})$。模意义下求等比和时，分母 $p_i-1$ 不可逆就逐项递推。

```cpp
vector<int> divisors(const vector<pii> &f) {
    vector<int> d{1};
    for (auto [p, e] : f) {
        int sz = d.size();
        int pe = 1;
        for (int k = 1; k <= e; ++k) {
            pe *= p;
            for (int i = 0; i < sz; ++i) d.push_back(d[i] * pe);
        }
    }
    return d;
}
```

### 裴蜀定理

> $ax+by=c\ (x \in Z^∗,y \in Z^∗)$ 成立的充要条件是 $gcd⁡(a, b) ∣ c$（ $Z^*$ 表示正整数集）；这不保证有非负解或正整数解。

$a_i$​ 不全为零时，整数线性组合能取得的最小正值为 $\gcd(|a_1|,\ldots,|a_n|)$​​。

例题：给定一个序列 $a$，找到一个序列 $x$，使得 $\sum_{i = 1}^n a_ix_i$ 最小。

```cpp
int n, ans = 0;
cin >> n;
for (int i = 0, x; i < n; ++i) {
    cin >> x;
    x = x > 0 ? x : -x;
    ans = gcd(ans, x);
}
cout << ans << endl;
```

### 扩展欧几里得

求解形如 $a\cdot x + b\cdot y = \gcd(a,b)$ 的不定方程的任意一组解。

```cpp
int exgcd(int a, int b, int &x, int &y) {
    if (!b) {
        x = 1, y = 0;
        return a;
    }
    int d = exgcd(b, a % b, y, x);
    y -= a / b * x;
    return d;
}
```

例题：求解二元一次不定方程 $a\cdot x + b\cdot y = c$ 。

若 $d\mid c$，把特解乘 $c/d$ 得 $(x_0,y_0)$，通解为
$$
x=x_0+t\frac bd,\qquad y=y_0-t\frac ad,\qquad t\in\mathbb Z.
$$
系数为负时，先对绝对值求解，再改变对应解的符号。缩放特解可能需要 `i128`。

当 $a,b>0$ 时，正整数解对应
$$
\left\lceil\frac{1-x_0}{b/d}\right\rceil\le t\le
\left\lfloor\frac{y_0-1}{a/d}\right\rfloor.
$$
```cpp
int floor(int a, int b) {
    return a / b - (a % b < 0);
}
int ceil(int a, int b) {
    return a / b + (a % b > 0);
}
```

### 逆元

#### 费马小定理

$\textrm{MOD}$ 必须是质数，且需要满足 $x$ 与 $MOD$ 互质，时间复杂度 $\mathcal O(\log X)$ 。

```cpp
int inv(int x) { return power(x, mod - 2, mod); }
```

#### 扩展欧几里得

$\textrm{MOD}$ 没有限制，复杂度为 $\mathcal O(\log X)$ ，但是比快速幂法常数大一些。

```cpp
int exgcd(int a, int b, int &x, int &y) {
    if (!b) {
        x = 1, y = 0;
        return a;
    }
    int d = exgcd(b, a % b, y, x);
    y -= a / b * x;
    return d;
}
int inv(int a, int m) {
    int x, y, d = exgcd(a, m, x, y);
    return d == 1 ? (x % m + m) % m : -1;
}
```

#### 线性递推

以 $\mathcal O(N)$ 的复杂度完成 $1-N$ 中全部逆元的计算。

```cpp
inv[1] = 1;
for (int i = 2; i <= n; ++i) 
    inv[i] = (p - p / i) * inv[p % i] % p;
```

#### 全值域 $O(1)$ 逆元（固定 $998244353$）

适合对整个有限域的大量随机元素求逆；只查 $1\ldots n$ 时用上一节。固定 $p=998244353$、$B=1024$，预处理时间、空间 $O(B^2)$，查询 $1\le x<p$ 为 $O(1)$，两张表约占 $12$ MiB。

对块首 $a=B\lfloor x/B\rfloor$，预先找 $1\le u\le B$ 使 $ua\bmod p$ 距离 $0$ 不超过 $B^2$，于是 $ux\bmod p$ 距离 $0$ 不超过 $2B^2$。先查小整数逆元，再乘 $u$；负方向使用 $(-v)^{-1}=-v^{-1}$。

```cpp
constexpr int P = 998244353, B = 1024, L = 2 * B * B;
vector<int> inv(L + 1), u((P - 1) / B + 1, 1);
void init() {
    inv[1] = 1;
    for (int i = 2; i <= L; ++i) inv[i] = P - (P / i) * inv[P % i] % P;
    for (int k = 1; k <= B; ++k) {
        int step = k * B;
        for (int j = 0; j < u.size(); ++j) {
            int r = step * j % P;
            if (r <= B * B)
                u[j] = k;
            else if (r >= P - B * B)
                u[j] = P - k;
            else
                j += (P - B * B - r) / step;
        }
    }
}
int inv(int x) {
    int k = u[x / B], v = k * x % P;
    return k * (v <= L ? inv[v] : P - inv[P - v]) % P;
}
```

### 同余方程组、扩展中国剩余定理

公式：$x \equiv b_i(\bmod\ a_i)$ ，即 $(x - b_i) \mid a_i$ 。

```cpp
pii excrt(const vector<pii> &eq) {
    int r = 0, M = 1;
    for (auto [m, a] : eq) {
        int x, y, d = exgcd(M, m, x, y), del = a - r;
        if (del % d) return {-1, 0};
        int q = m / d;
        int t = x * (del / d) % q;
        if (t < 0) t += q;
        r += M * t;
        M *= q;
    }
    return {r, M};
}
```

### 离散对数 bsgs 与 exbsgs

求 $a^x\equiv b\pmod m$ 的**最小非负**整数解，无解返回 $-1$。其中标准 $\tt BSGS$ 算法不能计算 $a$ 与 $\textrm{MOD}$ 互质的情况，而 $\tt exBSGS$ 则可以。

```cpp
int bsgs(int a, int b, int m) {
    if (m == 1) return 0;
    int s = mysqrt(m) + 1, cur = 1;
    unordered_map<int, int> pos;
    for (int j = 0; j < s; ++j) {
        pos.emplace(cur, j);
        cur = mul(cur, a, m);
    }
    int step = inv(power(a, s, m), m);
    cur = b;
    for (int i = 0; i < s; ++i) {
        if (auto it = pos.find(cur); it != pos.end()) return i * s + it->second;
        cur = mul(cur, step, m);
    }
    return -1;
}

int exbsgs(int a, int b, int m) {
    if (m == 1 || b == 1) return 0;
    int k = 0, v = 1;
    for (int d; (d = gcd(a, m)) > 1;) {
        if (b % d) return -1;
        b /= d, m /= d, ++k;
        v = mul(v, a / d % m, m);
        if (v == b) return k;
    }
    int x = bsgs(a % m, mul(b, inv(v, m), m), m);
    return x < 0 ? -1 : x + k;
}
```

#### Pohlig–Hellman（光滑群阶）

若 $p-1=\prod q_i^{e_i}$，逐位求 $x\bmod q_i^{e_i}$，再 $\texttt{CRT}$ 合并。仅在群阶的质因子较小时有优势，逐位枚举的时间为 $O(\sum e_i(\log p+q_i))$ 次模运算。

此处 $P-1 = 2^{23}\times 7^1\times 17^1$ 。

```cpp
constexpr int P = 998244353, G = 3;
int log(int a) {
    vector<pii> eq = {{2, 23}, {7, 1}, {17, 1}};
    for (auto [q, e] : eq) {
        int x = 0, pe = 1, base = power(G, (P - 1) / q, P);
        for (int i = 0; i < e; ++i, pe *= q) {
            int t = (P - 1) / (pe * q);
            int target = power(a, t, P), cur = power(G, x * t, P);
            for (int d = 0; d < q; ++d, cur = cur * base % P) {
                if (cur == target) {
                    x += d * pe;
                    break;
                }
            }
        }
        eq.push_back({pe, x});
    }
    return excrt(eq)[0];
}
```

### 模平方根

无解返回 $-1$，否则返回较小根，另一根为 $(p-x)\bmod p$ ，适用于质数模。

#### Tonelli–Shanks

```cpp
int tonelli(int a, int p) {
    if (a == 0 || p == 2) return a;
    if (power(a, (p - 1) / 2, p) != 1) return -1;
    if (p % 4 == 3) {
        int x = power(a, (p + 1LL) / 4, p);
        return min(x, p - x);
    }
    int s = __builtin_ctzll(p - 1), q = (p - 1) >> s;
    int z = 2;
    while (power(z, (p - 1) / 2, p) == 1) ++z;
    int x = power(a, (q + 1) / 2, p), b = power(a, q, p), c = power(z, q, p);
    while (b != 1) {
        int i = 0;
        for (int v = b; v != 1; v = v * v % p) ++i;
        int t = power(c, 1LL << (s - i - 1), p);
        x = x * t % p, c = t * t % p, b = b * c % p, s = i;
    }
    return min(x, p - x);
}
```

#### Cipolla

随机找 $w=t^2-a$ 为非二次剩余，在 $\mathbb F_p[\sqrt w]$ 中求 $(t+\sqrt w)^{(p+1)/2}$。期望 $O(\log p)$ 次模乘。

```cpp
mt19937_64 rng(chrono::steady_clock::now().time_since_epoch().count());
int cipolla(int a, int p) {
    if (a == 0 || p == 2) return a;
    if (power(a, (p - 1) / 2, p) != 1) return -1;
    int t, w;
    do {
        t = rng() % p;
        w = (t * t % p - a + p) % p;
    } while (power(w, (p - 1) / 2, p) != p - 1);
    auto prod = [&](pii x, pii y) -> pii {
        return {(x[0] * y[0] + x[1] * y[1] % p * w) % p,
                (x[0] * y[1] + x[1] * y[0]) % p};
    };
    pii r{1, 0}, b{t, 1};
    for (int e = (p + 1LL) / 2; e; e >>= 1, b = prod(b, b))
        if (e & 1) r = prod(r, b);
    return min(r[0], p - r[0]);
}
```

### 欧拉函数、原根与乘法阶

#### 单点欧拉函数

$\varphi(1)=1$，$\varphi(n)=n\prod_{p\mid n}(1-1/p)$ ，时间 $O(\sqrt n)$

```cpp
int phi(int n) {
    int r = n;
    for (int p = 2; p <= n / p; ++p) {
        if (n % p) continue;
        do {
            n /= p; 
        } while (n % p == 0);
        r = r / p * (p - 1);
    }
    if (n > 1) r = r / n * (n - 1);
    return r;
}
```

#### 原根

$m\ge2$ 存在原根当且仅当 $m=2,4,p^k,2p^k$，其中 $p$ 为奇质数。返回最小原根，不存在返回 $-1$。

```cpp
int primitive_root(int m) {
    if (m == 2) return 1;
    if (m == 4) return 3;
    int odd = m / (m % 2 == 0 ? 2 : 1);
    auto f = fact(odd);
    if (odd % 2 == 0 || f.size() != 1) return -1;
    int ph = phi(m);
    auto fac = fact(ph);
    for (int g = 2; g < m; ++g) {
        if (gcd(g, m) != 1) continue;
        bool ok = true;
        for (auto [p, e] : fac)
            if (power(g, ph / p, m) == 1) { ok = false; break; }
        if (ok) return g;
    }
    return -1;
}
```

若 $g$ 为原根，全部原根为 $g^k\bmod m$（$1\le k\le\varphi(m)$，$\gcd(k,\varphi(m))=1$），共 $\varphi(\varphi(m))$ 个。逐次乘 $g$ 即可枚举，无须重复求模幂。

#### 最小乘法阶

要求 $\gcd(a,m)=1$。试除分解适合较小模数，大数时将分解换成 $\texttt{Pollard–Rho}$ 。

```cpp
int order(int a, int m) {
    int k = phi(m);
    for (auto [p, e] : fact(k))
        while (k % p == 0 && power(a, k / p, m) == 1) k /= p;
    return k;
}
```

### 值域预处理 GCD

适合海量查询 `gcd(x,y)`，其中 $0\le x\le V$、$0\le y\le2^{63}-1$，预处理上界 $1\le V\le2^{31}-1$ 且数组能开下。预处理 $O(V)$ 时间、空间，单次 $O(1)$；常见 $V=10^6$。负数先取绝对值。

线性筛把 $x$ 拆为三个因子，每个因子要么为质数、要么不超过 $\lceil\sqrt V\rceil$。质数因子一次取模判断，小因子查 GCD 表；每提取一部分公因子都先从 $y$ 中除去，避免重复计数。

```cpp
namespace GCD {
    constexpr int N = 1e6;
    constexpr int b = 1e3 + 1;
    vector<int> spf(N + 1), primes;
    vector<array<int, 3>> fac(N + 1);
    vector<vector<int>> gd(b + 1, vector<int>(b + 1));
    void init() {
        for (int i = 0; i <= b; ++i) gd[i][0] = gd[0][i] = i;
        for (int i = 1; i <= b; ++i)
            for (int j = i; j <= b; ++j) gd[i][j] = gd[j][i] = gd[i][j % i];
        fac[1] = {1, 1, 1};
        for (int i = 2; i <= N; ++i) {
            if (!spf[i]) {
                spf[i] = i, primes.push_back(i);
                fac[i] = {1, 1, (int)i};
            }
            for (int p : primes) {
                if (p > N / i) break;
                int v = i * p;
                spf[v] = p, fac[v] = fac[i];
                *min_element(fac[v].begin(), fac[v].end()) *= p;
                if (p == spf[i]) break;
            }
        }
    }
    int ask(int x, int y) const {
        if (x == 0) return y;
        int ans = 1;
        for (int v : fac[x]) {
            int d = spf[v] == v ? (y % v == 0 ? v : 1) : gd[v][y % v];
            ans *= d, y /= d;
        }
        return ans;
    }
}; // namespace GCD
```

### 自动取模与 Barrett 约简

#### 固定质数模数

```cpp
constexpr int mod = 998244353;
struct Z {
    int x;
    Z(int v = 0) : x(v % mod) { if (x < 0) x += mod; }
    Z operator-() const { return Z(-x); }
    Z &operator+=(Z a) { x += a.x; if (x >= mod) x -= mod; return *this; }
    Z &operator-=(Z a) { x -= a.x; if (x < 0) x += mod; return *this; }
    Z &operator*=(Z a) { x = x * a.x % mod; return *this; }
    Z pow(int e) const {
        Z a = *this, r = 1;
        for (; e; e >>= 1, a *= a) if (e & 1) r *= a;
        return r;
    }
    Z inv() const { return pow(mod - 2); }
    Z &operator/=(Z a) { return *this *= a.inv(); }
    friend Z operator+(Z a, Z b) { return a += b; }
    friend Z operator-(Z a, Z b) { return a -= b; }
    friend Z operator*(Z a, Z b) { return a *= b; }
    friend Z operator/(Z a, Z b) { return a /= b; }
    friend bool operator==(Z a, Z b) { return a.x == b.x; }
    friend bool operator!=(Z a, Z b) { return a.x != b.x; }
    friend istream &operator>>(istream &is, Z &a) { int v; is >> v; a = Z(v); return is; }
    friend ostream &operator<<(ostream &os, Z a) { return os << a.x; }
};
```

#### Barrett 约减

```cpp
struct Barrett {
    u64 m;
    u128 B;
    Barrett(u64 m) : m(m), B((u128(1) << 64) / m) {}
    u64 reduce(u64 x) const {
        u64 q = B * x >> 64, r = x - q * m;
        return r < m ? r : r - m;
    }
};
```

### 组合数

#### debug

提供一组测试数据：$\binom{132}{66}=$ 377'389'666'165'540'953'244'592'352'291'892'721'700，模数为 $998244353$ 时为 $241'200'029$；$10^9+7$ 时为 $598375978$。

#### 阶乘与逆阶乘

质数模数，$\mathcal O(n)$ 预处理， $\mathcal O(1)$ 查询。

```cpp
struct Comb {
    int n;
    vector<int> _fac, _ifac, _inv;

    Comb() : n{0}, _fac{1}, _ifac{1}, _inv{0} {}
    Comb(int n) : Comb() {
        init(n);
    }

    void init(int m) {
        if (m <= n) return;
        _fac.resize(m + 1);
        _ifac.resize(m + 1);
        _inv.resize(m + 1);

        for (int i = n + 1; i <= m; ++i) {
            _fac[i] = _fac[i - 1] * i % mod;
        }
        _ifac[m] = inv(_fac[m], m);
        for (int i = m; i > n; --i) {
            _ifac[i - 1] = _ifac[i] * i % mod;
            _inv[i] = _ifac[i] * _fac[i - 1] % mod;
        }
        n = m;
    }

    int fac(int m) {
        if (m > n) init(m);
        return _fac[m];
    }
    int ifac(int m) {
        if (m > n) init(m);
        return _ifac[m];
    }
    int inv(int m) {
        if (m > n) init(m);
        return _inv[m];
    }
    int P(int n, int m) {
        if (n < 0 || m < 0 || m > n) return 0;
        return fac(n) * ifac(n - m) % mod;
    }
    int C(int n, int m) {
        if (n < 0 || m < 0 || m > n) return 0;
        return fac(n) * ifac(m) % mod * ifac(n - m) % mod;
    }
} C;
```

#### Lucas 定理

处理 $n$ 极大的情况。 

```cpp
auto C = [&](int n, int k) -> C {
    if (k < 0 || k > n) return Z(0);
    Z r = 1;
    while (n || k) {
        int x = n % p, y = k % p;
        if (y > x) return Z(0);
        r *= C.C(x, y);
        n /= p;
        k /= p;
    }
    return r;
}
```

#### 任意模数

用 Legendre 公式 $v_p(n!)=\sum_{j\ge1}\lfloor n/p^j\rfloor$ 求每个质因子的指数再相乘。

```cpp
int fact(int n, int p) {
    int ans = 0;
    while (n) n /= p, ans += n;
    return ans;
}
int C(int n, int k, int m) {
    if (k < 0 || k > n) return 0;
    int ans = 1 % m;
    for (int p : primes) {
        if (p > n) break;
        int e = fact(n, p) - fact(k, p) - fact(n - k, p);
        ans = mul(ans, power(p, e, m), m);
    }
    return ans;
}
```

#### exLucas

处理模数为合数的情况。

```cpp
struct ExLucas {
    struct Node { int p, pk, e; vector<int> f; };
    vector<Node> a;
    ExLucas(int m) {
        for (auto [p, e] : factor_trial(m)) {
            int pk = 1;
            for (int i = 0; i < e; ++i) pk *= p;
            Node x{p, pk, e, vector<int>(pk + 1, 1)};
            for (int i = 1; i <= pk; ++i)
                x.f[i] = x.f[i - 1] * (i % p ? i : 1) % pk;
            a.push_back(move(x));
        }
    }
    int fact(int n, const Node &x) const {
        int ans = 1;
        for (; n; n /= x.p)
            ans = ans * power(x.f[x.pk], n / x.pk, x.pk) % x.pk * x.f[n % x.pk] % x.pk;
        return ans;
    }
    int C(int n, int k) const {
        if (k < 0 || k > n) return 0;
        vector<pii> eq;
        for (const auto &x : a) {
            int e = factorial_vp(n, x.p) - factorial_vp(k, x.p) - factorial_vp(n - k, x.p);
            int r = 0;
            if (e < x.e) {
                r = fact(n, x);
                r = r * inv_mod(fact(k, x), x.pk) % x.pk;
                r = r * inv_mod(fact(n - k, x), x.pk) % x.pk;
                r = r * power(x.p, e, x.pk) % x.pk;
            }
            eq.push_back({x.pk, r});
        }
        return excrt(eq)[0];
    }
};
```

#### 杨辉三角

无需逆元；精确计算时完整第 $66$ 行能放入有符号 64 位，第 $130$ 行能放入有符号 128 位。更大时用高精度，模意义下可直接把元素类型改成 `Z`。

```cpp
vector<vector<i128>> pascal(int n) {
    vector<vector<i128>> c(n + 1, vector<i128>(n + 1));
    c[0][0] = 1;
    for (int i = 1; i <= n; ++i) {
        c[i][0] = 1;
        for (int j = 1; j <= i; ++j) c[i][j] = c[i - 1][j - 1] + c[i - 1][j];
    }
    return c;
}
```

### 整除分块与莫比乌斯反演

#### 整除分块

$\displaystyle \left\lfloor \frac{n}{l} \right\rfloor = \left\lfloor \frac{n}{l + 1} \right\rfloor = ... = \left\lfloor \frac{n}{r} \right\rfloor \iff \left\lfloor \frac{n}{l} \right\rfloor \le \frac{n}{r} < \left\lfloor \frac{n}{l} \right\rfloor + 1$ ，根据不等式左侧，得到 $\displaystyle r \le \left\lfloor \frac{n}{\lfloor \frac{n}{l} \rfloor} \right\rfloor$ 。

```cpp
void solve() {
    int n;
    cin >> n;
    int ans = 0;
    for (int l = 1, r; l <= n; l = r + 1) {
        r = n / (n / l);
        ans += (j - l + 1) * (n / i);
    }
    cout << ans << endl;
}
```

#### 莫比乌斯反演与常用卷积等式

莫比乌斯函数定义：$\displaystyle {\mu(n) = \begin{cases} 1 &n = 1 \\ (-1)^k &n = \prod_{i = 1}^k p_i \text{ 且 } p_i \text{ 互质 } \\ 0 &else \end{cases}}$ 。

> 莫比乌斯函数性质：对于任意正整数 $n$ 满足 $\displaystyle {\sum_{d|n}\mu(d) = \begin{cases} 1 & n = 1 \\ 0 & n \neq 1\end{cases}}$ ；$\displaystyle {\sum_{d|n} \frac{\mu(d)}{d} = \frac{\varphi(n)}{n}}$; 
>
> $\displaystyle {[\gcd(i,j)=k]=\sum_{d\mid\gcd(i/k,j/k)}\mu(d)},\; k\mid i\;\texttt{and}\;k\mid j $ 。 

$$
F(n)=\sum_{d\mid n}f(d)
\iff f(n)=\sum_{d\mid n}\mu(d)F(n/d).\\
G(n)=\sum_{k\ge1}f(nk)\iff f(n)=\sum_{k\ge1}\mu(k)G(nk)
$$
用于推导，记 $(f*g)(n)=\sum_{d\mid n}f(d)g(n/d)$，$\mathbf1(n)=1$，$id(n)=n$，$\varepsilon(n)=[n=1]$：
$$
\mu*\mathbf1=\varepsilon,\quad \varphi*\mathbf1=id,\quad
\mu*id=\varphi,\quad \mathbf1*\mathbf1=\tau,\quad \mathbf1*id=\sigma.
$$

例：令 $N=\lfloor n/k\rfloor,M=\lfloor m/k\rfloor$，则
$$
\sum_{x=1}^n\sum_{y=1}^m[\gcd(x,y)=k]
=\sum_{d=1}^{\min(N,M)}\mu(d)\left\lfloor\frac Nd\right\rfloor\left\lfloor\frac Md\right\rfloor.
$$
```cpp
constexpr int N = 2e5;

vector<int> primes, spf(N + 1), mu(N + 1);
vector<int> smu(N +1);

void init() {
    for (int i = 2; i <= N; ++i) {
        if (!spf[i]) spf[i] = i, primes.push_back(i), mu[i] = -1;
      
        for (auto p : primes) {
            if (i * p > N) break;
            int y = p * i;
            spf[y] = p;
            if (p == spf[i]) {
                mu[y] = 0;
                break;
            }
            mu[y] = -mu[i];
        }
    }
    for (int i = 1; i <= N; i++) {
        smu[i] = smu[i - 1] + mu[i];
    }
}

void solve() {
    int n, m, k;
    cin >> n >> m >> k;
    int ans = 0;
    for (int l = 1, r = 0; l <= m; l = r + 1) {
        r = min(n / (n / l), m / (m / l));
        ans += (smu[r] - smu[l - 1]) * (n / i) * (m / i);
    }
    cout << ans << endl;
}
```

### 积性函数前缀和

#### 杜教筛

适合 $\varphi/\mu$ 这类能找到简单狄利克雷卷积关系的积性函数，常用于互质计数，莫比乌斯反演，整除分块等，时间复杂度 $\mathcal O(n^{\frac{2}{3}})$ 。

一般取 $\textrm{lim} = n^{\frac{2}{3}}$ ，线性筛得到 $f(1...\textrm{lim})$ 的前缀和，较大的 $n$ 递推计算，用哈希表记忆化。

需要找到一个简单函数 $g$ 满足 $h = f * g$ ，于是有 $F(n) = (H(n) - \sum\limits_{d=2}^{n} g(d) F(\lfloor\frac{n}{d}\rfloor))/g(1)$ ，整除分块求解。

对于欧拉函数 $\varphi$ ，取 $g = 1;\;h = \textrm{id};\; H(n) = n(n+1)/2$。

对于莫比乌斯函数 $\mu$ ，取 $g = 1;\;h=\epsilon;\;H(n)=1$

```cpp
Z sumPhi(int n) {
    if (n <= lim) return prePhi[n];
    if (auto it = memo.find(n); it != memo.end()) return it -> second;
    Z ans = Z(n) * (n + 1) / 2;
    for (int l = 2, r; l <= n; l = r + 1) {
        r = n / (n / l);
        ans -= (r - l + 1) * sumPhi(n / l);
    }
    return memo[n] = ans;
}
```

加权 $\mu(n)n^k$ 可用 $(\mu\cdot id^k)*id^k=\varepsilon$：把预筛值改成 $\mu(i)i^k$，分块系数改成 $\sum_{i=l}^r i^k$。例如 $k=2$ 时用平方和公式；无需单独维护一套筛法。

#### Lehmer 质数计数

求 $\pi(n)$。下面固定预筛到 $10^6$，查询 $0\le n\le10^{12}$；时间为亚线性，经典 Lehmer 的常见上界为 $O(n/(\log n)^4)$。`phi` 小表约 $25$ MiB，总预处理空间约 $30$ MiB。[公式与复杂度参考](https://github.com/kimwalisch/primecount#algorithms)。

设 $\Phi(x,a)$ 为 $1\ldots x$ 中不被前 $a$ 个质数整除的数的个数，则
$\Phi(x,a)=\Phi(x,a-1)-\Phi(\lfloor x/p_a\rfloor,a-1)$。取 $a=\pi(n^{1/4})$，再减去剩余的二、三质因子乘积贡献。

```cpp
namespace Lehmer {
    static constexpr int N = 1000000, X = 65536, A = 100;
    vector<int> primes, pi;
    vector<array<int, A>> small;
    unordered_map<int, int> memo;
    Lehmer() : pi(N + 1), small(X) {
        vector<bool> composite(N + 1);
        for (int i = 2; i <= N; ++i) {
            if (!composite[i]) primes.push_back(i);
            for (int p : primes) {
                if (p > N / i) break;
                composite[i * p] = true;
                if (i % p == 0) break;
            }
            pi[i] = pi[i - 1] + !composite[i];
        }
        for (int x = 0; x < X; ++x) {
            small[x][0] = x;
            for (int a = 1; a < A; ++a) small[x][a] = small[x][a - 1] - small[x / primes[a - 1]][a - 1];
        }
    }
    static int root(int x, int k) {
        int r = powl(x, 1.L / k);
        auto pw = [&](int v) -> i128 { return k == 2 ? (i128)v * v : (i128)v * v * v; };
        while (pw(r + 1) <= x) ++r;
        while (pw(r) > x) --r;
        return r;
    }
    int phi(int x, int a) {
        if (a == 0) return x;
        if (x < X && a < A) return small[x][a];
        if (x <= N && (int)primes[a - 1] * primes[a - 1] > x) return x == 0 ? 0 : max(1LL, (int)pi[x] - a + 1);
        return phi(x, a - 1) - phi(x / primes[a - 1], a - 1);
    }
    int count(int n) {
        if (n <= N) return pi[n];
        if (auto it = memo.find(n); it != memo.end()) return it->second;
        int a = count(root(root(n, 2), 2)), b = count(root(n, 2)), c = count(root(n, 3));
        int ans = phi(n, a) + (b + a - 2) * (b - a + 1) / 2;
        for (int i = a; i < b; ++i) {
            int w = n / primes[i];
            ans -= count(w);
            if (i < c) {
                int lim = count(root(w, 2));
                for (int j = i; j < lim; ++j) ans -= count(w / primes[j]) - j;
            }
        }
        return memo[n] = ans;
    }
}; // namespace Lehmer
```

#### Min_25 筛

适合处理一般的积性函数，例如约数个数、约数和、欧拉函数加权和等，尤其擅长处理质数处的函数值很好表达的情况，时间复杂度 $\mathcal O(n^{\frac{3}{4}} / \log n)$。

需要找到一个好求前缀和的完全积性函数 $F$ 满足 $F(p) = f(p)$，并且 $f$ 可以快速求 $f(p)$ 和 $f(p^e)$ 。​ 

```cpp
template <int T, auto F, auto sF, auto fpe, int f1> struct Min25 {
    int n, m, sq;
    vector<int> id1, id2, val;
    vector<array<Z, T>> g, sFp;
    array<int, T> a;
    int id(int x) {
        return x <= sq ? id1[x] : id2[n / x];
    }

    Min25(int n, array<int, T> a) : n(n), a(a) {
        sq = sqrt(n);
        while ((sq + 1) * (sq + 1) <= n) sq++;
        while (sq * sq > n) sq--;
        id1.assign(sq + 1, -1);
        id2.assign(sq + 1, -1);
        for (int l = 1; l <= n;) {
            int x = n / l, r = n / x;
            int pos = val.size();
            val.push_back(x);
            if (x <= sq)
                id1[x] = pos;
            else
                id2[n / x] = pos;
            if (r == n) break;
            l = r + 1;
        }
        m = val.size();
        g.assign(m, array<Z, T>{});
        sFp.assign(primes.size() + 1, array<Z, T>{});
        for (int j = 0; j < T; ++j) {
            for (int i = 0; i < m; ++i) g[i][j] = sF(val[i], j) - F(1, j);
            for (int i = 0; i < primes.size(); ++i) sFp[i + 1][j] = sFp[i][j] + F(primes[i], j);
            for (int i = 0; i < primes.size() && primes[i] * primes[i] <= n; ++i) {
                int p = primes[i];
                Z fp = F(p, j);
                for (int k = 0; k < m && p * p <= val[k]; ++k) {
                    int q = id(val[k] / p);
                    g[k][j] -= fp * (g[q][j] - sFp[i][j]);
                }
            }
        }
    }

    Z P(int n, int j) {
        return g[id(n)][j];
    }

    Z S(int x, int y) {
        if (x <= 1 || (y && primes[y - 1] >= x)) return 0;
        Z ans = 0;
        for (int j = 0; j < T; ++j) ans += a[j] * (g[id(x)][j] - sFp[y][j]);
        for (int i = y; i < primes.size() && primes[i] <= x / primes[i]; ++i) {
            int p = primes[i], pe = p;
            for (int e = 1; pe <= x; ++e) {
                ans += fpe(p, e, pe) * (S(x / pe, i + 1) + (e != 1));
                if (pe > x / p) break;
                pe *= p;
            }
        }
        return ans;
    }

    Z sf(int n) {
        return f1 + S(n, 0);
    }
};

Z F(int x, int j) {
    return x;
}

Z sF(int n, int j) {
    return Z(n + 1) * n / 2;
}

Z fpe(int p, int e, int pe) {
    return pe;
}
```

### 容斥原理

> 定义：$\big|S_1 \cup S_2 \cup S_3 \cup … \cup S_n \big | =\sum_{i=1}^N|S_i|   -   \sum_{i,j=1}^N \big| S_i \cap S_j \big|   +   \sum_{i,j,k=1}^N \big| S_i \cap S_j \cap S_k \big| -…$ 

例题：给定一个整数 $n$ 和 $m$ 个不同的质数 $p_1, p_2, ..., p_m$，请你求出 1 ∼ $n$ 中能被 $p_1, p_2, ..., p_m$ 中的至少一个数整除的整数有多少个。

#### 二进制枚举

```cpp
void solve() {
    int n, m;
    cin >> n >> m;
    vector<int> p(m);
    for (auto &v : p) cin >> v;
    int ans = 0;
    for (int i = 1; i < (1 << m); ++i) {
        int t = 1, cnt = 0;
        for (int j = 0; j < m; ++j) {
            if (i >> j & 1) {
                cnt++;
                t *= p[j];
                if (t > n) {
                    t = -1;
                    break;
                }
            }
        }
        if (t != -1) {
            if (cnt & 1) ans += n / t;
            else ans -= n / t;
        }
    }
    cout << ans << endl;
}
```

#### dfs解

```cpp
void solve() {
    int n, m;
    cin >> n >> m;
    vector<int> p(m);
    for (auto &v : p) cin >> v;
    int ans = 0;
    auto dfs = [&](auto &&self, int x, int s, int f) -> void {
      	if (x == m) {
            if (s == 1) return;
            ans += f * (n / s);
            return;
        }  
        self(self, x + 1, s, f);
        if (s <= n / p[x]) self(self, x + 1, s * p[x], -f);
    };
    dfs(dfs, 0, 1, -1);
    cout << ans << endl;
}
```

### 高斯消元

$n$ 行 $m+1$ 列的增广矩阵，返回 $\{秩，一组解\}$ ，无解时秩返回 $-1$ 。

#### 实数域

部分选主元；`eps` 按题目数值尺度调整。秩小于 $m$ 时有无穷多解。

```cpp
pair<int, vector<ld>> gauss(vector<vector<ld>> a) {
    constexpr ld eps = 1e-12L;
    int n = a.size(), m = a[0].size() - 1, r = 0;
    vector<int> col;
    for (int c = 0; c < m && r < n; ++c) {
        int p = r;
        for (int i = r + 1; i < n; ++i)
            if (abs(a[i][c]) > abs(a[p][c])) p = i;
        if (abs(a[p][c]) < eps) continue;
        swap(a[r], a[p]);
        ld v = a[r][c];
        for (int j = c; j <= m; ++j) a[r][j] /= v;
        for (int i = 0; i < n; ++i)
            if (i != r) {
                ld t = a[i][c];
                for (int j = c; j <= m; ++j) a[i][j] -= t * a[r][j];
            }
        col.push_back(c), ++r;
    }
    for (int i = r; i < n; ++i)
        if (abs(a[i][m]) > eps) return {-1, {}};
    vector<ld> x(m);
    for (int i = 0; i < r; ++i) x[col[i]] = a[i][m];
    return {r, x};
}
```

#### 质数模域

秩为 $r$ 且有解时，恰有 $mod^{m-r}$ 个解。

```cpp
pair<int, vector<Z>> gauss(vector<vector<Z>> a) {
    int n = a.size(), m = a[0].size() - 1, r = 0;
    vector<int> col;
    for (int c = 0; c < m && r < n; ++c) {
        int p = r;
        while (p < n && a[p][c] == 0) ++p;
        if (p == n) continue;
        swap(a[p], a[r]);
        Z iv = a[r][c].inv();
        for (int j = c; j <= m; ++j) a[r][j] *= iv;
        for (int i = 0; i < n; ++i) if (i != r) {
            Z t = a[i][c];
            for (int j = c; j <= m; ++j) a[i][j] -= t * a[r][j];
        }
        col.push_back(c), ++r;
    }
    for (int i = r; i < n; ++i) if (a[i][m] != 0) return {-1, {}};
    vector<Z> x(m);
    for (int i = 0; i < r; ++i) x[col[i]] = a[i][m];
    return {r, x};
}
```

#### GF(2)：异或方程组

每行前 $m$ 位为系数，第 $m$ 位为常数，其余位为 $0$；$m<B$。返回约定同上，有解时解数为 $2^{m-r}$。位集消元每次整行异或约 $O(B/64)$。

```cpp
template <size_t B> pair<int, vector<int>> gauss(vector<bitset<B>> a, int m) {
    int n = a.size(), r = 0;
    vector<int> col;
    for (int c = 0; c < m && r < n; ++c) {
        int p = r;
        while (p < n && !a[p][c]) ++p;
        if (p == n) continue;
        swap(a[p], a[r]);
        for (int i = 0; i < n; ++i)
            if (i != r && a[i][c]) a[i] ^= a[r];
        col.push_back(c), ++r;
    }
    for (int i = r; i < n; ++i)
        if (a[i][m]) return {-1, {}};
    vector<int> x(m);
    for (int i = 0; i < r; ++i) x[col[i]] = a[i][m];
    return {r, x};
}
```

#### 对称正定矩阵：LDLT

实数对称正定矩阵 $A$ 可分解为 $LDL^T$，其中 $L$ 对角为 $1$，$D$ 为正对角阵。此时无需选主元；分解和求解 $Ax=b$ 为 $O(n^3)$。仅在题目保证正定时使用。

```cpp
vector<long double> ldlt_solve(const vector<vector<long double>> &a, vector<long double> b) {
    int n = a.size();
    vector<vector<long double>> l(n, vector<long double>(n));
    vector<long double> d(n), y(n), x(n);
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < i; ++j) {
            long double v = a[i][j];
            for (int k = 0; k < j; ++k) v -= l[i][k] * l[j][k] * d[k];
            l[i][j] = v / d[j];
        }
        d[i] = a[i][i];
        for (int j = 0; j < i; ++j) d[i] -= l[i][j] * l[i][j] * d[j];
        y[i] = b[i];
        for (int j = 0; j < i; ++j) y[i] -= l[i][j] * y[j];
    }
    for (int i = n - 1; i >= 0; --i) {
        x[i] = y[i] / d[i];
        for (int j = i + 1; j < n; ++j) x[i] -= l[j][i] * x[j];
    }
    return x;
}
```

### 线性规划：两阶段单纯形

求 $\max c^Tx$，约束为 $Ax\le b,x\ge0$，变量为实数。要求至少一个变量、一条约束；时间最坏指数级，适合中小规模连续线性规划。

返回 `{最优值, 最优解}`；无解时最优值为 NaN，无界时为正无穷，可用 `isnan` / `isinf` 判断。浮点 `EPS` 按题目尺度调整。

```cpp
struct Simplex {
    using ld = long double;
    static constexpr ld EPS = 1e-12L;
    int n, m;
    vector<int> basic, nonbasic;
    vector<vector<ld>> a;
    Simplex(const vector<vector<ld>> &A, const vector<ld> &b, const vector<ld> &c)
        : n(b.size()), m(c.size()), basic(n), nonbasic(m + 1), a(n + 2, vector<ld>(m + 2)) {
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < m; ++j) a[i][j] = A[i][j];
            basic[i] = m + i, a[i][m] = -1, a[i][m + 1] = b[i];
        }
        for (int j = 0; j < m; ++j) nonbasic[j] = j, a[n][j] = -c[j];
        nonbasic[m] = -1, a[n + 1][m] = 1;
    }
    void pivot(int r, int c) {
        ld iv = 1 / a[r][c];
        for (int i = 0; i < n + 2; ++i) if (i != r)
            for (int j = 0; j < m + 2; ++j) if (j != c)
                a[i][j] -= a[r][j] * a[i][c] * iv;
        for (int j = 0; j < m + 2; ++j) if (j != c) a[r][j] *= iv;
        for (int i = 0; i < n + 2; ++i) if (i != r) a[i][c] *= -iv;
        a[r][c] = iv;
        swap(basic[r], nonbasic[c]);
    }
    bool run(int phase) {
        int obj = phase == 1 ? n + 1 : n;
        while (true) {
            int c = -1;
            for (int j = 0; j <= m; ++j) {
                if (phase == 2 && nonbasic[j] == -1) continue;
                if (a[obj][j] < -EPS && (c == -1 || nonbasic[j] < nonbasic[c])) c = j;
            }
            if (c == -1) return true;
            int r = -1;
            for (int i = 0; i < n; ++i) if (a[i][c] > EPS) {
                if (r == -1) { r = i; continue; }
                ld d = a[i][m + 1] / a[i][c] - a[r][m + 1] / a[r][c];
                if (d < -EPS || (abs(d) <= EPS && basic[i] < basic[r])) r = i;
            }
            if (r == -1) return false;
            pivot(r, c);
        }
    }
    pair<ld, vector<ld>> solve() {
        int r = 0;
        for (int i = 1; i < n; ++i) if (a[i][m + 1] < a[r][m + 1]) r = i;
        if (a[r][m + 1] < -EPS) {
            pivot(r, m);
            if (!run(1) || abs(a[n + 1][m + 1]) > EPS)
                return {numeric_limits<ld>::quiet_NaN(), {}};
            for (int i = 0; i < n; ++i) if (basic[i] == -1) {
                int c = 0;
                for (int j = 1; j <= m; ++j) if (abs(a[i][j]) > abs(a[i][c])) c = j;
                if (abs(a[i][c]) > EPS) pivot(i, c);
            }
        }
        if (!run(2)) return {numeric_limits<ld>::infinity(), {}};
        vector<ld> x(m);
        for (int i = 0; i < n; ++i) if (0 <= basic[i] && basic[i] < m) x[basic[i]] = a[i][m + 1];
        return {a[n][m + 1], x};
    }
};
```

### 康托展开

输入为 $1\ldots n$ 的排列，排名从 $1$ 开始：$rank=1+\sum_i c_i(n-1-i)!$，其中 $c_i$ 为右侧比 $a_i$ 小的元素数。

#### 小数据：精确排名与反排名

$0\le n\le20$，`unrank_perm` 要求 $1\le rank\le n!$。时间 $O(n^2)$，空间 $O(n)$。

```cpp
int rank_perm(const vector<int> &a) {
    int n = a.size(), ans = 1;
    vector<int> fac(n + 1, 1);
    for (int i = 1; i <= n; ++i) fac[i] = fac[i - 1] * i;
    for (int i = 0; i < n; ++i) {
        int c = 0;
        for (int j = i + 1; j < n; ++j) c += a[j] < a[i];
        ans += c * fac[n - 1 - i];
    }
    return ans;
}
vector<int> unrank_perm(int n, int rank) {
    vector<int> fac(n + 1, 1), left(n), a;
    for (int i = 1; i <= n; ++i) fac[i] = fac[i - 1] * i;
    iota(left.begin(), left.end(), 1);
    --rank;
    for (int i = n; i; --i) {
        int j = rank / fac[i - 1];
        rank %= fac[i - 1];
        a.push_back(left[j]);
        left.erase(left.begin() + j);
    }
    return a;
}
```

#### 大数据：树状数组求模意义排名

依赖 `Z`。时间 $O(n\log n)$，空间 $O(n)$；不做逆元，$n\ge mod$ 时公式仍成立，但取模排名不能用于反排名。

```cpp
Z rank_perm_mod(const vector<int> &a) {
    int n = a.size();
    vector<int> bit(n + 1);
    vector<Z> fac(n + 1, 1);
    for (int i = 1; i <= n; ++i) bit[i] = i & -i, fac[i] = fac[i - 1] * i;
    Z ans = 1;
    for (int i = 0; i < n; ++i) {
        int cnt = 0;
        for (int x = a[i] - 1; x; x -= x & -x) cnt += bit[x];
        ans += fac[n - 1 - i] * cnt;
        for (int x = a[i]; x <= n; x += x & -x) --bit[x];
    }
    return ans;
}
```

### 矩阵

#### 方阵乘法与快速幂

依赖 `Z`。`Matrix<N>(d)` 为对角线是 $d$ 的矩阵；`pow(e)` 要求 $e\ge0$，时间 $O(N^3\log(e+1))$。列向量约定 $v_{t+1}=Av_t$，则 $v_k=A^kv_0$。

```cpp
template <int N> struct Matrix {
    array<array<Z, N>, N> a{};
    Matrix(int d = 0) { for (int i = 0; i < N; ++i) a[i][i] = d; }
    Matrix operator*(const Matrix &b) const {
        Matrix c;
        for (int i = 0; i < N; ++i)
            for (int k = 0; k < N; ++k)
                for (int j = 0; j < N; ++j) c.a[i][j] += a[i][k] * b.a[k][j];
        return c;
    }
    Matrix pow(int e) const {
        Matrix a = *this, r(1);
        for (; e; e >>= 1, a = a * a) if (e & 1) r = r * a;
        return r;
    }
};
```

#### Min-plus 矩阵：恰好走 $k$ 条边的最短路

乘法为 $C_{ij}=\min_t(A_{it}+B_{tj})$；单位矩阵对角为 $0$，其他为 $+\infty$。有限距离及其相加结果须处于 $(-INF,INF)$ 内；跳过无穷项是必要处理。

```cpp
struct MinPlus {
    static constexpr int INF = 4'000'000'000'000'000'000LL;
    int n;
    vector<vector<int>> a;
    MinPlus(int n, bool unit = false) : n(n), a(n, vector<int>(n, INF)) {
        if (unit) for (int i = 0; i < n; ++i) a[i][i] = 0;
    }
    MinPlus operator*(const MinPlus &b) const {
        MinPlus c(n);
        for (int i = 0; i < n; ++i)
            for (int k = 0; k < n; ++k) if (a[i][k] != INF)
                for (int j = 0; j < n; ++j) if (b.a[k][j] != INF)
                    c.a[i][j] = min(c.a[i][j], a[i][k] + b.a[k][j]);
        return c;
    }
    MinPlus pow(int e) const {
        MinPlus a = *this, r(n, true);
        for (; e; e >>= 1, a = a * a) if (e & 1) r = r * a;
        return r;
    }
};
```

#### 行列式与逆矩阵（质数模）

依赖 `Z`，输入为方阵。时间 $O(n^3)$，不适用于合数模下不可逆的主元。零阶行列式定义为 $1$。

```cpp
Z determinant(vector<vector<Z>> a) {
    int n = a.size();
    Z ans = 1;
    for (int c = 0; c < n; ++c) {
        int p = c;
        while (p < n && a[p][c] == 0) ++p;
        if (p == n) return 0;
        if (p != c) swap(a[p], a[c]), ans = -ans;
        ans *= a[c][c];
        Z iv = a[c][c].inv();
        for (int i = c + 1; i < n; ++i) {
            Z t = a[i][c] * iv;
            for (int j = c; j < n; ++j) a[i][j] -= t * a[c][j];
        }
    }
    return ans;
}

vector<vector<Z>> matrix_inverse(vector<vector<Z>> a) { // n>=1，奇异返回空
    int n = a.size();
    vector<vector<Z>> b(n, vector<Z>(n));
    for (int i = 0; i < n; ++i) b[i][i] = 1;
    for (int c = 0; c < n; ++c) {
        int p = c;
        while (p < n && a[p][c] == 0) ++p;
        if (p == n) return {};
        swap(a[p], a[c]), swap(b[p], b[c]);
        Z iv = a[c][c].inv();
        for (int j = 0; j < n; ++j) a[c][j] *= iv, b[c][j] *= iv;
        for (int i = 0; i < n; ++i) if (i != c) {
            Z t = a[i][c];
            for (int j = 0; j < n; ++j) a[i][j] -= t * a[c][j], b[i][j] -= t * b[c][j];
        }
    }
    return b;
}
```

#### 矩阵树定理

无向带权图的 Laplacian：每条边 $(u,v,w)$ 给 $L_{uu},L_{vv}$ 加 $w$，给 $L_{uv},L_{vu}$ 减 $w$。删去相同的一行一列后取行列式，得到生成树边权乘积之和；边权全为 $1$ 即生成树数量。

依赖 `determinant`；$n\ge1$，点号为 $0\ldots n-1$，允许重边，自环忽略。

```cpp
Z spanning_trees(int n, const vector<tuple<int, int, Z>> &edges) {
    vector<vector<Z>> a(n - 1, vector<Z>(n - 1));
    for (auto [u, v, w] : edges) {
        if (u == v) continue;
        if (u < n - 1) a[u][u] += w;
        if (v < n - 1) a[v][v] += w;
        if (u < n - 1 && v < n - 1) a[u][v] -= w, a[v][u] -= w;
    }
    return determinant(a);
}
```

### 类欧几里得算法

#### 线性取整和 floor_sum

求 $\sum_{i=0}^{n-1}\lfloor(ai+b)/m\rfloor$。要求 $n\ge0,m>0$，允许 $a,b<0$，依赖 `floor_div`。时间 $O(\log m)$（先约去整数部分），中间值与答案须能放入 `i128`。

```cpp
i128 floor_sum(int n, int m, int a, int b) {
    i128 N = n, M = m, A = a, B = b;
    i128 qa = floor_div(A, M), qb = floor_div(B, M);
    i128 ans = N * (N - 1) / 2 * qa + N * qb;
    A -= qa * M, B -= qb * M;
    while (true) {
        ans += N * (N - 1) / 2 * (A / M) + N * (B / M);
        A %= M, B %= M;
        i128 y = A * N + B;
        if (y < M) break;
        N = y / M, B = y % M;
        swap(A, M);
    }
    return ans;
}
```

转置格点区域，把斜率互换，参数按欧几里得算法下降；区间是 $0\ldots n-1$，若要求 $0\ldots n$，传入 `n+1`。[ACL 的非负参数转移](https://github.com/atcoder/ac-library/blob/master/atcoder/internal_math.hpp)。

#### 加权取整和：一阶、二阶矩

依赖 `Z`。设 $y_i=\lfloor(ai+b)/c\rfloor$，返回
$$
(S,T,U)=\left(\sum_{i=0}^n y_i,\ \sum_{i=0}^n i y_i,\ \sum_{i=0}^n y_i^2\right)\pmod {mod}.
$$
要求 $a,b,n\ge0,c>0$，参数不超过 $10^{18}$，$2,6$ 在模域可逆。时间 $O(\log(a+c))$ 次域运算。

```cpp
Z sum1(int n) { return Z(n) * (Z(n) + 1) / 2; }
Z sum2(int n) { return Z(n) * (Z(n) + 1) * (Z(n) * 2 + 1) / 6; }
struct Moments { Z s, t, u; };
Moments floor_moments(int a, int b, int c, int n) {
    if (a >= c || b >= c) {
        Z q = a / c, p = b / c;
        auto x = floor_moments(a % c, b % c, c, n);
        Z v = sum1(n), w = sum2(n), len = Z(n) + 1;
        return {q * v + p * len + x.s,
                q * w + p * v + x.t,
                q * q * w + p * p * len + 2 * q * p * v + 2 * q * x.t + 2 * p * x.s + x.u};
    }
    int m = ((i128)a * n + b) / c;
    if (m == 0) return {};
    auto x = floor_moments(c, c - b - 1, a, m - 1);
    return {Z(n) * m - x.s,
            Z(m) * sum1(n) - (x.u + x.s) / 2,
            Z(n) * m * m - 2 * x.t - x.s};
}
```

#### 万能欧几里得：有序矩阵积

依赖 `Matrix`。令 $y_i=\lfloor(ai+b)/c\rfloor$，返回按从左到右顺序相乘的
$$
U^{y_0}\prod_{i=1}^n\left(U^{y_i-y_{i-1}}R\right).
$$
对应依次走 $U$（高度加一）、$R$（横坐标加一）的路径。矩阵一般不可交换；使用行向量 $v\leftarrow vM$ 时，乘积顺序就是操作发生顺序。

要求 $a,b,n\ge0,c>0$，各参数、$\lfloor(an+b)/c\rfloor\le10^{18}$。平方矩阵边长为 $D$，时间可保守估计为 $O(D^3\log^2 L)$，$L$ 为参数数量级；求和只需上一节，无须套矩阵。

```cpp
template <int D>
Matrix<D> euclid_product(int a, int b, int c, int n, Matrix<D> U, Matrix<D> R) {
    if (b >= c) return U.pow(b / c) * euclid_product(a, b % c, c, n, U, R);
    if (a >= c) return euclid_product(a % c, b, c, n, U, U.pow(a / c) * R);
    int m = ((i128)a * n + b) / c;
    if (m == 0) return R.pow(n);
    int left = (c - b - 1) / a;
    int right = n - ((i128)c * m - b - 1) / a;
    return R.pow(left) * U * euclid_product(c, (c - b - 1) % a, a, m - 1, R, U) * R.pow(right);
}
```

### 连续数字的正约数集合

时间、输出空间 $O(n\log n)$。如果只统计约数个数，把 `push_back` 改为计数；更大范围的 $\tau$ 整表可按最小质因子递推。

```cpp
vector<vector<int>> all_divisors(int n) {
    vector<vector<int>> d(n + 1);
    for (int i = 1; i <= n; ++i)
        for (int j = i; j <= n; j += i) d[j].push_back(i);
    return d;
}
```

### 斐波那契数列

依赖 `Z`，约定 $F_0=0,F_1=1$。快速倍增返回 $(F_n,F_{n+1})$，时间 $O(\log(n+1))$，$n\ge0$。

```cpp
pair<Z, Z> fib(int n) {
    if (n == 0) return {0, 1};
    auto [x, y] = fib(n >> 1);
    Z a = x * (2 * y - x), b = x * x + y * y;
    return n & 1 ? pair<Z, Z>{b, a + b} : pair<Z, Z>{a, b};
}
```

### 插值与线性递推

#### 任意横坐标的拉格朗日插值

依赖 `Z`。给定 $n\ge1$ 个点，横坐标在模意义下两两不同，求次数小于 $n$ 的插值多项式在 $t$ 处的值。时间 $O(n^2+n\log mod)$，其中逆元用模幂。

```cpp
Z lagrange(const vector<Z> &x, const vector<Z> &y, Z t) {
    int n = x.size();
    Z ans = 0;
    for (int i = 0; i < n; ++i) {
        Z a = 1, b = 1;
        for (int j = 0; j < n; ++j) if (i != j) a *= t - x[j], b *= x[i] - x[j];
        ans += y[i] * a / b;
    }
    return ans;
}
```

#### 连续横坐标：$O(n)$ 插值

依赖 `Z`、`Comb`。已知 $y_i=f(i)$（$0\le i<n$），$1\le n\le mod$，$\deg f<n$。`comb` 预处理到 $n-1$；单次时间、空间 $O(n)$。常用来求幂和：$\sum_{i=1}^x i^k$ 为 $k+1$ 次多项式，准备 $k+2$ 个点即可。

```cpp
Z lagrange_consecutive(const vector<Z> &y, Z x, const Comb &comb) {
    int n = y.size();
    vector<Z> l(n + 1, 1), r(n + 1, 1);
    for (int i = 0; i < n; ++i) l[i + 1] = l[i] * (x - i);
    for (int i = n - 1; i >= 0; --i) r[i] = r[i + 1] * (x - i);
    Z ans = 0;
    for (int i = 0; i < n; ++i) {
        Z t = y[i] * l[i] * r[i + 1] * comb.ifac[i] * comb.ifac[n - 1 - i];
        ans += (n - 1 - i) & 1 ? -t : t;
    }
    return ans;
}
```

#### Berlekamp–Massey：恢复递推系数

依赖 `Z`。返回 $c$，满足 $a_t=\sum_{j=0}^{k-1}c_j a_{t-1-j}$（$t\ge k$）。输入全零时返回空数组。时间最坏 $O(n^2+n\log mod)$。

只能保证拟合输入前缀；若已知无限序列满足阶数至多 $k$ 的线性递推，至少给 $2k$ 项才能保证恢复后可外推。

```cpp
vector<Z> berlekamp_massey(const vector<Z> &a) {
    vector<Z> c{1}, b{1};
    int len = 0, shift = 1;
    Z last = 1;
    for (int i = 0; i < (int)a.size(); ++i) {
        Z d = a[i];
        for (int j = 1; j <= len; ++j) d += c[j] * a[i - j];
        if (d == 0) { ++shift; continue; }
        auto old = c;
        Z q = d / last;
        if (c.size() < b.size() + shift) c.resize(b.size() + shift);
        for (int j = 0; j < (int)b.size(); ++j) c[j + shift] -= q * b[j];
        if (2 * len <= i) {
            len = i + 1 - len, b = move(old), last = d, shift = 1;
        } else ++shift;
    }
    c.resize(len + 1);
    c.erase(c.begin());
    for (Z &x : c) x = -x;
    return c;
}
```

#### Kitamasa：线性递推第 $n$ 项

依赖 `Z`。`c` 使用上一节的系数方向，`a` 至少包含前 $k$ 项（`k=c.size()`），$n\ge0$。时间 $O(k^2\log(n+1))$，空间 $O(k)$；空系数代表全零序列。

```cpp
Z linear_nth(const vector<Z> &a, const vector<Z> &c, int n) {
    int k = c.size();
    if (k == 0) return 0;
    auto combine = [&](const vector<Z> &u, const vector<Z> &v) {
        vector<Z> t(2 * k - 1);
        for (int i = 0; i < k; ++i)
            for (int j = 0; j < k; ++j) t[i + j] += u[i] * v[j];
        for (int i = 2 * k - 2; i >= k; --i)
            for (int j = 0; j < k; ++j) t[i - 1 - j] += t[i] * c[j];
        t.resize(k);
        return t;
    };
    vector<Z> r(k), x(k);
    r[0] = 1;
    if (k == 1) x[0] = c[0];
    else x[1] = 1;
    for (; n; n >>= 1, x = combine(x, x)) if (n & 1) r = combine(r, x);
    Z ans = 0;
    for (int i = 0; i < k; ++i) ans += r[i] * a[i];
    return ans;
}
```

### 伯努利数与幂和

依赖 `Z`、`Comb`，约定 $B_1=-1/2$。要求 $d+1<mod$，预处理 $O(d^2+\log mod)$，单次 $O(k)$，返回 $\sum_{i=1}^n i^k$，$0\le k\le d,n\ge0$。

$$
B_0=1,\qquad B_m=-\frac1{m+1}\sum_{j=0}^{m-1}\binom{m+1}{j}B_j.
$$
对 $k\ge1$，
$$
\sum_{i=1}^n i^k=\frac1{k+1}\sum_{j=0}^k(-1)^j\binom{k+1}{j}B_j n^{k+1-j}.
$$
$k=0$ 单独返回 $n$。只求一种幂次时也可用连续点插值。

```cpp
struct Bernoulli {
    Comb comb;
    vector<Z> b;
    Bernoulli(int d) : comb(d + 1), b(d + 1) {
        b[0] = 1;
        for (int i = 1; i <= d; ++i) {
            Z s = 0;
            for (int j = 0; j < i; ++j) s += comb.C(i + 1, j) * b[j];
            b[i] = -s * comb.inv(i + 1);
        }
    }
    Z sum(int k, int n) const {
        if (k == 0) return Z(n);
        Z ans = 0, pw = n;
        for (int j = k; j >= 0; --j) {
            Z t = comb.C(k + 1, j) * b[j] * pw;
            ans += j & 1 ? -t : t;
            pw *= n;
        }
        return ans * comb.inv(k + 1);
    }
};
```

### 约瑟夫环

$n,k\ge1$，编号为 $0\ldots n-1$，从 $0$ 开始报数，报到第 $k$ 个删除，返回最后幸存者。时间 $O(n)$；题目编号从 $1$ 开始则答案加一。

```cpp
int josephus(int n, int k) {
    int ans = 0;
    for (int i = 2; i <= n; ++i) ans = (ans + k % i) % i;
    return ans;
}
```

### 常见组合数列

#### 斯特林数、错排数与 Bell 数

依赖 `Z`。无符号第一类斯特林数 $c(n,k)$ 计数恰有 $k$ 个环的排列；第二类 $S(n,k)$ 计数把 $n$ 个不同元素划为 $k$ 个非空、无标号集合。初值均为 $c(0,0)=S(0,0)=1$。
$$
c(n,k)=c(n-1,k-1)+(n-1)c(n-1,k),\quad
S(n,k)=S(n-1,k-1)+kS(n-1,k).
$$
错排数 $D_0=1,D_1=0,D_n=(n-1)(D_{n-1}+D_{n-2})$；Bell 数 $B_n=\sum_{k=0}^n S(n,k)$。整表时间、空间 $O(N^2)$；只需一行时倒序滚动更新。

```cpp
struct Counting {
    vector<vector<Z>> s1, s2;
    vector<Z> derange, bell;
    Counting(int n) : s1(n + 1, vector<Z>(n + 1)), s2(s1), derange(n + 1), bell(n + 1) {
        s1[0][0] = s2[0][0] = derange[0] = bell[0] = 1;
        for (int i = 1; i <= n; ++i) {
            for (int j = 1; j <= i; ++j) {
                s1[i][j] = s1[i - 1][j - 1] + (i - 1) * s1[i - 1][j];
                s2[i][j] = s2[i - 1][j - 1] + j * s2[i - 1][j];
                bell[i] += s2[i][j];
            }
            if (i >= 2) derange[i] = (i - 1) * (derange[i - 1] + derange[i - 2]);
        }
    }
};
```

#### 整数分拆：五边形数定理

依赖 `Z`。$p(n)$ 为把 $n$ 拆成若干正整数之和、不计顺序的方案数，$p(0)=1$。时间 $O(n\sqrt n)$，空间 $O(n)$。

```cpp
vector<Z> partitions(int n) {
    vector<Z> p(n + 1);
    p[0] = 1;
    for (int i = 1; i <= n; ++i) {
        for (int k = 1; k * (3 * k - 1) / 2 <= i; ++k) {
            int a = k * (3 * k - 1) / 2, b = k * (3 * k + 1) / 2;
            Z t = p[i - a];
            if (b <= i) t += p[i - b];
            p[i] += k & 1 ? t : -t;
        }
    }
    return p;
}
```

限制至多 $m$ 份时，由 Ferrers 图转置，等价于每份不超过 $m$；完全背包求 $[x^n]\prod_{i=1}^m(1-x^i)^{-1}$，时间 $O(nm)$：

```cpp
Z bounded_partitions(int n, int m) {
    vector<Z> dp(n + 1);
    dp[0] = 1;
    for (int v = 1; v <= min(n, m); ++v)
        for (int s = v; s <= n; ++s) dp[s] += dp[s - v];
    return dp[n];
}
```

### 常见结论

#### 球盒模型

$n$ 个球、$m$ 个盒，$n\ge0,m\ge1$。$P(m,n)=m!/(m-n)!$，$S$ 为第二类斯特林数，$p_{\le m}(n)$ 为至多分成 $m$ 份的整数分拆数。无合法方案时为 $0$；空球集的“允许空”方案数为 $1$。

| 球 | 盒 | 允许空 | 每盒非空 | 每盒至多一个 |
|---|---|---|---|---|
| 相同 | 不同 | $\binom{n+m-1}{m-1}$ | $\binom{n-1}{m-1}$ | $\binom mn$ |
| 不同 | 不同 | $m^n$ | $m!S(n,m)$ | $P(m,n)$ |
| 相同 | 相同 | $p_{\le m}(n)$ | $p_{\le m}(n-m)$ | $[n\le m]$ |
| 不同 | 相同 | $\sum_{j=0}^{\min(n,m)}S(n,j)$ | $S(n,m)$ | $[n\le m]$ |

#### 组合恒等式与二项式反演

统一约定 $\binom nk=0$（$n\ge0$ 且 $k<0$ 或 $k>n$）。
$$
\binom nk+\binom n{k+1}=\binom{n+1}{k+1},\qquad
k\binom nk=n\binom{n-1}{k-1},\qquad
\binom nk\binom kr=\binom nr\binom{n-r}{k-r}.
$$
$$
\sum_{k=0}^n\binom nk=2^n,\qquad
\sum_{k=0}^n(-1)^k\binom nk=[n=0].
$$
对 $n\ge1$：
$$
\sum_{k=0}^n k\binom nk=n2^{n-1},\qquad
\sum_{k=0}^n k^2\binom nk=n(n+1)2^{n-2},\qquad
\sum_{k=1}^n\frac{(-1)^{k+1}}k\binom nk=H_n.
$$
最后一式需要交错号；模意义使用它还要求各分母可逆。

范德蒙德与曲棍球杆（求和涵盖全部合法下标）：
$$
\sum_i\binom ni\binom m{k-i}=\binom{n+m}k,\qquad
\sum_i\binom ni\binom mi=\binom{n+m}n,\qquad
\sum_{i=0}^r\binom{s+i}i=\binom{s+r+1}r.
$$
其中取 $m=n$ 得 $\sum_i\binom ni^2=\binom{2n}n$。

二项式反演：
$$
f_n=\sum_{i=0}^n\binom ni g_i
\iff g_n=\sum_{i=0}^n(-1)^{n-i}\binom ni f_i.
$$
$$
f_k=\sum_{i=k}^n\binom ik g_i
\iff g_k=\sum_{i=k}^n(-1)^{i-k}\binom ik f_i.
$$

#### Catalan 数与反射原理

$$
Cat_n=\frac1{n+1}\binom{2n}n
=\binom{2n}n-\binom{2n}{n+1},\quad
Cat_0=1,\quad Cat_n=Cat_{n-1}\frac{4n-2}{n+1}.
$$
前十项：$1,1,2,5,14,42,132,429,1430,4862$。对应 $n$ 对合法括号、栈排列、$n$ 个节点的有序二叉树、凸 $(n+2)$ 边形三角剖分。

从 $(0,0)$ 只向右、向上走到 $(n,n)$，始终在对角线一侧（可接触）有 $Cat_n$ 条；除端点外不接触对角线、两侧均可选时，$n\ge1$ 有 $2Cat_{n-1}$ 条。

一般从 $(0,0)$ 到 $(a,b)$、$a\ge b\ge0$，始终满足 $x\ge y$ 的路径数为 $\binom{a+b}b-\binom{a+b}{b-1}$。先数全部路径，再将首次越界的坏路径反射。

模意义用递推除法时要求 $n+1$ 可逆；用阶乘差式时要求阶乘预处理范围覆盖 $2n<mod$，更大范围应换合适的组合数算法。

#### Burnside 与 Pólya

有限群 $G$ 作用下，等价类数为 $\frac1{|G|}\sum_{g\in G}|Fix(g)|$。对位置染 $q$ 种颜色，置换 $g$ 有 $c(g)$ 个循环，则 $|Fix(g)|=q^{c(g)}$；有颜色数量等限制时，按循环长度写生成函数。

长度 $n$、$q$ 色、仅旋转视为相同的项链数：
$$
\frac1n\sum_{k=0}^{n-1}q^{\gcd(n,k)}
=\frac1n\sum_{d\mid n}\varphi(d)q^{n/d}.
$$
模意义下群大小不可逆时，不能直接除；可在模 $|G|\cdot mod$ 下求分子，再作整数除法（注意乘积范围）。翻转也等价时，须把反射变换一并纳入群。

#### 生成函数速查

NTT、形式幂级数乘逆与指数运算见多项式章节；这里保留计数推导入口。特征为质数 $p$ 时，出现的阶乘、整数分母均须可逆。

| 对象 | 公式 |
|---|---|
| 第一类无符号斯特林数整行 | $\prod_{i=0}^{n-1}(x+i)=\sum_k c(n,k)x^k$ |
| 第二类斯特林数整行 | $S(n,k)=\sum_{i=0}^k\frac{i^n}{i!}\frac{(-1)^{k-i}}{(k-i)!}$，可卷积 |
| Bell 数 | $\sum_{n\ge0} B_n x^n/n!=\exp(e^x-1)$ |
| 整数分拆 | $\sum_{n\ge0}p(n)x^n=\prod_{i\ge1}(1-x^i)^{-1}=\exp(\sum_{k\ge1}\sigma(k)x^k/k)$ |
| 伯努利数 | $\sum_{n\ge0}B_nx^n/n!=x/(e^x-1)$ |
| 连续点有限差分 | $f(x)=\sum_k\Delta^k f(0)\binom xk$ |

大质数模下求单点 $n!$（$n<p$）：取块长 $B\approx\sqrt n$，构造 $F(x)=\prod_{i=1}^B(x+i)$，多点求 $F(0),F(B),\ldots,F((\lfloor n/B\rfloor-1)B)$ 后相乘，再补最后不足一块的部分。已有快速多项式乘法与多点求值时约 $O(\sqrt n\log^2 n)$；$n\ge p$ 时 $n!\equiv0\pmod p$。

已知 $k$ 阶递推，令 $Q(x)=1-\sum_{j=1}^kc_{j-1}x^j$、$A(x)=\sum_{i=0}^{k-1}a_ix^i$，则第 $n$ 项为 $[x^n](A(x)Q(x)\bmod x^k)/Q(x)$，可接 Bostan–Mori。

拉格朗日反演：$T=x\Phi(T)$，对 $n\ge1$ 有
$$
[x^n]H(T(x))=\frac1n[t^{n-1}]H'(t)\Phi(t)^n.
$$
单位根筛：若域中存在本原 $m$ 次根 $\omega$，则
$$
\sum_{k\equiv r\pmod m}a_kx^k=\frac1m\sum_{j=0}^{m-1}\omega^{-rj}A(\omega^jx).
$$
通常在质数模 $p$ 下要求 $m\mid p-1$。

#### 欧拉函数、GCD 与整除

- $\gcd(a,b)=1$ 时 $\varphi(ab)=\varphi(a)\varphi(b)$；若 $f$ 积性，则 $f(\prod p_i^{e_i})=\prod f(p_i^{e_i})$。
- 对 $n>1$，$[1,n]$ 中与 $n$ 互质的数之和为 $n\varphi(n)/2$。
- $\sum_{d\mid n}\varphi(d)=n$，$\sum_{i=1}^n\gcd(i,n)=\sum_{d\mid n}d\varphi(n/d)$。
- $\gcd(a_1+x,\ldots,a_n+x)=\gcd(a_1+x,a_2-a_1,\ldots,a_n-a_1)$。
- 令 $d=\gcd(a,m)$，模 $m$ 的 $x$ 中恰有 $\varphi(m/d)$ 个满足 $\gcd(a+x,m)=d$；准确条件为 $x=dt$ 且 $\gcd(a/d+t,m/d)=1$。
- 对 $n\ge2$，$\gcd_{i<j}(a_i a_j)=\gcd_{2\le j\le n}(a_j\gcd(a_1,\ldots,a_{j-1}))$，维护前缀 GCD 即可。
- 麦乐鸡定理：互质整数 $a,b>1$，不能表示为 $ax+by$（$x,y\ge0$）的最大整数为 $ab-a-b$，不能表示的正整数共有 $(a-1)(b-1)/2$ 个。
- 鸽巢原理：$N$ 个物体放入 $M>0$ 个盒子，至少一盒有 $\lceil N/M\rceil$ 个；前缀和按模 $m$ 分类常用于构造可被 $m$ 整除的区间和。

#### 斐波那契恒等式

约定 $F_0=0,F_1=1$。以下涉及 $F_{n-1}$ 时要求 $n\ge1$。
$$
F_{n+m}=F_nF_{m+1}+F_{n-1}F_m,\qquad
F_{n-1}F_{n+1}-F_n^2=(-1)^n.
$$
$$
F_n^2+F_{n+1}^2=F_{2n+1},\qquad F_{n+1}^2-F_{n-1}^2=F_{2n}.
$$
$$
\sum_{i=1}^n F_i=F_{n+2}-1,\quad
\sum_{i=1}^nF_{2i-1}=F_{2n},\quad
\sum_{i=1}^nF_{2i}=F_{2n+1}-1,\quad
\sum_{i=1}^n F_i^2=F_nF_{n+1}.
$$
$$
\sum_{i=1}^n iF_i=nF_{n+2}-F_{n+3}+2,\qquad
\sum_{i=1}^n(-1)^i F_i=(-1)^nF_{n-1}-1.
$$
- $\gcd(F_a,F_b)=F_{\gcd(a,b)}$。$a\mid b\Rightarrow F_a\mid F_b$；对 $a\ge3,b\ge1$ 反向也成立，$a=2$ 时因 $F_2=1$ 而例外。
- 对奇质数 $p\ne5$，$F_{p-(5/p)}\equiv0\pmod p$，其中 $(5/p)=1$（$p\equiv\pm1\pmod5$）或 $-1$（$p\equiv\pm2\pmod5$）。
- Zeckendorf 定理：每个正整数唯一表示为从 $F_2=1,F_3=2,\ldots$ 中选取若干个下标不相邻的数之和；从大到小贪心。

#### 按位运算与连续异或

对非负整数：
$$
x+y=(x\oplus y)+2(x\mathbin{\&}y)
=(x\mathbin{|}y)+(x\mathbin{\&}y).
$$
$\bigvee_i(X\mathbin{\&}a_i)=X\mathbin{\&}(\bigvee_i a_i)$，等于 $X$ 的前提是 $X$ 的每个 $1$ 位均被覆盖。C++ 的 `and`、`or` 是逻辑运算，不是位运算；按位用 `&`、`|`。

令 $t=(X-Y)/2$。两个非负整数满足 $a+b=X,a\oplus b=Y$ 的充要条件为 $X\ge Y$、$X-Y$ 为偶数、$t\mathbin{\&}Y=0$，可取 $(a,b)=(t,t+Y)$；三个非负整数只要求前两项，可取 $(t,t,Y)$。

连续异或 $0\oplus1\oplus\cdots\oplus n$ 按 $n\bmod4$ 周期变化。闭区间 $[l,r]$ 的异或为 `xor_prefix(r) ^ xor_prefix(l-1)`，$l=0$ 时第二项视为 $0$。

```cpp
int xor_prefix(int n) { // n>=0
    if (n % 4 == 0) return n;
    if (n % 4 == 1) return 1;
    if (n % 4 == 2) return n + 1;
    return 0;
}
```

#### 其他常用化简

- $x\ge0,m>0$：$x=\lfloor x/m\rfloor m+x\bmod m$。最少减去 $x\bmod m$，或加上 $(m-x\bmod m)\bmod m$，即可成为 $m$ 的倍数。
- 拉格朗日恒等式：$\sum_{i<j}(a_i b_j-a_j b_i)^2=(\sum_i a_i^2)(\sum_i b_i^2)-(\sum_i a_i b_i)^2$。
- $a,b>0$，令 $L=\operatorname{lcm}(a,b)$、$B=\max(a,b)$。对 $x\ge0$，$(x\bmod a)\bmod b\ne(x\bmod b)\bmod a$ 当且仅当 $x\bmod L\ge B$。
- $1\ldots n$ 中选三个数的最大 LCM：$n\le2$ 为 $n$；奇数 $n\ge3$ 为 $n(n-1)(n-2)$；偶数且 $3\nmid n$ 为 $n(n-1)(n-3)$；偶数且 $3\mid n$ 为 $(n-1)(n-2)(n-3)$。
- 长度 $2n$ 的数组任意均分为两个长度 $n$ 的数组，一个升序、另一个降序后对应绝对差之和，等于原数组最大的 $n$ 个数之和减去最小的 $n$ 个数之和。
- 若若干序列按各自原顺序交错合并，允许空前缀，则所有合并方式中能达到的最大前缀和，等于各序列最大前缀和之和。

### 常见例题

#### 删除最少元素，使等和划分不可能

正整数数组先做子集和判断：本来就不能等分时删 $0$ 个；能等分时删去 $v_2(a_i)$ 最小的任意一个元素即可。证明：全体除以最大的公共 $2$ 的幂后，总和为偶数，而被删元素为奇数，剩余总和为奇数。

#### 用恰好 $k$ 个 $2$ 的幂表示 $n$

要求 $n\ge1$、幂指数非负，充要条件为 $\operatorname{popcount}(n)\le k\le n$。从最高位开始将一个 $2^i$ 拆成两个 $2^{i-1}$，每次项数增加 $1$。以下返回每个幂次的数量，时间 $O(\log n)$；无解返回空。

```cpp
vector<int> split_powers(int n, int k) {
    int cnt = __builtin_popcountll(n);
    if (k < cnt || k > n) return {};
    vector<int> a(63);
    for (int i = 0; i < 63; ++i) a[i] = n >> i & 1;
    for (int i = 62; i > 0; --i) {
        int t = min(k - cnt, a[i]);
        a[i] -= t, a[i - 1] += 2 * t, cnt += t;
    }
    return a;
}
```

#### 有统一上界的非负整数解

求 $x_1+\cdots+x_n=m$，$0\le x_i<k$ 的方案数。$n,k\ge1,m\ge0$：
$$
\sum_{j=0}^{\min(n,\lfloor m/k\rfloor)}(-1)^j\binom nj\binom{m-jk+n-1}{n-1}.
$$
先隔板法，再对越界变量减去 $k$ 做容斥。依赖 `Comb`，预处理到 $\max(n,m+n-1)<mod$，时间 $O(\min(n,m/k)+1)$。

```cpp
Z bounded_compositions(int n, int k, int m, const Comb &comb) {
    Z ans = 0;
    for (int j = 0; j <= min(n, m / k); ++j) {
        Z t = comb.C(n, j) * comb.C(m - j * k + n - 1, n - 1);
        ans += j & 1 ? -t : t;
    }
    return ans;
}
```

<div style="page-break-after:always">/END/</div>
