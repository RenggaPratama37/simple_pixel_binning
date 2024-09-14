#include "denoising.h"
#include <stdlib.h>
#include <math.h>

// Helper function to sort array
void sort(int* arr, int n) {
    for (int i = 0; i < n-1; i++) {
        for (int j = i+1; j < n; j++) {
            if (arr[i] > arr[j]) {
                int temp = arr[i];
                arr[i] = arr[j];
                arr[j] = temp;
            }
        }
    }
}

void apply_median_filter(int* pixels, int width, int height, int filter_size) {
    int half_size = filter_size / 2;
    int* output = (int*)malloc(width * height * sizeof(int));
    if (output == NULL) {
        // Handle memory allocation failure
        return;
    }

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            int r[filter_size * filter_size];
            int g[filter_size * filter_size];
            int b[filter_size * filter_size];
            int count = 0;

            // Collect the surrounding pixels within the filter window
            for (int ky = -half_size; ky <= half_size; ky++) {
                for (int kx = -half_size; kx <= half_size; kx++) {
                    int newY = y + ky;
                    int newX = x + kx;

                    // Boundary check
                    if (newY >= 0 && newY < height && newX >= 0 && newX < width) {
                        int pixel = pixels[newY * width + newX];

                        r[count] = (pixel >> 16) & 0xFF;
                        g[count] = (pixel >> 8) & 0xFF;
                        b[count] = pixel & 0xFF;

                        count++;
                    }
                }
            }

            // Sort the color arrays to find the median
            sort(r, count);
            sort(g, count);
            sort(b, count);

            // Set the pixel to the median value of the surrounding pixels
            int medianRed = r[count / 2];
            int medianGreen = g[count / 2];
            int medianBlue = b[count / 2];

            output[y * width + x] = (medianRed << 16) | (medianGreen << 8) | medianBlue;
        }
    }

    // Copy the filtered image back to the original array
    for (int i = 0; i < width * height; i++) {
        pixels[i] = output[i];
    }

    free(output);
}
