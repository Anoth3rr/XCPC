cout << atoi("12") << '\n'; // 12
cout << atoi("   -12abc") << '\n'; // -12
cout << atoi("abc12") << '\n'; // 0

string s = "1234567890123";
i64 x = atoll(s.c_str());
