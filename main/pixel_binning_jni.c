#include "pixel_binning_jni.h"
#include <jni.h>

JNIEXPORT void JNICALL Java_project_PixelBinning_upscale2x2
  (JNIEnv *env, jobject obj, jintArray inputImage, jint width, jint height, jintArray outputImage) {

    jint* input = (*env)->GetIntArrayElements(env, inputImage, NULL);
    jint* output = (*env)->GetIntArrayElements(env, outputImage, NULL);

    upscale_2x2(input, width, height, output);

    (*env)->ReleaseIntArrayElements(env, inputImage, input, 0);
    (*env)->ReleaseIntArrayElements(env, outputImage, output, 0);
}
