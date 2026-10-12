transform(a.begin(), a.end(), a.begin(), [](int v) {
    return v * v;
}); // 赋值为每个元素的平方
