#include <algorithm>
#include <iostream>
#include <vector>

using namespace std;

class Solution {
public:
	/* time: O(n*log(n)), space: O(1) auxiliary (i.e. does not count input itself) */
	int training(vector<int>& students, int p) {
		const int n = students.size();
		make_heap(students.begin(), students.end());
		sort_heap(students.begin(), students.end());

		int L = 0, R = p - 1, cost = 0;
		for (int i = L; i < R; ++i)
			cost += students[R] - students[i];

		int ans = cost;
		while (R < n - 1) {
			cost -= students[R] - students[L];
			cost += (students[R + 1] - students[R]) * (p - 1);
			++L, ++R;
			ans = min(ans, cost);
		}
		return ans;
	}
};

int main() {
	int T;
	cin >> T;
	for (int i = 1; i <= T; ++i) {
		int n, p;
		cin >> n >> p;
		vector<int> students(n);
		for (int i = 0; i < n; ++i)
			cin >> students[i];

		Solution sol;
		int ans = sol.training(students, p);
		cout << "Case #" << i << ": " << ans << endl;
	}
	return 0;
}
