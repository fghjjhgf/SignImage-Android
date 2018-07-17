package com.lb.imagesigner.JNIUtil;

import android.graphics.Bitmap;

/**
 * Created by Administrator on 2018/7/11.
 */

public class JNIUtils {
    public static native String sayHelloFromJNI();

    public static native void imageEncrypt(Bitmap srcbitmap, Bitmap decbitmap);

    public static native void signImage(Bitmap srcbitmap, Bitmap signbitmap, Bitmap desrbitmap, int power, int rgbcheck, Bitmap redchanelbm, Bitmap greenchanelbm, Bitmap bluechanelbm, Bitmap redsign, Bitmap greensign, Bitmap bluesign);

    public static native void displaySourcImage(Bitmap srcbitmap, Bitmap redchanelbm, Bitmap greenchanelbm, Bitmap bluechanelbm, Bitmap redsign, Bitmap greensign, Bitmap bluesign);

    static{
        System.loadLibrary("jniutils");
    }
}
