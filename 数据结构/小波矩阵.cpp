struct BitRank {
	vector<ull> block;
	vector<int> count;
	BitRank() {}
	void resize(const int num) {
		block.assign(((num + 1) >> 6) + 1, 0);
		count.assign(block.size(), 0);
	}

	void set(const int i, const ll val) {
		block[i >> 6] |= (val << (i & 63));
	}

	void build() {
		for (int i = 1; i < block.size(); i++) {
			count[i] = count[i - 1] + __builtin_popcountll(block[i - 1]);
		}
	}

	int rank1(const int i) const {
		return count[i >> 6] + __builtin_popcountll(block[i >> 6] & ((1ull << (i & 63)) - 1ull));
	}

	int rank1(const int i, const int j) const {
		return rank1(j) - rank1(i);
	}

	int rank0(const int i) const {
		return i - rank1(i);
	}

	int rank0(const int i, const int j) const {
		return rank0(j) - rank0(i);
	}
};

class WaveletMatrix {
private:
	int height;
	vector<BitRank> B;
	vector<int> pos;

public:
	WaveletMatrix() {}
	WaveletMatrix(vector<int> vec) : WaveletMatrix(vec, *max_element(vec.begin(), vec.end()) + 1) {}
	WaveletMatrix(vector<int> vec, const int sigma) {
		init(vec, sigma);
	}

	void init(vector<int>& vec, const int sigma) {
		height = (sigma == 1) ? 1 : (64 - __builtin_clzll(sigma - 1));
		B.resize(height), pos.resize(height);
		for (int i = 0; i < height; ++i) {
			B[i].resize(vec.size());
			for (int j = 0; j < vec.size(); ++j) {
				B[i].set(j, get(vec[j], height - i - 1));
			}
			B[i].build();
			auto it = stable_partition(vec.begin(), vec.end(), [&](int c) {
				return !get(c, height - i - 1);
			});
			pos[i] = it - vec.begin();
		}
	}

	int get(const int val, const int i) {
		return val >> i & 1;
	}

	int rank(const int val, const int l, const int r) {
		return rank(val, r) - rank(val, l);
	}

	int rank(int val, int i) {
		int p = 0;
		for (int j = 0; j < height; ++j) {
			if (get(val, height - j - 1)) {
				p = pos[j] + B[j].rank1(p);
				i = pos[j] + B[j].rank1(i);
			} else {
				p = B[j].rank0(p);
				i = B[j].rank0(i);
			}
		}
		return i - p;
	}

	int quantile(int k, int l, int r) {
		int res = 0;
		for (int i = 0; i < height; ++i) {
			const int j = B[i].rank0(l, r);
			if (j > k) {
				l = B[i].rank0(l);
				r = B[i].rank0(r);
			} else {
				l = pos[i] + B[i].rank1(l);
				r = pos[i] + B[i].rank1(r);
				k -= j;
				res |= (1 << (height - i - 1));
			}
		}
		return res;
	}

	int rangefreq(const int i, const int j, const int a, const int b, const int l, const int r, const int x) {
		if (i == j || r <= a || b <= l) return 0;
		const int mid = (l + r) >> 1;
		if (a <= l && r <= b) {
			return j - i;
		} else {
			const int left = rangefreq(B[x].rank0(i), B[x].rank0(j), a, b, l, mid, x + 1);
			const int right = rangefreq(pos[x] + B[x].rank1(i), pos[x] + B[x].rank1(j), a, b, mid, r, x + 1);
			return left + right;
		}
	}

	int rangefreq(const int l, const int r, const int a, const int b) {
		return rangefreq(l, r, a, b, 0, 1 << height, 0);
	}

	int rangemin(const int i, const int j, const int a, const int b, const int l, const int r, const int x, const int val) {
		if (i == j || r <= a || b <= l) return -1;
		if (r - l == 1) return val;
		const int mid = (l + r) >> 1;
		const int res = rangemin(B[x].rank0(i), B[x].rank0(j), a, b, l, mid, x + 1, val);
		if (res < 0) {
			return rangemin(pos[x] + B[x].rank1(i), pos[x] + B[x].rank1(j), a, b, mid, r, x + 1, val + (1 << (height - x - 1)));
		} else {
			return res;
		}
	}

	int rangemin(int l, int r, int a, int b) {
		return rangemin(l, r, a, b, 0, 1 << height, 0, 0);
	}
};