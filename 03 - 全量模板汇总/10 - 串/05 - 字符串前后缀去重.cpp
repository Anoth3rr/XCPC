string compress(vector<string> in) { // 前后缀压缩
    vector<U> hash1{1};
    vector<V> hash2{1};
    string ans = "#";
    for (auto s : in) {
        s = "#" + s;
        int st = 0;
        U chk1 = 0;
        V chk2 = 0;
        for (int j = 1; j < s.size() && j < ans.size(); j++) {
            chk1 = chk1 * base1 + s[j];
            chk2 = chk2 * base2 + s[j];
            if ((hash1.back() == hash1[ans.size() - 1 - j] * val1[j] + chk1) &&
                (hash2.back() == hash2[ans.size() - 1 - j] * val2[j] + chk2)) {
                st = j;
            }
        }
        for (int j = st + 1; j < s.size(); j++) {
            ans += s[j];
            hash1.push_back(hash1.back() * base1 + s[j]);
            hash2.push_back(hash2.back() * base2 + s[j]);
        }
    }
    return ans.substr(1);
}
