#include <iostream>
#include <cstring>
#include <cstdlib>
#include <fstream>
#include <cstdio>

using namespace std;

//https://www.pianshen.com/article/4792452708/

void rgb_to_nv21(unsigned char *rgb, int w, int h, unsigned char **nv21) {
	if(*nv21 == nullptr) {
		*nv21 = new unsigned char[w*h*3/2];
	}
	int y,u,v,r,g,b;
	//vu起始位置
	long long vu_index = w*h;
	unsigned char *data = *nv21;
	for(int i=0; i<h; i++) {
		for(int j=0; j<w; j++) {
			int rd = i*w*3 + j*3;
			r = rgb[rd];
			g = rgb[rd+1];
			b = rgb[rd+2];

			y = (77 * r + 150 * g + 29 * b) >> 8;
			u = ((-44 * r - 87 * g + 131 * b) >> 8) + 128;
			v = ((131 * r - 110 * g - 21 * b) >> 8) + 128;	

			y = (unsigned char) (y > 255 ? 255 : (y < 0 ? 0 : y));
			u = (unsigned char) (u > 255 ? 255 : (u < 0 ? 0 : u));
			v = (unsigned char) (v > 255 ? 255 : (v < 0 ? 0 : v));

			data[i*w+j] = y;

			//行列index为偶数时采集一次vu，注意index从0开始
			if(i % 2 == 0 && j % 2 == 0) {
				data[vu_index] = v;
				data[vu_index+1] = u;
				vu_index += 2;
			}
		}
	}
}

int main(int argc, char **argv) {
	if(argc != 5) {
		cout << "error args, format: input_file width heigh output_file" << endl;
		return 1;
	}else {
		const char *input_file = argv[1];
		int w = atoi(argv[2]);
		int h = atoi(argv[3]);
		const char *output_file = argv[4];

		cout << "input_file: " << input_file << endl;
		cout << "w: " << w << " h: " << h << endl;
		cout << "output_file: " << output_file << endl;

		const long long len = w*h*3;
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

		cout << "read rgb ... " << endl;
		in.read(reinterpret_cast<char *>(data), len);
		//in.read(data, len);
		streamsize rds = in.gcount();
		in.close();
		cout << "read: " << rds << "/" << len << endl;

		cout << "change to nv21..." << endl;
		rgb_to_nv21(data, w, h, &data_out);

		cout << "write nv21..." << endl;
		out.write(reinterpret_cast<char *>(data_out), w*h*3/2);
		//out.write(data_out, len);
		out.close();
		cout << "write nv21 ok" << endl;

		delete[] data_out;
	}

	return 0;
}
