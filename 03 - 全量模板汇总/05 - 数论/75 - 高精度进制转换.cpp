struct numpy {
    vector<int> mp; // 将字符转化为数字
    vector<char> mp2; // 将数字转化为字符
    numpy() : mp(123), mp2(62) { // 0-9A-Za-z
        for (int i = 0; i < 10; i++) mp[i + 48] = i, mp2[i] = i + 48;
        for (int i = 10; i < 36; i++) mp[i + 55] = i, mp2[i] = i + 55;
        for (int i = 36; i < 62; i++) mp[i + 61] = i, mp2[i] = i + 61;
    }

    // 转换前进制 转换后进制 要转换的数据
    string solve(int a, int b, const string &s) {
        vector<int> nums, ans;
        for (auto c : s) {
            nums.push_back(mp[c]);
        }
        reverse(nums.begin(), nums.end());
        while (nums.size()) { // 短除法，将整个大数一直除 b ，取余数
            int remainder = 0;
            for (int i = nums.size() - 1; ~i; i--) {
                nums[i] += remainder * a;
                remainder = nums[i] % b;
                nums[i] /= b;
            }
            ans.push_back(remainder); // 得到余数
            while (nums.size() && nums.back() == 0) {
                nums.pop_back(); // 去掉前导 0
            }
        }
        reverse(ans.begin(), ans.end());

        string sh;
        for (int i : ans) sh += mp2[i];
        return sh;
    }
};
