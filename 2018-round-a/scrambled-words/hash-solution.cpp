#include <array>
#include <functional>
#include <iostream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

using namespace std;

#define MOD 1000000007
#define BASE 100003
static array<long long, 26> seeds;

class Counter {
private:
	array<long long, 26> count = {};
	long long h = 0;
	long long mod(long long x) const {
		const long long r = x % MOD;
		return (r >= 0) ? r : (r + MOD);
	}
	long long getSeed(char c) {
		if (!seeds[0]) {
			seeds[0] = 1;
			for (int i = 1; i < seeds.size(); ++i)
				seeds[i] = mod(seeds[i - 1] * BASE);
		}
		return seeds[c - 'a'];
	}
public:
	template <class I>
	Counter(I first, I last) {
		for (auto it = first; it != last; ++it)
			insert(*it);
	}
	void insert(char c) { ++count[c - 'a'], h = mod(h + getSeed(c)); }
	void erase(char c) { --count[c - 'a'], h = mod(h - getSeed(c)); }
	long long hash() const { return h; }
	bool operator==(const Counter& rhs) const { return count == rhs.count; }
};

namespace std {
	template <>
	struct hash<Counter> {
		size_t operator()(const Counter& key) const { return key.hash(); }
	};
}

class Solution {
public:
	/*
	 * time: O(n*sqrt(c)), space: O(L),
	 * where n = |str|, L = # of words in dict, c = total # of chars in dict,
	 * and sqrt(c) = # of distinct word lengths in dict
	 */
	int scrambledWords(const string& str, const vector<string>& dict) {
		const int n = str.size();
		unordered_set<int> lens;
		array<array<unordered_map<Counter, int>, 26>, 26> count;
		for (const auto& s : dict) {
			const int len = s.size(), L = s[0] - 'a', R = s[len - 1] - 'a';
			const Counter freqs(s.begin(), s.end());
			++count[L][R][freqs], lens.insert(len);
		}

		int ans = 0;
		for (int len : lens) {
			Counter freqs(str.begin(), str.begin() + len);
			for (int i = 0, j = len - 1; j < n; ++i, ++j) {
				const int L = str[i] - 'a', R = str[j] - 'a';
				const auto it = count[L][R].find(freqs);
				if (it != count[L][R].end())
					ans += it->second, count[L][R].erase(it);
				if (j < n - 1)
					freqs.erase(str[i]), freqs.insert(str[j + 1]);
			}
		}
		return ans;
	}
};

int main() {
	int T;
	cin >> T;
	for (int i = 1; i <= T; ++i) {
		int L;
		cin >> L;
		vector<string> dict(L);
		for (int i = 0; i < L; ++i)
			cin >> dict[i];

		int n;
		char s1, s2;
		long long a, b, c, d;
		cin >> s1 >> s2 >> n >> a >> b >> c >> d;

		string str(n, 0);
		str[0] = s1, str[1] = s2;
		long long x1 = s1, x2 = s2;
		for (int i = 2; i < n; ++i) {
			const long long nx = (a * x2 + b * x1 + c) % d;
			str[i] = 'a' + nx % 26, x1 = x2, x2 = nx;
		}

		Solution sol;
		int ans = sol.scrambledWords(str, dict);
		cout << "Case #" << i << ": " << ans << endl;
	}
	return 0;
}
