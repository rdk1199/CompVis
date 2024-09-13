#pragma once

#include <string>

#include "../ImagProc/image.h"


/*
*The labelled grayscale image file format is as follows : 
* byte 0 represents the category (to be interpreted as unsigned int8)
* bytes 1-4 represent the width (to be interpreted as an unsigned int64, little-endian
* bytes 5-8 represent the height (to be interpreted as an unsigned int64, little-endian
* each remaining byte represents the value (0-255) of a pixel in the image - to be interpreted as an unsigned int
*/

class GSImageLoader
{

public:

	//these put the category label into "label" 
	static Image load_as_image(const std::string& file, int& label);
	static std::vector<double> load_as_vector(const std::string& file, int& label);

};

