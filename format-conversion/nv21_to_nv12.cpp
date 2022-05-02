#include <iostream>
#include <cstring>
#include <cstdlib>
#include <fstream>
#include <cstdio>

using namespace std;

//void *memset(void *dest, int ch, std::size_t count);
//void *memcpy(void *dest, void *src, std::size_t count);	

void nv21_to_nv12(unsigned char *in, int w, int h, unsigned char **out) {
	//if(*out == nullptr) {
	//       	*out = (unsigned char *)malloc(w*h*3/2);
	//	memset(*out, 0, w*h*3/2);
	//}
	
	if(*out == nullptr) {
		*out = new unsigned char[w*h*3/2];
	}

	cout << "memcpy..." << endl;
	//先复制y
	memcpy(*out, in, w*h);

	//跳过y指向v
	unsigned char *vp = in + w*h;
	unsigned char *dvp = *out + w*h;

	//遍历vu,将vu转成uv
	long long len = w*h/2;
	for(long long i=0; i<len/2; i++) {

		if(i%100==0)printf("for %lld : %lld\n", i, len/2);
		
		*dvp = *(vp+1);
		*(dvp+1) = *vp;
		vp += 2;
		dvp += 2;
	}
}

int main(int argc, char **argv) {
	if(argc != 5) {
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
	}else {
		const char *input_file = argv[1];
		int w = atoi(argv[2]);
		int h = atoi(argv[3]);
		const char *output_file = argv[4];

		cout << "input_file: " << input_file << endl;
		cout << "w: " << w << " h: " << h << endl;
		cout << "output_file: " << output_file << endl;

		const long long len = w*h*3/2;
		unsigned char data[len];
		unsigned char *data_out = nullptr;
		
		ifstream in(input_file, ios::binary);
		//basic_ifstream<unsigned char> in(input_file, ios::binary);
		if(!in) {
			cout << "failed to open input file: " << input_file << endl;
			return 1;
		}
		ofstream out(output_file, ios::binary);
		//basic_ofstream<unsigned char> out(output_file, ios::binary);
		if(!out) {
			cout << "failed to open output file: " << output_file << endl;
			return 1;
		}

		cout << "read nv21 ... " << endl;
		in.read(reinterpret_cast<char *>(data), len);
		//in.read(data, len);
		streamsize rds = in.gcount();
		in.close();
		cout << "read: " << rds << "/" << len << endl;

		cout << "change to nv12..." << endl;
		nv21_to_nv12(data, w, h, &data_out);

		cout << "write nv12..." << endl;
		out.write(reinterpret_cast<char *>(data_out), len);
		//out.write(data_out, len);
		out.close();
		cout << "write nv12 ok" << endl;


		//free(data_out);
		delete [] data_out;
	}

	return 0;
}
