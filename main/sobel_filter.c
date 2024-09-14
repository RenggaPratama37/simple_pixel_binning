#include "sobel_filter.h"
#include <stdlib.h>
#include <math.h>

// Faktor peningkatan untuk penajaman
#define SHARPENING_FACTOR 0.40f

void apply_sobel_filter(int* pixels, int width, int height) {
    int* temp = (int*)malloc(width * height * sizeof(int));
    if (temp == NULL) {
        return;
    }

    // Sobel kernel untuk sumbu X dan Y
    int sobelX[3][3] = {
        {-1, 0, 1},
        {-2, 0, 2},
        {-1, 0, 1}
    };
    int sobelY[3][3] = {
        {-1, -2, -1},
        { 0,  0,  0},
        { 1,  2,  1}
    };

    for (int y = 1; y < height - 1; y++) {
        for (int x = 1; x < width - 1; x++) {
            float sumXRed = 0, sumXGreen = 0, sumXBlue = 0;
            float sumYRed = 0, sumYGreen = 0, sumYBlue = 0;

            // Menerapkan kernel Sobel
            for (int j = -1; j <= 1; j++) {
                for (int i = -1; i <= 1; i++) {
                    int pixelIndex = (y + j) * width + (x + i);
                    int pixel = pixels[pixelIndex];

                    int red = (pixel >> 16) & 0xFF;
                    int green = (pixel >> 8) & 0xFF;
                    int blue = pixel & 0xFF;

                    sumXRed += red * sobelX[j + 1][i + 1];
                    sumXGreen += green * sobelX[j + 1][i + 1];
                    sumXBlue += blue * sobelX[j + 1][i + 1];

                    sumYRed += red * sobelY[j + 1][i + 1];
                    sumYGreen += green * sobelY[j + 1][i + 1];
                    sumYBlue += blue * sobelY[j + 1][i + 1];
                }
            }

            // Menghitung magnitude dari gradien Sobel
            int newRed = (int)fminf(fmaxf(sqrtf(sumXRed * sumXRed + sumYRed * sumYRed), 0), 255);
            int newGreen = (int)fminf(fmaxf(sqrtf(sumXGreen * sumXGreen + sumYGreen * sumYGreen), 0), 255);
            int newBlue = (int)fminf(fmaxf(sqrtf(sumXBlue * sumXBlue + sumYBlue * sumYBlue), 0), 255);

            temp[y * width + x] = (newRed << 16) | (newGreen << 8) | newBlue;
        }
    }

    // Salin hasil Sobel ke array piksel asli
    for (int y = 1; y < height - 1; y++) {
        for (int x = 1; x < width - 1; x++) {
            pixels[y * width + x] = temp[y * width + x];
        }
    }

    free(temp);
}

// Fungsi untuk menajamkan gambar dengan Sobel
void sharpen_image_with_sobel(int* pixels, int width, int height) {
    int* sobelImage = (int*)malloc(width * height * sizeof(int));
    if (sobelImage == NULL) {
        return;
    }

    // Salin gambar asli untuk menerapkan Sobel
    for (int i = 0; i < width * height; i++) {
        sobelImage[i] = pixels[i];
    }

    // Terapkan Sobel filter pada salinan gambar
    apply_sobel_filter(sobelImage, width, height);

    // Tambahkan hasil Sobel ke gambar asli dengan faktor peningkatan
    for (int i = 0; i < width * height; i++) {
        int originalPixel = pixels[i];
        int sobelPixel = sobelImage[i];

        int redOriginal = (originalPixel >> 16) & 0xFF;
        int greenOriginal = (originalPixel >> 8) & 0xFF;
        int blueOriginal = originalPixel & 0xFF;

        int redSobel = (sobelPixel >> 16) & 0xFF;
        int greenSobel = (sobelPixel >> 8) & 0xFF;
        int blueSobel = sobelPixel & 0xFF;

        // Tambahkan nilai Sobel ke piksel asli dengan faktor peningkatan
        int newRed = (int)fminf(fmaxf(redOriginal + SHARPENING_FACTOR * redSobel, 0), 255);
        int newGreen = (int)fminf(fmaxf(greenOriginal + SHARPENING_FACTOR * greenSobel, 0), 255);
        int newBlue = (int)fminf(fmaxf(blueOriginal + SHARPENING_FACTOR * blueSobel, 0), 255);

        pixels[i] = (newRed << 16) | (newGreen << 8) | newBlue;
    }

    free(sobelImage);
}
