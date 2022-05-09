#include<iostream>

using namespace std;

template <typename T>
struct Node {
	T data;
	struct Node *next;
};
//typedef struct Node<T> Node<T>;

template <typename T>
class WList {
	public:
		WList() {
			head = new Node<T>;
			head->next = nullptr;
		}

		WList(const WList &) = delete;
		WList &operator=(const WList &) = delete;

		//析构，清空链表并释放头结点
		~WList() {
			clear();
			delete head;
			head = nullptr;
		}

		//清空链表，释放除头结点外的所有其他结点，size归零
		void clear() {
			Node<T> *tmp = nullptr;
			while(head->next) {
				tmp = head->next->next;
				delete head->next;
				head->next = tmp;
			}
			size = 0;
		}

		bool empty() {
			return size == 0;
		}

		int get_size() {
			return size;
		}

		//获取指定位置元素
		bool get(int index, T *data) {
			Node<T> *node = find_index(index);
			if(node) {
				*data = node->data;
				return true;
			}
			return false;
		}		

		//查找元素data是否在链表中，返回第一个找到的位置，-1表示失败
		int find(const T &data) {
			int index = -1;
			find_value(data, &index);
			return index;
		}

		//插入元素data到指定位置
		bool insert(int index, const T &data) {
			if(index < 0 || index > size) return false; //要插入的位置不在范围内，返回false，这样可以排除干扰
			Node<T> *pre_node = find_index(index - 1);    //先查找要插入的位置之前一个位置的结点
			if(!pre_node) return false;             //没找到直接返回false

			Node<T> *newNode = new Node<T>;        //创建新结点
			newNode->data = data;               //赋值data给新结点
		        newNode->next = pre_node->next;     //新结点next指向前一个结点的下个结点
			pre_node->next = newNode;           //前一个结点next指向新结点

			size++;                              //size++
			
			return true;
		}

		//删除index位置的元素，并通过参数返回元素值，如果不需要则使用默认实参nullptr
		bool remove(int index, T *data = nullptr) {
			if(index < 0 || index >= size) return false; //要删除的元素不在范围内，返回false，这样可以排除干扰
			Node<T> *pre_node = find_index(index - 1);  //先查找删除位置之前一个位置的结点
			if(!pre_node) return false;           //没找到直接返回false

			Node<T> *tmp = pre_node->next;         //暂存这个要删除的元素
			//先判断这个要删除的元素是否存在，因为找的是它的前一个元素,这个元素是否存在就不确定了
			if(tmp) {
				pre_node->next = pre_node->next->next; //前一个元素指向后一个元素的后一个元素
				if(data) *data = tmp->data;            //将要删除的元素赋值给参数
				delete tmp;                            //释放要删除的元素
				size--;                                //size--
				
				return true;
			}
							      
			
			return false;
		}

	private:
		Node<T> *head; //头指针，数据域无效
		int size = 0;      //链表数据元素数量

		//查找指定位置的结点，返回结点指针
		//1. index<0,返回头指针
		//2. 0<=index<size,返回正常结点指针
		//3. index>=size,返回nullptr
		Node<T> *find_index(int index) {
			if(index < 0)return head;
			if(index >= size) return nullptr;
			Node<T> *tmp = head;
			for(int i=0; i<=index; ++i) {
				tmp = tmp->next;
			}

			return tmp;
		}

		//查找值为data的结点
		//1. 找到第一个就返回
		//2. 返回值为找到的结点指针，没找到返回nullptr
		//3. 通过location参数返回结点位置，没找到为-1,用户不需要这个位置信息将使用默认实参nullptr
		Node<T> *find_value(const T &data, int *location = nullptr) {
			if(location)*location = -1;   //先将位置设为-1,表示没有找到
			if(size == 0)return nullptr;  //链表没有元素直接返回nullptr，表示没有找到
			int index = 0;
			//从头开始遍历链表
			Node<T> *tmp = head;
			while(tmp->next){
				//因为有头结点，所以先next
				tmp = tmp->next;
				//找到了，记录位置并返回结点指针
				if(tmp->data == data) {
					if(location)*location = index; 
					return tmp;
				}
				//没找到，继续下一个结点
				index++;
			}

			//遍历完也没找到
			return nullptr;
		}
};

int main(int argc, char **argv) {
	cout << "create list: 0,1,2,3,4" << endl;
	WList<int> list;
	list.insert(0,0);
	list.insert(1,1);
	list.insert(2,2);
	list.insert(3,3);
	list.insert(4,4);
	
	int data = -1;
	if(list.get(3,&data)){
		cout << "get index 3 is: " << data << endl;
	}

	string text = ">>> ";
	cout << text << "find 4 index is: " << list.find(4) << endl;
	cout << text << "list size is: " << list.get_size() << endl;
	cout << text << "list empty? : " << list.empty() << endl;

	cout << "delete index 2..." << endl;
	list.remove(2);
	cout << text << "list size is: " << list.get_size() << endl;

	data = -1; list.get(0,&data);
	cout << "get index 0 is: " << data << endl;
	data = -1; list.get(1,&data);
	cout << "get index 1 is: " << data << endl;
	data = -1; list.get(2,&data);
	cout << "get index 2 is: " << data << endl;
	data = -1; list.get(3,&data);
	cout << "get index 3 is: " << data << endl;
	data = -1; list.get(4,&data);
	cout << "get index 4 is: " << data << endl;

	list.clear();
	cout << "clear list..." << endl;
	cout << text << "list size is: " << list.get_size() << endl;
	cout << text << "list empty? : " << list.empty() << endl;

	return 0;
}
