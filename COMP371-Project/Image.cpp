//
//  Image.cpp
//  COMP371-Project
//
//  Created by Mathew Plouffe on 2018-06-10.
//  Copyright © 2018 Mathew Plouffe. All rights reserved.
//

#include "Image.h"
using namespace cimg_library;

template <class T>
Image<T>::Image(void)
{
	cimg::exception_mode(0);
	try
	{
		image_pixels = CImg<T>(1000, 1000, 1, 3, 0);
	}
	catch (CImgException &ex)
	{
		std::cout << ex.what() << std::endl;
		throw;
	}
	std::cout << "MESSAGE: Image Resolution: (w: " << image_pixels.width() << ", h: " << image_pixels.height() << ")" << std::endl;
	std::cout << "MESSAGE: Image instantiated correctly" << std::endl;
}
template <class T>
Image<T>::Image(const long& width, const long& height, const long& channels, const double& depth)
{
	cimg::exception_mode(0);
	try
	{
		image_pixels = CImg<T>(width, height, 1, channels, 0);
	}
	catch (CImgException &ex)
	{
		std::cout << ex.what() << std::endl;
		throw;
	}
	image_pixels.get_shared_channel(3).fill(depth);
	std::cout << "MESSAGE: Image Resolution: (w: " << image_pixels.width() << ", h: " << image_pixels.height() << ")" << std::endl;
	std::cout << "MESSAGE: Image instantiated correctly" << std::endl;
}
template <class T>
void Image<T>::set_colour_at(const Point& pixel, const Colour& c)
{
	image_pixels(pixel.x, pixel.y, 0, 0) = c.r;
	image_pixels(pixel.x, pixel.y, 0, 1) = c.g;
	image_pixels(pixel.x, pixel.y, 0, 2) = c.b;
}
template <class T>
void Image<T>::set_colour_at(const Point& pixel, const Colour& base_colour, const Colour& lighting_colour, const long& light_count, const long& light_bias_count)
{
	//https://medium.com/@kevinsimper/how-to-average-rgb-colors-together-6cd3ef1ff1e5
	//need to take a better average between colours
	//1. create an unbiased summation between the base and lighting colours, using the number of lights
	Colour pixel_colour = (lighting_colour + base_colour * ((double)light_count));
	//2. now compute the square average of the colours in the pixel
	pixel_colour *= pixel_colour;
	pixel_colour /= (light_count);
	pixel_colour.r = sqrt(pixel_colour.x);
	pixel_colour.g = sqrt(pixel_colour.y);
	pixel_colour.b = sqrt(pixel_colour.z);
	//3. clamp the colour betweeen 0 and 1 (for the gamma correction
	pixel_colour = glm::clamp(pixel_colour, 0.0, 1.0);
	
	//https://stackoverflow.com/questions/16521003/gamma-correction-formula-gamma-or-1-gamma?utm_medium=organic&utm_source=google_rich_qa&utm_campaign=google_rich_qa
	//4. apply a gamma correction
	pixel_colour.r = pow(pixel_colour.r, 2.2);
	pixel_colour.g = pow(pixel_colour.g, 2.2);
	pixel_colour.b = pow(pixel_colour.b, 2.2);
	pixel_colour *= std::numeric_limits<double>::max();
	//5. set the pixel colour
	this->set_colour_at(pixel, pixel_colour);
}
template <class T>
void Image<T>::save_image_to_file(const std::string& filepath)
{
	std::cout << "MESSAGE: Normalising image to RGB-256" << std::endl;
	image_pixels.normalize(0.0, 255.0);
	try
	{
		image_pixels.save("./output.bmp");
	} catch (CImgException &ex)
	{
		std::cout << ex.what() << std::endl;
		return;
	}
	std::cout << "MESSAGE: Saving Complete" << std::endl;
}
template <class T>
bool Image<T>::test_depth_at(const Point& pixel, const double& depth)
{
	return (std::abs(depth) < std::abs(image_pixels(pixel.x, pixel.y, 0, 3))) ? true : false;
}
template <class T>
void Image<T>::set_depth_at(const Point& pixel, const double& depth)
{
	image_pixels(pixel.x, pixel.y, 0, 3) = depth;
}