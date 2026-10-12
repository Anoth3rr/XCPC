 Z clac(int n, int k, int m) {
    Z ans = 0; 
        ans += C(n, i) * C(m - i * k + n - 1, n - 1) * pow(-1, i);
    }
    return ans;
}
