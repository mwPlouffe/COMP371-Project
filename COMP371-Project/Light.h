//
//  Light.h
//  COMP371-Project
//
//  Created by Mathew Plouffe on 2018-05-31.
//  Copyright © 2018 Mathew Plouffe. All rights reserved.
//

#ifndef Light_h
#define Light_h

#include "Entity.h"

//a light that has a position and a colour
//is a point light
class Light : public Entity
{
	
	protected:
	Colour colour;
	public:
	Light(void);
	Light(const Colour& c);
	Light(const Colour& c, const Point& p);
	virtual ~Light(void) = default;
	inline Colour light_colour(void) const
	{
		return colour;
	}
	virtual inline std::string to_string(void)
	{
		std::stringstream ss;
		ss <<	"Light Properties:\n" <<
		"\t" << this->Entity::to_string() << "\n" <<
		"\tColour: (" << colour.r << ", " << colour.g << ", " << colour.b << ")";
		return ss.str();
	}
};


#endif /* Light_h */
