vector<pair<int, int>> edge(m);
for (auto &[u, v] : edge) { // C++17 结构化绑定，直接修改容器元素
    cin >> u >> v;
}
