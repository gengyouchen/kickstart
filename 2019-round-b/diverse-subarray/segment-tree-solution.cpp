#include <algorithm>
#include <iostream>
#include <unordered_map>
#include <utility>
#include <vector>

using namespace std;

class SegTree {
private:
	vector<int> S; /* sum */
	vector<int> P; /* max prefix sum */
public:
	SegTree(int n) {
		int nLeaf = 1;
		while (nLeaf < n)
			nLeaf *= 2;
		S.resize(nLeaf * 2, 0);
		P.resize(nLeaf * 2, 0);
	}
	void update(int i, int val) {
		int x = S.size() / 2 + i;
		S[x] = val;
		P[x] = val;
		while (x /= 2) {
			S[x] = S[x * 2] + S[x * 2 + 1];
			P[x] = max(P[x * 2], S[x * 2] + P[x * 2 + 1]);
		}
	}
	int getMaxPrefixSum() const { return P[1]; }
};

class Solution {
public:
	/* time: O(n*log(n)), space: O(n), where n = # of trinkets */
	int diverseSubarray(const vector<int>& A, int s) {
		const int n = A.size();
		unordered_map<int, vector<int>> type2positions;
		for (int i = 0; i < n; ++i)
			type2positions[A[i]].push_back(i);

		SegTree gain(n);
		unordered_map<int, pair<int, int>> type2window;
		for (const auto& it : type2positions) {
			const int type = it.first;
			const auto& positions = it.second;
			type2window[type] = {0, s};
			for (int i = 0; i < positions.size(); ++i) {
				if (i < s)
					gain.update(positions[i], 1);
				if (i == s)
					gain.update(positions[i], -s);
			}
		}

		int ans = 0;
		for (int i = 0; i < n; ++i) {
			ans = max(ans, gain.getMaxPrefixSum());

			const int type = A[i];
			const auto& positions = type2positions[type];
			auto& window = type2window[type];
			if (window.first < positions.size())
				gain.update(positions[window.first], 0);
			if (window.second < positions.size())
				gain.update(positions[window.second], 1);
			if (window.second + 1 < positions.size())
				gain.update(positions[window.second + 1], -s);
			++window.first, ++window.second;
		}
		return ans;
	}
};

int main() {
	int T;
	cin >> T;
	for (int i = 1; i <= T; ++i) {
		int n, s;
		cin >> n >> s;
		vector<int> A(n);
		for (int i = 0; i < n; ++i)
			cin >> A[i];

		Solution sol;
		int ans = sol.diverseSubarray(A, s);
		cout << "Case #" << i << ": " << ans << endl;
	}
	return 0;
}
