# TiffGrayscaleMapLoader

The TiffGrayscaleMapLoader class provides methods for loading grayscale TIFF images using the libtiff library.

### TiffGrayscaleMapLoader class

###### Methods
`std::vector<uint8_t> load8bit(const char* filename, uint32_t& width, uint32_t& height)` - Loads an 8-bit grayscale TIFF image from the specified file. It returns a std::vector<uint8_t> containing 8-bit grayscale pixel values in row-major order.  

`std::vector<uint16_t> load16bit(const char* filename, uint32_t& width, uint32_t& height, float& avgPixelVal)` - Loads a 16-bit grayscale TIFF image from the specified file. It returns a std::vector<uint16_t> containing 16-bit grayscale pixel values in row-major order.  
