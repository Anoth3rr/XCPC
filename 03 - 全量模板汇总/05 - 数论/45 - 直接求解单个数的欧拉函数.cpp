int phi(int n) {
    int ans = n;
    for(int i = 2; i <= n / i; i ++) {
        if(n % i == 0) {
            ans = ans / i * (i - 1);
            while(n % i == 0) n /= i;
        }
    }
    if(n > 1) ans = ans / n * (n - 1);
    return ans;
} 
