#include <algorithm>
#include <iostream>
#include <vector>

using namespace std;

struct Stone {
	int timeToEat, energy, lossPerSec;
};

class Solution {
public:
	/* time: O(n*t), space: O(t), where t = sum(timeToEat for all stones), n = # of stones */
	int energyStones(vector<Stone>& stones) {
		const int n = stones.size();
		int t = 0;
		for (const auto& stone : stones)
			t += stone.timeToEat;

		/*
		 * For all the stones you've decided to eat, there exists an optimal eating sequence.
		 * Assume an optimal eating sequence is s(0), s(1), s(2), ..., s(k).
		 * For any i < k, it must satisfy that:
		 * s(i+1).lossPerSec * s(i).timeToEat <= s(i).lossPerSec * s(i+1).timeToEat
		 * Otherwise, we can swap s(i) and s(i+1) to get more energy.
		 *
		 * According to the transitive relation,
		 * for any i, j such that i < j <= k, it must satisfy that:
		 * s(j).lossPerSec * s(i).timeToEat <= s(i).lossPerSec * s(j).timeToEat
		 */
		auto comp = [](const auto& a, const auto& b) {
			return b.lossPerSec * a.timeToEat < a.lossPerSec * b.timeToEat;
		};
		sort(stones.begin(), stones.end(), comp);

		/* After sorted, the problem is reduced to the 0/1 Knapsack problem */
		vector<vector<int>> _dp(2, vector<int>(t + 1));
#define DP(i, j) _dp[(i) % 2][j]
		int ans = 0;
		for (int i = 1; i <= n; ++i) {
			for (int j = 1; j <= t; ++j) {
				const int beginTime = j - stones[i - 1].timeToEat;
				const int remainingEnergy = max(0, stones[i - 1].energy - stones[i - 1].lossPerSec * beginTime);
				if (beginTime >= 0)
					DP(i, j) = max(DP(i - 1, j), DP(i - 1, beginTime) + remainingEnergy);
				else
					DP(i, j) = DP(i - 1, j);
				ans = max(ans, DP(i, j));
			}
		}
		return ans;
#undef DP
	}
};

int main() {
	int T;
	cin >> T;
	for (int i = 1; i <= T; ++i) {
		int n;
		cin >> n;
		vector<Stone> stones(n);
		for (int i = 0; i < n; ++i)
			cin >> stones[i].timeToEat >> stones[i].energy >> stones[i].lossPerSec;

		Solution sol;
		int ans = sol.energyStones(stones);
		cout << "Case #" << i << ": " << ans << endl;
	}
	return 0;
}
