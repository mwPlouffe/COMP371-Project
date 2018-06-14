//
//  Tracer.cpp
//  COMP371-Project
//
//  Created by Mathew Plouffe on 2018-06-14.
//  Copyright © 2018 Mathew Plouffe. All rights reserved.
//

#include "Tracer.h"

Tracer::Tracer(const Scene& scn) :
	pixel_samples(MAX_RAYS),
	sample_noise(NOISE_RANGE),
	scene(scn),
	material_colour(0.0),
	lighting_colour(0.0)
{
	//intentionally empty
}
Tracer::Tracer(const long& ps, const double& sn, const Scene& scn) :
	pixel_samples(ps),
	sample_noise(sn),
	scene(scn),
	material_colour(0.0),
	lighting_colour(0.0)
{
	//intentionally empty
}
Tracer::Tracer(const long& ps, const double& sn, const Scene& scn, Image_d& img) :
	pixel_samples(ps),
	sample_noise(sn),
	scene(scn),
	material_colour(0.0),
	lighting_colour(0.0)
{
	//intentionally empty
}
Colour Tracer::trace_colour(const Object& reciever, const Point& intersection)
{
	Colour colour = Colour(0.0);
	Colour base = reciever.ambient_colour();
	Ray shadow_ray;
	for (std::vector<Light*>::const_iterator light = scene.lights.begin(); light != scene.lights.end(); light++)
	{
		shadow_ray = Ray(intersection,(*light)->location() - intersection);

		bool is_shadowed = is_occluded(reciever, shadow_ray, intersection);
		
		if (is_shadowed == false)
		{
			//light calculation here
			Colour temp = reciever.surface_colour(intersection, *(*light), scene.scene_camera().location());
			colour += temp * temp;
		}
		else
		{
			//shadow calculation here
			Colour temp = reciever.shadow_colour(*(*light));
			base = temp * temp;
		}
	}
	return colour + base;
}
bool Tracer::is_occluded(const Object& reciever, const Ray& shadow_ray, const Point& p) const
{
	bool ret = false;
	for (std::vector<Object*>::const_iterator occluder = scene.objects.begin(); occluder != scene.objects.end(); occluder++)
	{
		if ((*occluder)->intersects(shadow_ray) == true && Utility::almost_equals((*occluder)->intersection(shadow_ray), p) == false)
		{
			ret = true;
			break;
		}
	}
	return ret;
}
void Tracer::trace(const Object& reciever, const Ray& r)
{
	//the pixel exists at the end of the ray (which is the focal length)
	Point pixel = r.cast(scene.scene_camera().f_length());
	Point inter = reciever.intersection(r);
	Colour ret;
	if (inter != (Point) NULL && image->test_depth_at(pixel, glm::distance(inter, scene.scene_camera().location())) == true)
	{
		image->set_depth_at(pixel, glm::distance(inter, scene.scene_camera().location()));
		trace_colour(reciever, inter);
	}
}