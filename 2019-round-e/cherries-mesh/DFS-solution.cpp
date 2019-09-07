#include <iostream>
#include <stack>
#include <utility>
#include <vector>

using namespace std;

class Solution {
public:
	/* time: O(n + m), space: O(n + m) */
	int cherriesMesh(int n, const vector<pair<int, int>>& blacks) {
		vector<vector<int>> adjLists(n);
		for (const auto& black : blacks)
			adjLists[black.first - 1].push_back(black.second - 1);

		vector<int> visited(n);
		auto visit = [&](int src) {
			if (visited[src])
				return false;
			stack<int> s;
			visited[src] = true, s.push(src);
			while (!s.empty()) {
				const int u = s.top();
				s.pop();
				for (int v : adjLists[u]) {
					if (!visited[v])
						visited[v] = true, s.push(v);
				}
			}
			return true;
		};

		int count = 0;
		for (int u = 0; u < n; ++u) {
			if (visit(u))
				++count;
		}
		return (n - 1) + (count - 1);
	}
};

int main() {
	int T;
	cin >> T;
	for (int i = 1; i <= T; ++i) {
		int n, m;
		cin >> n >> m;
		vector<pair<int, int>> blacks(m);
		for (int i = 0; i < m; ++i)
			cin >> blacks[i].first >> blacks[i].second;

		Solution sol;
		int ans = sol.cherriesMesh(n, blacks);
		cout << "Case #" << i << ": " << ans << endl;
	}
	return 0;
}
