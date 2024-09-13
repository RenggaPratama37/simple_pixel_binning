#include "gaussian_blur.h"
#include <stdlib.h>

// Kernel Gaussian 3x3
const float gaussian_kernel[3][3] = {
    {1.0 / 16, 2.0 / 16, 1.0 / 16},
    {2.0 / 16, 4.0 / 16, 2.0 / 16},
    {1.0 / 16, 2.0 / 16, 1.0 / 16}
};

void apply_gaussian_blur(int* pixels, int width, int height) {
    int* temp = (int*)malloc(width * height * sizeof(int));

    for (int y = 1; y < height - 1; y++) {
        for (int x = 1; x < width - 1; x++) {
            float sumRed = 0, sumGreen = 0, sumBlue = 0;

            // Apply Gaussian kernel to the current pixel and its neighbors
            for (int j = -1; j <= 1; j++) {
                for (int i = -1; i <= 1; i++) {
                    int neighborIndex = (y + j) * width + (x + i);
                    int neighborPixel = pixels[neighborIndex];

                    // Extract RGB components from the neighbor pixel
                    int red = (neighborPixel >> 16) & 0xFF;
                    int green = (neighborPixel >> 8) & 0xFF;
                    int blue = neighborPixel & 0xFF;

                    // Apply Gaussian kernel weight
                    float kernelValue = gaussian_kernel[j + 1][i + 1];
                    sumRed += red * kernelValue;
                    sumGreen += green * kernelValue;
                    sumBlue += blue * kernelValue;
                }
            }

            // Set the blurred pixel back
            int newRed = (int)sumRed;
            int newGreen = (int)sumGreen;
            int newBlue = (int)sumBlue;

            // Recombine RGB components into one pixel
            temp[y * width + x] = (newRed << 16) | (newGreen << 8) | newBlue;
        }
    }

    // Copy the blurred pixels back to the original array
    for (int y = 1; y < height - 1; y++) {
        for (int x = 1; x < width - 1; x++) {
            pixels[y * width + x] = temp[y * width + x];
        }
    }

    free(temp);
}
