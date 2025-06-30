# TiffGrayscaleMapLoader

The TiffGrayscaleMapLoader class provides static methods for loading grayscale TIFF images using the libtiff library. It supports both 8-bit and 16-bit grayscale formats and handles both tiled and scanline-based image layouts. In the case of 16-bit images, it also computes the average pixel value.

###### Public Static Methods
`std::vector<uint8_t> load8bit(const char* filename, uint32_t& width, uint32_t& height)`
Loads an 8-bit grayscale TIFF image from the specified file.

Parameters:

filename: Path to the TIFF file.

width: Output parameter that receives the width of the image.

height: Output parameter that receives the height of the image.

Returns:
A flat std::vector<uint8_t> containing grayscale pixel values in row-major order. Returns an empty vector if loading fails or the format is unsupported.

`std::vector<uint16_t> load16bit(const char* filename, uint32_t& width, uint32_t& height, float& avgPixelVal)`
Loads a 16-bit grayscale TIFF image from the specified file.

Parameters:

filename: Path to the TIFF file.

width: Output parameter that receives the width of the image.

height: Output parameter that receives the height of the image.

avgPixelVal: Output parameter that receives the average grayscale value of all pixels in the image.

Returns:
A flat std::vector<uint16_t> containing 16-bit grayscale pixel values in row-major order. Returns an empty vector if loading fails or the format is unsupported.

Supported TIFF Requirements
Both methods only support grayscale TIFF images where:

The image has 1 sample per pixel

The bit depth is exactly 8 or 16 bits per sample

The layout is either tiled or scanline-based

Unsupported configurations will result in an error message and an empty return value.