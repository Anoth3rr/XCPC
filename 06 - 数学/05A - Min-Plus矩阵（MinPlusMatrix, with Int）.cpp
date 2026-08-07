// 性能：默认使用宏展开后的 int；若大规模数据因此超时或超空间，可将纯下标、状态或计数局部改为 i32。

template <int N, class T = int> struct Matrix {
    static constexpr T INF = numeric_limits<T>::max() / 4;
    array<array<T, N>, N> a;

    Matrix(T v = INF) {
        for (int i = 0; i < N; ++i) a[i].fill(v);
    }

    static Matrix unit() {
        Matrix res;
        for (int i = 0; i < N; ++i) res[i][i] = 0;
        return res;
    }

    array<T, N> &operator[](int i) {
        return a[i];
    }
    const array<T, N> &operator[](int i) const {
        return a[i];
    }

    Matrix operator+(const Matrix &o) const {
        Matrix res;
        for (int i = 0; i < N; ++i)
            for (int j = 0; j < N; ++j) res[i][j] = min(a[i][j], o[i][j]);
        return res;
    }

    Matrix operator*(const Matrix &o) const {
        Matrix res;
        for (int i = 0; i < N; ++i) {
            for (int k = 0; k < N; ++k) {
                if (a[i][k] >= INF) continue;
                for (int j = 0; j < N; ++j) {
                    if (o[k][j] >= INF) continue;
                    res[i][j] = min(res[i][j], a[i][k] + o[k][j]);
                }
            }
        }
        return res;
    }

    Matrix &operator+=(const Matrix &o) {
        return *this = *this + o;
    }
    Matrix &operator*=(const Matrix &o) {
        return *this = *this * o;
    }

    Matrix ksm(int b) const {
        assert(b >= 0);
        Matrix a = *this, res = unit();
        while (b) {
            if (b & 1) res *= a;
            a *= a;
            b >>= 1;
        }
        return res;
    }
};
