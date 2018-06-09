//
//  Sphere.cpp
//  COMP371-Project
//
//  Created by Mathew Plouffe on 2018-05-31.
//  Copyright © 2018 Mathew Plouffe. All rights reserved.
//

#include "Sphere.h"
Sphere::Sphere(void)
{
	radius = 0.0;
}
Sphere::Sphere(const Point& p, double r) : Object(p)
{
	radius = r;
}
Sphere::Sphere(const Point& p, double r, const Material& m) : Object(p,m)
{
	radius = r;
}
bool Sphere::intersects(const Ray& r, bool is_furthest)
{
	return ((intersection(r, is_furthest) != (Point) NULL) ? true : false);
}
Point Sphere::intersection(const Ray& r)
{
	return intersection(r, false);
}
Point Sphere::intersection(const Ray& r, bool is_furthest)
{
	//calculate the a,b,c constants for the quadratic equation (they all boil down to some form of dot product between the ray direction, the ray position, and/or the sphere centre)
	double a = glm::dot(r.vector(),r.vector());
	double b = glm::dot(2.0 * r.vector(), r.location() - this->position);
	double c = glm::dot(r.location() - this->position, r.location() - this->position) - radius * radius;
	
	//then the discriminant
	double discriminant = (b * b) - (4 * a * c);
	
	//ain't having none if your roots aint real (no intersections for negative discriminant
	if (discriminant < 0.0)
	{
		//std::cout << "WARNING: no real roots found" << std::endl;
		return (Point) NULL;
	}
	
	//calculate the roots (as an input 't' for the ray
	double root1 = ((-1.0 * b) + sqrt(discriminant)) / (2.0 * a) + 0.00000000001;
	double root2 = ((-1.0 * b) - sqrt(discriminant)) / (2.0 * a) + 0.00000000001;
	
	if (root1 < 0.0 || root2 < 0.0)
	{
		//std::cout << "WARNING: roots exist behind camera" << std::endl;
		return (Point) NULL;
	}
	//if they are too close, they are probably a repeat root (tangent to the sphere)
	if (std::abs(root1-root2) < std::numeric_limits<double>::epsilon())
	{
		//std::cout << "MESSAGE: repeated roots found" << std::endl;
		return r.cast(root1);
	}
	else
	{
		return (root1 < root2) ? r.cast(root1) : r.cast(root2);
	}
}
Vector Sphere::normal_at(const Point& p)
{
	return glm::normalize(this->position - p);
}
