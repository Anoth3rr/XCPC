auto check = [&](int n) {
    if (n <= 1)
        return false;
    int p = -1;

    for (int i = 2; i * i <= n; ++i) {
        if (n % i == 0) {
            p = i;
            while (n % i == 0)
                n /= i;
            break;
        }
    }

    if (p == -1)
        return true;

    return n == 1;
};

auto check = [&](int n) {
    if (n <= 1)
        return false;
    int p = spf[n];
    while (n % p == 0)
        n /= p;
    return n == 1;
};