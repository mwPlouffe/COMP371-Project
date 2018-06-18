//
//  Object.h
//  COMP371-Project
//
//  Created by Mathew Plouffe on 2018-05-31.
//  Copyright © 2018 Mathew Plouffe. All rights reserved.
//

#ifndef Object_h
#define Object_h

#include "Entity.h"
#include "Ray.h"
#include "Material.h"
#include "ColourMixer.h"
#include "Light.h"

//#define cached

class Object : public Entity
{
	protected:
	Material material;
#ifdef cached
	Point cache_intersect;
#endif
	public:
	Object(void);
	Object(const Point& p);
	Object(const Point& p, const Material& m);
	virtual ~Object(void)=default;
	virtual bool intersects(const Ray& r) const ;
	virtual Point intersection(const Ray& r) const =0;
	virtual Colour surface_colour(const Point& intersect, const Light& l, const Point& cameraPos) const;
	Colour shadow_colour(const Light& l) const;
	virtual Vector normal_at(const Point& p) const=0;
	inline std::string to_string(void)
	{
		std::stringstream ss;
		ss <<	"Object Properties:\n" <<
		"\t" << this->Entity::to_string() << "\n" <<
		"\t" << this->material.to_string();
		return ss.str();
	}
	inline Colour ambient_colour(void) const
	{
		return material.ambient_colour();
	}
#ifdef cached
	inline Point cached_intersect(void)
	{
		Point ret = cache_intersect;
		clear_cache();
		return ret;
	}
	inline void clear_cache(void)
	{
		cache_intersect = (Point) NULL;
	}
#endif
};


#endif /* Object_h */
