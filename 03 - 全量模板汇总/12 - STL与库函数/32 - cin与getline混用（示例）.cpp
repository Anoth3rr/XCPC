int x;
cin >> x;

string line;
getline(cin, line); // 空读，丢弃当前行剩余内容
// 也可以使用 cin.ignore(numeric_limits<streamsize>::max(), '\n');
getline(cin, line);
