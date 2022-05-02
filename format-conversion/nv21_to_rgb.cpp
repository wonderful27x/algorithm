#include <iostream>
#include <cstring>
#include <cstdlib>
#include <fstream>
#include <cstdio>

using namespace std;

//错误示例子: 同一行连续的每四个Y对应一组vu,正确的应该是相邻两行上下对称的四个Y对应一组vu
void nv21_to_rgb2(unsigned char *nv21, int w, int h, unsigned char **rgb) {
	if(*rgb == nullptr) {
		*rgb = new unsigned char[w*h*3];
	}

	//y、v位置
	unsigned char *yp = nv21;
	unsigned char *vp = nv21 + w*h;

	unsigned char *rgb_p = *rgb;
	int y,u,v;
	int count = 0;

	for(int i=0; i<w*h; i++) {
		y = *yp;
		u = *(vp+1);
		v = *vp;

		//每次循环y向前走一步
		yp += 1;    
		//y每走四步，vp走两步，即四个y对应一组vu
		if(++count == 4) {
			vp += 2;
			count = 0;
		}

		//yuv公式转换并归一化到0-255
                int R = y + ((360 * (v - 128))>>8) ;
                int G = y - ((( 88 * (u - 128)  + 184 * (v - 128)))>>8) ;
                int B = y +((455 * (u - 128))>>8) ;
                R = (unsigned char) (R > 255 ? 255 : (R < 0 ? 0 : R));
                G = (unsigned char) (G > 255 ? 255 : (G < 0 ? 0 : G));
                B = (unsigned char) (B > 255 ? 255 : (B < 0 ? 0 : B));

		rgb_p[0] = R;
		rgb_p[1] = G;
		rgb_p[2] = B;
		rgb_p += 3;
	}
}



/**
 * NV21转RGB,这是根据NV21数据结构和存储方式自己写的算法
 * @param nv21
 * @param width
 * @param height
 * @return
 * https://www.pianshen.com/article/4792452708/
 */
void nv21_to_rgb(unsigned char *nv21,int width,int height, unsigned char **data) {
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

            //y u v分量位置索引算法（自己：效率低，但容易理解）
            //当前像素y分量对应的uv分量的索引，有些复杂，可以一步一步验证
	    //解释:
	    //首先循环的单位是像素，即图片宽高，而不是字节
	    //frameSize: 为y分量的大小，跳过y,将指向vu分量的位置
	    //h/2*width: 注意nv21的采样方式，每四个Y对应一组vu,且这四个Y是相邻两行上下对陈的四个Y，而不是同一行连续的四个Y，这一点非常重要
	    //也就是每两行Y对应所需的vu的字节数为width*2/2=width,所以h/2*width代表遍历的Y所对应的需要跳过的vu字节数
	    //w/2*2: 代表横向每两个像素共用一组vu,一组vu占两个字节
            int index = frameSize + h / 2 * width + w / 2 * 2;
            //这里使用的是NV21格式，计算方式需要结合NV21的排列
            p_y = h * width + w;
            p_v = index;
            p_u = index + 1;

            y = nv21[p_y];
            v = nv21[p_v];
            u = nv21[p_u];

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

		cout << "read nv21 ... " << endl;
		in.read(reinterpret_cast<char *>(data), len);
		//in.read(data, len);
		streamsize rds = in.gcount();
		in.close();
		cout << "read: " << rds << "/" << len << endl;

		cout << "change to rgb..." << endl;
		nv21_to_rgb(data, w, h, &data_out);
		//nv21_to_rgb2(data, w, h, &data_out);

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
