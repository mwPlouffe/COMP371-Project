//
//  Image.h
//  COMP371-Project
//
//  Created by Mathew Plouffe on 2018-06-10.
//  Copyright © 2018 Mathew Plouffe. All rights reserved.
//

#ifndef Image_h
#define Image_h

#include "Libraries.h"

#define GAMMA 2.2
#define IMAGE_RICHNESS 256.0
#define GLOBAL_IMAGE_INTENSITY 1.0

#define cimg_display 0
#include "CImg.h"

template <class T>
class Image
{
	protected:
	cimg_library::CImg <T> image_pixels;
	
	public:
	Image(void);
	Image(const long& width, const long& height, const long& channels, const double& depth);
	
	void set_colour_at(const Point& pixel, const Colour& c);
	void set_colour_at(const Point& pixel, const Colour& base_colour, const Colour& lighting_colour, const long& ray_count);
	void set_colour_at(const Point& pixel, const Colour& colour, const long& ray_count);
	void save_image_to_file(const std::string& filepath);
	bool test_depth_at(const Point& pixel, const double& depth);
	void set_depth_at(const Point& pixel, const double& depth);
	inline long get_image_width(void) const
	{
		return image_pixels.width();
	}
	inline long get_image_height(void) const
	{
		return image_pixels.height();
	}
	inline long size(void) const
	{
		return image_pixels.size();
	}
	void anti_alias(int radius);	
	void normalise(double max_value);
};

typedef Image<double> Image_d;

#endif /* Image_h */
