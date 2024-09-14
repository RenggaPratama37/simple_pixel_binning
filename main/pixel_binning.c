#include "pixel_binning.h"
#include "feature_processing.h"
#include <math.h>

void upscale_2x2(int* input, int width, int height, int* output) {
    int newWidth = width * 2;
    int newHeight = height * 2;

    for (int i = 0; i < height - 1; i++) {
        for (int j = 0; j < width - 1; j++) {
            // Pixel position in the output image
            int x = j * 2;
            int y = i * 2;
            
            // Get input pixel colors
            int p1 = input[i * width + j];
            int p2 = input[i * width + (j + 1)];
            int p3 = input[(i + 1) * width + j];
            int p4 = input[(i + 1) * width + (j + 1)];
            
            // Separate color components (RGB)
            int r1 = (p1 >> 16) & 0xFF;
            int g1 = (p1 >> 8) & 0xFF;
            int b1 = p1 & 0xFF;
            
            int r2 = (p2 >> 16) & 0xFF;
            int g2 = (p2 >> 8) & 0xFF;
            int b2 = p2 & 0xFF;
            
            int r3 = (p3 >> 16) & 0xFF;
            int g3 = (p3 >> 8) & 0xFF;
            int b3 = p3 & 0xFF;
            
            int r4 = (p4 >> 16) & 0xFF;
            int g4 = (p4 >> 8) & 0xFF;
            int b4 = p4 & 0xFF;
            
            // Bilinear interpolation for each new pixel
            for (int dy = 0; dy < 2; dy++) {
                for (int dx = 0; dx < 2; dx++) {
                    float wx = dx / 1.0f;
                    float wy = dy / 1.0f;

                    float r = r1 * (1 - wx) * (1 - wy) + r2 * wx * (1 - wy) + r3 * (1 - wx) * wy + r4 * wx * wy;
                    float g = g1 * (1 - wx) * (1 - wy) + g2 * wx * (1 - wy) + g3 * (1 - wx) * wy + g4 * wx * wy;
                    float b = b1 * (1 - wx) * (1 - wy) + b2 * wx * (1 - wy) + b3 * (1 - wx) * wy + b4 * wx * wy;

                    int new_r = (int)r;
                    int new_g = (int)g;
                    int new_b = (int)b;

                    int new_pixel = (new_r << 16) | (new_g << 8) | new_b;
                    output[(y + dy) * newWidth + (x + dx)] = new_pixel;
                }
            }
        }
    }

    // Handle image edge cases
    for (int i = 0; i < height; i++) {
        if (i < height - 1) {
            for (int j = 0; j < width; j++) {
                int x = j * 2;
                int y = i * 2 + 1;

                int p1 = input[i * width + j];
                int p2 = input[(i + 1) * width + j];

                int r1 = (p1 >> 16) & 0xFF;
                int g1 = (p1 >> 8) & 0xFF;
                int b1 = p1 & 0xFF;
                
                int r2 = (p2 >> 16) & 0xFF;
                int g2 = (p2 >> 8) & 0xFF;
                int b2 = p2 & 0xFF;

                for (int dx = 0; dx < 2; dx++) {
                    int new_r = (r1 + r2) / 2;
                    int new_g = (g1 + g2) / 2;
                    int new_b = (b1 + b2) / 2;

                    int new_pixel = (new_r << 16) | (new_g << 8) | new_b;
                    output[y * newWidth + (x + dx)] = new_pixel;
                }
            }
        }
    }

    for (int j = 0; j < width; j++) {
        if (j < width - 1) {
            for (int i = 0; i < height; i++) {
                int x = j * 2 + 1;
                int y = i * 2;

                int p1 = input[i * width + j];
                int p2 = input[i * width + (j + 1)];

                int r1 = (p1 >> 16) & 0xFF;
                int g1 = (p1 >> 8) & 0xFF;
                int b1 = p1 & 0xFF;
                
                int r2 = (p2 >> 16) & 0xFF;
                int g2 = (p2 >> 8) & 0xFF;
                int b2 = p2 & 0xFF;

                for (int dy = 0; dy < 2; dy++) {
                    int new_r = (r1 + r2) / 2;
                    int new_g = (g1 + g2) / 2;
                    int new_b = (b1 + b2) / 2;

                    int new_pixel = (new_r << 16) | (new_g << 8) | new_b;
                    output[(y + dy) * newWidth + x] = new_pixel;
                }
            }
        }
    }

    // Handle image corners
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            if (i == height - 1 && j == width - 1) {
                int x = j * 2;
                int y = i * 2;

                int p1 = input[i * width + j];
                int r1 = (p1 >> 16) & 0xFF;
                int g1 = (p1 >> 8) & 0xFF;
                int b1 = p1 & 0xFF;

                int new_pixel = (r1 << 16) | (g1 << 8) | b1;
                output[y * newWidth + x] = new_pixel;
                output[(y + 1) * newWidth + x] = new_pixel;
                output[y * newWidth + (x + 1)] = new_pixel;
                output[(y + 1) * newWidth + (x + 1)] = new_pixel;
            }
        }
    }
}

// upscaling 4*4
void upscale_4x4(int* input, int width, int height, int* output) {
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            int pixel = input[i * width + j];

            // Upscale 4x4: duplicate the pixel into 16
            for (int y = 0; y < 4; y++) {
                for (int x = 0; x < 4; x++) {
                    output[(i * 4 + y) * (width * 4) + (j * 4 + x)] = pixel;
                }
            }
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
