//
//  Ray.cpp
//  COMP371-Project
//
//  Created by Mathew Plouffe on 2018-06-04.
//  Copyright © 2018 Mathew Plouffe. All rights reserved.
//

#include "Ray.h"

Ray::Ray(void)
{
	direction = Vector(0.0);
}
Ray::Ray(const Vector& dir)
{
	direction = glm::normalize(dir);
}
Ray::Ray(const Point& p, const Vector& dir) : Entity(p)
{
	direction = glm::normalize(dir);
}