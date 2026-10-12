for (auto it = a.begin(); it != a.end();) {
    if (*it < 0) it = a.erase(it); // erase 返回下一个迭代器
    else ++it;
}
