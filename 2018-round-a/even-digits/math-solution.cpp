#include <algorithm>
#include <iostream>
#include <iterator>
#include <string>

using namespace std;

class Solution {
public:
	/* time: O(log(n)), space: O(log(n)) */
	long long evenDigits(long long n) {
		auto N = to_string(n);
		reverse(N.begin(), N.end());

		const auto it = find_if(N.rbegin(), N.rend(), [](int d) { return d % 2; });
		if (it == N.rend())
			return 0; /* no odd digit */
		const int oddMSB = distance(it, N.rend()) - 1;

		auto L = N;
		--L[oddMSB], fill_n(L.begin(), oddMSB, '8'), reverse(L.begin(), L.end());

		auto H = N;
		bool carry = true;
		for (int i = oddMSB; carry; ++i) {
			if (i == H.size())
				H.push_back('0');

			if (i == oddMSB)
				++H[i];
			else
				H[i] += 2;
			carry = (H[i] > '9');
			if (carry)
				H[i] = '0';
		}
		fill_n(H.begin(), oddMSB, '0'), reverse(H.begin(), H.end());

		return min(stoll(H) - n, n - stoll(L));
	}
};

int main() {
	int T;
	cin >> T;
	for (int i = 1; i <= T; ++i) {
		long long n;
		cin >> n;

		Solution sol;
		long long ans = sol.evenDigits(n);
		cout << "Case #" << i << ": " << ans << endl;
	}
	return 0;
}
