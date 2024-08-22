/*
  Image Processing with C++
  Author: Seyed Sobhan Hosseini
  Date: Wednesday, July 10, 2024, 12:20:34 PM
*/

#include <iostream>
#include "Image.h"

int main(int args, char** argv){
	Image test("test.jpg");
	
	double ker[] = {1/16.0, 2/16.0, 1/16.0,
					2/16.0, 4/16.0, 2/16.0,
					1/16.0, 2/16.0, 1/16.0};	//gaussian blur
					
	test.std_convolve_clamp_to_0(0, 3, 3, ker, 1, 1);
	test.std_convolve_clamp_to_0(1, 3, 3, ker, 1, 1);
	test.std_convolve_clamp_to_0(2, 3, 3, ker, 1, 1);
	
	test.write("blurred4.png");
	return 0;
}
