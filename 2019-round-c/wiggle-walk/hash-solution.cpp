#include <iostream>
#include <string>
#include <unordered_map>
#include <utility>

using namespace std;

class Solution {
private:
	struct Square {
		int up, down, left, right;
	};
public:
	/* time: O(n), space: O(n), where n = # of operations */
	pair<int, int> wiggleWalk(int y, int x, int row, int col, const string& ops) {
		unordered_map<int, unordered_map<int, Square>> h;
		auto visited = [&](int y, int x) {
			return h.count(y) && h[y].count(x);
		};

		for (char op : ops) {
			h[y][x].up = h[y][x].down = y;
			h[y][x].left = h[y][x].right = x;

			if (visited(y, x - 1) && visited(y, x + 1)) {
				const int left = h[y][x - 1].left, right = h[y][x + 1].right;
				h[y][left].right = right, h[y][right].left = left;
			} else if (visited(y, x - 1)) {
				const int left = h[y][x - 1].left;
				h[y][left].right = x, h[y][x].left = left;
			} else if (visited(y, x + 1)) {
				const int right = h[y][x + 1].right;
				h[y][right].left = x, h[y][x].right = right;
			}

			if (visited(y - 1, x) && visited(y + 1, x)) {
				const int up = h[y - 1][x].up, down = h[y + 1][x].down;
				h[up][x].down = down, h[down][x].up = up;
			} else if (visited(y - 1, x)) {
				const int up = h[y - 1][x].up;
				h[up][x].down = y, h[y][x].up = up;
			} else if (visited(y + 1, x)) {
				const int down = h[y + 1][x].down;
				h[down][x].up = y, h[y][x].down = down;
			}

			switch (op) {
				case 'N':
					if (visited(y - 1, x))
						y = h[y - 1][x].up;
					--y;
					break;
				case 'E':
					if (visited(y, x + 1))
						x = h[y][x + 1].right;
					++x;
					break;
				case 'W':
					if (visited(y, x - 1))
						x = h[y][x - 1].left;
					--x;
					break;
				case 'S':
					if (visited(y + 1, x))
						y = h[y + 1][x].down;
					++y;
					break;
			}
		}
		return {y, x};
	}
};

int main() {
	int T;
	cin >> T;
	for (int i = 1; i <= T; ++i) {
		int n, row, col, y, x;
		string ops;
		cin >> n >> row >> col >> y >> x >> ops;

		Solution sol;
		pair<int, int> ans = sol.wiggleWalk(y, x, row, col, ops);
		cout << "Case #" << i << ": " << ans.first << " " << ans.second << endl;
	}
	return 0;
}
