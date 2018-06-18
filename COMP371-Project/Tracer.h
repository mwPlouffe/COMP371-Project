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
#include "Image.cpp"
#include "ColourMixer.h"


/*** directives that control the AA functionality ***/
//#define TRACER_RANDOM
#define TRACER_UNIFORM
#define MAX_RAYS 16
#define NOISE_RANGE 0.1

#define LIGHT_SAMPLES 3

//traces the pixels of the images
class Tracer
{
	private:
	//const parameters for thread-safe-ness
	const long pixel_samples;
	const double sample_noise;
	const Scene scene;
	//image container to render to
	Image_d *image;
		
	public:
	//constructors
	Tracer(const Scene& scn);
	Tracer(const long& ps, const double& sn, const Scene& scn);
	Tracer(const long& ps, const double& sn, const Scene& scn, Image_d& img);
	//destructor
	virtual ~Tracer(void) = default;
	
	void trace_random(const Point& pixel);														//use random sampling for AA when tracing
	void trace_uniform(const Point& pixel);														//use grid-based sampling for AA when tracing (can be jittered through NOISE_RANGE)
	bool trace_depth(const Point& pixel, Object& reciever, const Ray& r, ColourMixer& cm);		//determines if the object is nearest to the camera and should be traced.
	void trace_colour(Object& reciever, const Point& intersection, ColourMixer& cm);			//trace the colour of a pixel, based on the lights being point lights
	void trace_colour_soft(Object& reciever, const Point& intersection, ColourMixer& cm);		//trace the colour of a pixel, based on the lights being aread lights
	
	//mutator for the image
	inline void set_image(Image_d& img)
	{
		image = &img;
	}
	private:
	
	//checks if a shadow ray hits an object from the intersection
	bool is_occluded(const Ray& shadow_ray, const Point& p) const;
};


#endif /* Tracer_h */
