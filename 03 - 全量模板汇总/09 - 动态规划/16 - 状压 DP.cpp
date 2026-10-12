#include <bits/stdc++.h>
using namespace std;
#define LL long long
const int N = 15, M = 150, K = 1500;
LL n, k;
LL cnt[K];    //每个状态的二进制中 1 的数量
LL tot;    //合法状态的数量
LL st[K];    //合法的状态
LL dp[N][M][K];    //第 i 行，放置了 j 个国王，状态为 k 的方案数
int main(){
    ios::sync_with_stdio(false);cin.tie(0);
    cin >> n >> k;
    for (int s = 0; s < (1 << n); s ++ ){  //找出合法状态
        LL sum = 0, t = s;
        while(t){  //计算 1 的数量
            sum += (t & 1);
            t >>= 1;
        }
        cnt[s] = sum;
        if ( (( (s << 1) | (s >> 1) ) & s) == 0 ){  //判断合法性
            st[ ++ tot] = s;
        }
    }
    dp[0][0][0] = 1;
    for (int i = 1; i <= n + 1; i ++ ){
        for (int j1 = 1; j1 <= tot; j1 ++ ){    //当前的状态
            LL s1 = st[j1];
            for (int j2 = 1; j2 <= tot; j2 ++ ){    //上一行的状态
                LL s2 = st[j2];
                if ( ( (s2 | (s2 << 1) | (s2 >> 1)) & s1 ) == 0 ){
                    for (int j = 0; j <= k; j ++ ){
                        if (j - cnt[s1] >= 0)
                            dp[i][j][s1] += dp[i - 1][j - cnt[s1]][s2];
                    }
                }
            }
        }
    }
    cout << dp[n + 1][k][0] << "\n";
    return 0;
}
