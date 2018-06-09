//
//  main.cpp
//  COMP371-Project
//
//  Created by Mathew Plouffe on 2018-05-31.
//  Copyright © 2018 Mathew Plouffe. All rights reserved.
//
#include "main.h"

using namespace cimg_library;

int main(void)
{
	std::map<std::string,Entity*> entities;
	cimg::exception_mode(0);
	CImg <double> *output_image;
	std::cout << "MESSAGE: Loading entities from file" << std::endl;
	try
	{
		Utility::load_entities(entities, "./scenes/scene5.txt");
	}
	catch (IOException &ex)
	{
		std::cout << ex.what() << std::endl;
		return -1;
	}
	
	//find the camera and compute the image size
	std::cout << "MESSAGE: Computing image size" << std::endl;
	Camera *c;
	if (entities.find("Camera") == entities.end())
	{
		std::cout << "ERROR: There is no camera in the scene, exiting.." <<std::endl;
		return -1;
	}
	c = dynamic_cast<Camera*>(entities.find("Camera")->second);
	//1. determine image resolution
	
	output_image = new CImg<double>(static_cast<int>(c->image_width()), static_cast<int>(c->image_height()), 1, 4, 0);
	
	//set the depth channel to a large value!
	output_image->get_shared_channel(3).fill(c->f_length());
	std::cout << "Image Resolution: (w: " << output_image->width() << ", h: " << output_image->height() << ")" << std::endl;
	
	//a) strip out the objects and lights from the entity list
	std::map<std::string,Entity*>::iterator entity;
	std::vector<Light*> lights;
	std::vector<Object*> objects;
	
	std::cout << "MESSAGE: Entity roster:" << std::endl;
	for (entity = entities.begin(); entity != entities.end(); entity++)
	{
		std::cout << entity->second->to_string() << std::endl;
		//if there is data still in the pointer, it is an object
		if (dynamic_cast<Object*>(entity->second) != NULL)
		{
			objects.push_back(dynamic_cast<Object*>(entity->second));
			//cannot be a light and an object, start next iteration
			continue;
		}
		//if there is data still in the pointer, it is a light
		if (dynamic_cast<Light*>(entity->second) !=  NULL)
		{
			lights.push_back(dynamic_cast<Light*>(entity->second));
			//cannot be a light and an object, start next iteration
			continue;
		}
	}
	//2. 'cast' a ray through each pixel to the scene
	std::cout << "MESSAGE: Starting to Render Image" << std::endl;
	Vector direction;
	Point pixel;
	Ray r;
	Colour colour;
	Colour base;
	for (int x = -1 * output_image->width()/2 ; x < output_image->width()/2; x++)
	{
		for (int y = -1 * output_image->height()/2; y < output_image->height()/2; y++)
		{
			//starts at the camera position and exists from t[0,inf[ along the vector dir, which is the direction from camera to pixel
			//the pixel location is simply x,y,focal_length
			direction = c->location() - Point(x, y, c->f_length());
			//std::cout << "Ray Direction: " << Utility::display(direction) << std::endl;
			r = Ray(c->location(), direction);
			pixel = Point(x + static_cast<int>(output_image->width()/2), y + static_cast<int>(output_image->height()/2), c->f_length());
			
			//3. determine if the ray intersects an object
			std::vector<Object*>::iterator reciever;
			for (reciever = objects.begin(); reciever != objects.end(); reciever++)
			{
				//4. if there is an intersection, determine if the surface is lit
				if ((*reciever)->intersects(r))
				{
					Point inter = (*reciever)->intersection(r);
					//std::cout << "Object-Ray Intersection: " << Utility::display(inter) << std::endl;
					
					//4. a) if the object is closer to the camera than the currently stored depth, compute the colour
					if (depth_test(*output_image, pixel, glm::distance(inter,c->location())) == true)
					{
						set_depth(*output_image, pixel, glm::distance(inter,c->location()));
						
						//4. b) is the light going to the intersect obstructed by other objects in the scene?
						std::vector<Light*>::iterator light;
						Ray shadow_ray;
						colour = Colour();
						base = (*reciever)->ambient_colour();
						for (light = lights.begin(); light != lights.end(); light++)
						{
							shadow_ray = Ray(inter,(*light)->location() - inter);
							std::vector<Object*>::iterator occluder;
							bool is_shadowed = false;
							for (occluder = objects.begin(); occluder != objects.end(); occluder++)
							{
								if ((*occluder)->intersects(shadow_ray) == true && almost_equals((*occluder)->intersection(shadow_ray), inter) == false)
								{
									is_shadowed = true;
									break;
								}
							}
							if (is_shadowed == false)
							{
								colour +=  (*reciever)->surface_colour(inter, *(*light), c->location());
							}
							else
							{
								double frac = 0.99;
								double dark = 0.7;
								base = dark * ( frac * base + (1.0 - frac) * Colour(0.0, 0.0, 0.0001));
							}
						}
						colour = glm::clamp(base + colour, 0.0, 1.0);
						colour *= std::numeric_limits<double>::max();
						set_lighting(*output_image, colour, pixel);
					}
				}
				
			}
		}
		
	}
	output_image->normalize(0.0, 255.0);
	std::cout << "MESSAGE: Rendering Complete. Saving To file: ./output.bmp" << std::endl;
	output_image->save("./output.bmp");
	std::cout << "MESSAGE: Saving Complete. Exiting.." << std::endl;
	return 0;
}
void add_lighting(cimg_library::CImg<double>& image, const Colour& additive, const Point& pixel)
{
	image(pixel.x, pixel.y, 0, 0) += additive.r;
	image(pixel.x, pixel.y, 0, 1) += additive.g;
	image(pixel.x, pixel.y, 0, 2) += additive.b;
}
void set_lighting(cimg_library::CImg<double>& image, const Colour& base, const Point& pixel)
{
	image(pixel.x, pixel.y, 0, 0) = base.r;
	image(pixel.x, pixel.y, 0, 1) = base.g;
	image(pixel.x, pixel.y, 0, 2) = base.b;
}
bool depth_test(const cimg_library::CImg<double>& image, const Point& pixel, double depth)
{
	//the object is closer is the depth test returns true, otherwise the object is farther and returns false
	return (std::abs(depth) < std::abs(image(pixel.x, pixel.y, 0, 3))) ? true : false;
}
void set_depth(cimg_library::CImg<double>& image, const Point& pixel, double depth)
{
	image(pixel.x, pixel.y, 0, 3) = depth;
}
bool almost_equals(const Point& p, const Point& q)
{
	Point result = p - q;
	result.x = std::abs(result.x);
	result.y = std::abs(result.y);
	result.z = std::abs(result.z);
	
	return (   (result.x < 0.0001)
			&& (result.y < 0.0001)
			&& (result.z < 0.0001)
		   );
}
