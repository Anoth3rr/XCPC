string s = "AAABCDEFGGAG";
if (s.find("go") == string::npos) {
    cout << "not found" << '\n';
}

cout << s.find("A") << '\n'; // 返回第一次出现的位置：0
cout << s.find("A", 3) << '\n'; // 从位置 3 开始查找第一次出现的位置：10
cout << s.rfind("A") << '\n'; // 返回最后一次出现的位置：10
cout << s.rfind("A", 9) << '\n'; // 从位置 9 开始查找最后一次出现的位置：2
