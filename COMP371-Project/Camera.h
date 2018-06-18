//
//  Camera.h
//  COMP371-Project
//
//  Created by Mathew Plouffe on 2018-06-04.
//  Copyright © 2018 Mathew Plouffe. All rights reserved.
//

#ifndef Camera_h
#define Camera_h

#include "Entity.h"


class Camera : public Entity
{
	private:
	double field_of_view;
	double focal_length;
	double aspect_ratio;
	
	public:
	Camera(void);
	//camera is located by default at 0,0,f, where f is the focal length, or distance from the image
	Camera(double f, double fov, double a);
	Camera(double f, double fov, double a, const Point& p);
	virtual inline std::string to_string(void)
	{
		std::stringstream ss;
		ss <<	"Camera Properties:\n" <<
				"\t" << this->Entity::to_string() <<
				"\n\tFOV: " << field_of_view <<
				"\n\tFocal Length: " << focal_length <<
				"\n\tAspect Ratio: " << aspect_ratio;
		return ss.str();
	}
	inline double image_width(void)
	{
		double ret = tan( (field_of_view * PI / 180.0) / 2.0);
		//ret = (ret > 0) ? ret : -1.0 * ret;
		return 2.0 * focal_length * aspect_ratio * ret;
	}
	inline double image_height(void)
	{
		double ret = tan( (field_of_view * PI / 180.0) / 2.0);
		//ret = (ret > 0) ? ret : -1.0 * ret;
		return 2.0 * focal_length * ret;
	}
	inline double f_length(void)
	{
		return focal_length;
	}
};

#endif /* Camera_h */
