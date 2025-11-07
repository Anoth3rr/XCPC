template<class Int>
struct Matrix {
    vector<vector<Int>> g;
    int n, m;

    Matrix(int n_, int m_): n(n_), m(m_) {
        g.assign(n, vector<Int>(m, 0));
    }

    Matrix Identity(int n_) {
        Matrix res(n_, n_);
        for (int i = 0; i < n_; ++i) res.g[i][i] = Int(1);
        return res;
    }

    constexpr Matrix &operator+=(Matrix rhs) & {
        assert(n == rhs.n);
        assert(m == rhs.m);
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < m; ++j) {
                g[i][j] += rhs.g[i][j];
            }
        }
        return *this;
    }

    friend constexpr Matrix operator+(Matrix lhs, Matrix rhs) {
        assert(lhs.n == rhs.n);
        assert(lhs.m == rhs.m);
        Matrix res = lhs;
        res += rhs;
        return res;
    }

    friend constexpr Matrix operator*(Matrix lhs, Matrix rhs) {
        assert(lhs.m == rhs.n);
        Matrix res(lhs.n, rhs.m);
        for (int i = 0; i < lhs.n; ++i) {
            for (int k = 0; k < lhs.m; ++k) {
                if (lhs.g[i][k] == Int(0)) continue;
                for (int j = 0; j < rhs.m; ++j) {
                    res.g[i][j] += lhs.g[i][k] * rhs.g[k][j];
                }
            }
        }
        return res;
    }

    friend istream &operator>>(istream &is, Matrix &a) {
        for (int i = 0; i < a.n; ++i) {
            for (int j = 0; j < a.m; ++j) {
                is >> a.g[i][j];
            }
        }
        return is;
    }

    friend constexpr ostream &operator<<(ostream &os, const Matrix &a) {
        for (int i = 0; i < a.n; ++i) {
            for (int j = 0; j < a.m; ++j) {
                os << a.g[i][j] << " ";
            }
            os << "\n";
        }
        return os;
    }
};
