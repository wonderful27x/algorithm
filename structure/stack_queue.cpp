#include <iostream>
#include <stack>

template <typename T>
class Queue {
	public:
		int count() const {
			return size;
		}

		void push(const T &data) {
			if(!out.empty()) move(in, out);
			in.push(data);
			size++;
		}

		bool pop(T *data) {
			if(!in.empty()) move(out, in);
			if(out.empty()) return false;
			*data = out.top();
			out.pop();
			size--;
			return true;
		}	
	private:
		int size = 0;
		std::stack<T> in;
		std::stack<T> out;

		void move (std::stack<T> &des, std::stack<T> &src) {
			while(!src.empty()) {
				des.push(src.top());
				src.pop();
			}
		}
};

int main() {
	Queue<int> q;
	std::cout << "push 1, 2" << std::endl;
	q.push(1);
	q.push(2);
	std::cout << "size: " << q.count() << std::endl;
	std::cout << "push 3, 4" << std::endl;
	q.push(3);
	q.push(4);
	std::cout << "size: " << q.count() << std::endl;
	std::cout << "pop 2 times" << std::endl;
	int data;
	q.pop(&data);
	std::cout << data << std::endl;
	q.pop(&data);
	std::cout << data << std::endl;
	std::cout << "size: " << q.count() << std::endl;
	std::cout << "push 100" << std::endl;
	q.push(100);
	std::cout << "size: " << q.count() << std::endl;
	std::cout << "pop 3 times" << std::endl;
	q.pop(&data);
	std::cout << data << std::endl;
	q.pop(&data);
	std::cout << data << std::endl;
	q.pop(&data);
	std::cout << data << std::endl;
	std::cout << "size: " << q.count() << std::endl;

	return 0;
}

