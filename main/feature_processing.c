#include "feature_processing.h"
#include "gaussian_blur.h"
#include "smoothing.h"
#include "unsharp_masking.h"
#include "denoising.h"
#include "gamma_correction.h"
#include "sobel_filter.h"
#include "adaptive_sharpening.h"

// Fungsi untuk menerapkan fitur setelah upscaling
void apply_features_after_upscale(int* image, int width, int height) {
    // Terapkan denoising
    apply_median_filter(image, width, height, 3);

    // Gaussian Blur
    apply_gaussian_blur(image, width, height, 1.0f);  // Tambahkan nilai sigma

    // Smoothing
    apply_smoothing(image, width, height);

    // Gamma Correction
    apply_gamma_correction(image, width, height, 2.2f);

    // Sharpening
    apply_unsharp_masking(image, width, height, 1.0, 1.5);

    // Sobel Filter
    sharpen_image_with_sobel(image, width, height);

    // Adaptive Sharpening
    apply_adaptive_sharpening(image, width, height, 1.0f, 3);
}
