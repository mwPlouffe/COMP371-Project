//
//  Tracer.h
//  COMP371-Project
//
//  Created by Mathew Plouffe on 2018-06-13.
//  Copyright © 2018 Mathew Plouffe. All rights reserved.
//

#ifndef Tracer_h
#define Tracer_h

#include "Libraries.h"
#include "Scene.h"
#include "Image.h"

#define MAX_RAYS 36
#define NOISE_RANGE 0.5

class Tracer
{
	private:
	Colour material_colour;
	Colour lighting_colour;
	const long pixel_samples;
	const double sample_noise;
	const Scene scene;
	Image_d *image;
		
	public:
	Tracer(const Scene& scn);
	Tracer(const long& ps, const double& sn, const Scene& scn);
	Tracer(const long& ps, const double& sn, const Scene& scn, Image_d& img);
	virtual ~Tracer(void) = default;
	void trace(const Object& reciever, const Ray& r);
	Colour trace_colour(const Object& reciever, const Point& intersection);
	inline void set_image(Image_d& img)
	{
		image = &img;
	}
	private:
	bool is_occluded(const Object& reciever, const Ray& shadow_ray, const Point& p) const;
};


#endif /* Tracer_h */
