#include <algorithm>
#include <iostream>
#include <iterator>
#include <vector>

using namespace std;

class Solution {
private:
	vector<int> num2digits(long long num) {
		vector<int> digits;
		while (num)
			digits.push_back(num % 10), num /= 10;
		return digits;
	}
	long long digits2num(const vector<int>& digits) {
		long long num = 0;
		for (int i = digits.size() - 1; i >= 0; --i)
			num *= 10, num += digits[i];
		return num;
	}
public:
	/* time: O(log(n)), space: O(log(n)) */
	long long evenDigits(long long n) {
		const auto N = num2digits(n);

		const auto it = find_if(N.rbegin(), N.rend(), [](int d) { return d % 2; });
		if (it == N.rend())
			return 0; /* no odd digit */
		const int oddMSB = distance(it, N.rend()) - 1;

		auto L = N;
		--L[oddMSB], fill_n(L.begin(), oddMSB, 8);

		auto H = N;
		bool carry = true;
		for (int i = oddMSB; carry; ++i) {
			if (i == H.size())
				H.push_back(0);

			if (i == oddMSB)
				++H[i];
			else
				H[i] += 2;
			carry = (H[i] == 10);
			if (carry)
				H[i] = 0;
		}
		fill_n(H.begin(), oddMSB, 0);

		return min(digits2num(H) - n, n - digits2num(L));
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
