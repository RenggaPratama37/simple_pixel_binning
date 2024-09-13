#include "pixel_binning.h"

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
