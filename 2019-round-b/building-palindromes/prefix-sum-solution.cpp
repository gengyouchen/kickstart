#include <iostream>
#include <string>
#include <vector>
#include <utility>

using namespace std;

class Solution {
private:
	typedef vector<int> V1D;
	typedef vector<V1D> V2D;
public:
	/* time: O((n+q)*|charset|), space: O(n*|charset|), where n = # of blocks, q = # of questions */
	int buildingPalindromes(const string& blocks, const vector<pair<int, int>>& questions) {
		const int n = blocks.size();
		V2D letter2prefixes(26, V1D(n + 1));
		for (int i = 0; i < 26; ++i) {
			const char c = 'A' + i;
			auto& prefixes = letter2prefixes[i];
			for (int j = 0; j < n; ++j)
				prefixes[j + 1] = prefixes[j] + (blocks[j] == c ? 1 : 0);
		}
		int ans = 0;
		for (const auto& question : questions) {
			const int L = question.first, R = question.second, len = R - L + 1;
			int nOdd = 0;
			for (int i = 0; i < 26; ++i) {
				auto& prefixes = letter2prefixes[i];
				const int count = prefixes[R] - prefixes[L - 1];
				if (count % 2)
					++nOdd;
			}
			if ((len % 2 == 1 && nOdd == 1) || (len % 2 == 0 && nOdd == 0))
				++ans;
		}
		return ans;
	}
};

int main() {
	int T;
	cin >> T;
	for (int i = 1; i <= T; ++i) {
		int n, q;
		string blocks;
		cin >> n >> q >> blocks;
		vector<pair<int, int>> questions(q);
		for (int i = 0; i < q; ++i)
			cin >> questions[i].first >> questions[i].second;

		Solution sol;
		int ans = sol.buildingPalindromes(blocks, questions);
		cout << "Case #" << i << ": " << ans << endl;
	}
	return 0;
}
