#define STB_IMAGE_IMPLEMENTAYION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#ifndef IMAGE_H
#define IMAGE_H
#include <stdint.h>
#include <cstdio>

#define STEG_HEADER_SIZE sizeof(uint8_t) * 8	//size of our header value * 8

enum ImageType{
	PNG, JPG, BMP, TGA
};

struct Image{
	//basics : 
	uint8_t* data = NULL;	//one byte
	size_t size = 0;
	int w;	//width
	int h;	//height
	int channels;	//how many color values in a pixel
	
	Image(const char* filename);
	Image(int w, int h, int channels);
	Image(const Image& img);	//copy cunstructor
	~Image();	//image destructor
	
	bool read(const char* filename);
	bool write(const char* filename);
	
	ImageType getFileType(const char* filename);
	
	//grayScale : 
	Image& grayscale_avg();	//gray scale average
	Image& grayscale_lum();	//gray scale weighted average
	
	//colorMask
	Image& colorMask(float r, float g, float b);
	
	//steganography(hiding secret message in images)
	Image& encodeMessage(const char* message);
	Image& decodeMessage(char* buffer, size_t* messageLength);
	
	//Diffmap :
	Image& diffmap(Image& img);
	Image& diffmap_scale(Image& img, uint8_t scl = 0);
	
	//basic convolution : 
	Image& std_convolve_clamp_to_0(uint8_t channel, uint32_t ker_w, uint32_t ker_h, double ker[], uint32_t cr, uint32_t cc);	
		//cr is center row and cc is center column
	
	
};

#endif
