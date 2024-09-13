#include "unsharp_masking.h"
#include "gaussian_blur.h"
#include <stdlib.h>
#include <math.h>

// Helper function to calculate the difference between original and blurred image
void calculate_sharpening_diff(int* original, int* blurred, int* output, int width, int height) {
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            int index = y * width + x;

            // Extract RGB components from the original and blurred pixels
            int origPixel = original[index];
            int blurPixel = blurred[index];

            int origRed = (origPixel >> 16) & 0xFF;
            int origGreen = (origPixel >> 8) & 0xFF;
            int origBlue = origPixel & 0xFF;

            int blurRed = (blurPixel >> 16) & 0xFF;
            int blurGreen = (blurPixel >> 8) & 0xFF;
            int blurBlue = blurPixel & 0xFF;

            // Compute the difference
            int diffRed = origRed - blurRed;
            int diffGreen = origGreen - blurGreen;
            int diffBlue = origBlue - blurBlue;

            // Enhance the difference
            int newRed = origRed + (int)(diffRed * 1.5);
            int newGreen = origGreen + (int)(diffGreen * 1.5);
            int newBlue = origBlue + (int)(diffBlue * 1.5);

            // Clamp values to be within the 0-255 range
            newRed = (int)fmaxf(0, fminf(255, newRed));
            newGreen = (int)fmaxf(0, fminf(255, newGreen));
            newBlue = (int)fmaxf(0, fminf(255, newBlue));

            // Combine RGB components into one pixel
            output[index] = (newRed << 16) | (newGreen << 8) | newBlue;
        }
    }
}

void apply_unsharp_masking(int* pixels, int width, int height, float sigma, float amount) {
    int* blurred = (int*)malloc(width * height * sizeof(int));
    int* sharpened = (int*)malloc(width * height * sizeof(int));

    // Apply Gaussian blur to create the blurred image
    apply_gaussian_blur(pixels, width, height);

    // Calculate the difference between the original and blurred image
    calculate_sharpening_diff(pixels, blurred, sharpened, width, height);

    // Copy the sharpened image back to the original array
    for (int i = 0; i < width * height; i++) {
        pixels[i] = sharpened[i];
    }

    free(blurred);
    free(sharpened);
}
