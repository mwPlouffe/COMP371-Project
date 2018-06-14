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
Point Sphere::intersection(const Ray& r) const
{
#ifdef cached
	if (cache_intersect != (Point) NULL)
	{
		return cache_intersect;
	}
#endif
	//calculate the a,b,c constants for the quadratic equation (they all boil down to some form of dot product between the ray direction, the ray position, and/or the sphere centre)
	double a = glm::dot(r.vector(),r.vector());
	double b = 2.0 * glm::dot(r.vector(), r.location() - this->position);
	double c = glm::dot(r.location() - this->position, r.location() - this->position) - radius * radius;
	
	//then the discriminant
	double discriminant = (b * b) - (4 * a * c);
	
	//ain't having none if your roots aint real (no intersections for negative discriminant
	if (discriminant < 0.0)
	{
		//std::cout << "WARNING: no real roots found" << std::endl;
		#ifdef cached
		cache_intersect = (Point) NULL;
		return cache_intersect;
		#endif
		return (Point) NULL;
	}
	
	//calculate the roots (as an input 't' for the ray
	double root1 = ((-1.0 * b) + sqrt(discriminant)) / (2.0 * a) + 0.00000000001;
	double root2 = ((-1.0 * b) - sqrt(discriminant)) / (2.0 * a) + 0.00000000001;
	
	if (root1 < 0.0 || root2 < 0.0)
	{
		//std::cout << "WARNING: roots exist behind camera" << std::endl;
#ifdef cached
		cache_intersect = (Point) NULL;
		return cache_intersect;
#endif
		return (Point) NULL;
	}
	//if they are too close, they are probably a repeat root (tangent to the sphere)
	if (std::abs(root1-root2) < std::numeric_limits<double>::epsilon())
	{
		//std::cout << "MESSAGE: repeated roots found" << std::endl;
#ifdef cached
		cache_intersect = r.cast(root1);
#endif
		return r.cast(root1);
	}
	else
	{
#ifdef cached
		cache_intersect = ((root1 < root2)) ? r.cast(root1) : r.cast(root2);
#endif
		return ((root1 < root2)) ? r.cast(root1) : r.cast(root2);
	}
#ifdef cached
	return cache_intersect;
#endif
}
Vector Sphere::normal_at(const Point& p) const
{
	return glm::normalize(this->position - p);
}
