//
//  ColourMixer.cpp
//  COMP371-Project
//
//  Created by Mathew Plouffe on 2018-06-16.
//  Copyright © 2018 Mathew Plouffe. All rights reserved.
//

#include "ColourMixer.h"

ColourMixer::ColourMixer(void)
{
	lighting_colours	= std::vector<Colour>();
	base_colours		= std::vector<Colour>();
}

void ColourMixer::add_base(const Colour& base)
{
	base_colours.push_back(base);
}
void ColourMixer::add_lighting(const Colour& lighting)
{
	lighting_colours.push_back(lighting);
}
void ColourMixer::merge(const ColourMixer& cm)
{
	for (std::vector<Colour>::const_iterator base = cm.base_colours.begin(); base != cm.base_colours.end(); base++)
	{
			this->base_colours.push_back(*base);
	}
	for (std::vector<Colour>::const_iterator light = cm.lighting_colours.begin(); light != cm.lighting_colours.end(); light++)
	{
		this->base_colours.push_back(*light);
	}
}
void ColourMixer::clear(void)
{
	lighting_colours	= std::vector<Colour>();
	base_colours		= std::vector<Colour>();
}
Colour ColourMixer::simple_average(void)
{
	//calculates the simple average of the colours in the mixer
	int count = base_colours.size() + lighting_colours.size();
	Colour avg = Colour(0.0);
	for (std::vector<Colour>::iterator base = base_colours.begin(); base != base_colours.end(); base++)
	{
		avg += *base;
	}
	for (std::vector<Colour>::iterator light = lighting_colours.begin(); light != lighting_colours.end(); light++)
	{
		avg += *light;
	}
	avg /= (1.0 * count);
	return glm::clamp(avg, 0.0, 1.0);
}
Colour ColourMixer::square_average(void)
{
	//calculates the square average of the colours in the mixer
	int count = base_colours.size() + lighting_colours.size();
	Colour avg = Colour(0.0);
	for (std::vector<Colour>::iterator base = base_colours.begin(); base != base_colours.end(); base++)
	{
		avg += *base * *base;
	}
	for (std::vector<Colour>::iterator light = lighting_colours.begin(); light != lighting_colours.end(); light++)
	{
		avg += *light * *light;
	}
	avg /= (1.0 * count);
	avg.r = sqrt(avg.r);
	avg.g = sqrt(avg.g);
	avg.b = sqrt(avg.b);
	return glm::clamp(avg, 0.0, 1.0);
}
void ColourMixer::average(weight_func func, double weight, bool is_base)
{
	std::vector<Colour>* colours;
	if (is_base)
	{
		colours = &base_colours;
	}
	else
	{
		colours = &lighting_colours;
	}
	Colour ret;
	for (std::vector<Colour>::iterator c = colours->begin(); c != colours->end(); c++)
	{
		*c = func(*c, weight);
	}
}
void ColourMixer::base_blend(blend_func func)
{
	if (base_colours.size() == 0)
	{
		return;
	}
	for (std::vector<Colour>::iterator base = base_colours.begin(); base != base_colours.end(); base++)
	{
		*base = func(*base);
	}
}