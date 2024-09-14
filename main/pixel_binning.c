#include "pixel_binning.h"
#include "feature_processing.h"

// upscaling 2*2
void upscale_2x2(int* input, int width, int height, int* output) {
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            int pixel = input[i * width + j];

            // Pisahkan komponen warna (RGB)
            int red = (pixel >> 16) & 0xFF;
            int green = (pixel >> 8) & 0xFF;
            int blue = pixel & 0xFF;

            // Upscale 2x2
            int new_pixel = (red << 16) | (green << 8) | blue;
            output[(i * 2) * (width * 2) + (j * 2)] = new_pixel;
            output[(i * 2) * (width * 2) + (j * 2 + 1)] = new_pixel;
            output[(i * 2 + 1) * (width * 2) + (j * 2)] = new_pixel;
            output[(i * 2 + 1) * (width * 2) + (j * 2 + 1)] = new_pixel;
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
