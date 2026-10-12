template <int G = 3> struct Poly {
    static void ntt(vector<Z> &a, bool iv) {
        int n = a.size();
        assert(n && !(n & (n - 1)));
        if (n == 1) return;

        int h = __builtin_ctzll(n);
        int rank = __builtin_ctzll(Z::getMod() - 1);
        assert(h <= rank);

        static array<Z, 64> root, iroot, rate2, rate3, irate3;
        static decltype(Z::getMod()) cached_mod = 0;
        if (cached_mod != Z::getMod()) {
            cached_mod = Z::getMod();
            root[rank] = power(Z(G), (Z::getMod() - 1) >> rank);
            iroot[rank] = root[rank].inv();
            for (int i = rank - 1; i >= 0; i--) {
                root[i] = root[i + 1] * root[i + 1];
                iroot[i] = iroot[i + 1] * iroot[i + 1];
            }

            Z prod = 1;
            for (int i = 0; i + 2 <= rank; i++) {
                rate2[i] = root[i + 2] * prod;
                prod *= iroot[i + 2];
            }

            prod = 1;
            Z iprod = 1;
            for (int i = 0; i + 3 <= rank; i++) {
                rate3[i] = root[i + 3] * prod;
                irate3[i] = iroot[i + 3] * iprod;
                prod *= iroot[i + 3];
                iprod *= root[i + 3];
            }
        }

        if (!iv) {
            int len = 0;
            while (len < h) {
                int blocks = 1LL << len;
                if (h - len == 1) {
                    Z rot = 1;
                    for (int s = 0; s < blocks; s++) {
                        int offset = s << 1;
                        Z l = a[offset], r = a[offset + 1] * rot;
                        a[offset] = l + r;
                        a[offset + 1] = l - r;
                        if (s + 1 < blocks) rot *= rate2[__builtin_ctzll(~s)];
                    }
                    len++;
                } else {
                    int p = 1LL << (h - len - 2);
                    Z rot = 1, imag = root[2];
                    for (int s = 0; s < blocks; s++) {
                        Z rot2 = rot * rot, rot3 = rot2 * rot;
                        int offset = s << (h - len);
                        for (int i = 0; i < p; i++) {
                            Z a0 = a[offset + i];
                            Z a1 = a[offset + i + p] * rot;
                            Z a2 = a[offset + i + 2 * p] * rot2;
                            Z a3 = a[offset + i + 3 * p] * rot3;
                            Z t = (a1 - a3) * imag;
                            a[offset + i] = a0 + a2 + a1 + a3;
                            a[offset + i + p] = a0 + a2 - a1 - a3;
                            a[offset + i + 2 * p] = a0 - a2 + t;
                            a[offset + i + 3 * p] = a0 - a2 - t;
                        }
                        if (s + 1 < blocks) rot *= rate3[__builtin_ctzll(~s)];
                    }
                    len += 2;
                }
            }
        } else {
            int len = h;
            while (len) {
                if (len == 1) {
                    int p = n / 2;
                    for (int i = 0; i < p; i++) {
                        Z l = a[i], r = a[i + p];
                        a[i] = l + r;
                        a[i + p] = l - r;
                    }
                    len--;
                } else {
                    int p = 1LL << (h - len);
                    int blocks = 1LL << (len - 2);
                    Z rot = 1, imag = iroot[2];
                    for (int s = 0; s < blocks; s++) {
                        Z rot2 = rot * rot, rot3 = rot2 * rot;
                        int offset = s << (h - len + 2);
                        for (int i = 0; i < p; i++) {
                            Z a0 = a[offset + i];
                            Z a1 = a[offset + i + p];
                            Z a2 = a[offset + i + 2 * p];
                            Z a3 = a[offset + i + 3 * p];
                            Z t = (a2 - a3) * imag;
                            a[offset + i] = a0 + a1 + a2 + a3;
                            a[offset + i + p] = (a0 - a1 + t) * rot;
                            a[offset + i + 2 * p] = (a0 + a1 - a2 - a3) * rot2;
                            a[offset + i + 3 * p] = (a0 - a1 - t) * rot3;
                        }
                        if (s + 1 < blocks) rot *= irate3[__builtin_ctzll(~s)];
                    }
                    len -= 2;
                }
            }
            Z invn = Z(n).inv();
            for (Z &x : a) x *= invn;
        }
    }

    static vector<Z> mul(vector<Z> a, vector<Z> b) {
        int n = a.size(), m = b.size();
        if (!n || !m) return {};

        if (min(n, m) <= 64) {
            vector<Z> c(n + m - 1);
            for (int i = 0; i < n; i++)
                for (int j = 0; j < m; j++) c[i + j] += a[i] * b[j];
            return c;
        }

        int cnt = 1;
        while (cnt < n + m - 1) cnt <<= 1;

        if (n + m - 3 <= cnt / 2) {
            Z al = a.back(), bl = b.back();
            a.pop_back(), b.pop_back();
            vector<Z> c = mul(a, b);
            c.resize(n + m - 1);
            c[n + m - 2] = al * bl;
            for (int i = 0; i < n - 1; i++) c[i + m - 1] += a[i] * bl;
            for (int i = 0; i < m - 1; i++) c[i + n - 1] += b[i] * al;
            return c;
        }

        a.resize(cnt), b.resize(cnt);
        bool same = a == b;
        ntt(a, false);
        if (same) {
            for (Z &x : a) x *= x;
        } else {
            ntt(b, false);
            for (int i = 0; i < cnt; i++) a[i] *= b[i];
        }
        ntt(a, true);
        a.resize(n + m - 1);
        return a;
    }
};

using NTT = Poly<3>;
