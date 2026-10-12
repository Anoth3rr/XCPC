// 参数依次为“开始位置（大于 size() 会越界报错）”，“长度（留空自动取到末尾，超过末尾自动截断）”
string p = "ABCDEFG";
cout << p.substr(2, 2) << "\n"; // CD
cout << p.substr(2) << "\n"; // CDEFG
cout << p.substr(2, 10) << "\n"; // CDEFG
