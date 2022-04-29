void nv21_to_rgb(unsigned char *nv21, int w, int h, unsigned char **rgb) {
	if(*rgb == nullptr) {
		*rgb = (unsigned char *)malloc(w*h*3);
		memset(*rgb, 0, w*h*3);
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
 */
uint8_t *WonderfulOpenCv::nv21ToRgb(uint8_t *nv21,int width,int height) {
    //定义一个二维数组存储像素，与openCv的Mat矩阵式是对应的，
    //Mat(高，宽), CV_8UC3:三个通道，每个通道8位
    uint8_t rgb[height][width * 3];
    uint8_t r, g, b;                   //r g b分量
    int y, v, u;                       //y u v分量
    int p_y, p_v, p_u;                 //y u v分量的位置索引
    int frameSize = width * height;

    for (int h = 0; h < height; h++) {
        for (int w = 0; w < width; w++) {
            //y u v分量位置索引算法（自己：效率低，但容易理解）
            //当前像素y分量对应的uv分量的索引，有些复杂，可以一步一步验证
            int index = frameSize + h / 2 * width + w / 2 * 2;
            //这里使用的是NV21格式，计算方式需要结合NV21的排列
            p_y = h * width + w;
            p_v = index;
            p_u = index + 1;

            y = nv21[p_y];
            v = nv21[p_v];
            u = nv21[p_u];

//            //公式一（老师给出）
//            double R = y + 1.402 * (v - 128);
//            double G = y - 0.34413 * (u - 128) - 0.71414 * (v - 128);
//            double B = y + 1.772 * (u - 128);

//            //公式二（网上）
//            double R = (uint8_t)(y + 1.4075 * (v - 128));
//            double G = (uint8_t)((y - 0.3455 * (u - 128) - 0.7169 * (v - 128)));
//            double B = (uint8_t)((y + 1.779 * (u - 128)));

//            //公式三（网上）
//            double R = ((y-16) * 1.164 + (v-128) * 1.596);
//            double G = ((y-16) * 1.164 - (u-128) * 0.392 - (v-128) * 0.813);
//            double B = ((y-16) * 1.164 + (u-128) * 2.017);

//            //公式四（网上）
//            double R = (1.164*y + 1.596 * v - 222.9);
//            double G = (1.164*y - 0.392 * u - 0.823 * v + 135.6);
//            double B = (1.164*y + 2.017 * u- 276.8);

//            //公式五（网上）
//            int R = (298*y + 411 * v - 57344)>>8;
//            int G = (298*y - 101* u - 211* v+ 34739)>>8;
//            int B = (298*y + 519* u - 71117)>>8;

            //公式六（网上）
            int R = y + ((360 * (v - 128))>>8) ;
            int G = y - ((( 88 * (u - 128)  + 184 * (v - 128)))>>8) ;
            int B = y +((455 * (u - 128))>>8) ;

            r = (uint8_t) (R > 255 ? 255 : (R < 0 ? 0 : R));
            g = (uint8_t) (G > 255 ? 255 : (G < 0 ? 0 : G));
            b = (uint8_t) (B > 255 ? 255 : (B < 0 ? 0 : B));

            rgb[h][w * 3 + 0] = r;
            rgb[h][w * 3 + 1] = g;;
            rgb[h][w * 3 + 2] = b;
        }
    }
    return *rgb;
}
