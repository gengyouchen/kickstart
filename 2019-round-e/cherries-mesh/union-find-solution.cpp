#include <iostream>
#include <numeric>
#include <utility>
#include <vector>

using namespace std;

class DisjointSet {
private:
	vector<int> p, rank;
	int nSet;
public:
	DisjointSet(int n) : p(n), rank(n), nSet(n) {
		iota(p.begin(), p.end(), 0);
	}
	int FindSet(int x) {
		if (p[x] != x)
			p[x] = FindSet(p[x]);
		return p[x];
	}
	void Union(int x, int y) {
		x = FindSet(x), y = FindSet(y);
		if (x != y) {
			--nSet;
			if (rank[x] > rank[y]) {
				p[y] = x;
			} else {
				p[x] = y;
				if (rank[x] == rank[y])
					++rank[y];
			}
		}
	}
	int CountSet() const {
		return nSet;
	};
};

class Solution {
public:
	/*
	 * time: O(n + m * alpha(n)) ~= O(n + m), space: O(n),
	 * where n = # of cherries, m = # of black sweet strands,
	 * and alpha(x) is the inverse Ackermann function.
	 *
	 * Because alpha(x) < 5 for any value x that can be written in this physical universe,
	 * all the disjoint-set operations take place in essentially constant time.
	 */
	int cherriesMesh(int n, const vector<pair<int, int>>& blacks) {
		DisjointSet s(n);
		for (const auto& black : blacks)
			s.Union(black.first - 1, black.second - 1);
		return (n - 1) + (s.CountSet() - 1);
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
