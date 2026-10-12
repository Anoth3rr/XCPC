constexpr int N = 1E7;

bitset<N + 1> isp;

void init() {
    isp.set(); // 将所有位置设为 1
    isp[0] = isp[1] = 0;
    for (int i = 2; i <= N / i; i++) {
        if (isp[i]) {
            for (int j = i * i; j <= N; j += i) isp[j] = 0;
        }
    }
}

bool isprime(int n) {
    return isp[n];
}
