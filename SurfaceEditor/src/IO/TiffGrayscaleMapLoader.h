#pragma once
#include <tiff/tiffio.h>
#include <iostream>
#include <vector>
#include <cstdint>

class TiffGrayscaleMapLoader
{
public:
	static std::vector<uint8_t> load8bit(const char* filename, uint32_t& width, uint32_t& height)
	{
		TIFF* tif = TIFFOpen(filename, "r");
		if (!tif) {
			std::cerr << "Failed to open TIFF file: " << filename << std::endl;
			return {};
		}

		uint16_t bitsPerSample = 0, samplesPerPixel = 0;
		TIFFGetField(tif, TIFFTAG_IMAGEWIDTH, &width);
		TIFFGetField(tif, TIFFTAG_IMAGELENGTH, &height);
		TIFFGetField(tif, TIFFTAG_SAMPLESPERPIXEL, &samplesPerPixel);
		TIFFGetField(tif, TIFFTAG_BITSPERSAMPLE, &bitsPerSample);

		if (samplesPerPixel != 1 || bitsPerSample != 8) {
			std::cerr << "Unsupported TIFF format (only 8-bit grayscale expected)." << std::endl;
			TIFFClose(tif);
			return {};
		}

		size_t pixelCount = width * height;
		std::vector<uint8_t> heightmap8(pixelCount);

		if (TIFFIsTiled(tif)) {
			uint32_t tileWidth, tileHeight;
			TIFFGetField(tif, TIFFTAG_TILEWIDTH, &tileWidth);
			TIFFGetField(tif, TIFFTAG_TILELENGTH, &tileHeight);

			std::vector<uint8_t> tileBuffer((tileWidth * tileHeight));

			for (uint32_t row = 0; row < height; row += tileHeight) {
				for (uint32_t col = 0; col < width; col += tileWidth) {

					if (TIFFReadTile(tif, tileBuffer.data(), col, row, 0, 0) < 0) {
						std::cerr << "Failed to read tile at (" << col << ", " << row << ")" << std::endl;
						TIFFClose(tif);
						return {};
					}

					for (uint32_t ty = 0; ty < tileHeight; ++ty) {
						for (uint32_t tx = 0; tx < tileWidth; ++tx) {
							uint32_t x = col + tx;
							uint32_t y = row + ty;

							if (x < width && y < height) {
								heightmap8[y * width + x] = tileBuffer[ty * tileWidth + tx];
							}
						}
					}
				}
			}
		}
		else {
			std::vector<uint8_t> scanline(width);

			for (uint32_t row = 0; row < height; ++row) {
				if (TIFFReadScanline(tif, scanline.data(), row) != 1) {
					std::cerr << "Failed to read row " << row << std::endl;
					TIFFClose(tif);
					return {};
				}

				for (uint32_t col = 0; col < width; ++col) {
					heightmap8[row * width + col] = scanline[col];
				}
			}
		}

		TIFFClose(tif);
		return heightmap8;
	}

	static std::vector<uint16_t> load16bit(const char* filename, uint32_t& width, uint32_t& height, float& avgPixelVal)
	{
		TIFF* tif = TIFFOpen(filename, "r");
		if (!tif) {
			std::cerr << "Failed to open TIFF file: " << filename << std::endl;
			return {};
		}

		uint16_t bitsPerSample = 0, samplesPerPixel = 0;
		TIFFGetField(tif, TIFFTAG_IMAGEWIDTH, &width);
		TIFFGetField(tif, TIFFTAG_IMAGELENGTH, &height);
		TIFFGetField(tif, TIFFTAG_SAMPLESPERPIXEL, &samplesPerPixel);
		TIFFGetField(tif, TIFFTAG_BITSPERSAMPLE, &bitsPerSample);

		if (samplesPerPixel != 1 || bitsPerSample != 16) {
			std::cerr << "Unsupported TIFF format (only 16-bit grayscale expected)." << std::endl;
			TIFFClose(tif);
			return {};
		}

		size_t pixelCount = static_cast<size_t>(width) * static_cast<size_t>(height);
		std::vector<uint16_t> heightmap16(pixelCount);

		uint64_t sumHeight = 0;

		if (TIFFIsTiled(tif)) {
			uint32_t tileWidth, tileHeight;
			TIFFGetField(tif, TIFFTAG_TILEWIDTH, &tileWidth);
			TIFFGetField(tif, TIFFTAG_TILELENGTH, &tileHeight);

			std::vector<uint16_t> tileBuffer(tileWidth * tileHeight);

			for (uint32_t row = 0; row < height; row += tileHeight) {
				for (uint32_t col = 0; col < width; col += tileWidth) {

					if (TIFFReadTile(tif, tileBuffer.data(), col, row, 0, 0) < 0) {
						std::cerr << "Failed to read tile at (" << col << ", " << row << ")" << std::endl;
						TIFFClose(tif);
						return {};
					}

					for (uint32_t ty = 0; ty < tileHeight; ++ty) {
						for (uint32_t tx = 0; tx < tileWidth; ++tx) {
							uint32_t x = col + tx;
							uint32_t y = row + ty;

							if (x < width && y < height) {
								uint16_t value = tileBuffer[ty * tileWidth + tx];
								heightmap16[y * width + x] = value;
								sumHeight += value;
							}
						}
					}
				}
			}
		}
		else {
			std::vector<uint16_t> scanline(width);

			for (uint32_t row = 0; row < height; ++row) {
				if (TIFFReadScanline(tif, scanline.data(), row) != 1) {
					std::cerr << "Failed to read row " << row << std::endl;
					TIFFClose(tif);
					return {};
				}

				for (uint32_t col = 0; col < width; ++col) {
					uint16_t value = scanline[col];
					heightmap16[row * width + col] = value;
					sumHeight += value;
				}
			}
		}

		TIFFClose(tif);

		if (pixelCount > 0)
			avgPixelVal = static_cast<float>(sumHeight) / static_cast<float>(pixelCount);
		else
			avgPixelVal = 0.0f;

		return heightmap16;
	}
};