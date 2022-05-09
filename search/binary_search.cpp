#include <iostream>

using namespace std;

template <typename T>
int binary_search(T *data, int len, const T &target) {
	int left = 0;
	int right = len - 1;
	while(left <= right) {
		int mid = left + (right - left) / 2;
		if(target == data[mid]) {
			return mid;
		}else if(target < data[mid]) {
			right = mid - 1;
		}else {
			left = mid + 1;
		}
	}

	return -1;
}

int main() {
	int a[] = {0, 2, 6, 10, 55, 87, 99, 205, 1000, 4096};
	cout << "array: ";
	for(int i : a) {
		cout << i << " ";
	}
	cout << endl;

	cout << "input search number: " << endl;
	int target;
	cin >> target;
	cout << "binary_search for: " << target << "..." << endl;
	int index = binary_search(a, sizeof(a)/sizeof(a[0]), target);
	cout << "find index: " << index << endl;

	return 0;
}
