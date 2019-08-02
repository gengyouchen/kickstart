#include <algorithm>
#include <cmath>
#include <iostream>
#include <iterator>
#include <vector>

using namespace std;

class Solution {
public:
	/* time: O((n+k)*log(n)), space: O(n), where n = # of items */
	double luckyDip(vector<int>& items, int k) {
		const int n = items.size();
		sort(items.begin(), items.end());

		vector<long long> prefix(n + 1);
		for (int i = 1; i <= n; ++i)
			prefix[i] = prefix[i - 1] + items[i - 1];

		double ans = (double)(prefix[n] - prefix[0]) / n;
		for (int i = 0; i < k; ++i) {
			const auto it = upper_bound(items.begin(), items.end(), floor(ans));
			const int nRedip = distance(items.begin(), it);
			ans = (ans * nRedip + (prefix[n] - prefix[nRedip])) / n;
		}
		return ans;
	}
};

int main() {
	int T;
	cin >> T;
	for (int i = 1; i <= T; ++i) {
		int n, k;
		cin >> n >> k;

		vector<int> items(n);
		for (int i = 0; i < n; ++i)
			cin >> items[i];

		Solution sol;
		double ans = sol.luckyDip(items, k);
		cout << "Case #" << i << ": " << fixed << ans << endl;
	}
	return 0;
}
