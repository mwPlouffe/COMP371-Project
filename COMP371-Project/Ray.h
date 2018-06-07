//
//  Ray.h
//  COMP371-Project
//
//  Created by Mathew Plouffe on 2018-05-31.
//  Copyright © 2018 Mathew Plouffe. All rights reserved.
//

#ifndef Ray_h
#define Ray_h

#include "Entity.h"

class Ray : public Entity
{
	private:
	Vector	direction;
	
	public:
	Ray(void);
	Ray(const Vector& dir);
	Ray(const Point& p, const Vector& dir);
	~Ray(void) = default;
	inline Vector vector(void) const
	{
		return direction;
	}
	inline Point cast(double t) const
	{
		return position + (direction * t);
	}
	virtual inline std::string to_string(void)
	{
		std::stringstream ss;
		ss <<	"Ray Properties:\n" <<
		"\t" << this->Entity::to_string() << "\n" <<
		"\tDirection: (" << direction.x << ", " << direction.y << ", " << direction.z << ")";
		return ss.str();
	}
};

#endif /* Ray_h */
