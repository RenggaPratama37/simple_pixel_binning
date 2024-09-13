#include "smoothing.h"
#include <stdlib.h>
#include <stdio.h>

// Fungsi untuk smoothing sederhana setelah Gaussian blur
void apply_smoothing(int* pixels, int width, int height) {
    int* temp = (int*)malloc(width * height * sizeof(int));
    if (temp == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return;
    }

    // Definisikan offset untuk 8 arah (kiri, kanan, atas, bawah, dan diagonal)
    int offsets[8][2] = {{0, -1}, {0, 1}, {-1, 0}, {1, 0}, 
                         {-1, -1}, {-1, 1}, {1, -1}, {1, 1}};

    // Variabel untuk menyimpan rata-rata warna
    float* sumRed = (float*)malloc(width * height * sizeof(float));
    float* sumGreen = (float*)malloc(width * height * sizeof(float));
    float* sumBlue = (float*)malloc(width * height * sizeof(float));
    if (sumRed == NULL || sumGreen == NULL || sumBlue == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        free(temp);
        return;
    }

    // Hitung rata-rata warna untuk setiap piksel
    for (int y = 1; y < height - 1; y++) {
        for (int x = 1; x < width - 1; x++) {
            float totalRed = 0, totalGreen = 0, totalBlue = 0;
            int count = 0;

            for (int i = 0; i < 8; i++) {
                int offsetX = offsets[i][0];
                int offsetY = offsets[i][1];
                int neighborX = x + offsetX;
                int neighborY = y + offsetY;

                if (neighborX >= 0 && neighborX < width && neighborY >= 0 && neighborY < height) {
                    int neighborIndex = neighborY * width + neighborX;
                    int neighborPixel = pixels[neighborIndex];

                    // Ambil komponen warna RGB dari piksel tetangga
                    int red = (neighborPixel >> 16) & 0xFF;
                    int green = (neighborPixel >> 8) & 0xFF;
                    int blue = neighborPixel & 0xFF;

                    totalRed += red;
                    totalGreen += green;
                    totalBlue += blue;
                    count++;
                }
            }

            // Hitung rata-rata warna tetangga
            sumRed[y * width + x] = totalRed / count;
            sumGreen[y * width + x] = totalGreen / count;
            sumBlue[y * width + x] = totalBlue / count;
        }
    }

    // Gabungkan rata-rata warna menjadi satu nilai piksel
    for (int y = 1; y < height - 1; y++) {
        for (int x = 1; x < width - 1; x++) {
            int avgRed = (int)(sumRed[y * width + x]);
            int avgGreen = (int)(sumGreen[y * width + x]);
            int avgBlue = (int)(sumBlue[y * width + x]);

            // Gabungkan warna menjadi satu nilai piksel
            temp[y * width + x] = (avgRed << 16) | (avgGreen << 8) | avgBlue;
        }
    }

    // Salin hasil smoothing kembali ke array asli
    for (int y = 1; y < height - 1; y++) {
        for (int x = 1; x < width - 1; x++) {
            pixels[y * width + x] = temp[y * width + x];
        }
    }

    free(temp);
    free(sumRed);
    free(sumGreen);
    free(sumBlue);
}
