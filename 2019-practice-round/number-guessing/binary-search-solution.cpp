#include <iostream>
#include <string>

using namespace std;

/* Forward declaration of guess API */
int guess(int num);

class Solution {
public:
	/* time: O(log(w)), space: O(1), where w = |high - low| */
	void numberGuessing(int low, int high) {
		while (low < high) {
			const int mid = low + (high - low) / 2, ret = guess(mid);
			if (ret == 1)
				high = mid - 1;
			else if (ret == -1)
				low = mid + 1;
			else
				return;
		}
		guess(low);
	}
};

int guess(int num) {
	string feedback;
	cout << num << endl;
	cin >> feedback;
	if (feedback == "TOO_SMALL")
		return -1;
	if (feedback == "TOO_BIG")
		return 1;
	return 0;
}

int main() {
	int T;
	cin >> T;
	for (int i = 1; i <= T; ++i) {
		int a, b, n;
		cin >> a >> b >> n;

		Solution sol;
		sol.numberGuessing(a + 1, b);
	}
	return 0;
}
