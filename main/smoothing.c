#include "smoothing.h"
#include <stdlib.h>

// Fungsi untuk smoothing sederhana setelah Gaussian blur
void apply_smoothing(int* pixels, int width, int height) {
    int* temp = (int*)malloc(width * height * sizeof(int));

    for (int y = 1; y < height - 1; y++) {
        for (int x = 1; x < width - 1; x++) {
            float sumRed = 0, sumGreen = 0, sumBlue = 0;
            int count = 0;

            // Mengambil rata-rata dari piksel tetangga terdekat (4 arah: kiri, kanan, atas, bawah)
            int offsets[4][2] = {{0, -1}, {0, 1}, {-1, 0}, {1, 0}};
            
            for (int i = 0; i < 4; i++) {
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

                    sumRed += red;
                    sumGreen += green;
                    sumBlue += blue;
                    count++;
                }
            }

            // Hitung rata-rata warna tetangga
            int avgRed = (int)(sumRed / count);
            int avgGreen = (int)(sumGreen / count);
            int avgBlue = (int)(sumBlue / count);

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
}
