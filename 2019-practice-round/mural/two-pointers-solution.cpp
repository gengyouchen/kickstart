#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

class Solution {
public:
	/* time: O(n), space: O(1), where n = # of sections */
	int mural(const vector<int>& scores) {
		const int n = scores.size(), k = (n + 1) / 2;
		/*
		 * We can freely choose to paint any sub-array S with its length = ceil(n/2)
		 * because given any S, we always have a policy to guarantee it is achievable.
		 *
		 * Assume S is at [L, R], where 0 <= L <= R <= n - 1
		 * At day 1, we can always select our initial painting section [P, P] such that:
		 * a. distance(0, L) = distance(L, P)
		 * b. distance(P, R) = distance(R, n - 1)
		 *
		 * Therefore, starting from day 2, our policy becomes very simple:
		 * a. If the wall is crumble from the left side, we also extend our painted section on the left.
		 * b. If the wall is crumble from the right side, we also extend our painted section on the right.
		 */
		int curr = 0;
		for (int i = 0; i < k; ++i)
			curr += scores[i];
		int ans = curr, L = 0, R = k - 1;
		while (R < n - 1)
			curr += scores[++R] - scores[L++], ans = max(ans, curr);
		return ans;
	}
};

int main() {
	int T;
	cin >> T;
	for (int i = 1; i <= T; ++i) {
		int n;
		string wall;
		cin >> n >> wall;
		vector<int> scores(n);
		for (int i = 0; i < n; ++i)
			scores[i] = wall[i] - '0';

		Solution sol;
		int ans = sol.mural(scores);
		cout << "Case #" << i << ": " << ans << endl;
	}
	return 0;
}
