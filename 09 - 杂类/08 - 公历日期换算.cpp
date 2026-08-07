/*
用途：公历日期与相对 1970-01-01 的天数互转，支持负年份；所有操作 O(1)。
weekday 返回 0..6，分别表示周日到周六。
*/
struct Date {
    int y, m, d;
};

bool leap(int y) {
    return y % 4 == 0 && (y % 100 != 0 || y % 400 == 0);
}

int days(int y, int m) {
    static constexpr int a[] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    return a[m] + (m == 2 && leap(y));
}

int days(Date a) {
    int y = a.y - (a.m <= 2);
    int e = (y >= 0 ? y : y - 399) / 400;
    int u = y - e * 400;
    int m = a.m + (a.m > 2 ? -3 : 9);
    int d = (153 * m + 2) / 5 + a.d - 1;
    return e * 146097 + u * 365 + u / 4 - u / 100 + d - 719468;
}

Date date(int z) {
    z += 719468;
    int e = (z >= 0 ? z : z - 146096) / 146097;
    int d = z - e * 146097;
    int u = (d - d / 1460 + d / 36524 - d / 146096) / 365;
    int y = u + e * 400;
    d -= 365 * u + u / 4 - u / 100;
    int m = (5 * d + 2) / 153;
    int x = d - (153 * m + 2) / 5 + 1;
    m += m < 10 ? 3 : -9;
    return {y + (m <= 2), m, x};
}

int weekday(Date a) {
    int x = (days(a) + 4) % 7;
    return x < 0 ? x + 7 : x;
}
