#include <algorithm>
#include <climits>
#include <iostream>
#include <set>
#include <unordered_set>
#include <utility>
#include <vector>

using namespace std;

class SegTree {
private:
	vector<int> T, lazy; /* tracking min */
	vector<unordered_set<int>> reqs; /* can hook requests */
	void propagate(int x, int L, int R) {
		if (lazy[x]) {
			T[x] += lazy[x];
			if (L < R)
				lazy[x * 2] += lazy[x], lazy[x * 2 + 1] += lazy[x];
			lazy[x] = 0;
		}
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
			ret.first = L;
		if (ret.second == -1 && !reqs[x].empty())
			ret.second = *reqs[x].begin();
		return ret;
	}
public:
	SegTree(int n) {
		int nLeaf = 1;
		while (nLeaf < n)
			nLeaf *= 2;
		T.resize(nLeaf * 2), lazy.resize(nLeaf * 2), reqs.resize(nLeaf * 2);
		fill(T.begin() + nLeaf + n, T.end(), INT_MAX);
		for (int x = nLeaf - 1; x > 0; --x)
			T[x] = min(T[x * 2], T[x * 2 + 1]);
	}
	void update(int i, int j, int delta) {
		const int x = 1, L = 0, R = T.size() / 2 - 1;
		return update(i, j, delta, x, L, R);
	}
	int query(int i, int j) {
		const int x = 1, L = 0, R = T.size() / 2 - 1;
		return query(i, j, x, L, R);
	}
	void set(int i, int val) {
		update(i, i, val - query(i, i));
	}
	void hook(int i, int j, int req) {
		const int x = 1, L = 0, R = T.size() / 2 - 1;
		hook(i, j, req, false, x, L, R);
	}
	void unhook(int i, int j, int req) {
		const int x = 1, L = 0, R = T.size() / 2 - 1;
		hook(i, j, req, true, x, L, R);
	}
	pair<int, int> searchMin() {
		const int x = 1, L = 0, R = T.size() / 2 - 1;
		return searchMin(x, L, R);
	}
};

class Solution {
private:
	vector<pair<int, int>> getIdles(int n, vector<pair<int, int>>& bookings) {
		vector<pair<int, int>> idles;
		int idleBegin = 1;
		for (const auto& booking : bookings) {
			if (booking.first > idleBegin)
				idles.emplace_back(idleBegin, booking.first - 1);
			idleBegin = max(idleBegin, booking.second + 1);
		}
		if (idleBegin <= n)
			idles.emplace_back(idleBegin, n);
		return idles;
	}
	vector<int> getEvents(const vector<pair<int, int>>& bookings, const vector<pair<int, int>>& idles) {
		unordered_set<int> events;
		for (const auto& booking : bookings)
			events.insert(booking.first), events.insert(booking.second + 1);
		for (const auto& idle : idles)
			events.insert(idle.first), events.insert(idle.second + 1);
		vector<int> sortedEvents(events.begin(), events.end());
		sort(sortedEvents.begin(), sortedEvents.end());
		return sortedEvents;
	}
	int findEvent(int seat, const vector<int>& events) {
		const auto it = upper_bound(events.begin(), events.end(), seat) - 1;
		return distance(events.begin(), it);
	}
public:
	/* time: O(q*log(q)), space: O(q), where q = # of bookings */
	int contention(int n, vector<pair<int, int>>& bookings) {
		sort(bookings.begin(), bookings.end());
		const auto idles = getIdles(n, bookings);
		const auto events = getEvents(bookings, idles);
		const int q = bookings.size(), k = events.size() - 1;

		SegTree s(k);
		vector<int> gain(q);
		set<pair<int, int>> candidates;
		for (const auto& idle : idles) {
			const int L = findEvent(idle.first, events), R = findEvent(idle.second, events);
			s.update(L, R, INT_MAX);
		}
		for (int req = 0; req < q; ++req) {
			const int L = findEvent(bookings[req].first, events), R = findEvent(bookings[req].second, events);
			s.update(L, R, 1), s.hook(L, R, req), candidates.emplace(gain[req], req);
		}

		int ans = INT_MAX;
		while (!candidates.empty()) {
			while (s.query(0, k - 1) == 1) {
				const auto victim = s.searchMin();
				const int i = victim.first, nSeats = events[i + 1] - events[i], req = victim.second;
				candidates.erase({gain[req], req}), gain[req] += nSeats, candidates.emplace(gain[req], req);
				s.set(i, INT_MAX);
			}
			const int req = candidates.rbegin()->second;
			ans = min(ans, gain[req]);

			const int L = findEvent(bookings[req].first, events), R = findEvent(bookings[req].second, events);
			s.update(L, R, -1), s.unhook(L, R, req), candidates.erase({gain[req], req});
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
