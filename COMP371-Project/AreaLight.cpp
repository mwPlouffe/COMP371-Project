//
//  AreaLight.cpp
//  COMP371-Project
//
//  Created by Mathew Plouffe on 2018-06-09.
//  Copyright © 2018 Mathew Plouffe. All rights reserved.
//

#include "AreaLight.h"

AreaLight::AreaLight(void) : Light(Point(0.0), Colour(1.0))
{
	radius = 0.0;
}
AreaLight::AreaLight(const Point& p, const Colour& c) : Light(p,c)
{
	radius = 0.1;
}
AreaLight::AreaLight(const Point& p, const Colour& c, const double& r) : Light(p,c)
{
	radius = r;
}
Point AreaLight::sample(void)
{
	//samples a sphere in spherical coordinates to generate a point that lies within the radius of the sphere in all directions
	
	double r_sample		= radius *		(std::rand() / RAND_MAX);
	double theta_sample = 1.0 * PI *	(std::rand() / RAND_MAX);
	double phi_sample	= 2.0 * PI *	(std::rand() / RAND_MAX);
	
	//spherical to cartesian conversion
	Point ret(r_sample * cos(theta_sample) * sin(phi_sample),
			  r_sample * sin(theta_sample) * sin(phi_sample),
			  r_sample * cos(phi_sample));
	
	//object space to world space conversion
	ret += position;
	
	return ret;
}