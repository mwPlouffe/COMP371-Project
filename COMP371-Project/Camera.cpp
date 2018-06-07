//
//  Camera.cpp
//  COMP371-Project
//
//  Created by Mathew Plouffe on 2018-06-04.
//  Copyright © 2018 Mathew Plouffe. All rights reserved.
//

#include "Camera.h"

Camera::Camera(void) : Entity(Point(0.0))
{
	field_of_view = 0.0;
	focal_length  = 0.0;
	aspect_ratio  = 0.0;
}
//camera is located by default at 0,0,f, where f is the focal length, or distance from the image
Camera::Camera(double focal, double fov, double a) : Entity(Point(0.0, 0.0, 0.0))
{
	field_of_view = fov;
	focal_length  = focal;
	aspect_ratio  = a;
}
Camera::Camera(double focal, double fov, double a, const Point& p) : Entity(p)
{
	field_of_view = fov;
	focal_length  = focal;
	aspect_ratio  = a;
}