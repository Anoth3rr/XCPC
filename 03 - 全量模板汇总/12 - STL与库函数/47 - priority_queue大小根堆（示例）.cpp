priority_queue<int> max_heap;
priority_queue<int, vector<int>, greater<int>> min_heap;

max_heap.push(x); // 注意，复杂度为 O(log SIZE)
max_heap.emplace(x); // 注意，复杂度为 O(log SIZE)

max_heap.pop(); // 注意，复杂度为 O(log SIZE)
max_heap.top(); // 复杂度为 O(1)
max_heap.empty();
max_heap.size();
