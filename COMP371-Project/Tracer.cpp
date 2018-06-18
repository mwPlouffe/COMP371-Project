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
	scene(scn)
{
	//intentionally empty
}
Tracer::Tracer(const long& ps, const double& sn, const Scene& scn) :
	pixel_samples(ps),
	sample_noise(sn),
	scene(scn)
{
	//intentionally empty
}
Tracer::Tracer(const long& ps, const double& sn, const Scene& scn, Image_d& img) :
	pixel_samples(ps),
	sample_noise(sn),
	scene(scn)
{
	image = &img;
}
void Tracer::trace_uniform(const Point& pixel)
{
	//performs the tracing analogously to the trace_random, but using jittered sampling
	Point s_pixel(pixel.x + image->get_image_width()/2,
				  pixel.y + image->get_image_height()/2,
				  pixel.z);
	Point sub_pixel;
	Ray r;
	Vector direction;
	ColourMixer mixer = ColourMixer();
	ColourMixer mx;
	
	double x_off = -0.5;
	double y_off = -0.5;
	
	double step = 1.0/(double)pixel_samples;
	for (int i = 0; i < pixel_samples; i++)
	{
			double x_noise	= sample_noise * (1.0 * std::rand()/ RAND_MAX - 0.5);
			double y_noise	= sample_noise * (1.0 * std::rand()/ RAND_MAX - 0.5);
			sub_pixel		= Point(x_off + pixel.x + x_noise, y_off + pixel.y + y_noise, pixel.z);
		
			//starts at the camera position and exists from t[0,inf[ along the vector dir, which is the direction from camera to pixel
			//the pixel location is simply x,y,focal_length
			direction = scene.scene_camera().location() - sub_pixel;
			//std::cout << "Ray Direction: " << Utility::display(direction) << std::endl;
			r = Ray(scene.scene_camera().location(), direction);
			
			//3. determine if the ray intersects an object
			for (std::vector<Object*>::const_iterator receiver = (scene.objects).begin(); receiver != scene.objects.end(); receiver++)
			{
				ColourMixer cm = ColourMixer();
				if (this->trace_depth(s_pixel, *(*receiver), r, cm) == true)
				{
					mixer = cm;
				}
			}
			mx.merge(mixer);
			//colour aggregation here
			x_off+=step;
			y_off+=step;
	}
	mx.base_blend([](Colour c)
					{
						return (0.999 * (c) + 0.001 * (Colour(0.0, 0.0, 1.0)));
					}
				);
	mx.average([](Colour c, double w)
			   {
				   return c * w;
			   },
			   0.5,
			   true
				);
	image->set_colour_at(s_pixel, mx.square_average());
}
void Tracer::trace_random(const Point& pixel)
{
	Point s_pixel(pixel.x + image->get_image_width()/2,
				  pixel.y + image->get_image_height()/2,
				  scene.scene_camera().f_length());
	Ray r;
	Vector direction;
	ColourMixer mixer = ColourMixer();
	
	for (int i = 0; i < pixel_samples; i++)
	{
		double x_noise	= sample_noise * (1.0 * std::rand()/ RAND_MAX - 0.5);
		double y_noise	= sample_noise * (1.0 * std::rand()/ RAND_MAX - 0.5);
		//starts at the camera position and exists from t[0,inf[ along the vector dir, which is the direction from camera to pixel
		//the pixel location is simply x,y,focal_length
		direction = scene.scene_camera().location() - Point(pixel.x + x_noise, pixel.y + y_noise, scene.scene_camera().f_length());
		r = Ray(scene.scene_camera().location(), direction);
		
		//3. determine if the ray intersects an object
		
		for (std::vector<Object*>::const_iterator receiver = (scene.objects).begin(); receiver != scene.objects.end(); receiver++)
		{
			ColourMixer cm = ColourMixer();
			if (this->trace_depth(s_pixel, *(*receiver), r, cm) == true)
			{
				mixer = cm;
			}
		}
	}
	//shading the base colours
	image->set_colour_at(s_pixel, mixer.square_average());
}
void Tracer::trace_colour(Object& reciever, const Point& intersection, ColourMixer& cm)
{
	cm.add_base(reciever.ambient_colour());
	Ray shadow_ray;
	for (std::vector<Light*>::const_iterator light = scene.lights.begin(); light != scene.lights.end(); light++)
	{
		shadow_ray = Ray(intersection,(*light)->location() - intersection);

		bool is_shadowed = is_occluded(shadow_ray, intersection);
		
		if (is_shadowed == false)
		{
			//light calculation here
			cm.add_lighting(reciever.surface_colour(intersection, *(*light), scene.scene_camera().location()));
		}
		else
		{
			cm.add_base(0.1 * reciever.shadow_colour(*(*light)));
		}
	}
}
void Tracer::trace_colour_soft(Object& reciever, const Point& intersection, ColourMixer& cm)
{
	cm.add_base(reciever.ambient_colour());
	ColourMixer mixer = ColourMixer();
	Ray shadow_ray;
	for (std::vector<Light*>::const_iterator light = scene.lights.begin(); light != scene.lights.end(); light++)
	{
		for (int i = 0; i < LIGHT_SAMPLES; i++)
		{
			shadow_ray = Ray(intersection,(*light)->location() - intersection);
			
			bool is_shadowed = is_occluded(shadow_ray, intersection);
			
			if (is_shadowed == false)
			{
				//light calculation here
				mixer.add_lighting(reciever.surface_colour(intersection, *(*light), scene.scene_camera().location()));
			}
			else
			{
				mixer.add_base(0.1 * reciever.shadow_colour(*(*light)));
			}
		}
	}
	cm.add_base(mixer.square_average());
}
bool Tracer::is_occluded(const Ray& shadow_ray, const Point& p) const
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
bool Tracer::trace_depth(const Point& pixel, Object& reciever, const Ray& r, ColourMixer& cm)
{
	//the pixel exists at the end of the ray (which is the focal length)
	if (reciever.intersects(r) == true)
	{
		Point inter = reciever.intersection(r);
		double distance = glm::distance(inter, scene.scene_camera().location());
		if (image->test_depth_at(pixel, distance) == true)
		{
			cm = ColourMixer();
			image->set_depth_at(pixel, distance);
#ifndef AREA_LIGHTS
			trace_colour(reciever, inter, cm);
#else
		trace_colour_soft(reciever, inter, cm);
#endif
			return true;
		}
	}
	return false;
}