#include <algorithm>
#include <iostream>
#include <unordered_map>
#include <unordered_set>
#include <set>
#include <utility>
#include <vector>
#include <climits>
#include <queue>

using namespace std;

class Solution {
public:
	/* time: O(n*log(n)), space: O(n), where n = # of spots */
	long long foodStalls(vector<pair<int, int>>& spots, int k) {
		const int n = spots.size(), k1 = k / 2, k2 = k - k1;
		sort(spots.begin(), spots.end());

		vector<long long> L(n);
		if (k1) {
			long long sum = 0;
			priority_queue<int> pq;
			for (int i = 0; i < n; ++i) {
				if (pq.size() == k1)
					L[i] = sum + (long long)spots[i].first * k1;

				const int cost = spots[i].second - spots[i].first;
				sum += cost, pq.push(cost);
				if (pq.size() > k1)
					sum -= pq.top(), pq.pop();
			}
		}

		vector<long long> R(n);
		if (k2) {
			long long sum = 0;
			priority_queue<int> pq;
			for (int i = n - 1; i >= 0; --i) {
				if (pq.size() == k2)
					R[i] = sum - (long long)spots[i].first * k2;

				const int cost = spots[i].second + spots[i].first;
				sum += cost, pq.push(cost);
				if (pq.size() > k2)
					sum -= pq.top(), pq.pop();
			}
		}

		long long ans = LLONG_MAX;
		for (int i = k1; i < n - k2; ++i)
			ans = min(ans, L[i] + R[i] + spots[i].second);
		return ans;
	}
};

int main() {
	int T;
	cin >> T;
	for (int i = 1; i <= T; ++i) {
		int k, n;
		cin >> k >> n;
		vector<pair<int, int>> spots(n);
		for (int i = 0; i < n; ++i)
			cin >> spots[i].first;
		for (int i = 0; i < n; ++i)
			cin >> spots[i].second;

		Solution sol;
		long long ans = sol.foodStalls(spots, k);
		cout << "Case #" << i << ": " << ans << endl;
	}
	return 0;
}
