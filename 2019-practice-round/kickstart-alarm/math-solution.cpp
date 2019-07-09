#include <iostream>
#include <vector>

using namespace std;

#define MOD 1000000007

class Solution {
private:
	long long mod(long long x) {
		const long long r = x % MOD;
		return (r >= 0) ? r : (r + MOD);
	}
	long long pow(long long base, long long exponent) {
		long long ret = 1;
		for (long long bit = 1; bit; bit <<= 1) {
			if (exponent & bit)
				ret = mod(ret * base);
			base = mod(base * base);
		}
		return ret;
	}
	long long inv(long long x) {
		/*
		 * Because MOD is a prime number, for any 1 <= x < MOD,
		 * we can use the Euler theorm to find x's modular multiplicative inverse
		 */
		return pow(x, MOD - 2);
	}
	long long geometricSeries(long long a, long long r, long long n) {
		if (r == 1)
			return mod(a * n);
		return mod(mod(a * mod(pow(r, n) - 1)) * inv(r - 1));
	}
public:
	/* time: O(n*log(k)), space: O(1), where n = # of parameters */
	int kickstartAlarm(const vector<int>& A, int k) {
		/*
		 * First, given a fixed i, we try to find power(1,n):
		 *
		 * Define:
		 *     delta(n) = power(1,n) - power(1,n-1)
		 *
		 * It's obvious that:
		 *     delta(n) = power(n,n) + power(n-1,n) + power(n-2,n) + ... + power(1,n)
		 *
		 * Because:
		 *     power(n,n) = A[n] * 1^i
		 *     power(n-1,n) = power(n-1,n-1) + A[n] * 2^i
		 *     power(n-2,n) = power(n-2,n-1) + A[n] * 3^i
		 *     ...
		 *     power(1,n) = power(1,n-1) + A[n] * n^i
		 *
		 * We get the recurrence relation:
		 *     delta(n) = delta(n-1) + A[n] * (1^i + 2^i + 3^i + ... + n^i), where delta(0) = 0
		 *
		 * As the result:
		 *     power(1,n) = delta(1) + delta(2) + delta(3) + ... + delta(n)
		 *     delta(n) = delta(n-1) + A[n] * f(n), where delta(0) = 0
		 *     f(n) = f(n-1) + n^i, where f(0) = 0
		 *
		 * Now, consider i is not fixed.
		 *
		 * Define:
		 *     Spower(1,n) = sum(power(1,n) for i=1~k)
		 *     Sdelta(n) = sum(delta(n) for i=1~k)
		 *     Sf(n) = sum(f(n) for i=1~k)
		 *
		 * Our goal is to find Spower(1,n):
		 *     Spower(1,n) = Sdelta(1) + Sdelta(2) + Sdelta(3) + ... + Sdelta(n)
		 *     Sdelta(n) = Sdelta(n-1) + A[n] * Sf(n), where Sdelta(0) = 0
		 *     Sf(n) = Sf(n-1) + sum(n^i for i=1~k), where Sf(0) = 0
		 *
		 * Notice that sum(n^i for i=1~k) is a geometric series:
		 *     if n > 1, sum(n^i for i=1~k) = n * (n^k - 1) / (n-1)
		 *     if n == 1, sum(n^i for i=1~k) = k
		 *
		 * As the result:
		 *     Spower(1,n) = Sdelta(1) + Sdelta(2) + Sdelta(3) + ... + Sdelta(n)
		 *     Sdelta(n) = Sdelta(n-1) + A[n] * Sf(n), where Sdelta(0) = 0
		 *     Sf(1) = k, Sf(n) = Sf(n-1) + n * (n^k - 1) / (n-1)
		 */
		const int n = A.size();
		long long Sf = 0, Sdelta = 0, Spower = 0;
		for (int len = 1; len <= n; ++len) {
			Sf = mod(Sf + geometricSeries(len, len, k));
			Sdelta = mod(Sdelta + mod(A[len - 1] * Sf));
			Spower = mod(Spower + Sdelta);
		}
		return Spower;
	}
};

int main() {
	int T;
	cin >> T;
	for (int i = 1; i <= T; ++i) {
		int n, k, x1, y1, c, d, e1, e2, f;
		cin >> n >> k >> x1 >> y1 >> c >> d >> e1 >> e2 >> f;

		vector<int> A(n);
		long long x = x1, y = y1;
		for (int i = 0; i < n; ++i) {
			A[i] = (x + y) % f;
			const long long nx = (c * x + d * y + e1) % f;
			const long long ny = (d * x + c * y + e2) % f;
			x = nx, y = ny;
		}

		Solution sol;
		int ans = sol.kickstartAlarm(A, k);
		cout << "Case #" << i << ": " << ans << endl;
	}
	return 0;
}
