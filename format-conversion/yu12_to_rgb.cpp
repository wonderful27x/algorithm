#include <iostream>
#include <cstring>
#include <cstdlib>
#include <fstream>
#include <cstdio>

using namespace std;

void yu12_to_rgb(unsigned char *yuv,int width,int height, unsigned char **data) {
    if(*data == nullptr) {
	    *data = new unsigned char[height*width*3];
    }

    //动态分配内存连续的二维数组
    //data是一块连续的内存
    //rgb是一个指针数组，数组的每个元素是一个unsigned char的指针,rgb是动态分配的，需要delete[]
    //在for循环中让数组的每个元素指针指向连续的内存地址
    unsigned char **rgb = new unsigned char *[height];
    for(int i=0; i<height; i++) {
	    rgb[i] = *data + i*width*3;
    }

    ////rgb是一个指针数组，但不是动态分配的，rgb本身不需要delete[]
    //unsigned char *rgb[] = unsigned char *[height];
    //for(int i=0; i<height; i++) {
    //        rgb[i] = *data + i*width*3;
    //}

    //定义一个二维数组存储像素，与openCv的Mat矩阵式是对应的，
    //Mat(高，宽), CV_8UC3:三个通道，每个通道8位
    //unsigned char rgb[height][width * 3];
    unsigned char r, g, b;                   //r g b分量
    int y, v, u;                       //y u v分量
    int p_y, p_v, p_u;                 //y u v分量的位置索引
    int frameSize = width * height;

    long long i = 0;
    for (int h = 0; h < height; h++) {
        for (int w = 0; w < width; w++) {

	    if((++i)%100==0)printf("for %lld : %lld\n", i, (long long)width*height);

            int index_u = frameSize + h / 2 * width / 2 + w / 2;
            int index_v = frameSize + frameSize/4 + h / 2 * width / 2 + w / 2;
            p_y = h * width + w;
            p_u = index_u;
            p_v = index_v;

            y = yuv[p_y];
            u = yuv[p_u];
            v = yuv[p_v];

            //公式六（网上）
            int R = y + ((360 * (v - 128))>>8) ;
            int G = y - ((( 88 * (u - 128)  + 184 * (v - 128)))>>8) ;
            int B = y +((455 * (u - 128))>>8) ;

            r = (unsigned char) (R > 255 ? 255 : (R < 0 ? 0 : R));
            g = (unsigned char) (G > 255 ? 255 : (G < 0 ? 0 : G));
            b = (unsigned char) (B > 255 ? 255 : (B < 0 ? 0 : B));

            rgb[h][w * 3 + 0] = r;
            rgb[h][w * 3 + 1] = g;;
            rgb[h][w * 3 + 2] = b;
        }
    }
    delete[] rgb;
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

		cout << "read yu12 ... " << endl;
		in.read(reinterpret_cast<char *>(data), len);
		//in.read(data, len);
		streamsize rds = in.gcount();
		in.close();
		cout << "read: " << rds << "/" << len << endl;

		cout << "change to rgb..." << endl;
		yu12_to_rgb(data, w, h, &data_out);

		cout << "write rgb..." << endl;
		out.write(reinterpret_cast<char *>(data_out), w*h*3);
		//out.write(data_out, len);
		out.close();
		cout << "write rgb ok" << endl;

		delete[] data_out;
	}

	return 0;
}










//void nv21_to_rgb3(unsigned char *nv21, int w, int h, unsigned char **rgb) {
//	if(*rgb == nullptr) {
//		*rgb = new unsigned char[w*h*3];
//	}
//
//	//v位置
//	unsigned char *vp = nv21 + w*h;
//
//	//指定四个指针，即共用同一组vu的四个Y
//	unsigned char *y0 = *rgb;
//	unsigned char *y1 = y0+1;
//	unsigned char *y2 = y0+w;
//	unsigned char *y3 = y2+1;
//
//	//每次横向和纵向都走两步
//	for(int i=0; i<h/2; i += 2) {
//		for(j=0; j<w/2; j += 2) {
//			y0 += i*2*w + j*2;
//			y1 = y0+1;
//			y2 = y0+w;
//			y3 = y2+1;
//			vp = i*w + j
//			//...?
//		}
//	}
//}
