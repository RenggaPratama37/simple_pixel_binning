#include "adaptive_sharpening.h"
#include "gaussian_blur.h"
#include "sobel_filter.h"
#include <stdlib.h>
#include <math.h>

// Function to calculate local variance (sharpness)
void calculate_local_variance(int* pixels, int width, int height, int filter_size, float* variance) {
    int half_size = filter_size / 2;
    int* temp = (int*)malloc(width * height * sizeof(int));
    
    // Calculate variance for each pixel
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            float sum = 0;
            float mean = 0;
            int count = 0;
            
            // Collect the surrounding pixels
            for (int ky = -half_size; ky <= half_size; ky++) {
                for (int kx = -half_size; kx <= half_size; kx++) {
                    int newY = y + ky;
                    int newX = x + kx;

                    if (newY >= 0 && newY < height && newX >= 0 && newX < width) {
                        int pixel = pixels[newY * width + newX];
                        float value = (pixel >> 16) & 0xFF; // Red channel for simplicity
                        sum += value;
                        count++;
                    }
                }
            }
            
            mean = sum / count;
            float variance_sum = 0;
            for (int ky = -half_size; ky <= half_size; ky++) {
                for (int kx = -half_size; kx <= half_size; kx++) {
                    int newY = y + ky;
                    int newX = x + kx;

                    if (newY >= 0 && newY < height && newX >= 0 && newX < width) {
                        int pixel = pixels[newY * width + newX];
                        float value = (pixel >> 16) & 0xFF; // Red channel for simplicity
                        variance_sum += (value - mean) * (value - mean);
                    }
                }
            }
            
            variance[y * width + x] = sqrtf(variance_sum / count);
        }
    }
    
    free(temp);
}

// Function to apply adaptive sharpening
void apply_adaptive_sharpening(int* pixels, int width, int height, float sigma, int filter_size) {
    float* variance = (float*)malloc(width * height * sizeof(float));
    
    // Calculate local variance
    calculate_local_variance(pixels, width, height, filter_size, variance);
    
    // Apply Gaussian blur
    apply_gaussian_blur(pixels, width, height, sigma);
    
    // Apply sharpening based on local variance
    int* sharpened_pixels = (int*)malloc(width * height * sizeof(int));
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            int pixel = pixels[y * width + x];
            float local_variance = variance[y * width + x];
            float sharpening_factor = fminf(fmaxf(local_variance / 50.0f, 1.0f), 3.0f); // Example scaling
            
            // Simple sharpening adjustment (you can customize this)
            int red = (pixel >> 16) & 0xFF;
            int green = (pixel >> 8) & 0xFF;
            int blue = pixel & 0xFF;
            
            red = (int)fminf(fmaxf(red * sharpening_factor, 0), 255);
            green = (int)fminf(fmaxf(green * sharpening_factor, 0), 255);
            blue = (int)fminf(fmaxf(blue * sharpening_factor, 0), 255);
            
            sharpened_pixels[y * width + x] = (red << 16) | (green << 8) | blue;
        }
    }
    
    // Copy the sharpened image back
    for (int i = 0; i < width * height; i++) {
        pixels[i] = sharpened_pixels[i];
    }
    
    free(variance);
    free(sharpened_pixels);
}
