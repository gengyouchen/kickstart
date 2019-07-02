#include <algorithm>
#include <deque>
#include <iostream>
#include <iterator>
#include <queue>
#include <stack>
#include <vector>

using namespace std;

/* See LeetCode 239 - Sliding Window Maximum */
class MonoQueue {
private:
	queue<int> all;
	deque<int> maxCandidates; /* non-increasing */
	deque<int> minCandidates; /* non-decreasing */
public:
	void push_back(int val) {
		while (!maxCandidates.empty() && val > maxCandidates.back())
			maxCandidates.pop_back();
		maxCandidates.push_back(val);
		while (!minCandidates.empty() && val < minCandidates.back())
			minCandidates.pop_back();
		minCandidates.push_back(val);
		all.push(val);
	}
	void pop_front() {
		if (all.front() == maxCandidates.front())
			maxCandidates.pop_front();
		if (all.front() == minCandidates.front())
			minCandidates.pop_front();
		all.pop();
	}
	int max() const { return maxCandidates.front(); }
	int min() const { return minCandidates.front(); }
	int size() const { return all.size(); }
};

class Solution {
private:
	/* See LeetCode 84 - Largest Rectangle in Histogram */
	int largestRectangleArea(vector<int>& heights) {
		heights.insert(heights.begin(), -1);
		heights.push_back(0);

		int ans = 0;
		stack<vector<int>::iterator> mono; /* strictly increasing */
		for (auto it = heights.begin(); it != heights.end(); ++it) {
			while (!mono.empty() && *it <= *mono.top()) {
				const auto target = mono.top(), firstLessOrEqualAfterTarget = it;
				mono.pop();
				const auto firstLessBeforeTarget = mono.top();

				const int h = *target;
				const int w = distance(firstLessBeforeTarget, firstLessOrEqualAfterTarget) - 1;
				ans = max(ans, w * h);
			}
			mono.push(it);
		}
		return ans;
	}
public:
	/* time: O(m*n), space: O(m*n), where m = # of rows, n = # of columns */
	int circuitBoard(const vector<vector<int>>& board, int k) {
		const int m = board.size(), n = board[0].size();
		vector<vector<int>> rightmost(m, vector<int>(n));
		for (int y = 0; y < m; ++y) {
			MonoQueue mono;
			int L = 0;
			for (int R = 0; R < n; ++R) {
				mono.push_back(board[y][R]);
				while (mono.max() - mono.min() > k)
					rightmost[y][L] = R - L, mono.pop_front(), ++L;
			}
			while (L < n)
				rightmost[y][L] = n - L, mono.pop_front(), ++L;
		}

		int ans = 0;
		for (int x = 0; x < n; ++x) {
			vector<int> heights(m);
			for (int y = 0; y < m; ++y)
				heights[y] = rightmost[y][x];
			ans = max(ans, largestRectangleArea(heights));
		}
		return ans;
	}
};

int main() {
	int T;
	cin >> T;
	for (int i = 1; i <= T; ++i) {
		int row, col, k;
		cin >> row >> col >> k;
		vector<vector<int>> board(row, vector<int>(col));
		for (int y = 0; y < row; ++y) {
			for (int x = 0; x < col; ++x)
				cin >> board[y][x];
		}

		Solution sol;
		int ans = sol.circuitBoard(board, k);
		cout << "Case #" << i << ": " << ans << endl;
	}
	return 0;
}
