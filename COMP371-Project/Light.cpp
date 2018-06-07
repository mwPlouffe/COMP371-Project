//
//  Light.cpp
//  COMP371-Project
//
//  Created by Mathew Plouffe on 2018-06-04.
//  Copyright © 2018 Mathew Plouffe. All rights reserved.
//

#include "Light.h"

Light::Light(void)
{
	colour = Colour(1.0);
}
Light::Light(const Colour& c)
{
	colour = c;
}
Light::Light(const Colour& c, const Point& p) : Entity(p)
{
	colour = c;
}