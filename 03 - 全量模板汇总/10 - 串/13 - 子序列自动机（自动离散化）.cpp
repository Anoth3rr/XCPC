template<class T> struct SequenceAutomaton {
    vector<T> alls;
    vector<vector<int>> ver;

    SequenceAutomaton(auto in) {
        for (auto &i : in) {
            alls.push_back(i);
        }
        sort(alls.begin(), alls.end());
        alls.erase(unique(alls.begin(), alls.end()), alls.end());

        ver.resize(alls.size() + 1);
        for (int i = 0; i < in.size(); i++) {
            ver[get(in[i])].push_back(i + 1);
        }
    }
    bool count(T x) {
        return binary_search(alls.begin(), alls.end(), x);
    }
    int get(T x) {
        return lower_bound(alls.begin(), alls.end(), x) - alls.begin();
    }
    bool contains(auto in) {
        int at = 0;
        for (auto &i : in) {
            if (!count(i)) {
                return false;
            }

            auto j = get(i);
            auto it = lower_bound(ver[j].begin(), ver[j].end(), at + 1);
            if (it == ver[j].end()) {
                return false;
            }
            at = *it;
        }
        return true;
    }
};
