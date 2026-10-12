struct SequenceAutomaton {
    vector<vector<int>> ver;

    SequenceAutomaton(vector<int> &in, int size) : ver(size + 1) {
        for (int i = 0; i < in.size(); i++) {
            ver[in[i]].push_back(i + 1);
        }
    }
    bool contains(vector<int> &in) {
        int at = 0;
        for (auto &i : in) {
            auto it = lower_bound(ver[i].begin(), ver[i].end(), at + 1);
            if (it == ver[i].end()) {
                return false;
            }
            at = *it;
        }
        return true;
    }
};
