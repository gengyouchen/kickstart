#include <algorithm>
#include <cmath>
#include <iostream>
#include <tuple>
#include <utility>
#include <vector>

using namespace std;

class StreetCheckers {
private:
	vector<int> primes;
	int jumpToStart(int curr, int start, int step) {
		if (curr < start) {
			const int remain = (start - curr) % step;
			curr = start + (remain ? (step - remain) : 0);
		}
		return curr;
	}
public:
	/* time: O(sqrt(M) * log(log(M))), space: O(sqrt(M)), where M = max(R for all test cases) */
	StreetCheckers(int M) {
		const int n = sqrt(M);
		/* Perform the sieve of Eratosthenes in O(n*log(log(n))) time */
		vector<bool> sieve(n + 1, true);
		for (int i = 0; i * i <= n; ++i) {
			if (i < 2) {
				sieve[i] = false;
			} else if (sieve[i]) {
				for (int j = i * i; j <= n; j += i)
					sieve[j] = false;
			}
		}

		for (int i = 0; i <= n; ++i) {
			if (sieve[i])
				primes.push_back(i);
		}
	}

	/* time: O((R - L) * log(log(R))), space: O(R - L) */
	int count(int L, int R) {
		const int R4 = R / 4, L4 = L / 4;
		vector<bool> sieve(R - L + 1, true), sieve4(R4 - L4 + 1, true);
		for (int i : primes) {
			for (int j = jumpToStart(i * i, L, i); j <= R; j += i)
				sieve[j - L] = false;
			for (int j = jumpToStart(i * i, L4, i); j <= R4; j += i)
				sieve4[j - L4] = false;
		}

		/*
		 * Let i = 2^x * 3^y * 5^z * 7^w * ...
		 * Alice will get: (y+1) * (z+1) * (w+1) * ...
		 * Blob will get: x * (y+1) * (z+1) * (w+1) * ...
		 *
		 * Therefore, game i is interesting if-and-only-if:
		 *    (a) x is 0 and (3^y * 5^z * 7^w * ...) is prime or 1
		 * OR (b) x is 1
		 * OR (c) x is 2 and (3^y * 5^z * 7^w * ...) is prime or 1
		 * OR (d) x is 3 and (3^y * 5^z * 7^w * ...) is 1
		 */
		int ans = 0;
		for (int i = L; i <= R; ++i) {
			if (i % 2) {
				if (sieve[i - L])
					++ans;
			} else if (i % 4) {
				++ans;
			} else if (i % 8) {
				if (sieve4[i / 4 - L4])
					++ans;
			} else {
				if (i / 8 == 1)
					++ans;
			}
		}
		return ans;
	}
};

int main() {
	int T;
	cin >> T;

	vector<pair<int, int>> queries(T);
	int M = 0;
	for (int i = 1; i <= T; ++i) {
		int L, R;
		cin >> L >> R;
		queries[i - 1] = {L, R}, M = max(M, R);
	}

	StreetCheckers sol(M);
	for (int i = 1; i <= T; ++i) {
		int L, R, ans;
		tie(L, R) = queries[i - 1], ans = sol.count(L, R);
		cout << "Case #" << i << ": " << ans << endl;
	}
	return 0;
}
