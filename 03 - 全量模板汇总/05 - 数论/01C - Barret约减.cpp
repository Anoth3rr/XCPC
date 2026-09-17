struct Barrett {
    int m;
    u128 B;
    Barrett(int m = 2) : m(m), B((u128(1) << 64) / m) {}
    friend int operator%(int a, const Barrett &mod) {
        u64 x = a < 0 ? -(i128)a : a;
        u64 q = mod.B * x >> 64;
        u64 r = x - q * mod.m;
        if (r >= mod.m) r -= mod.m;
        int ans = r;
        return a < 0 ? -ans : ans;
    }
    friend int operator%=(int &a, const Barrett &mod) {
        return a = a % mod;
    }
    friend int operator+(const int a, const Barrett &mod) {
        return a + mod.m;
    }
    friend int operator-(const int a, const Barrett &mod) {
        return a - mod.m;
    }
    friend int operator-(const Barrett &mod, const int a) {
        return mod.m - a;
    }
    friend int operator/(const Barrett &mod, const int a) {
        return mod.m / a;
    }
    friend int operator%(const Barrett &mod, const int a) {
        return mod.m % a;
    }
} mod;
