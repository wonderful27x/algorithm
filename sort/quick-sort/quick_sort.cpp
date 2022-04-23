#include <iostream>

using namespace std;

//快速排序，通过一趟排序将数据分割为独立的两部分，一种一部分的关键字均小于
//另一部分的关键字，再递归的对这两部分进行快速排序，下面是排序一般步骤:  
//1. 选择一个合适的枢轴
//	如果枢轴的值在序列中间位置，排序具有较好的性能
//	因此通常可以用三数取中(左端、中间、右端)的方法来选定枢轴
//2. 将选定的枢轴与左端数据交换，也就是另左端第一个数为枢轴
//3. 从右往左依次与枢轴比较,如果大于枢轴一直往左走;如果小于枢轴则与枢轴交换，
//并退出循环，注意交换后枢轴跑到了右侧
//4. 从左往右依次与枢轴比较，如果小于枢轴一直往右走；如果大与枢轴与枢轴交换，
//并退出循环，注意交换后枢轴又跑到了左侧
//5. 重复3、4直到左右指针相遇，一轮循环结束，此时左右指针都指向了枢轴，
//指针的位置就是就是这一趟循环后数据的分割点，枢轴左侧的都它小，右侧的都比它大
//6. 将分割后的两个序列继续执行上述相同的操作，递归结束条件是右指针>=左侧指针

template <typename T>
void wswap(T *data, int index1, int index2) {
	T tmp = data[index1];
	data[index1] = data[index2];
	data[index2] = tmp;
}

//template <typename T>
//int partition(T *t, int low, int high) {
//	//三数取中，选择枢轴,并另左端为枢轴
//	int m = low + (high - low)/2;
//	T tmp;
//	//保证左端较小
//	if(t[low] > t[high]) {
//		tmp = t[low];
//		t[low] = t[high];
//		t[high] = tmp;
//	}
//	//保证中间较小
//	if(t[m] > t[high]) {
//		tmp = t[m];
//		t[m] = t[high];
//		t[high] = tmp;
//	}
//	//两个较小的比较,t[low]取大的,这时t[low]为中间值
//	if(t[m] > t[low]) {
//		tmp = t[m];
//		t[m] = t[low];
//		t[low] = tmp;
//	}
//	T pivotKey = t[low];
//
//	while(low<high) {
//		//从右往左比较
//		while(low < high && t[high] >= pivotKey) high--;
//		//交换枢轴
//		tmp = t[low];
//		t[low] = t[high];
//		t[high] = tmp;
//		//从左往右比较
//		while(low < high && t[low] <= pivotKey) low++;
//		//交换枢轴
//		tmp = t[low];
//		t[low] = t[high];
//		t[high] = tmp;
//	}
//	return low;
//}

template <typename T>
int partition(T *t, int low, int high) {

	//三数取中，选择枢轴,并另左端为枢轴
	int m = low + (high - low)/2;
	//保证左端较小
	if(t[low] > t[high]) wswap(t, low, high);
	//保证中间较小
	if(t[m] > t[high]) wswap(t, m, high);
	//两个较小的比较,t[low]取大的,这时t[low]为中间值
	if(t[m] > t[low]) wswap(t, m, low);

	T keyPivot = t[low];
	
	while(low<high) {
		//从右往左比较
		while(low < high && t[high] >= keyPivot) high--;
		//交换枢轴,注意交换后枢轴跑到右侧了
		wswap(t, low, high);
		//从左往右比较
		while(low < high && t[low] <= keyPivot) low++;
		//交换枢轴,注意交换后枢轴又跑到左侧了
		wswap(t, low, high);
	}

	return low;
}
	

template <typename T>
void quickSort(T *t, int low, int high) {
	int pivot;
	if(low<high) {
		pivot = partition<T>(t, low, high); //一趟循环分割为两部分，返回分割点位置
		quickSort(t, low, pivot-1); //递归左半部分
		quickSort(t, pivot+1, high); //递归右半部分
	}
}

int main(int argc, char **argv) {
	//int array[] = {100,99,88,77,66};
	//cout << "array = {100,99,88,77,66}, quickSort..." << endl;
	int array[] = {27,100,22,5,88,77,66};
	cout << "array = {27,100,22,5,88,77,66}, quickSort..." << endl;
	int len = sizeof(array)/sizeof(int);
	quickSort(array, 0, len-1);
	cout << "array = : " << endl;
	for(int i=0; i<len; i++) {
		cout << array[i] << endl;
	}

	return 0;
}
