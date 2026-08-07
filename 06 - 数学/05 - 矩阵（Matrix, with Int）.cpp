
template <class T> struct Matrix {
    int n, m;
    vector<vector<T>> a;

    Matrix(int n = 0, int m = 0, T val = T()) : n(n), m(m), a(n, vector<T>(m, val)) {}

    static Matrix unit(int n) {
        Matrix I(n, n);
        for (int i = 0; i < n; i++) I.a[i][i] = T(1);
        return I;
    }

    vector<T> &operator[](int i) {
        return a[i];
    }
    const vector<T> &operator[](int i) const {
        return a[i];
    }

    Matrix operator+(const Matrix &o) const {
        assert(n == o.n && m == o.m);
        Matrix res(n, m);
        for (int i = 0; i < n; i++)
            for (int j = 0; j < m; j++) res[i][j] = a[i][j] + o[i][j];
        return res;
    }

    Matrix operator-(const Matrix &o) const {
        assert(n == o.n && m == o.m);
        Matrix res(n, m);
        for (int i = 0; i < n; i++)
            for (int j = 0; j < m; j++) res[i][j] = a[i][j] - o[i][j];
        return res;
    }

    Matrix operator*(const Matrix &o) const {
        assert(m == o.n);
        Matrix res(n, o.m, T(0));
        for (int i = 0; i < n; i++)
            for (int k = 0; k < m; k++)
                for (int j = 0; j < o.m; j++) res[i][j] = res[i][j] + a[i][k] * o[k][j];
        return res;
    }

    Matrix operator*(const T &k) const {
        Matrix res(n, m);
        for (int i = 0; i < n; i++)
            for (int j = 0; j < m; j++) res[i][j] = a[i][j] * k;
        return res;
    }

    Matrix trans() const {
        Matrix res(m, n);
        for (int i = 0; i < n; i++)
            for (int j = 0; j < m; j++) res[j][i] = a[i][j];
        return res;
    }

    Matrix ksm(int exp) const {
        assert(n == m);
        assert(exp >= 0);
        Matrix bas = *this;
        Matrix res = unit(n);
        while (exp > 0) {
            if (exp & 1) res = res * bas;
            bas = bas * bas;
            exp >>= 1;
        }
        return res;
    }
};

template <class T> T det(int n, Matrix<T> &mat) {
    T det = T(1);
    int sgn = 1;

    for (int i = 0; i < n; i++) {
        int piv = i;
        while (piv < n && mat[piv][i] == T()) piv++;
        if (piv == n) return T();

        if (piv != i) {
            swap(mat[i], mat[piv]);
            sgn = -sgn;
        }

        det *= mat[i][i];
        for (int j = i + 1; j < n; j++) {
            if (!(mat[j][i] == T())) {
                T k = mat[j][i] / mat[i][i];
                for (int k = i; k < n; k++) {
                    mat[j][k] -= k * mat[i][k];
                }
            }
        }
    }

    if (sgn == -1) {
        det = T() - det;
    }
    return det;
}
