#ifndef PIXEL_BINNING_H
#define PIXEL_BINNING_H

#ifdef __cplusplus
extern "C" {
#endif

void upscale_2x2(int* input_image, int width, int height, int* output_image);
void upscale_4x4(int* input_image, int width, int height, int* output_image);

#ifdef __cplusplus
}
#endif

#endif // PIXEL_BINNING_H
