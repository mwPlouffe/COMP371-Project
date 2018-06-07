//
//  Entity.h
//  COMP371-Project
//
//  Created by Mathew Plouffe on 2018-06-04.
//  Copyright © 2018 Mathew Plouffe. All rights reserved.
//

#ifndef Entity_h
#define Entity_h

#include "Libraries.h"

class Entity
{
	protected:
	Point position;

	public:
	Entity(void);
	Entity(const Point& pos);
	virtual ~Entity(void) = default;
	virtual inline Point location(void) const
	{
		return position;
	}
	virtual inline std::string to_string(void)
	{
		std::stringstream ss;
		ss <<	"Entity Properties:\n" <<
				"\tPosition: (" << position.x << ", " << position.y << ", " << position.z << ")";
		return ss.str();
	}
};
#endif /* Entity_h */
