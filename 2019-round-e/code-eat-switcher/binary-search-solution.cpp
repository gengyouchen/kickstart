#include <algorithm>
#include <iterator>
#include <iostream>
#include <numeric>
#include <vector>

using namespace std;
using LL = long long;

struct Activity {
	LL coding, eating;
	Activity() : coding(0), eating(0) {}
	Activity(LL coding, LL eating) : coding(coding), eating(eating) {}
};

class Solution {
public:
	/*
	 * time: O((s + d) * log(s)), space: O(s) auxiliary (i.e. does not count output itself),
	 * where s = # of slots, d = # of days
	 */
	vector<bool> codeEatSwitcher(vector<Activity>& slots, const vector<Activity>& days) {
		const int d = days.size(), s = slots.size();

		/* order by their C/P ratios (i.e. coding / eating): high to low */
		auto byCPRatio = [](const auto& a, const auto& b) {
			return a.coding * b.eating > b.coding * a.eating;
		};
		sort(slots.begin(), slots.end(), byCPRatio);

		vector<Activity> prefix(s);
		auto sum = [](const auto& a, const auto& b) {
			return Activity(a.coding + b.coding, a.eating + b.eating);
		};
		partial_sum(slots.begin(), slots.end(), prefix.begin(), sum);

		vector<bool> ans(d);
		for (int i = 0; i < d; ++i) {
			const auto& goal = days[i];

			auto byCoding = [](const auto& a, const auto& b) {
				return a.coding < b.coding;
			};
			const auto it = lower_bound(prefix.begin(), prefix.end(), goal, byCoding);
			if (it == prefix.end())
				continue; /* failed: even if we code all the day, we cannot meet our goal */

			const int k = distance(prefix.begin(), it);
			const int lastCoding = goal.coding - (k > 0 ? prefix[k - 1].coding : 0);
			const double firstEating = slots[k].eating - (double)lastCoding * slots[k].eating / slots[k].coding;

			const double totalEating = firstEating + (prefix.back().eating - prefix[k].eating);
			if (totalEating >= goal.eating)
				ans[i] = true;
		}
		return ans;
	}
};

int main() {
	int T;
	cin >> T;
	for (int i = 1; i <= T; ++i) {
		int d, s;
		cin >> d >> s;
		vector<Activity> slots(s);
		for (auto& slot : slots)
			cin >> slot.coding >> slot.eating;

		vector<Activity> days(d);
		for (auto& day : days)
			cin >> day.coding >> day.eating;

		Solution sol;
		vector<bool> ans = sol.codeEatSwitcher(slots, days);
		cout << "Case #" << i << ": ";
		for (int i = 0; i < d; ++i)
			cout << (ans[i] ? "Y" : "N");
		cout << endl;
	}
	return 0;
}
