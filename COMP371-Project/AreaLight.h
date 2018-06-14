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
	double radius;									//the radius of the sphere
	public:
	
	AreaLight(void);
	AreaLight(const Point& p, const Colour& c);
	AreaLight(const Point& p, const Colour& c, const double& r);
	virtual ~AreaLight(void) = default;
	Point sample(void);
};

#endif /* AreaLight_h */
