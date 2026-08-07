# Python 竞赛短代码

以下片段面向 Python 3 标准库，按需复制，不依赖第三方包。

## 快速输入输出

```python
import sys
input = sys.stdin.buffer.readline

n = int(input())
a = list(map(int, input().split()))
sys.stdout.write(" ".join(map(str, a)))
```

一次读完所有整数，适合输入结构简单的数据：

```python
import sys
it = iter(map(int, sys.stdin.buffer.read().split()))
n = next(it)
a = [next(it) for _ in range(n)]
```

## 大整数与常用整数函数

```python
import sys
from math import gcd, lcm, isqrt, comb, perm

if hasattr(sys, "set_int_max_str_digits"):
    sys.set_int_max_str_digits(0)   # Python 3.11+ 允许超长十进制整数

x = int(input())
y = pow(x, 10)          # 大整数幂
z = pow(x, -1, 998244353)  # 模逆，要求互质
r = isqrt(x)            # floor(sqrt(x))，完全精确
c = comb(1000, 500)
```

## 进制转换

```python
x = int("101101", 2)
s2, s8, s16 = bin(x)[2:], oct(x)[2:], hex(x)[2:]

def base(x, b):
    d = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ"
    if x == 0:
        return "0"
    sg = "-" if x < 0 else ""
    x = abs(x)
    s = ""
    while x:
        x, r = divmod(x, b)
        s += d[r]
    return sg + s[::-1]
```

## 精确分数与高精度小数

```python
from fractions import Fraction

a = Fraction(1, 3) + Fraction(2, 5)
print(a.numerator, a.denominator)
```

```python
from decimal import Decimal, getcontext

getcontext().prec = 100
x = Decimal(2).sqrt()
print(x)
```

`Decimal` 必须从字符串或整数构造；从 `float` 构造会把二进制浮点误差带入。

## 枚举与组合暴力

```python
from itertools import permutations, combinations, product

for p in permutations(range(n)):
    pass
for s in combinations(range(n), k):
    pass
for a in product(range(3), repeat=n):  # 3^n 状态
    pass
```

## 常用容器

```python
from collections import Counter, defaultdict, deque
from bisect import bisect_left, bisect_right, insort
from heapq import heappush, heappop

cnt = Counter(a)
g = defaultdict(list)
q = deque([s])
p = bisect_left(a, x)
```

最小堆直接存值；最大堆存相反数：

```python
h = []
heappush(h, -x)
x = -heappop(h)
```

## 用 Python 整数充当动态位集

子集和可达性：

```python
f = 1
for x in a:
    f |= f << x
ok = (f >> s) & 1
```

枚举置位位置：

```python
x = f
while x:
    b = x & -x
    i = b.bit_length() - 1
    x ^= b
```

## 记忆化与递归

```python
import sys
from functools import cache
sys.setrecursionlimit(1_000_000)

@cache
def dfs(x, y):
    return 0
```

链状树和百万层状态不要仅依赖 `setrecursionlimit`，应改写为显式栈。

## 分组与排序键

```python
from itertools import groupby

a.sort(key=lambda x: (x[0], -x[1]))
for k, g in groupby(a, key=lambda x: x[0]):
    g = list(g)
```

## C++ 式向 0 整除

Python 的 `//` 对负数向下取整；需要复现 C++ 行为时：

```python
def div0(a, b):
    q = abs(a) // abs(b)
    return -q if (a < 0) ^ (b < 0) else q

q = div0(a, b)
r = a - q * b
```
