//
//  Plane.cpp
//  COMP371-Project
//
//  Created by Mathew Plouffe on 2018-06-04.
//  Copyright © 2018 Mathew Plouffe. All rights reserved.
//

#include "Plane.h"

Plane::Plane(void)
{
	normal = Vector(0.0);
}
Plane::Plane(const Point& p, const Vector& n) : Object(p)
{
	normal = glm::normalize(n);
}
Plane::Plane(const Point& p, const Vector& n, const Material& m) : Object(p,m)
{
	normal = glm::normalize(n);
}
Point Plane::intersection(const Ray& r)
{
	//calculate the discriminant
	double discriminant = glm::dot(this->normal, r.vector());
	
	//ensure that a real root exists
	if (std::abs(discriminant) < std::numeric_limits<double>::epsilon())
	{
		//std::cout << "WARNING: no real roots found" << std::endl;
		return (Point) NULL;
	}
	
	//calculate the real root
	//the value of d is calculated through taking the dot of the normal and the origin of the ray
	double root  = ((glm::dot(this->normal, this->location()) - glm::dot(this->normal, r.location())) / discriminant) + 0.000001;

	//check if it's behind the camera
	if (root < 0.0)
	{
		//std::cout << "WARNING: Intersection is behind the camera" << std::endl;
		return (Point) NULL;
	}
	//return the position at the specified root to the user
	return r.cast(root);
}
Vector Plane::normal_at(const Point& p)
{
	return normal;
}
