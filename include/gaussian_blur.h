#ifndef GAUSSIAN_BLUR_H
#define GAUSSIAN_BLUR_H

void apply_gaussian_blur(int* pixels, int width, int height, float sigma);
void apply_denoising_after_blur(int* pixels, int width, int height, float sigma, int filter_size);

#endif // GAUSSIAN_BLUR_H
