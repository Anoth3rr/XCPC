auto [it1, it2] = mm.equal_range(2);
for (auto it = it1; it != it2; ++ it) {
    cout << it->second << " ";
}
