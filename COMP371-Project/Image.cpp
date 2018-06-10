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
void Image<T>::set_colour_at(const Point& pixel, const Colour& base_colour, const Colour& lighting_colour, const long& light_count)
{
	//need to take a better average between colours
	//1. create an unbiased summation between the base and lighting colours, using the number of lights
	Colour pixel_colour = lighting_colour + (static_cast<double>(light_count) * base_colour);
	//2. now compute the square average of the colours in the pixel
	pixel_colour *= pixel_colour;
	pixel_colour /= light_count;
	pixel_colour.r = sqrt(pixel_colour.x);
	pixel_colour.g = sqrt(pixel_colour.y);
	pixel_colour.b = sqrt(pixel_colour.z);
	//3. clamp the colour and set it to the double range
	pixel_colour = glm::clamp(pixel_colour, 0.0, 1.0);
	pixel_colour *= std::numeric_limits<double>::max();
	//4. set the pixel colour
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