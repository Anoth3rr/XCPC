deque<int> D;
int n,k,x,a[MAX];
int main(){
    IOS();
    cin>>n>>k;
    for(int i=1;i<=n;i++) cin>>a[i];
    for(int i=1;i<=n;i++){
        while(!D.empty() && a[D.back()]<=a[i]) D.pop_back();
        D.emplace_back(i);
        if(!D.empty()) if(i-D.front()>=k) D.pop_front();
        if(i>=k)cout<<a[D.front()]<<endl;
    }
    return 0;
}
