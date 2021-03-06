//
//  Object.cpp
//  COMP371-Project
//
//  Created by Mathew Plouffe on 2018-06-04.
//  Copyright © 2018 Mathew Plouffe. All rights reserved.
//

#include "Object.h"

Object::Object(void)
{
	material = Material();
}
Object::Object(const Point& p) : Entity(p)
{
	material = Material();
}
Object::Object(const Point& p, const Material& m) : Entity(p)
{
	material = m;
}
bool Object::intersects(const Ray& r) const
{
#ifdef cached
	cache_intersect = intersection(r);
	return (cache_intersect != (Point) NULL) ? true : false;
#endif
	return (intersection(r) != (Point) NULL) ? true : false;
}
Colour Object::surface_colour(const Point& intersect, const Light& l, const Point& cameraPos) const
{
	return material.calculate_colour(glm::normalize(intersect - cameraPos), normal_at(intersect), glm::normalize(intersect - l.location()), l.light_colour());
}
Colour Object::shadow_colour(const Light& l) const
{
	return material.calculate_shadow_colour(l.light_colour());
}

