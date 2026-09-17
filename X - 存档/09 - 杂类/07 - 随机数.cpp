namespace Random {
    mt19937_64 rng(chrono::steady_clock::now().time_since_epoch().count());

    template <class T> T randint(T a, T b) {
        static_assert(std::is_integral_v<T> && !std::is_same_v<std::remove_cv_t<T>, bool>);
        assert(a <= b);
        return uniform_int_distribution<T>(a, b)(rng);
    }
} // namespace Random

using namespace Random;
