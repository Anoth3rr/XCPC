const int N = 110, mod = 1e9 + 7;
LL n, k, a[N][N], b[N][N], t[N][N];
void matrixQp(LL y){
    while (y){
        if (y & 1){
            memset(t, 0, sizeof t);
            for (int i = 1; i <= n; i ++ )
                for (int j = 1; j <= n; j ++ )
                    for (int k = 1; k <= n; k ++ )
                        t[i][j] = ( t[i][j] + (a[i][k] * b[k][j]) % mod ) % mod;
            memcpy(b, t, sizeof t);
        }
        y >>= 1;
        memset(t, 0, sizeof t);
        for (int i = 1; i <= n; i ++ )
            for (int j = 1; j <= n; j ++ )
                for (int k = 1; k <= n; k ++ )
                    t[i][j] = ( t[i][j] + (a[i][k] * a[k][j]) % mod ) % mod;
        memcpy(a, t, sizeof t);
    }
}
int main(){
    cin >> n >> k;
    for (int i = 1; i <= n; i ++ )
        for (int j = 1; j <= n; j ++ ){
            cin >> b[i][j];
            a[i][j] = b[i][j];
        }
    matrixQp(k - 1);
    for (int i = 1; i <= n; i ++ )
        for (int j = 1; j <= n; j ++ )
            cout << b[i][j] << " \n"[j == n];
    return 0;
}
