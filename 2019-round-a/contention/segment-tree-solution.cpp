#include <algorithm>
#include <climits>
#include <iostream>
#include <queue>
#include <utility>
#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <set>

using namespace std;

class SegTree {
private:
	vector<int> T, lazy; /* sum */
	vector<unordered_set<int>> reqs; /* max prefix sum */
	void propagate(int x, int L, int R) {
		if (lazy[x]) {
			T[x] += lazy[x];
			if (L < R)
				lazy[x * 2] += lazy[x], lazy[x * 2 + 1] += lazy[x];
			lazy[x] = 0;
		}
	}
	void update(int i, int j, int delta, int x, int L, int R) {
		propagate(x, L, R);
		if (j < L || R < i) /* no cover */
			return;
		if (i <= L && R <= j) { /* total cover */
			lazy[x] += delta, propagate(x, L, R);
			return;
		}
		const int mid = L + (R - L) / 2;
		update(i, j, delta, x * 2, L, mid);
		update(i, j, delta, x * 2 + 1, mid + 1, R);
		T[x] = min(T[x * 2], T[x * 2 + 1]);
	}
	int query(int i, int j, int x, int L, int R) {
		propagate(x, L, R);
		if (j < L || R < i) /* no cover */
			return INT_MAX;
		if (i <= L && R <= j) /* total cover */
			return T[x];
		const int mid = L + (R - L) / 2;
		return min(query(i, j, x * 2, L, mid), query(i, j, x * 2 + 1, mid + 1, R));
	}
	void hook(int i, int j, int req, bool deleted, int x, int L, int R) {
		if (j < L || R < i) /* no cover */
			return;
		if (i <= L && R <= j) { /* total cover */
			if (deleted)
				reqs[x].erase(req);
			else
				reqs[x].insert(req);
			return;
		}
		const int mid = L + (R - L) / 2;
		hook(i, j, req, deleted, x * 2, L, mid);
		hook(i, j, req, deleted, x * 2 + 1, mid + 1, R);
	}
	pair<int, int> searchMin(int x, int L, int R) {
		pair<int, int> ret = {-1, -1};
		if (L < R) {
			const int mid = L + (R - L) / 2;
			if (query(L, mid, x * 2, L, mid) < query(mid + 1, R, x * 2 + 1, mid + 1, R))
				ret = searchMin(x * 2, L, mid);
			else
				ret = searchMin(x * 2 + 1, mid + 1, R);
		}
		if (L == R)
			ret.second = L;
		if (ret.first == -1 && !reqs[x].empty()) {
			//cout << "Found min at L=" << L << ", R=" << R << ", where val=" << query(L, R, x, L, R) << endl;
			ret.first = *reqs[x].begin();
		}
		return ret;
	}
public:
	SegTree(int n) {
		int nLeaf = 1;
		while (nLeaf < n)
			nLeaf *= 2;
		T.resize(nLeaf * 2);
		lazy.resize(nLeaf * 2);
		reqs.resize(nLeaf * 2);
		fill(T.begin() + nLeaf + n, T.end(), INT_MAX);
		for (int x = nLeaf - 1; x > 0; --x)
			T[x] = min(T[x * 2], T[x * 2 + 1]);
	}
	void update(int i, int j, int delta) { return update(i, j, delta, 1, 0, T.size() / 2 - 1); }
	int query(int i, int j) { return query(i, j, 1, 0, T.size() / 2 - 1); }
	int get(int i) { return query(i, i); }
	void set(int i, int val) { update(i, i, val - get(i)); }
	void hook(int i, int j, int req, bool deleted) { return hook(i, j, req, deleted, 1, 0, T.size() / 2 - 1); }
	int query() { return T[1]; }
	pair<int, int> searchMin() { return searchMin(1, 0, T.size() / 2 - 1); }
};

class Solution {
private:
	template <class T, class F>
	T lowestTrue(T low, T high, F cond) {
		while (low < high) {
			const T mid = low + (high - low) / 2;
			if (cond(mid))
				high = mid;
			else
				low = mid + 1;
		}
		return low;
	}
public:
	int contention(int n, vector<pair<int, int>>& bookings) {
		vector<pair<int, int>> idles;
		int idleBegin = 1;
		sort(bookings.begin(), bookings.end());
		for (const auto& booking : bookings) {
			if (booking.first > idleBegin)
				idles.emplace_back(idleBegin, booking.first - 1);
			idleBegin = max(idleBegin, booking.second + 1);
		}
		if (idleBegin <= n)
			idles.emplace_back(idleBegin, n);

		unordered_set<int> events;
		for (const auto& booking : bookings)
			events.insert(booking.first), events.insert(booking.second + 1);
		for (const auto& idle : idles)
			events.insert(idle.first), events.insert(idle.second + 1);
		vector<int> sortedEvents(events.begin(), events.end());
		sort(sortedEvents.begin(), sortedEvents.end());
		sortedEvents.pop_back();
#if 0
		cout << "sortedEvents: ";
		for (int i : sortedEvents)
			cout << i << ", ";
		cout << endl;
#endif

		SegTree s(sortedEvents.size());

		for (const auto& idle : idles) {
			auto it1 = upper_bound(sortedEvents.begin(), sortedEvents.end(), idle.first) - 1;
			auto it2 = upper_bound(sortedEvents.begin(), sortedEvents.end(), idle.second) - 1;
			const int L = distance(sortedEvents.begin(), it1);
			const int R = distance(sortedEvents.begin(), it2);
			//cout << "idle L=" << idle.first << ", R=" << idle.second << ". L=" << L << ", R=" << R << endl;
			s.update(L, R, INT_MAX);
		}

		const int q = bookings.size();
		vector<int> gain(q);
		set<pair<int, int>> unbook;
		for (int i = 0; i < q; ++i) {
			auto it1 = upper_bound(sortedEvents.begin(), sortedEvents.end(), bookings[i].first) - 1;
			auto it2 = upper_bound(sortedEvents.begin(), sortedEvents.end(), bookings[i].second) - 1;
			const int L = distance(sortedEvents.begin(), it1);
			const int R = distance(sortedEvents.begin(), it2);
			//cout << "add L=" << L << ", R=" << R << endl;
			s.hook(L, R, i, false);
			s.update(L, R, 1);
			unbook.emplace(gain[i], i);
		}
		sortedEvents.push_back(n + 1);
		int ans = INT_MAX;
		while (!unbook.empty()) {
			while (s.query() == 1) {
				auto ret = s.searchMin();
				int req = ret.first, pos = ret.second;
				int w = sortedEvents[pos + 1] - sortedEvents[pos];

				//cout << "searchMin: req=" << req << ", pos=" << pos << ", w=" << w << endl;
				unbook.erase({gain[req], req});
				gain[req] += w;
				s.set(pos, INT_MAX);

				unbook.emplace(gain[req], req);
			}
			int req = unbook.rbegin()->second;
			unbook.erase({gain[req], req});
			//cout << "extrat: req=" << req << ", gain=" << gain[req] << endl;
			ans = min(ans, gain[req]);

			auto it1 = upper_bound(sortedEvents.begin(), sortedEvents.end(), bookings[req].first) - 1;
			auto it2 = upper_bound(sortedEvents.begin(), sortedEvents.end(), bookings[req].second) - 1;
			const int L = distance(sortedEvents.begin(), it1);
			const int R = distance(sortedEvents.begin(), it2);
			//cout << "add L=" << L << ", R=" << R << endl;
			s.hook(L, R, req, true);
			s.update(L, R, -1);


		}
		return ans;
	}
};

int main() {
	int T;
	cin >> T;
	for (int i = 1; i <= T; ++i) {
		int n, q;
		cin >> n >> q;
		vector<pair<int, int>> bookings(q);
		for (int i = 0; i < q; ++i)
			cin >> bookings[i].first >> bookings[i].second;

		Solution sol;
		int ans = sol.contention(n, bookings);
		cout << "Case #" << i << ": " << ans << endl;
	}
	return 0;
}
