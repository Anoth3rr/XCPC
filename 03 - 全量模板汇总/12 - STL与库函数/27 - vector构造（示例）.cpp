int n = 5;
vector<int> a(n); // n 个 0
vector<int> b(n, 7); // n 个 7
vector<int> c = {4, 1, 5};

a.resize(2 * n); // 改变 size()，新增元素为 0
a.assign(n, -1); // 将内容替换为 n 个 -1
a.reserve(2 * n); // 预留空间（注意，不实际改变 size()），避免多次扩容，略微提高效率

vector dp(n, vector(m, vector<int>(k, 0))); // C++17 类模板实参推导
