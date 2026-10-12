int lim = 10, sum = 0;

auto by_ref = [&](int x) {
    sum += x; // 修改外部 sum
    return x <= lim; // 每次读取当前 lim，这里使用的是为更新后的 lim = 3
};

auto by_val = [=](int x) {
    return x <= lim; // 保存构造 Lambda 时的 lim = 10
};

lim = 3;
cout << by_ref(4) << '\n'; // false，且使得 sum 增加 4
cout << by_val(4) << '\n'; // true
