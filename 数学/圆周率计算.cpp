// Machin n\leq 2·10^3
FixedPoint Machin(int n) {
	const int extras = 40;
	const int prec = n + extras;
	FixedPoint::setDefaultPrecision(prec);

	auto arctan = [&](int invx)->FixedPoint {
		FixedPoint term = FixedPoint(1) / FixedPoint((long long)invx);
		FixedPoint sum = term;
		int sign = -1;
		FixedPoint invx2 = FixedPoint(1) / FixedPoint((long long)invx * (long long)invx);
		FixedPoint eps(BigInt(1), prec + 15, 1);
		for (ll k = 3;; k += 2) {
			term = term * invx2;
			FixedPoint cur = term / FixedPoint((long long)k);
			if (cur.abs() < eps) break;
			if (sign > 0) sum = sum + cur;
			else sum = sum - cur;
			sign = -sign;
		}
		sum.roundKeep(FixedPoint::DEFAULT_PRECISION);
		sum.normalize();
		return sum;
	};

	FixedPoint a1 = arctan(5);
	FixedPoint a2 = arctan(239);
	FixedPoint pi = a1 * FixedPoint(16) - a2 * FixedPoint(4);
	pi.roundKeep(n);
	pi.normalize();
	return pi;
}

// Chudnovsky n\leq 2·10^5
FixedPoint Chudnovsky(int n) {
	const int extras = 20;
	const int prec = n + extras;
	FixedPoint::setDefaultPrecision(prec);

	BigInt C = BigInt(640320);
	BigInt C3_OVER_24 = BigInt(640320 * 640320 * 640320 / 24);

	auto factorial = [](int x) -> BigInt {
		BigInt res(1);
		for (int i = 2; i <= x; ++i) res *= BigInt(i);
		return res;
	};

	FixedPoint sum(0);

	string epsstr = "0." + string(prec + 5, '0') + "1";
	FixedPoint eps(epsstr);

	for (int k = 0; ; ++k) {
		BigInt numerator = factorial(6 * k) * (BigInt(545140134) * k + BigInt(13591409));
		BigInt denominator = factorial(3 * k) * pow(BigInt(factorial(k)), 3) * BigInt(pow(640320, 3 * k));
		FixedPoint term = FixedPoint(numerator) / FixedPoint(denominator);
		if (k % 2 == 1) term = FixedPoint(0) - term;
		sum += term;
		if (term.abs() < eps) break;
	}

	FixedPoint pi = FixedPoint(1) / (sum * FixedPoint(12));
	pi.roundKeep(n);
	pi.normalize();
	return pi;
}