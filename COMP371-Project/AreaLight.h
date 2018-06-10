//
//  AreaLight.h
//  COMP371-Project
//
//  Created by Mathew Plouffe on 2018-06-09.
//  Copyright © 2018 Mathew Plouffe. All rights reserved.
//

#ifndef AreaLight_h
#define AreaLight_h

#include "Light.h"

class AreaLight : public Light
{
	//assumes that the position inherited from the entity class is the centre of a circle
	protected:
	double radius;									//the radius of the circle
	double offset;									//the angle at which the light is offset by the radius
	void calculate_light_number(long lights);		//computes the spacing between the lights from the number of lights
	
	public:
	
	AreaLight(void);
	AreaLight(const Point& p, const Colour& c, const double& offset);
	virtual ~AreaLight(void) = default;
	std::vector<Light*> return_lights(void);
};

#endif /* AreaLight_h */
