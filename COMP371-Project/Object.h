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
#include "Light.h"

class Object : public Entity
{
	protected:
	Material material;
	
	public:
	Object(void);
	Object(const Point& p);
	Object(const Point& p, const Material& m);
	virtual ~Object(void)=default;
	virtual bool intersects(const Ray& r);
	virtual Point intersection(const Ray& r)=0;
	virtual Colour surface_colour(const Point& p, const Light& l, const Point& cameraPos);
	virtual Vector normal_at(const Point& p)=0;
	virtual inline std::string to_string(void)
	{
		std::stringstream ss;
		ss <<	"Object Properties:\n" <<
		"\t" << this->Entity::to_string() << "\n" <<
		"\t" << this->material.to_string();
		return ss.str();
	}
	inline Colour ambient_colour(void)
	{
		return material.ambient_colour();
	}
};


#endif /* Object_h */
