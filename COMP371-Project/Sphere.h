//
//  Sphere.h
//  COMP371-Project
//
//  Created by Mathew Plouffe on 2018-05-31.
//  Copyright © 2018 Mathew Plouffe. All rights reserved.
//

#ifndef Sphere_h
#define Sphere_h

#include "Object.h"
#include "Material.h"

class Sphere : public Object
{
	private:
	double radius;
	
	public:
	Sphere(void);
	Sphere(const Point& p, double r);
	Sphere(const Point& p, double r, const Material& m);
	virtual ~Sphere(void) = default;
	virtual Point intersection(const Ray& r);
	virtual Vector normal_at(const Point& p);
	virtual inline std::string to_string(void)
	{
		std::stringstream ss;
		ss <<	"Sphere Properties:\n" <<
		"\t" << this->Object::to_string() << "\n" <<
		"\tRadius: " << radius;
		return ss.str();
	}
};

#endif /* Sphere_h */
