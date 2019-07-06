#include <algorithm>
#include <iostream>
#include <vector>
#include <utility>
#include <queue>
#include <climits>

using namespace std;

class Solution {
private:
	template <class T, class F>
	T lowestTrue(T low, T high, F cond) {
		while (low < high) {
			const T mid = low + (high - low) / 2;
			if (cond(mid))
				high = mid;
			else
				low = mid + 1;
		}
		return low;
	}
public:
	/* time: O(m*n*log(m+n)), space: O(m*n) */
	int parcels(const vector<vector<char>>& squares) {
		const int m = squares.size(), n = squares[0].size();
		const vector<pair<int, int>> dirs = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};

		vector<vector<int>> d(m, vector<int>(n, -1));
		queue<pair<int, int>> Q;
		for (int y = 0; y < m; ++y) {
			for (int x = 0; x < n; ++x) {
				if (squares[y][x] == '1')
					Q.emplace(y, x), d[y][x] = 0;
			}
		}

		for (int depth = 1; !Q.empty(); ++depth) {
			for (int i = Q.size(); i > 0; --i) {
				const int y = Q.front().first, x = Q.front().second;
				Q.pop();
				for (const auto& dir : dirs) {
					const int y1 = y + dir.first, x1 = x + dir.second;
					if (y1 >= 0 && y1 < m && x1 >= 0 && x1 < n && d[y1][x1] == -1)
						Q.emplace(y1, x1), d[y1][x1] = depth;
				}
			}
		}

		auto cond = [&](int k) {
			int uMax = INT_MAX, uMin = INT_MIN, vMax = INT_MAX, vMin = INT_MIN;
			for (int y = 0; y < m; ++y) {
				for (int x = 0; x < n; ++x) {
					const int u = x + y, v = x - y;
					if (d[y][x] > k) {
						uMax = min(uMax, u + k), uMin = max(uMin, u - k);
						vMax = min(vMax, v + k), vMin = max(vMin, v - k);
					}
				}
			}
			for (int y = 0; y < m; ++y) {
				for (int x = 0; x < n; ++x) {
					const int u = x + y, v = x - y;
					if (u <= uMax && u >= uMin && v <= vMax && v >= vMin)
						return true;
				}
			}
			return false;
		};
		return lowestTrue(0, m + n, cond);
	}
};

int main() {
	int T;
	cin >> T;
	for (int i = 1; i <= T; ++i) {
		int row, col;
		cin >> row >> col;
		vector<vector<char>> squares(row, vector<char>(col));
		for (int i = 0; i < row; ++i) {
			for (int j = 0; j < col; ++j)
				cin >> squares[i][j];
		}

		Solution sol;
		int ans = sol.parcels(squares);
		cout << "Case #" << i << ": " << ans << endl;
	}
	return 0;
}
