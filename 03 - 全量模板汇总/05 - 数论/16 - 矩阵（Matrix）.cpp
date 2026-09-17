template <class T> struct Matrix {
    int n, m;
    vector<vector<T>> a;

    Matrix(int n = 0, int m = 0, T val = T()) : n(n), m(m), a(n, vector<T>(m, val)) {}

    static Matrix identity(int n) {
        Matrix I(n, n);
        for (int i = 0; i < n; ++i) I.a[i][i] = T(1);
        return I;
    }

    vector<T> &operator[](int i) {
        return a[i];
    }
    const vector<T> &operator[](int i) const {
        return a[i];
    }

    Matrix operator+(const Matrix &other) const {
        assert(n == other.n && m == other.m);
        Matrix res(n, m);
        for (int i = 0; i < n; ++i)
            for (int j = 0; j < m; ++j) res[i][j] = a[i][j] + other[i][j];
        return res;
    }

    Matrix operator-(const Matrix &other) const {
        assert(n == other.n && m == other.m);
        Matrix res(n, m);
        for (int i = 0; i < n; ++i)
            for (int j = 0; j < m; ++j) res[i][j] = a[i][j] - other[i][j];
        return res;
    }

    Matrix operator*(const Matrix &other) const {
        assert(m == other.n);
        Matrix res(n, other.m, T(0));
        for (int i = 0; i < n; ++i)
            for (int k = 0; k < m; k++)
                for (int j = 0; j < other.m; ++j) res[i][j] = res[i][j] + a[i][k] * other[k][j];
        return res;
    }

    Matrix operator*(const T &k) const {
        Matrix res(n, m);
        for (int i = 0; i < n; ++i)
            for (int j = 0; j < m; ++j) res[i][j] = a[i][j] * k;
        return res;
    }

    Matrix transpose() const {
        Matrix res(m, n);
        for (int i = 0; i < n; ++i)
            for (int j = 0; j < m; ++j) res[j][i] = a[i][j];
        return res;
    }

    Matrix power(int b) const {
        assert(n == m);
        Matrix a = *this;
        Matrix r = identity(n);
        for (; b; b >>= 1, a = a * a)
            if (b & 1) r = r * a;
        return r;
    }
};

Matrix<int> inverse(Matrix<T> a, int mod) {
    int n = a.n;
    Matrix<int> b = Matrix<int>::identity(n);

    for (int i = 0; i < n; ++i) {
        int p = i;
        while (p < n && a[p][i] % mod == 0) ++p;
        if (p == n) return {};

        swap(a[p], a[i]);
        swap(b[p], b[i]);

        int iv = power((a[i][i] % mod + mod) % mod, mod - 2, mod);
        for (int j = 0; j < n; ++j) {
            a[i][j] = a[i][j] * iv % mod;
            b[i][j] = b[i][j] * iv % mod;
        }

        for (int k = 0; k < n; ++k) {
            if (k == i) continue;
            int c = a[k][i];
            for (int j = 0; j < n; ++j) {
                a[k][j] = (a[k][j] - c * a[i][j] % mod + mod) % mod;
                b[k][j] = (b[k][j] - c * b[i][j] % mod + mod) % mod;
            }
        }
    }
    return b;
}