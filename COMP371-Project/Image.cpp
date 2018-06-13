//
//  Image.cpp
//  COMP371-Project
//
//  Created by Mathew Plouffe on 2018-06-10.
//  Copyright © 2018 Mathew Plouffe. All rights reserved.
//

#include "Image.h"
#include "Utility.h"
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
	image_pixels.get_shared_channel(channels - 1).fill(depth);
	std::cout << "MESSAGE: Image Resolution: (w: " << image_pixels.width() << ", h: " << image_pixels.height() << ")" << std::endl;
	std::cout << "MESSAGE: Image instantiated successfully" << std::endl;
}
template <class T>
void Image<T>::set_colour_at(const Point& pixel, const Colour& c)
{
	image_pixels(pixel.x, pixel.y, 0, 0) = c.r;
	image_pixels(pixel.x, pixel.y, 0, 1) = c.g;
	image_pixels(pixel.x, pixel.y, 0, 2) = c.b;
}
template <class T>
void Image<T>::set_colour_at(const Point& pixel, const Colour& colour, const long& ray_count)
{
	Colour pixel_colour = colour /(double) ray_count;
	pixel_colour = glm::clamp(pixel_colour, 0.0, 1.0);
	this->set_colour_at(pixel, pixel_colour);
}

template <class T>
void Image<T>::set_colour_at(const Point& pixel, const Colour& base_colour, const Colour& lighting_colour, const long& ray_count)
{
	//https://medium.com/@kevinsimper/how-to-average-rgb-colors-together-6cd3ef1ff1e5
	//need to take a better average between colours
	//1. create an unbiased summation between the base and lighting colours, using the number of lights
	Colour pixel_colour = (GLOBAL_IMAGE_INTENSITY * lighting_colour + base_colour);
	//2. now compute the square average of the colours in the pixel
	pixel_colour /= (ray_count + GLOBAL_IMAGE_INTENSITY + 1.0);
	pixel_colour.r = sqrt(pixel_colour.r);
	pixel_colour.g = sqrt(pixel_colour.g);
	pixel_colour.b = sqrt(pixel_colour.b);
	//3. clamp the colour betweeen 0 and 1 (for the gamma correction
	pixel_colour = glm::clamp(pixel_colour, 0.0, 1.0);
	
	//https://stackoverflow.com/questions/16521003/gamma-correction-formula-gamma-or-1-gamma?utm_medium=organic&utm_source=google_rich_qa&utm_campaign=google_rich_qa
	//4. apply a gamma correction
#ifdef GAMMA
	pixel_colour.r = pow(pixel_colour.r, GAMMA);
	pixel_colour.g = pow(pixel_colour.g, GAMMA);
	pixel_colour.b = pow(pixel_colour.b, GAMMA);
#endif
	
	//5. set the pixel colour
	this->set_colour_at(pixel, pixel_colour);
}
template <class T>
void Image<T>::save_image_to_file(const std::string& filepath)
{
	std::cout << "MESSAGE: Normalising image to RGB-256" << std::endl;
	image_pixels.normalize(0.0, IMAGE_RICHNESS - 1.0);
	image_pixels.mirror("x");
	const char* fp = filepath.c_str();
	try
	{
		image_pixels.save(fp);
	}
	catch (CImgException &ex)
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
template <class T>
void  Image<T>::normalise(double max_value)
{
	
	for (int x = 0; x < image_pixels.width(); x++)
	{
		for (int y = 0; y < image_pixels.height(); y++)
		{
			Point pixel(x, y, 0);
			image_pixels(pixel.x, pixel.y, 0, 0) *= max_value;
			image_pixels(pixel.x, pixel.y, 0, 1) *= max_value;
			image_pixels(pixel.x, pixel.y, 0, 2) *= max_value;
			image_pixels(pixel.x, pixel.y, 0, 3) = 0.0;
		}
	}
}
template <class T>
void Image<T>::anti_alias(int radius)
{
	if (radius < 1)
	{
		std::cout << "WARNING: cannot anti-alias on a radius of 0 or less" << std::endl;
		return;
	}
	std::cout << "MESSAGE: Applying Post-Rendering MSAA" << std::flush;
	const CImg<double> sampler(this->image_pixels);
	for (int x = radius; x < image_pixels.width() - radius; x++)
	{
		for (int y = radius; y < image_pixels.height() - radius; y++)
		{
			Colour aggregate(0.0);
			bool bugged = false;
			for (int x_sample = -1 * radius; x_sample <= radius; x_sample++)
			{
				for (int y_sample = -1 * radius; y_sample <= radius; y_sample++)
				{
					aggregate.r += sampler(x + x_sample, y + y_sample, 0, 0);
					aggregate.g += sampler(x + x_sample, y + y_sample, 0, 1);
					aggregate.b += sampler(x + x_sample, y + y_sample, 0, 2);
					if (aggregate.r > 100 || aggregate.g > 100 || aggregate.b > 100)
					{
						//	std::cout <<"Pixel : " << Utility::display(Point(x,y,0)) << std::endl;
						//std::cout <<"Colour : " << Utility::display(aggregate) << std::endl;
						bugged = true;
					}
				}
			}
#ifdef GAMMA 
			aggregate.r = pow(aggregate.r, GAMMA);
			aggregate.g = pow(aggregate.g, GAMMA);
			aggregate.b = pow(aggregate.b, GAMMA);
#endif
			this->set_colour_at(Point(x ,y, 0), aggregate, (radius + 2) * (radius + 2));
			
			if (bugged)
			{
				//this->set_colour_at(Point(x ,y, 0), Colour(1.0, 0.0, 0.0));
			}
			if ((std::abs(x) + std::abs(y) * image_pixels.width()) % (static_cast<long>(image_pixels.size() * 0.05)) == 0 )
			{
				std::cout << "." << std::flush;
			}
		}
	}
}






