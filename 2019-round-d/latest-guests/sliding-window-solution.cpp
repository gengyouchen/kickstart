#include <iostream>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

using namespace std;

class Solution {
public:
	/* time: O(g + n), space: O(g), where g = # of guests */
	vector<int> latestGuests(const vector<pair<int, char>>& guests, int n, int m) {
		const int g = guests.size();
		auto mod = [&](int x) {
			const int r = x % n;
			return (r >= 0) ? r : (r + n);
		};

		unordered_set<int> groupC, groupAC;
		unordered_map<int, int> guest2groupC, guest2groupAC;
		for (int i = 0; i < g; ++i) {
			if (guests[i].second == 'C') {
				const int group = mod(guests[i].first + m - 1);
				guest2groupC[i] = group, groupC.insert(group);
			} else {
				const int group = mod(guests[i].first - m - 1);
				guest2groupAC[i] = group, groupAC.insert(group);
			}
		}

		int R = -1, L = -1;
		if (!groupC.empty()) {
			for (int i = 0; i < n; ++i) {
				if (groupC.count(i)) {
					R = i;
					break;
				}
			}
		}
		if (!groupAC.empty()) {
			for (int i = n - 1; i >= 0; --i) {
				if (groupAC.count(i)) {
					L = i;
					break;
				}
			}
		}

		unordered_map<int, int> group2consulateCountC, group2consulateCountAC;
		for (int i = 0; i < n; ++i) {
			int C = -1, AC = -1;
			if (R != -1) {
				C = R;
				if (mod(C - i) > m)
					C = -1;
				if (R == i) {
					R = mod(R + 1);
					while (!groupC.count(R))
						R = mod(R + 1);
				}
			}
			if (L != -1) {
				if (groupAC.count(i))
					L = i;
				AC = L;
				if (mod(i - AC) > m)
					AC = -1;
			}

			if (C != -1 && AC != -1) {
				if (mod(C - i) > mod(i - AC))
					C = -1;
				else if (mod(C - i) < mod(i - AC))
					AC = -1;
			}

			if (C != -1)
				++group2consulateCountC[C];
			if (AC != -1)
				++group2consulateCountAC[AC];
		}

		vector<int> ans(g);
		for (int i = 0; i < g; ++i) {
			if (guests[i].second == 'C')
				ans[i] = group2consulateCountC[guest2groupC[i]];
			else
				ans[i] = group2consulateCountAC[guest2groupAC[i]];
		}
		return ans;
	}
};

int main() {
	int T;
	cin >> T;
	for (int i = 1; i <= T; ++i) {
		int n, g, m;
		cin >> n >> g >> m;
		vector<pair<int, char>> guests(g);
		for (int i = 0; i < g; ++i)
			cin >> guests[i].first >> guests[i].second;

		Solution sol;
		vector<int> ans = sol.latestGuests(guests, n, m);
		cout << "Case #" << i << ":";
		for (int i = 0; i < g; ++i)
			cout << " " << ans[i];
		cout << endl;
	}
	return 0;
}
