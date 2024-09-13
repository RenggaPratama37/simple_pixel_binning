#include "gamma_correction.h"
#include <math.h>

// Fungsi untuk mengkoreksi gamma
void apply_gamma_correction(int* pixels, int width, int height, float gamma) {
    for (int i = 0; i < width * height; i++) {
        int pixel = pixels[i];

        // Pisahkan komponen RGB
        int red = (pixel >> 16) & 0xFF;
        int green = (pixel >> 8) & 0xFF;
        int blue = pixel & 0xFF;

        // Normalisasi nilai piksel (𝑓 + 0.5) / 256
        float redNorm = (red + 0.5f) / 256.0f;
        float greenNorm = (green + 0.5f) / 256.0f;
        float blueNorm = (blue + 0.5f) / 256.0f;

        // Terapkan rumus gamma correction (warna^gamma)
        redNorm = powf(redNorm, gamma);
        greenNorm = powf(greenNorm, gamma);
        blueNorm = powf(blueNorm, gamma);

        // Denormalisasi nilai yang dikoreksi ke rentang 0 - 255 (s * 256 - 0.5)
        int newRed = (int)(redNorm * 256.0f - 0.5f);
        int newGreen = (int)(greenNorm * 256.0f - 0.5f);
        int newBlue = (int)(blueNorm * 256.0f - 0.5f);

        // Pastikan nilai tetap berada di dalam rentang 0-255
        if (newRed > 255) newRed = 255;
        if (newGreen > 255) newGreen = 255;
        if (newBlue > 255) newBlue = 255;
        if (newRed < 0) newRed = 0;
        if (newGreen < 0) newGreen = 0;
        if (newBlue < 0) newBlue = 0;

        // Terapkan inverse gamma correction untuk menjaga warna tetap konsisten
        redNorm = (newRed + 0.5f) / 256.0f;
        greenNorm = (newGreen + 0.5f) / 256.0f;
        blueNorm = (newBlue + 0.5f) / 256.0f;

        // Inverse gamma correction (warna^(1/gamma))
        float invGamma = 1.0f / gamma;
        redNorm = powf(redNorm, invGamma);
        greenNorm = powf(greenNorm, invGamma);
        blueNorm = powf(blueNorm, invGamma);

        // Kembalikan nilai ke rentang 0 - 255
        newRed = (int)(redNorm * 256.0f - 0.5f);
        newGreen = (int)(greenNorm * 256.0f - 0.5f);
        newBlue = (int)(blueNorm * 256.0f - 0.5f);

        // Pastikan kembali berada di dalam rentang 0-255
        if (newRed > 255) newRed = 255;
        if (newGreen > 255) newGreen = 255;
        if (newBlue > 255) newBlue = 255;
        if (newRed < 0) newRed = 0;
        if (newGreen < 0) newGreen = 0;
        if (newBlue < 0) newBlue = 0;

        // Gabungkan kembali nilai RGB yang telah dikoreksi
        pixels[i] = (newRed << 16) | (newGreen << 8) | newBlue;
    }
}
