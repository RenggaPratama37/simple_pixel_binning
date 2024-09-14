# Nama library yang akan dihasilkan
LIBRARY_NAME = libpixel_binning.so

# Direktori sumber dan header
SRC_DIR = main
INCLUDE_DIR = include

# File sumber C yang akan dikompilasi
SOURCES = $(SRC_DIR)/pixel_binning.c \
          $(SRC_DIR)/pixel_binning_jni.c \
          $(SRC_DIR)/gaussian_blur.c \
          $(SRC_DIR)/unsharp_masking.c \
          $(SRC_DIR)/smoothing.c \
          $(SRC_DIR)/denoising.c \
          $(SRC_DIR)/gamma_correction.c \
          $(SRC_DIR)/sobel_filter.c \
          $(SRC_DIR)/adaptive_sharpening.c \
          $(SRC_DIR)/feature_processing.c \
          $(SRC_DIR)/supersampling.c

# Compiler dan opsi kompilasi
CC = gcc
CFLAGS = -shared -fPIC

# Direktori Java dan header JNI
JAVA_HOME = /usr/lib/jvm/java-21-openjdk-amd64
JNI_INCLUDE = -I$(JAVA_HOME)/include -I$(JAVA_HOME)/include/linux

# Direktori header lainnya
INCLUDES = -I$(INCLUDE_DIR)

# Target utama
all: $(LIBRARY_NAME)

# Target untuk menghasilkan shared library
$(LIBRARY_NAME): $(SOURCES)
	$(CC) $(CFLAGS) $(SOURCES) -o $(LIBRARY_NAME) $(JNI_INCLUDE) $(INCLUDES)

# Target untuk membersihkan file hasil kompilasi
clean:
	rm -f $(LIBRARY_NAME)
