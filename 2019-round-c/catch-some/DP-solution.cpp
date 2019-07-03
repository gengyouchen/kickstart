#include <algorithm>
#include <climits>
#include <iostream>
#include <utility>
#include <vector>

using namespace std;

class Solution {
private:
	typedef vector<int> V1D;
	typedef vector<V1D> V2D;
public:
	/* time: O(n^2), space: O(n), where n = # of dogs */
	int catchSome(vector<pair<int, int>>& dogs, int k) {
		const int n = dogs.size();
		sort(dogs.begin(), dogs.end());
		V2D color2positions;
		for (int i = 0; i < n; ++i) {
			if (i == 0 || dogs[i - 1].first != dogs[i].first)
				color2positions.emplace_back();
			color2positions.back().push_back(dogs[i].second);
		}
		const int c = color2positions.size();

		V2D _done(2, V1D(k + 1, INT_MAX)), _home(2, V1D(k + 1, INT_MAX));
#define DONE(i, j) _done[(i) % 2][j]
#define HOME(i, j) _home[(i) % 2][j]
		DONE(0, 0) = HOME(0, 0) = 0;
		for (int i = 1; i <= c; ++i) {
			const auto& p = color2positions[i - 1];
			DONE(i, 0) = HOME(i, 0) = 0;
			for (int j = 1; j <= k; ++j) {
				DONE(i, j) = DONE(i - 1, j), HOME(i, j) = HOME(i - 1, j);
				for (int len = 1; len <= p.size() && len <= j; ++len) {
					if (HOME(i - 1, j - len) != INT_MAX) {
						DONE(i, j) = min(DONE(i, j), min(HOME(i - 1, j - len) + p[len - 1], DONE(i - 1, j - len) + p[len - 1] * 2));
						HOME(i, j) = min(HOME(i, j), HOME(i - 1, j - len) + p[len - 1] * 2);
					}
				}
			}
		}
		return DONE(c, k);
#undef HOME
#undef DONE
	}
};

int main() {
	int T;
	cin >> T;
	for (int i = 1; i <= T; ++i) {
		int n, k;
		cin >> n >> k;
		vector<pair<int, int>> dogs(n);
		for (int i = 0; i < n; ++i)
			cin >> dogs[i].second;
		for (int i = 0; i < n; ++i)
			cin >> dogs[i].first;
		Solution sol;
		int ans = sol.catchSome(dogs, k);
		cout << "Case #" << i << ": " << ans << endl;
	}
	return 0;
}
