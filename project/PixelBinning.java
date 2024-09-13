package project;

import java.awt.image.BufferedImage;
import java.io.File;
import java.io.IOException;
import javax.imageio.ImageIO;
import java.nio.file.Path;
import java.nio.file.Paths;

public class PixelBinning {
    static {
        System.loadLibrary("pixel_binning");
    }

    public native void upscale2x2(int[] inputImage, int width, int height, int[] outputImage);

    public static void main(String[] args) {
        try {
            // Mendapatkan direktori kerja saat ini
            Path currentRelativePath = Paths.get("");
            String currentDirectory = currentRelativePath.toAbsolutePath().toString();
            System.out.println("Current Directory: " + currentDirectory);

            // Mengatur path untuk file input dan output
            File inputFile = new File(currentDirectory + "/project/1726030621336.jpg");
            System.out.println("Input File Path: " + inputFile.getAbsolutePath());
            BufferedImage inputImage = ImageIO.read(inputFile);

            int width = inputImage.getWidth();
            int height = inputImage.getHeight();
            int[] pixels = new int[width * height];

            inputImage.getRGB(0, 0, width, height, pixels, 0, width);

            int[] upscaledPixels = new int[width * height * 4];

            new PixelBinning().upscale2x2(pixels, width, height, upscaledPixels);

            BufferedImage outputImage = new BufferedImage(width * 2, height * 2, BufferedImage.TYPE_INT_RGB);
            outputImage.setRGB(0, 0, width * 2, height * 2, upscaledPixels, 0, width * 2);

            File outputFile = new File(currentDirectory + "/project/1726030621336_1.jpg");
            System.out.println("Output File Path: " + outputFile.getAbsolutePath());
            ImageIO.write(outputImage, "jpg", outputFile);

            System.out.println("Upscaling berhasil! Gambar disimpan sebagai 1726030621336_1.jpg");
        } catch (IOException e) {
            e.printStackTrace();
        }
    }
}
