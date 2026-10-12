mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());
shuffle(a.begin(), a.end(), rng);
