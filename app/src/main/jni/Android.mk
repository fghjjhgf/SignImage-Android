LOCAL_PATH:=$(call my-dir)
include $(CLEAR_VARS)
LOCAL_MODULE := NDKUtils
LOCAL_SRC_FILES := com_lb_imagesigner_JNIUtil_JNIUtils.c
LOCAL_SRC_FILES += Pt_FFT.c
include $(BUILD_SHARED_LIBRARY)