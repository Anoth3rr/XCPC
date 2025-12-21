template<class Int>
struct Matrix {
    int n, m;
    vector<vector<Int>> a;

    Matrix(int n = 0, int m = 0, Int val = Int())
        : n(n), m(m), a(n, vector<Int>(m, val)) {}

    static Matrix identity(int n) {
        Matrix I(n, n);
        for (int i = 0; i < n; i++)
            I.a[i][i] = Int(1);
        return I;
    }

    vector<Int>& operator[](int i) {
        return a[i];
    }
    const vector<Int>& operator[](int i) const {
        return a[i];
    }

    Matrix operator + (const Matrix& other) const {
        assert(n == other.n && m == other.m);
        Matrix res(n, m);
        for (int i = 0; i < n; i++)
            for (int j = 0; j < m; j++)
                res[i][j] = a[i][j] + other[i][j];
        return res;
    }

    Matrix operator - (const Matrix& other) const {
        assert(n == other.n && m == other.m);
        Matrix res(n, m);
        for (int i = 0; i < n; i++)
            for (int j = 0; j < m; j++)
                res[i][j] = a[i][j] - other[i][j];
        return res;
    }

    Matrix operator*(const Matrix& other) const {
        assert(m == other.n);
        Matrix res(n, other.m, Int(0));
        for (int i = 0; i < n; i++)
            for (int k = 0; k < m; k++)
                for (int j = 0; j < other.m; j++)
                    res[i][j] = res[i][j] + a[i][k] * other[k][j];
        return res;
    }

    Matrix operator*(const Int& k) const {
        Matrix res(n, m);
        for (int i = 0; i < n; i++)
            for (int j = 0; j < m; j++)
                res[i][j] = a[i][j] * k;
        return res;
    }

    Matrix transpose() const {
        Matrix res(m, n);
        for (int i = 0; i < n; i++)
            for (int j = 0; j < m; j++)
                res[j][i] = a[i][j];
        return res;
    }

    Matrix ksm(ll exp) const {
        assert(n == m);
        Matrix base = *this;
        Matrix res = identity(n);
        while (exp > 0) {
            if (exp & 1) res = res * base;
            base = base * base;
            exp >>= 1;
        }
        return res;
    }
};
