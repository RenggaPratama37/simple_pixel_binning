#include "supersampling.h"
#include "gaussian_blur.h"
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define SCALE_FACTOR 2

// Function to perform bicubic interpolation
int bicubic_interpolate(float x, float y, int* image, int width, int height) {
    int ix = (int)x;
    int iy = (int)y;
    float fx = x - ix;
    float fy = y - iy;

    float result = 0.0f;
    float sum_weights = 0.0f;
    float a = -0.5f;

    for (int j = -1; j <= 2; j++) {
        for (int i = -1; i <= 2; i++) {
            float dx = i - fx;
            float dy = j - fy;
            float weight_x = (dx <= 0 ? (a + 2) * dx * dx * dx - (a + 3) * dx * dx + 1
                                      : (a * dx - 5 * a) * dx * dx * dx + (2 * a - 3 * a) * dx * dx + 1);
            float weight_y = (dy <= 0 ? (a + 2) * dy * dy * dy - (a + 3) * dy * dy + 1
                                      : (a * dy - 5 * a) * dy * dy * dy + (2 * a - 3 * a) * dy * dy + 1);
            float weight = weight_x * weight_y;

            int xi = ix + i;
            int yi = iy + j;
            if (xi >= 0 && xi < width && yi >= 0 && yi < height) {
                result += weight * image[yi * width + xi];
                sum_weights += weight;
            }
        }
    }

    return (int)(result / sum_weights);
}

// Function to downsample using a box filter
void box_filter_downsample(int* high_res_image, int* low_res_image, int high_res_width, int high_res_height, int scale_factor) {
    int low_res_width = high_res_width / scale_factor;
    int low_res_height = high_res_height / scale_factor;

    for (int y = 0; y < low_res_height; y++) {
        for (int x = 0; x < low_res_width; x++) {
            int sumRed = 0, sumGreen = 0, sumBlue = 0;
            int count = 0;

            for (int ky = 0; ky < scale_factor; ky++) {
                for (int kx = 0; kx < scale_factor; kx++) {
                    int high_res_x = x * scale_factor + kx;
                    int high_res_y = y * scale_factor + ky;

                    if (high_res_x < high_res_width && high_res_y < high_res_height) {
                        int pixel = high_res_image[high_res_y * high_res_width + high_res_x];
                        sumRed += (pixel >> 16) & 0xFF;
                        sumGreen += (pixel >> 8) & 0xFF;
                        sumBlue += pixel & 0xFF;
                        count++;
                    }
                }
            }

            int avgRed = sumRed / count;
            int avgGreen = sumGreen / count;
            int avgBlue = sumBlue / count;

            low_res_image[y * low_res_width + x] = (avgRed << 16) | (avgGreen << 8) | avgBlue;
        }
    }
}

// Function to apply supersampling
void apply_supersampling(int* image, int width, int height, float sigma) {
    int high_res_width = width * SCALE_FACTOR;
    int high_res_height = height * SCALE_FACTOR;

    // Allocate memory for high resolution and low resolution images
    int* high_res_image = (int*)calloc(high_res_width * high_res_height, sizeof(int));
    int* low_res_image = (int*)malloc(width * height * sizeof(int));

    // Upscale the original image to high resolution using bicubic interpolation
    for (int y = 0; y < high_res_height; y++) {
        for (int x = 0; x < high_res_width; x++) {
            float orig_x = x / (float)SCALE_FACTOR;
            float orig_y = y / (float)SCALE_FACTOR;
            high_res_image[y * high_res_width + x] = bicubic_interpolate(orig_x, orig_y, image, width, height);
        }
    }

    // Apply Gaussian blur to the high-resolution image to smooth out edges
    apply_gaussian_blur(high_res_image, high_res_width, high_res_height, sigma);

    // Downscale the high-resolution image to the original size using box filter
    box_filter_downsample(high_res_image, low_res_image, high_res_width, high_res_height, SCALE_FACTOR);

    // Copy the downsampled image back to the original image
    memcpy(image, low_res_image, width * height * sizeof(int));

    free(high_res_image);
    free(low_res_image);
}

