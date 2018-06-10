//
//  AreaLight.cpp
//  COMP371-Project
//
//  Created by Mathew Plouffe on 2018-06-09.
//  Copyright © 2018 Mathew Plouffe. All rights reserved.
//

#include "AreaLight.h"

AreaLight::AreaLight(void) : Light(Point(0.0),Colour(1.0))
{
	//by defualt
	this->radius	= 0.1;
	this->offset	= 5.0;	//degrees
	
}
AreaLight::AreaLight(const Point& p, const Colour& c, const double& offset) : Light(p,c)
{
	radius = 0.1;
	this->offset = offset;
}
std::vector<Light*> AreaLight::return_lights(void)
{
	std::vector<Light*> ret;
	
	for (double angle = 0; angle < 360.0; angle+=offset)
	{
		ret.push_back(new Light(this->colour, Point(radius * cos(offset * PI / 180.0), radius * sin(offset * PI / 180.0),this->position.z)));
	}
	return ret;
}
void AreaLight::calculate_light_number(long lights)
{
	offset = 360.0 / lights;
}
