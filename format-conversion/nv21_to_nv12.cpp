#include <iostream>
#include <cstring>
#include <cstdlib>

using namespace std;

//void *memset(void *dest, int ch, std::size_t count);
//void *memcpy(void *dest, void *src, std::size_t count);	

void nv21_to_nv12(unsigned char *in, int w, int h, unsigned char **out) {
	if(*out == nullptr) {
	       	*out = (unsigned char *)malloc(w*h*3/2);
		memset(*out, 0, w*h*3/2);
	}

	//先复制y
	memcpy(*out, in, w*h);

	//跳过y指向v
	unsigned char *vp = in + w*h;
	unsigned char *dvp = *out + w*h;

	//遍历vu,将vu转成uv
	int len = w*h/2;
	for(int i=0; i<len/2; i++) {
		*dvp = *(vp+1);
		*(dvp+1) = *vp;
		vp += 2;
		dvp += 2;
	}
}

int main(int argc, char **argv) {
	unsigned char nv21[] = {
		8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,
		2,1,    2,1,    2,1,    2,1
	};

	cout << "nv21: " << endl;
	for(int i=0; i<24; i++) {
		cout << (int)nv21[i] << ",";
	}
	cout << endl;

	unsigned char *nv12 = nullptr;
	nv21_to_nv12(nv21,4,4,&nv12);

	cout << "nv12: " << endl;
	for(int j=0; j<24; j++) {
		cout << (int)nv12[j] << ",";
	}
	cout << endl;
	
	return 0;
}
