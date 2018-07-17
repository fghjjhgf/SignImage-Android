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
    (JNIEnv * env, jclass clazz, jobject srcbm, jobject signbm, jobject destbm, jint powervalue, jint rgbcheck, jint,jobject redchanelbm,
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

        //init pixels
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

        int sourcewidth = redchanelbm.width;
        int sourceheight = redchanelbm.height;
        int signimage_width = signbm.width;
        int signimage_height = signbm.height;

        int color;
        int red;
        int green;
        int blue;

        int32_t *srcPixs = (int32_t *) srcbmpixels;

        (int32_t *)srcbmpixelsbuf = (int32_t *)srcbmpixels;
        (int32_t *)signbmpixelsbuf = (int32_t *)signbmpixels;
        (int32_t *)destbmpixelsbuf = (int32_t *)destbmpixels;
        (int32_t *)redchanelbmpixelsbuf = (int32_t *)redchanelbmpixels;
        (int32_t *)greenchanelbmpixelsbuf = (int32_t *)greenchanelbmpixels;
        (int32_t *)bluechanelbmpixelsbuf = (int32_t *)bluechanelbmpixels;
        (int32_t *)redsignpixelsbuf = (int32_t *)redsignpixels;
        (int32_t *)greensignpixelsbuf = (int32_t *)greensignpixels;
        (int32_t *)bluesignpixelsbuf = (int32_t *)bluesignpixels;

        (int32_t *)redbufcount = (int32_t *)malloc(sizeof(int32_t) * sw * sh);
        (int32_t *)greenbufcount = (int32_t *)malloc(sizeof(int32_t) * sw * sh);
        (int32_t *)bluebufcount = (int32_t *)malloc(sizeof(int32_t) * sw * sh);

        for(int i=0;i<sourceheight;i++)
        {
           for(int j=0;j<sourcewidth;j++){
                color = srcbmpixelsbuf[sourcewidth * i + j];
                red = ((color & 0x00FF0000) >> 16);
                green = ((color & 0x0000FF00) >> 8);
                blue = color & 0x000000FF;
                redchanelbmpixelsbuf[sourcewidth * i + j] = red;
                redbufcount = red;
                greenchanelbmpixelsbuf[sourcewidth * i + j] = green;
                greenbufcount = green;
                bluechanelbmpixelsbuf[sourcewidth * i + j] = blue;
                bluebufcount = blue;
           }
        }

        int *power,Max;float *Amp;float *mapScale;

        power=(int *)malloc(sizeof(int) * sw * sh);
        Amp=(float *)malloc(sizeof(float) * sw * sh);
        pcplx=(complex *)malloc(sizeof(complex) * sw * sh);

        mapScale=(float *)malloc(sizeof(float) * sw * sh / 4);

        //Initialize Map Scale

        for(int i = 0; i < signimage_height; i++){
            for(int j = 0; j < signimage_width; j++){
                mapScale[signimage_width * i + j] = signbmpixelsbuf[signimage_width * i + j] & 0x00FFFFFF ? 1.0f / powervalue : 1.0f;
            }
        }

        //Sort R
        Max=0;

        for(int i = 0; i < sourceheight; i++){
            for(int j = 0; j < sourcewidth; j++){
                pcplx[i * sourcewidth + j].im = 0;
                color = srcbmpixelsbuf[i * sourcewidth + j];
                red = ((color & 0x00FF0000) >> 16);
                pcplx[i * sourcewidth + j].re = red;
            }
        }

        FFT_2(pcplx,pcplx,sourceheight);

        for(int y = 1; y < sourceheight/2-1; y++){
            for(int x = 1; x < sourcewidth/2-1; x++){
                pcplx[y*w+x].re*=mapScale[y*w/2+x];
                pcplx[y*w+x].im*=mapScale[y*w/2+x];

                pcplx[(h-y)*w+w-x].re*=mapScale[y*w/2+x];
                pcplx[(h-y)*w+w-x].im*=mapScale[y*w/2+x];
            }
        }

        FFT_2_Shift(pcplx,pcplx,sourceheight);

        for (int y=0;y<sourceheight;y++){
        	for (int x=0;x<sourcewidth;x++)
                power[y*h+x]=sqrt(pcplx[y*h+x].im*pcplx[y*h+x].im+pcplx[y*h+x].re+pcplx[y*h+x].re);
                if(pcplx[y*h+x].re*pcplx[y*h+x].re<0.00000001)
                	Amp[y*h+x]=0;
                else
                	Amp[y*h+x]=atan(pcplx[y*h+x].im/pcplx[y*h+x].re);
                power[y*h+x]=log(long double(1+3*power[y*h+x]));
                if (power[y*h+x]>Max)
                {
                	Max=power[y*h+x];
                }
            }
        }

        if (Max == 0){
            for (int y=0;y<sourceheight;y++){
            	for (int x=0;x<sourcewidth;x++){
            		//freqRedImage.setPixel(x,y,QColor(0,0,0).rgba());
                    redsignpixelsbuf[y * sourcewidth + x] = 0xFF000000;
            	}
            }
        } else {
            for (int y=0;y<sourceheight;y++){
		    	for (int x=0;x<sourcewidth;x++){
		    		//freqRedImage.setPixel(x,y,QColor(power[y*h+x]*255/Max,power[y*h+x]*255/Max,power[y*h+x]*255/Max).rgba());
                    //redsignpixelsbuf[y * sourcewidth + x]
		        }
		    }
        }

        //release bitmap
        do{
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

