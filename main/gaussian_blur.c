#include "gaussian_blur.h"
#include "denoising.h"
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <stdio.h> // Include this header for perror

// Function to create Gaussian kernel
void generate_gaussian_kernel(float **kernel, int *size, float sigma) {
    int ksize = (int)ceil(6 * sigma);
    if (ksize % 2 == 0) ksize++;
    *size = ksize;

    *kernel = (float*)malloc(ksize * ksize * sizeof(float));
    if (*kernel == NULL) {
        // Handle memory allocation failure
        perror("Failed to allocate memory for Gaussian kernel");
        exit(EXIT_FAILURE);
    }

    float sum = 0.0f;
    int half = ksize / 2;
    float s2 = sigma * sigma;

    for (int y = -half; y <= half; y++) {
        for (int x = -half; x <= half; x++) {
            float value = expf(-(x * x + y * y) / (2 * s2)) / (M_PI * 2 * s2);
            (*kernel)[(y + half) * ksize + (x + half)] = value;
            sum += value;
        }
    }

    // Normalize the kernel
    for (int i = 0; i < ksize * ksize; i++) {
        (*kernel)[i] /= sum;
    }
}

// Function to apply Gaussian blur
void apply_gaussian_blur(int* pixels, int width, int height, float sigma) {
    float* gaussian_kernel;
    int kernel_size;
    generate_gaussian_kernel(&gaussian_kernel, &kernel_size, sigma);

    int half = kernel_size / 2;
    int* temp = (int*)malloc(width * height * sizeof(int));
    if (temp == NULL) {
        // Handle memory allocation failure
        perror("Failed to allocate memory for temporary pixel buffer");
        free(gaussian_kernel);
        exit(EXIT_FAILURE);
    }

    // Apply Gaussian blur
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            float sumRed = 0, sumGreen = 0, sumBlue = 0;

            for (int j = -half; j <= half; j++) {
                for (int i = -half; i <= half; i++) {
                    int ny = y + j;
                    int nx = x + i;

                    if (nx >= 0 && nx < width && ny >= 0 && ny < height) {
                        int neighborIndex = ny * width + nx;
                        int neighborPixel = pixels[neighborIndex];

                        int red = (neighborPixel >> 16) & 0xFF;
                        int green = (neighborPixel >> 8) & 0xFF;
                        int blue = neighborPixel & 0xFF;

                        float kernelValue = gaussian_kernel[(j + half) * kernel_size + (i + half)];
                        sumRed += red * kernelValue;
                        sumGreen += green * kernelValue;
                        sumBlue += blue * kernelValue;
                    }
                }
            }

            int newRed = (int)fminf(fmaxf(sumRed, 0), 255);
            int newGreen = (int)fminf(fmaxf(sumGreen, 0), 255);
            int newBlue = (int)fminf(fmaxf(sumBlue, 0), 255);

            temp[y * width + x] = (newRed << 16) | (newGreen << 8) | newBlue;
        }
    }

    // Copy the result back to the original image
    memcpy(pixels, temp, width * height * sizeof(int));

    free(temp);
    free(gaussian_kernel);
}

void apply_denoising_after_blur(int* pixels, int width, int height, float sigma, int filter_size) {
    apply_gaussian_blur(pixels, width, height, sigma);
    apply_median_filter(pixels, width, height, filter_size);
}
