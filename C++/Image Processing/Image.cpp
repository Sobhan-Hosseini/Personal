#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#define BYTE_BOUND(value) value < 0 ? 0 : (value > 255 ? 255 : value)
#include "Image.h"
#include "stb_image.h"
#include "stb_image_write.h"

//basics : 
Image::Image(const char* filename){
	if(read(filename)){
		printf("Read %s\n" , filename);
		size = w * h * channels;
	}else{
		printf("Faile to read %s\n" , filename);
	}
}

Image::Image(int w, int h, int channels) : w(w), h(h), channels(channels){
	size = w * h * channels;
	data = new uint8_t[size];
}

Image::Image(const Image& img) : Image(img.w , img.h , img.channels){
	memcpy(data, img.data , size);
}

Image::~Image(){
	stbi_image_free(data);
}
	
bool Image::read(const char* filename){
	data = stbi_load(filename, &w, &h, &channels, 0);
	return data != NULL;
}

bool Image::write(const char* filename){
	ImageType type = getFileType(filename);
	int success;
	switch(type){
		case PNG:
			success = stbi_write_png(filename, w, h, channels, data, w * channels);
			break;
		case JPG:
			success = stbi_write_png(filename, w, h, channels, data,100);
			break;
		case BMP:
			success = stbi_write_png(filename, w, h, channels, data, w * channels);	//highest quality
			break;
		case TGA:
			success = stbi_write_png(filename, w, h, channels, data, w * channels);
			break;
		default: 
        	success = 0; 
	}
	return success != 0;
}

ImageType Image::getFileType(const char* filename){
	
	const char* ext = strrchr(filename, '.');	//extention
	if(ext != nullptr){
		if(strcmp(ext, ".png") == 0){
			return PNG;
		}else if(strcmp(ext, ".jpg") == 0){
			return JPG;
		}else if(strcmp(ext, ".bmp") == 0){
			return BMP;
		}else if(strcmp(ext, ".tga") == 0){
			return TGA;
		}
	}
	return PNG;
}

//gray scale : 
Image& Image::grayscale_avg(){
	//(r + g + b) / 3 --> average
	if(channels < 3){
		printf("Image %p has less than 3 channels, it is assumed to already be grayscale.", this);
	}else{
		for(int i = 0; i < size; i += channels){
			int gray = (data[i] + data[i + 1] + data[i + 2]) / 3;
			memset(data + i, gray, 3);	//set each of the channels to their gray value
		}
	}
	return *this;
}

Image& Image::grayscale_lum(){
	//-->weighted average
	if(channels < 3){
		printf("Image %p has less than 3 channels, it is assumed to already be grayscale.", this);
	}else{
		for(int i = 0; i < size; i += channels){
			int gray = (0.2126 * data[i] + 0.7152 * data[i + 1] + 0.0722 * data[i + 2]) / 3;	//equation from wikipedia
			memset(data + i, gray, 3);	//set each of the channels to their gray value
		}
	}
	return *this;
}

//colorMask :  
Image& Image::colorMask(float r, float g, float b){
	if(channels < 3){
		printf("\e[31m[ERROR] Color mask requiers at least 3 channels, but this image has %d channles\e[0m\n", channels);
	}else{
	    for(int i = 0; i < size; i += channels){
			data[i] += r;
			data[i + 1] += g;
			data[i + 2] += b;	
		}
	}
	return *this;
}

//steganography(hiding secret message in images) ?! : 
Image& Image::encodeMessage(const char* message){
	uint32_t len = strlen(message) * sizeof(char);	//length of message
	if(len * STEG_HEADER_SIZE > size){
		printf("\e[31m[ERROR] This message is too large (%lu bits / %zu bits)\e[0m\n", len + STEG_HEADER_SIZE, size);
	}
	
	for(uint8_t i = 0; i < STEG_HEADER_SIZE; ++i){
	/*set the least significant bit of every byte in our
 	image data to the corresponding bit in the message 
 	that we are trying to encode.*/
 		printf("LENGTH: %d\n", len);
 		data[i] &= 0xFE;
		data[i] |= (len >> (STEG_HEADER_SIZE - 1 - i)) & 1UL;	
		/*taking the length and shift
		 it over "i" bits so that you
		  are basically getting the ith
		   bit of length and then we are
		    ending that with one to clear
			 all of the upper bits so that
			  we just have a one or zero.*/	
	}
	
	for(uint32_t i = 0; i < len; ++i){
		printf("LENGTH: %d\n", len);
 		data[i + STEG_HEADER_SIZE] &= 0xFE;
		data[i + STEG_HEADER_SIZE] |= (message[i / 8] >> ((len - 1 - i) % 8)) & 1;
	}
	return *this;	
}

Image& Image::decodeMessage(char* buffer, size_t* messageLength){
	uint32_t len = 0;
	for(uint8_t i = 0; i < STEG_HEADER_SIZE; ++i){
		len = (len << 1) | (data[i] & 1);
	}
	
	*messageLength = len / 8;
	
	for(uint32_t i = 0; i < len; ++i){
		buffer[i] = (buffer[i / 8] << 1) | (data[i + STEG_HEADER_SIZE] & 1);
	}
	printf("LENGTH: %d\n", len);
	return *this;
}

//Diffmap : 
Image& Image::diffmap(Image& img){
	int compare_width = fmin(w,img.w);	//how much are we going to compare between these two images
	int compare_height = fmin(h,img.h);
	int compare_channels = fmin(channels,img.channels);
	for(uint32_t i = 0; i < compare_height; ++i){
		for(uint32_t j = 0; j < compare_width; ++j){
			for(uint8_t k = 0; k < compare_channels; ++k){
				data[(i * w + j) * channels + k] = BYTE_BOUND(abs(data[(i * w + j) * channels + k] - img.data[(i * img.w + j) * img.channels + k]));
				//diffrence between data already at that pixel and data of the same pixel in the other image.
			}
		}
	}
	return *this;
}

//Diffmap scale : 
Image& Image::diffmap_scale(Image& img, uint8_t scl){
	int compare_width = fmin(w,img.w);	//how much are we going to compare between these two images
	int compare_height = fmin(h,img.h);
	int compare_channels = fmin(channels,img.channels);
	uint8_t largest = 0;
	for(uint32_t i = 0; i < compare_height; ++i){
		for(uint32_t j = 0; j < compare_width; ++j){
			for(uint8_t k = 0; k < compare_channels; ++k){
				data[(i * w + j) * channels + k] = BYTE_BOUND(abs(data[(i * w + j) * channels + k] - img.data[(i * img.w + j) * img.channels + k]));
				//diffrence between data already at that pixel and data of the same pixel in the other image.
				largest = fmax(largest, data[(i * w + j) * channels + k]);
			}
		}
	}
	scl = 255 / fmax(1, fmax(scl, largest));
	for(int i = 0; i < size; ++i){
		data[i] *= scl;
	}
	return *this;
}

//basic convolution : 
Image& Image::std_convolve_clamp_to_0(uint8_t channel, uint32_t ker_w, uint32_t ker_h, double ker[], uint32_t cr, uint32_t cc) {
	uint8_t new_data[w*h];
	uint64_t center = cr*ker_w + cc;
	for(uint64_t k=channel; k<size; k+=channels) {
		double c = 0;
		for(long i = -((long)cr); i<(long)ker_h-cr; ++i) {
			long row = ((long)k/channels)/w-i;
			if(row < 0 || row > h-1) {
				continue;
			}
			for(long j = -((long)cc); j<(long)ker_w-cc; ++j) {
				long col = ((long)k/channels)%w-j;
				if(col < 0 || col > w-1) {
					continue;
				}
				c += ker[center+i*(long)ker_w+j]*data[(row*w+col)*channels+channel];
			}
		}
		new_data[k/channels] = (uint8_t)BYTE_BOUND(round(c));
	}
	for(uint64_t k=channel; k<size; k+=channels) {
		data[k] = new_data[k/channels];
	}
	return *this;
}


//Image& Image::std_convolve_clamp_to_0(uint8_t channel, uint32_t ker_w, uint32_t ker_h, double ker[], uint32_t cr, uint32_t cc){
//	uint8_t new_data[w * h];
//	uint64_t center = cr * ker_w + cc;
//	for(uint64_t k = channel; k < size; k += channels){
//		double c = 0;	//hold the sum
//		for(long i = -((long)cr); i < (long)ker_h - cr; ++i){
//			long row = ((long)k / channels) / w - i;
//			if(row < 0 || row > h - 1){
//				continue;
//			}
//			for(long j = -((long)cc); j < (long)ker_w - cc; ++j){
//				long col = ((long)k / channels) % w - j;
//				if(col < 0 || col > w - 1){
//					continue;
//				}
//				c += ker[center + i * (long)ker_w + j] * data[(row * w + col) * channels + channel];
//			}
//		}
//		new_data[k / channels] = (uint8_t)BYTE_BOUND(round(c));
//	}
//	for(uint64_t k = channel; k < size; k += channels){
//		data[k] = new_data[k / channels];
//	}
//	return *this;
//}
