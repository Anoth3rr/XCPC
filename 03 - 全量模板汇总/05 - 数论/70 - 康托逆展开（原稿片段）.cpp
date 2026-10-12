string str;
int kangtuo(){
    int ans = 1;  //注意，因为 12345 是算作0开始计算的，最后结果要把12345看作是第一个
    int len = str.length();
    for(int i = 0; i < len; i++){
        int tmp = 0;//用来计数的
        for(int j = i + 1; j < len; j++){
            if(str[i] > str[j]) tmp++;
            //计算str[i]是第几大的数，或者说计算有几个比他小的数
        }
        ans += tmp * f[len - i - 1];
    }
    return ans;
}
int main(){
    jie_cheng(10);
    string str = "52413";
    cout<<kangtuo()<<endl;
}
