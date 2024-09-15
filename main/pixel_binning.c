#include "pixel_binning.h"
#include "feature_processing.h"
#include <math.h>
#include <stdio.h>

// Helper function to clamp values between 0 and 255
static inline int clamp(int value) {
    return value < 0 ? 0 : (value > 255 ? 255 : value);
}

// Function to perform bilinear interpolation for one pixel
static inline void bilinear_interpolation(int* input, int width, int height, int x, int y, int newWidth, int newHeight, int* output) {
    // Determine the coordinates of the four surrounding pixels in the input image
    int x0 = x / 2;
    int y0 = y / 2;
    int x1 = x0 + 1;
    int y1 = y0 + 1;

    // Ensure coordinates are within bounds
    if (x1 >= width) x1 = x0;
    if (y1 >= height) y1 = y0;

    // Retrieve the four surrounding pixel values
    int p00 = input[y0 * width + x0];
    int p10 = input[y0 * width + x1];
    int p01 = input[y1 * width + x0];
    int p11 = input[y1 * width + x1];

    // Extract RGB components for each pixel
    int r00 = (p00 >> 16) & 0xFF;
    int g00 = (p00 >> 8) & 0xFF;
    int b00 = p00 & 0xFF;

    int r10 = (p10 >> 16) & 0xFF;
    int g10 = (p10 >> 8) & 0xFF;
    int b10 = p10 & 0xFF;

    int r01 = (p01 >> 16) & 0xFF;
    int g01 = (p01 >> 8) & 0xFF;
    int b01 = p01 & 0xFF;

    int r11 = (p11 >> 16) & 0xFF;
    int g11 = (p11 >> 8) & 0xFF;
    int b11 = p11 & 0xFF;

    // Calculate fractional part of the coordinates
    float dx = (x % 2) / 1.0f;
    float dy = (y % 2) / 1.0f;

    // Perform bilinear interpolation for each color component
    float r = r00 * (1 - dx) * (1 - dy) + r10 * dx * (1 - dy) + r01 * (1 - dx) * dy + r11 * dx * dy;
    float g = g00 * (1 - dx) * (1 - dy) + g10 * dx * (1 - dy) + g01 * (1 - dx) * dy + g11 * dx * dy;
    float b = b00 * (1 - dx) * (1 - dy) + b10 * dx * (1 - dy) + b01 * (1 - dx) * dy + b11 * dx * dy;

    // Clamp values to ensure they are within 0-255 range
    int new_r = clamp((int)r);
    int new_g = clamp((int)g);
    int new_b = clamp((int)b);

    // Combine RGB components into a single pixel value
    int new_pixel = (new_r << 16) | (new_g << 8) | new_b;

    // Store the new pixel value in the output image
    output[y * newWidth + x] = new_pixel;
}

// Function to upscale the image by a factor of 2x2
void upscale_2x2(int* input, int width, int height, int* output) {
    int newWidth = width * 2;
    int newHeight = height * 2;

    // Perform bilinear interpolation for each pixel in the output image
    for (int y = 0; y < newHeight; y++) {
        for (int x = 0; x < newWidth; x++) {
            bilinear_interpolation(input, width, height, x, y, newWidth, newHeight, output);
        }
    }
}

// Main function to merge upscale and filter
void process_image(int* input, int width, int height, int* output) {
    // Upscaling
    upscale_2x2(input, width, height, output);

    // Apply feature 
    apply_features_after_upscale(output, width * 2, height * 2);
}

