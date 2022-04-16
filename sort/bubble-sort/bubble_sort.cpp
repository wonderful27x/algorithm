#include <iostream>

using namespace std;

//冒泡排序是一种交换排序，两两比较相邻记录的关键字，反序则交换，一趟排序使得最大或最小的关键字像气泡一样上浮到顶端。

template <typename T>
void bubbleSort(T *t, int length) {
	int i,j;
	for(i=0; i<length-1; i++) {
		for(j=0; j<length-1-i; j++) {
			if(t[j] > t[j+1]) {
				T tmp = t[j];
				t[j] = t[j+1];
				t[j+1] = tmp;
			}
		}
	}
}

int main(int argc, char **argv) {
	int array[] = {27,44,56,10,7};
	cout << "array = {27,44,56,10,7}, bubble_sort..." << endl;
	int len = sizeof(array)/sizeof(int);
	bubbleSort(array,len);
	cout << "array = :" << endl;
	for(int i=0; i<len; i++) {
		cout << array[i] << endl;
	}

	return 0;
}
