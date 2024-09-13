#include "pixel_binning.h"
#include "gaussian_blur.h"
#include "smoothing.h"
#include "unsharp_masking.h"
#include "denoising.h"
#include "gamma_correction.h"

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

    // Terapkan denoising
    apply_median_filter(output, width * 2, height * 2, 3);

    // Gaussian Blur
    apply_gaussian_blur(output, width * 2, height * 2, 1.0f);  // Tambahkan nilai sigma

    // Smoothing
    apply_smoothing(output, width * 2, height * 2);

    // Gamma Correction
    apply_gamma_correction(output, width * 2, height * 2, 2.2f);  // 2.2 adalah nilai gamma umum

    // Sharpening
    apply_unsharp_masking(output, width * 2, height * 2, 1.0, 1.5);
}
