string s;
s += 'a';
s += "abc";

int n = 5;
s.resize(2 * n); // 改变 size()，新增元素为 '\0'
s.assign(n, 'b'); // 将内容替换为 n 个 'b'
s.reserve(2 * n); // 预留空间（注意，不实际改变 size()），避免多次扩容，略微提高效率
