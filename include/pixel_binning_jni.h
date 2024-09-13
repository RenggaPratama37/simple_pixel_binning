#ifndef PIXELBINNING_H
#define PIXELBINNING_H
#include "jni.h"
#include "../include/pixel_binning.h"

#ifdef __cplusplus
extern "C" {
#endif

JNIEXPORT void JNICALL Java_PixelBinning_upscale2x2
  (JNIEnv *, jobject, jintArray, jint, jint, jintArray);

#ifdef __cplusplus
}
#endif

#endif // PIXELBINNING_H
