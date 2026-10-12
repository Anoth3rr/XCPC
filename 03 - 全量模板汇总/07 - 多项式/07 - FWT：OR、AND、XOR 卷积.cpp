struct FWT {
    static void fwt(vector<Z> &a, char op, bool iv = false) {
        int n = a.size();
        assert(n && !(n & (n - 1)));
        assert(op == '|' || op == '&' || op == '^');
        for (int k = 1; k < n; k <<= 1) {
            for (int i = 0; i < n; i += k << 1) {
                for (int j = 0; j < k; j++) {
                    Z x = a[i + j], y = a[i + j + k];
                    if (op == '|') {
                        a[i + j + k] = iv ? y - x : y + x;
                    } else if (op == '&') {
                        a[i + j] = iv ? x - y : x + y;
                    } else {
                        a[i + j] = x + y;
                        a[i + j + k] = x - y;
                    }
                }
            }
        }
        if (op == '^' && iv) {
            Z invn = Z(n).inv();
            for (Z &x : a) x *= invn;
        }
    }

    static vector<Z> mul(vector<Z> a, vector<Z> b, char op) {
        if (a.empty() || b.empty()) return {};
        int n = 1;
        while (n < max(a.size(), b.size())) n <<= 1;
        a.resize(n), b.resize(n);
        fwt(a, op), fwt(b, op);
        for (int i = 0; i < n; i++) a[i] *= b[i];
        fwt(a, op, true);
        return a;
    }
};

vector<Z> operator|(vector<Z> a, vector<Z> b) {
    return FWT::mul(move(a), move(b), '|');
}
vector<Z> operator&(vector<Z> a, vector<Z> b) {
    return FWT::mul(move(a), move(b), '&');
}
vector<Z> operator^(vector<Z> a, vector<Z> b) {
    return FWT::mul(move(a), move(b), '^');
}
