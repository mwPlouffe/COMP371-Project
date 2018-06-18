//
//  Plane.h
//  COMP371-Project
//
//  Created by Mathew Plouffe on 2018-05-31.
//  Copyright © 2018 Mathew Plouffe. All rights reserved.
//

#ifndef Plane_h
#define Plane_h

#include "Object.h"

//the plane object, overrides the intersection and normal function
class Plane : public Object
{
	private:
	Vector normal;
	
	public:
	Plane(void);
	Plane(const Point& p, const Vector& n);
	Plane(const Point& p, const Vector& n, const Material& m);
	virtual ~Plane() = default;
	virtual Point intersection(const Ray& r) const;
	inline std::string to_string(void)
	{
		std::stringstream ss;
		ss <<	"Plane Properties:\n" <<
		"\t" << this->Object::to_string() << "\n" <<
		"\tNormal: (" << normal.x << ", " << normal.y << ", " << normal.z << ")";
		return ss.str();
	}
	virtual Vector normal_at(const Point& p) const;
};

#endif /* Plane_h */
