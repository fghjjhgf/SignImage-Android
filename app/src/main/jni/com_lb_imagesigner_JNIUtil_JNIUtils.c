#include "com_lb_imagesigner_JNIUtil_JNIUtils.h"

#include "Pt_FFT.h"

#include <android/bitmap.h>
#include <android/log.h>
#include <math.h>
#include <string.h>


#define  LOG_TAG    "SIGNING IMAGE"
#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)

JNIEXPORT jstring JNICALL Java_com_lb_imagesigner_JNIUtil_JNIUtils_sayHelloFromJNI(
    JNIEnv* env, jclass clazz){

    return (*env)->NewStringUTF(env,"hello, I am from C");
  }

JNIEXPORT void JNICALL Java_com_lb_imagesigner_JNIUtil_JNIUtils_imageEncrypt(
    JNIEnv* env, jclass clazz, jobject srcbm, jobject encryptbm){

    AndroidBitmapInfo srcbitmap, encryptbitmap;

    void * srcpixels;
    void * encryptpixels;

    int ret;

    if ((ret = AndroidBitmap_getInfo(env, srcbm, &srcbitmap)) < 0) {
		LOGE("AndroidBitmap_getInfo() failed srcbitmap! error=%d", ret);
		return;
	}

	if ((ret = AndroidBitmap_getInfo(env, encryptbm, &encryptbitmap)) < 0) {
    	LOGE("AndroidBitmap_getInfo() failed encryptbitmap! error=%d", ret);
    	return;
    }

    if ((ret = AndroidBitmap_lockPixels(env, srcbm, &srcpixels)) < 0) {
    	LOGE("AndroidBitmap_lockPixels() failed srcpixels! error=%d", ret);
    }

    if ((ret = AndroidBitmap_lockPixels(env, encryptbm, &encryptpixels)) < 0) {
    	LOGE("AndroidBitmap_lockPixels() failed encryptpixels! error=%d", ret);
    }


    int32_t *srcPixs = (int32_t *) srcpixels;
    int32_t *encryptPixs = (int32_t *) encryptpixels;

    int w = srcbitmap.width;
    int h = srcbitmap.height;
    int color;
    int red;
    int green;
    int blue;

    for(int i = 0; i < h; i++){
        for(int j = 0; j < w; j++){
            color = srcPixs[w * i + j];
            red = ((color & 0x00FF0000) >> 16);
            green = ((color & 0x0000FF00) >> 8);
            blue = color & 0x000000FF;
            encryptPixs[w * i + j] = color;
        }
    }

    AndroidBitmap_unlockPixels(env, srcbm);
    AndroidBitmap_unlockPixels(env, encryptbm);
  }

  JNIEXPORT void JNICALL Java_com_lb_imagesigner_JNIUtil_JNIUtils_signImage
    (JNIEnv * env, jclass clazz, jobject srcbm, jobject signbm, jobject destbm, jint powervalue, jint rgbcheck, jobject redchanelbm,
    jobject greenchanelbm, jobject bluechanelbm, jobject redsign, jobject greensign, jobject bluesign){

        AndroidBitmapInfo srcbminfo, signbminfo, destbminfo, redchanelbminfo, greenchanelbminfo,
                    bluechanelbminfo, redsigninfo, greensigninfo, bluesigninfo;

        void* srcbmpixels;
        void* signbmpixels;
        void* destbmpixels;
        void* redchanelbmpixels;
        void* greenchanelbmpixels;
        void* bluechanelbmpixels;
        void* redsignpixels;
        void* greensignpixels;
        void* bluesignpixels;

        int ret;
        //init AndroidBitmapInfo
        do{
            if ((ret = AndroidBitmap_getInfo(env, srcbm, &srcbminfo)) < 0) {
            		LOGE("AndroidBitmap_getInfo() failed srcbminfo! error=%d", ret);
            		return;
            }
            if ((ret = AndroidBitmap_getInfo(env, signbm, &signbminfo)) < 0) {
                LOGE("AndroidBitmap_getInfo() failed signbminfo! error=%d", ret);
                return;
            }
            if ((ret = AndroidBitmap_getInfo(env, destbm, &destbminfo)) < 0) {
                LOGE("AndroidBitmap_getInfo() failed destbminfo! error=%d", ret);
                return;
            }
            if ((ret = AndroidBitmap_getInfo(env, redchanelbm, &redchanelbminfo)) < 0) {
                LOGE("AndroidBitmap_getInfo() failed redchanelbminfo! error=%d", ret);
                return;
            }
            if ((ret = AndroidBitmap_getInfo(env, greenchanelbm, &greenchanelbminfo)) < 0) {
                LOGE("AndroidBitmap_getInfo() failed greenchanelbminfo! error=%d", ret);
                return;
            }
            if ((ret = AndroidBitmap_getInfo(env, bluechanelbm, &bluechanelbminfo)) < 0) {
                LOGE("AndroidBitmap_getInfo() failed bluechanelbminfo! error=%d", ret);
                return;
            }
            if ((ret = AndroidBitmap_getInfo(env, redsign, &redchanelbminfo)) < 0) {
                LOGE("AndroidBitmap_getInfo() failed redchanelbminfo! error=%d", ret);
                return;
            }
            if ((ret = AndroidBitmap_getInfo(env, greensign, &greensigninfo)) < 0) {
                LOGE("AndroidBitmap_getInfo() failed greensigninfo! error=%d", ret);
                return;
            }
            if ((ret = AndroidBitmap_getInfo(env, bluesign, &bluesigninfo)) < 0) {
                LOGE("AndroidBitmap_getInfo() failed bluesigninfo! error=%d", ret);
                return;
            }
        }while(0);

        //init pixels buf
        do{
            if ((ret = AndroidBitmap_lockPixels(env, srcbm, &srcbmpixels)) < 0) {
            		LOGE("AndroidBitmap_lockPixels() failed srcbmpixels! error=%d", ret);
            }
            if ((ret = AndroidBitmap_lockPixels(env, signbm, &signbmpixels)) < 0) {
            		LOGE("AndroidBitmap_lockPixels() failed signbmpixels! error=%d", ret);
            }
            if ((ret = AndroidBitmap_lockPixels(env, destbm, &destbmpixels)) < 0) {
            		LOGE("AndroidBitmap_lockPixels() failed srcbmpixels! error=%d", ret);
            }
            if ((ret = AndroidBitmap_lockPixels(env, redchanelbm, &redchanelbmpixels)) < 0) {
            		LOGE("AndroidBitmap_lockPixels() failed redchanelbmpixels! error=%d", ret);
            }
            if ((ret = AndroidBitmap_lockPixels(env, greenchanelbm, &greenchanelbmpixels)) < 0) {
            		LOGE("AndroidBitmap_lockPixels() failed greenchanelbmpixels! error=%d", ret);
            }
            if ((ret = AndroidBitmap_lockPixels(env, bluechanelbm, &bluechanelbmpixels)) < 0) {
            		LOGE("AndroidBitmap_lockPixels() failed bluechanelbmpixels! error=%d", ret);
            }
            if ((ret = AndroidBitmap_lockPixels(env, redsign, &redsignpixels)) < 0) {
            		LOGE("AndroidBitmap_lockPixels() failed redsignpixels! error=%d", ret);
            }
            if ((ret = AndroidBitmap_lockPixels(env, greensign, &greensignpixels)) < 0) {
            		LOGE("AndroidBitmap_lockPixels() failed greensignpixels! error=%d", ret);
            }
            if ((ret = AndroidBitmap_lockPixels(env, bluesign, &bluesignpixels)) < 0) {
            		LOGE("AndroidBitmap_lockPixels() failed bluesignpixels! error=%d", ret);
            }
        }while(0);

        int sourcewidth = srcbminfo.width;
        int sourceheight = srcbminfo.height;
        int signimage_width = signbminfo.width;
        int signimage_height = signbminfo.height;
        int chanelimage_width = redchanelbminfo.width;
        int chanelimage_height = redchanelbminfo.height;

        int color;
        int red;
        int green;
        int blue;

        int32_t *srcbmpixelsbuf = (int32_t *)srcbmpixels;
        int32_t *signbmpixelsbuf = (int32_t *)signbmpixels;
        int32_t *destbmpixelsbuf = (int32_t *)destbmpixels;
        int32_t *redchanelbmpixelsbuf = (int32_t *)redchanelbmpixels;
        int32_t *greenchanelbmpixelsbuf = (int32_t *)greenchanelbmpixels;
        int32_t *bluechanelbmpixelsbuf = (int32_t *)bluechanelbmpixels;
        int32_t *redsignpixelsbuf = (int32_t *)redsignpixels;
        int32_t *greensignpixelsbuf = (int32_t *)greensignpixels;
        int32_t *bluesignpixelsbuf = (int32_t *)bluesignpixels;

        int32_t *redbufcount = (int32_t *)malloc(sizeof(int32_t) * chanelimage_width * chanelimage_height);
        int32_t *greenbufcount = (int32_t *)malloc(sizeof(int32_t) * chanelimage_width * chanelimage_height);
        int32_t *bluebufcount = (int32_t *)malloc(sizeof(int32_t) * chanelimage_width * chanelimage_height);

        int32_t *half_signbmpixelsbuf = (int32_t*)malloc(sizeof(int32_t) * chanelimage_width * chanelimage_height /4);


        /**/
        for(int i=0;i<sourceheight;i++){
            for(int j=0;j<sourcewidth;j++){

                color = srcbmpixelsbuf[sourcewidth * i + j];
                destbmpixelsbuf[sourcewidth * i + j] = color;

            }
        }

        for(int i=0;i<chanelimage_height;i++)
        {
           for(int j=0;j<chanelimage_width;j++){
                color = srcbmpixelsbuf[sourcewidth * i + j];
                blue = ((color & 0x00FF0000) >> 16);
                green = ((color & 0x0000FF00) >> 8);
                red = color & 0x000000FF;
                bluechanelbmpixelsbuf[chanelimage_width * i + j] = color & 0xFFFF0000;
                bluebufcount[chanelimage_width * i + j] = blue;
                greenchanelbmpixelsbuf[chanelimage_width * i + j] = color & 0xFF00FF00;
                greenbufcount[chanelimage_width * i + j] = green;
                redchanelbmpixelsbuf[chanelimage_width * i + j] = color & 0xFF0000FF;
                redbufcount[chanelimage_width * i + j] = red;
           }
        }

        complex *pcplx;
        int *power,Max;float *Amp;float *mapScale;
        power=(int *)malloc(sizeof(int) * chanelimage_width * chanelimage_height);
        Amp=(float *)malloc(sizeof(float) * chanelimage_width * chanelimage_height);
        pcplx=(complex *)malloc(sizeof(complex) * chanelimage_width * chanelimage_height);
        mapScale=(float *)malloc(sizeof(float) * chanelimage_width * chanelimage_height / 4);

        //resize signimage
        int resizeRate = 2;
        for(int i = 0;i<signimage_height/2;i++){
            int tsrcH = (int)(resizeRate * i + 0.5);
            for(int j = 0;j<signimage_width/2;j++){
                int tsrcW = (int)(resizeRate * j + 0.5);
                half_signbmpixelsbuf[i * signimage_width/2 + j] = signbmpixelsbuf[tsrcH * signimage_width + tsrcW];
                //destbmpixelsbuf[sourcewidth * i + j] = signbmpixelsbuf[tsrcH * signimage_width + tsrcW];
            }
        }

        //Initialize Map Scale

        for(int i = 0; i < chanelimage_height/2; i++){
            for(int j = 0; j < chanelimage_width/2; j++){
                mapScale[chanelimage_width/2 * i + j] = signbmpixelsbuf[chanelimage_width/2 * i + j] & 0x00FFFFFF ? 1.0f/powervalue : 1.0f;
            }
        }/**/

        do{
           //Sort R
            Max=0;

             for(int i = 0; i < chanelimage_height; i++){
                 for(int j = 0; j < chanelimage_width; j++){
                     pcplx[i * chanelimage_width + j].im = 0;
                     color = srcbmpixelsbuf[i * sourcewidth + j];
                     red = color & 0x000000FF;
                     pcplx[i * chanelimage_width + j].re = red;
                 }
             }

             FFT_2(pcplx,pcplx,chanelimage_height);

             for(int y = 1; y < chanelimage_height/2-1; y++){
                 for(int x = 1; x < chanelimage_width/2-1; x++){
                     pcplx[y*chanelimage_width+x].re*=mapScale[y*chanelimage_width/2+x];
                     pcplx[y*chanelimage_width+x].im*=mapScale[y*chanelimage_width/2+x];

                     pcplx[(chanelimage_width-y)*chanelimage_width+chanelimage_width-x].re*=mapScale[y*chanelimage_width/2+x];
                     pcplx[(chanelimage_width-y)*chanelimage_width+chanelimage_width-x].im*=mapScale[y*chanelimage_width/2+x];
                 }
             }

             FFT_2_Shift(pcplx,pcplx,chanelimage_height);

             for (int y=0;y<chanelimage_height;y++){
             	for (int x=0;x<chanelimage_width;x++){
             	    //power[y*h+x]=sqrt(pcplx[y*h+x].im*pcplx[y*h+x].im+pcplx[y*h+x].re+pcplx[y*h+x].re);
                    power[y*chanelimage_height+x]=sqrt(pcplx[y*chanelimage_height+x].im*pcplx[y*chanelimage_height+x].im+pcplx[y*chanelimage_height+x].re+pcplx[y*chanelimage_height+x].re);

                    //power[y*h+x]=log(long double(1+3*power[y*h+x]));
                    power[y*chanelimage_height+x]=log((double)(1+3*power[y*chanelimage_height+x]));
                    if (power[y*chanelimage_height+x] > Max)
                    {
                    	Max=power[y*chanelimage_height+x];
                    }
                 }
             }

             if (Max == 0){
                 for (int y=0;y<chanelimage_height;y++){
                 	for (int x=0;x<chanelimage_width;x++){
                 		//freqRedImage.setPixel(x,y,QColor(0,0,0).rgba());
                         redsignpixelsbuf[y * chanelimage_width + x] = 0x00000000;
                 	}
                 }
             } else {
                 for (int y=0;y<chanelimage_height;y++){
                	for (int x=0;x<chanelimage_width;x++){
                	    color = power[chanelimage_width * y + x];
                        red = power[chanelimage_width * y + x] * 255 / Max;
                        green = ((power[chanelimage_width * y + x] * 255 / Max) << 8)&0xFF00FF00;
                        blue = ((power[chanelimage_width * y + x] * 255 / Max) << 16)&0xFFFF0000;

                	    //freqRedImage.setPixel(x,y,QColor(power[y*h+x]*255/Max,power[y*h+x]*255/Max,power[y*h+x]*255/Max).rgba());

                        redsignpixelsbuf[y * chanelimage_width + x] = 0xFF000000|blue|green|red;
                    }
                }
             }

             FFT_2_Shift(pcplx,pcplx,chanelimage_height);
             IFFT_2(pcplx,pcplx,chanelimage_height);

             for (int y=0;y<chanelimage_height;y++)
             {
             		for (int x=0;x<chanelimage_height;x++)
             		{
             			if(pcplx[y*chanelimage_width+x].re>255)
             			{
             					//RedChannelImage.setPixel(x,y,QColor(255,0,0).rgba());
             					redchanelbmpixelsbuf[y * chanelimage_height +x] = 0xFF0000FF;
             					continue;
             			}

             			if(pcplx[y*chanelimage_height+x].re<0)
             			{
             					//RedChannelImage.setPixel(x,y,QColor(0,0,0).rgba());
             					redchanelbmpixelsbuf[y * chanelimage_height +x] = 0xFF000000;
             					continue;
             			}

             			//RedChannelImage.setPixel(x,y,QColor(pcplx[y*h+x].re,0,0).rgba());
             			color = pcplx[chanelimage_height * y + x].re;
             			color = color | 0xFF000000;
             			redchanelbmpixelsbuf[y * chanelimage_height +x] = color;
             		}
             }

             //end Sort R

             //Sort G
             Max=0;

             for(int i = 0; i < chanelimage_height; i++){
                 for(int j = 0; j < chanelimage_width; j++){
                     pcplx[i * chanelimage_width + j].im = 0;
                     color = srcbmpixelsbuf[i * sourcewidth + j];
                     green = (color & 0x0000FF00) >> 8;
                     pcplx[i * chanelimage_width + j].re = green;
                 }
             }

             FFT_2(pcplx,pcplx,chanelimage_height);

             for(int y = 1; y < chanelimage_height/2-1; y++){
                 for(int x = 1; x < chanelimage_width/2-1; x++){
                     pcplx[y*chanelimage_width+x].re*=mapScale[y*chanelimage_width/2+x];
                     pcplx[y*chanelimage_width+x].im*=mapScale[y*chanelimage_width/2+x];

                     pcplx[(chanelimage_width-y)*chanelimage_width+chanelimage_width-x].re*=mapScale[y*chanelimage_width/2+x];
                     pcplx[(chanelimage_width-y)*chanelimage_width+chanelimage_width-x].im*=mapScale[y*chanelimage_width/2+x];
                 }
             }

             FFT_2_Shift(pcplx,pcplx,chanelimage_height);

             for (int y=0;y<chanelimage_height;y++){
                for (int x=0;x<chanelimage_width;x++){
                    //power[y*h+x]=sqrt(pcplx[y*h+x].im*pcplx[y*h+x].im+pcplx[y*h+x].re+pcplx[y*h+x].re);
                    power[y*chanelimage_height+x]=sqrt(pcplx[y*chanelimage_height+x].im*pcplx[y*chanelimage_height+x].im+pcplx[y*chanelimage_height+x].re+pcplx[y*chanelimage_height+x].re);

                    //power[y*h+x]=log(long double(1+3*power[y*h+x]));
                    power[y*chanelimage_height+x]=log((double)(1+3*power[y*chanelimage_height+x]));
                    if (power[y*chanelimage_height+x] > Max)
                    {
                        Max=power[y*chanelimage_height+x];
                    }
                 }
             }

             if (Max == 0){
                 for (int y=0;y<chanelimage_height;y++){
                    for (int x=0;x<chanelimage_width;x++){
                        //freqRedImage.setPixel(x,y,QColor(0,0,0).rgba());
                         greensignpixelsbuf[y * chanelimage_width + x] = 0x00000000;
                    }
                 }
             } else {
                 for (int y=0;y<chanelimage_height;y++){
                    for (int x=0;x<chanelimage_width;x++){
                        color = power[chanelimage_height * y + x];
                        red = color * 255 / Max;
                        green = ((color * 255 / Max) << 8)&0xFF00FF00;
                        blue = ((color * 255 / Max) << 16)&0xFFFF0000;

                        //freqRedImage.setPixel(x,y,QColor(power[y*h+x]*255/Max,power[y*h+x]*255/Max,power[y*h+x]*255/Max).rgba());

                        greensignpixelsbuf[y * chanelimage_width + x] = 0xFF000000|blue|green|red;
                    }
                }
             }

             FFT_2_Shift(pcplx,pcplx,chanelimage_height);
             IFFT_2(pcplx,pcplx,chanelimage_height);

             for (int y=0;y<chanelimage_height;y++)
             {
                    for (int x=0;x<chanelimage_height;x++)
                    {
                        if(pcplx[y*chanelimage_width+x].re>255)
                        {
                                //RedChannelImage.setPixel(x,y,QColor(255,0,0).rgba());
                                greenchanelbmpixelsbuf[y * chanelimage_height +x] = 0xFF00FF00;
                                continue;
                        }

                        if(pcplx[y*chanelimage_height+x].re<0)
                        {
                                //RedChannelImage.setPixel(x,y,QColor(0,0,0).rgba());
                                greenchanelbmpixelsbuf[y * chanelimage_height +x] = 0xFF000000;
                                continue;
                        }

                        //RedChannelImage.setPixel(x,y,QColor(pcplx[y*h+x].re,0,0).rgba());
                        color = pcplx[chanelimage_height * y + x].re;
                        color = (color << 8)&0x0000FF00;
                        greenchanelbmpixelsbuf[y * chanelimage_height +x] = color | 0xFF000000;
                    }
             }
             //end Sort G

             //sort B
             Max=0;

             for(int i = 0; i < chanelimage_height; i++){
                 for(int j = 0; j < chanelimage_width; j++){
                     pcplx[i * chanelimage_width + j].im = 0;
                     color = srcbmpixelsbuf[i * sourcewidth + j];
                     blue = (color & 0x00FF0000) >> 16;
                     pcplx[i * chanelimage_width + j].re = blue;
                 }
             }

             FFT_2(pcplx,pcplx,chanelimage_height);

             for(int y = 1; y < chanelimage_height/2-1; y++){
                 for(int x = 1; x < chanelimage_width/2-1; x++){
                     pcplx[y*chanelimage_width+x].re*=mapScale[y*chanelimage_width/2+x];
                     pcplx[y*chanelimage_width+x].im*=mapScale[y*chanelimage_width/2+x];

                     pcplx[(chanelimage_width-y)*chanelimage_width+chanelimage_width-x].re*=mapScale[y*chanelimage_width/2+x];
                     pcplx[(chanelimage_width-y)*chanelimage_width+chanelimage_width-x].im*=mapScale[y*chanelimage_width/2+x];
                 }
             }

             FFT_2_Shift(pcplx,pcplx,chanelimage_height);

             for (int y=0;y<chanelimage_height;y++){
                for (int x=0;x<chanelimage_width;x++){
                    //power[y*h+x]=sqrt(pcplx[y*h+x].im*pcplx[y*h+x].im+pcplx[y*h+x].re+pcplx[y*h+x].re);
                    power[y*chanelimage_height+x]=sqrt(pcplx[y*chanelimage_height+x].im*pcplx[y*chanelimage_height+x].im+pcplx[y*chanelimage_height+x].re+pcplx[y*chanelimage_height+x].re);

                    //power[y*h+x]=log(long double(1+3*power[y*h+x]));
                    power[y*chanelimage_height+x]=log((double)(1+3*power[y*chanelimage_height+x]));
                    if (power[y*chanelimage_height+x] > Max)
                    {
                        Max=power[y*chanelimage_height+x];
                    }
                 }
             }

             if (Max == 0){
                 for (int y=0;y<chanelimage_height;y++){
                    for (int x=0;x<chanelimage_width;x++){
                        //freqRedImage.setPixel(x,y,QColor(0,0,0).rgba());
                         bluesignpixelsbuf[y * chanelimage_width + x] = 0x00000000;
                    }
                 }
             } else {
                 for (int y=0;y<chanelimage_height;y++){
                    for (int x=0;x<chanelimage_width;x++){
                        color = power[chanelimage_width * y + x];
                        red = power[chanelimage_width * y + x] * 255 / Max;
                        green = ((power[chanelimage_width * y + x] * 255 / Max) << 8)&0xFF00FF00;
                        blue = ((power[chanelimage_width * y + x] * 255 / Max) << 16)&0xFFFF0000;

                        //freqRedImage.setPixel(x,y,QColor(power[y*h+x]*255/Max,power[y*h+x]*255/Max,power[y*h+x]*255/Max).rgba());

                        bluesignpixelsbuf[y * chanelimage_width + x] = 0xFF000000|blue|green|red;
                    }
                }
             }

             FFT_2_Shift(pcplx,pcplx,chanelimage_height);
             IFFT_2(pcplx,pcplx,chanelimage_height);

             for (int y=0;y<chanelimage_height;y++)
             {
                    for (int x=0;x<chanelimage_height;x++)
                    {
                        if(pcplx[y*chanelimage_width+x].re>255)
                        {
                                //RedChannelImage.setPixel(x,y,QColor(255,0,0).rgba());
                                bluechanelbmpixelsbuf[y * chanelimage_height +x] = 0xFFFF0000;
                                continue;
                        }

                        if(pcplx[y*chanelimage_height+x].re<0)
                        {
                                //RedChannelImage.setPixel(x,y,QColor(0,0,0).rgba());
                                bluechanelbmpixelsbuf[y * chanelimage_height +x] = 0xFF000000;
                                continue;
                        }

                        //RedChannelImage.setPixel(x,y,QColor(pcplx[y*h+x].re,0,0).rgba());
                        color = pcplx[chanelimage_height * y + x].re;
                        color = (color <<16) & 0x00FF0000;
                        bluechanelbmpixelsbuf[y * chanelimage_height +x] = color | 0xFF000000;
                    }
             }

             //end sort B

             //
             for (int y=0;y<chanelimage_height;y++){
             	for (int x=0;x<chanelimage_width;x++){
             	    red = redchanelbmpixelsbuf[chanelimage_width * y + x];

             	    green = greenchanelbmpixelsbuf[chanelimage_width * y + x];

             	    blue = bluechanelbmpixelsbuf[chanelimage_width * y + x];

                    destbmpixelsbuf[y * sourcewidth + x] = 0xFF000000 | blue | green | red;

             	}
             }/**/
        }while(0);


        //release bitmap
        do{
            free(power);
            free(Amp);
            free(pcplx);
            free(mapScale);
            free(redbufcount);
            free(greenbufcount);
            free(bluebufcount);
            free(half_signbmpixelsbuf);
            AndroidBitmap_unlockPixels(env, srcbm);
            AndroidBitmap_unlockPixels(env, signbm);
            AndroidBitmap_unlockPixels(env, destbm);
            AndroidBitmap_unlockPixels(env, redchanelbm);
            AndroidBitmap_unlockPixels(env, greenchanelbm);
            AndroidBitmap_unlockPixels(env, bluechanelbm);
            AndroidBitmap_unlockPixels(env, redsign);
            AndroidBitmap_unlockPixels(env, greensign);
            AndroidBitmap_unlockPixels(env, bluesign);
        }while(0);


    }

  JNIEXPORT void JNICALL Java_com_lb_imagesigner_JNIUtil_JNIUtils_displaySourcImage
    (JNIEnv *env, jclass clazz, jobject srcbm, jobject redchanelbm, jobject greenchanelbm,
    jobject bluechanelbm, jobject redsign, jobject greensign, jobject bluesign){


    }

