#include <algorithm>
#include <iostream>
#include <set>
#include <utility>
#include <vector>

using namespace std;

class Solution {
private:
	bool isOdd(int x) {
		int count = 0;
		while (x)
			x &= (x - 1), ++count;
		return count % 2;
	}
public:
	/*
	 * time: O((n+q)*log(n)), space: O(n) auxiliary (i.e. does not count output itself),
	 * where n = # of integers, q = # of modifications
	 */
	vector<int> xOrWhat(const vector<int>& A, const vector<pair<int, int>>& changes) {
		const int n = A.size(), q = changes.size();
		set<int> odd;
		for (int i = 0; i < n; ++i) {
			if (isOdd(A[i]))
				odd.insert(i);
		}

		vector<int> ans(q);
		for (int i = 0; i < q; ++i) {
			const int idx = changes[i].first, val = changes[i].second;
			const auto it = odd.find(idx);
			if (it != odd.end()) {
				if (!isOdd(val))
					odd.erase(it);
			} else {
				if (isOdd(val))
					odd.insert(idx);
			}

			if (odd.size() % 2) {
				const int L = *odd.begin(), R = *odd.rbegin();
				ans[i] = max(R - 0, (n - 1) - L);
			} else {
				ans[i] = n;
			}
		}
		return ans;
	}
};

int main() {
	int T;
	cin >> T;
	for (int i = 1; i <= T; ++i) {
		int n, q;
		cin >> n >> q;
		vector<int> A(n);
		vector<pair<int, int>> changes(q);
		for (int i = 0; i < n; ++i)
			cin >> A[i];
		for (int i = 0; i < q; ++i)
			cin >> changes[i].first >> changes[i].second;

		Solution sol;
		vector<int> ans = sol.xOrWhat(A, changes);
		cout << "Case #" << i << ":";
		for (int i = 0; i < q; ++i)
			cout << " " << ans[i];
		cout << endl;
	}
	return 0;
}
