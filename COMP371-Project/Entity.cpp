//
//  Entity.cpp
//  COMP371-Project
//
//  Created by Mathew Plouffe on 2018-06-04.
//  Copyright © 2018 Mathew Plouffe. All rights reserved.
//

#include "Entity.h"

Entity::Entity(void)
{
	position = Point(0.0);
}
Entity::Entity(const Point& pos)
{
	position = pos;
}