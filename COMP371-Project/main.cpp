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
	CImg <float> *output_image;
	std::cout << "MESSAGE: Loading entities from file" << std::endl;
	try
	{
		Utility::load_entities(entities, "./scenes/scene1.txt");
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
	
	output_image = new CImg<float>(static_cast<int>(c->image_width()), static_cast<int>(c->image_height()), 1, 4, 0);
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
	for (int x = -1 * output_image->width()/2 ; x < output_image->width()/2; x++)
	{
		for (int y = -1 * output_image->height()/2; y < output_image->height()/2; y++)
		{
			//starts at the camera position and exists from t[0,inf[ along the vector dir, which is the direction from camera to pixel
			//the pixel location is simply x,y,focal_length
			direction = Point(x,y,c->f_length()) - c->location();
			//std::cout << "Ray Direction: " << Utility::display(direction) << std::endl;
			r = Ray(c->location(), -1.0 * direction);
			pixel = Point(x + static_cast<int>(output_image->width()/2), y + static_cast<int>(output_image->height()/2), 0.0);
			
			//3. determine if the ray intersects an object
			std::vector<Object*>::iterator reciever;
			for (reciever = objects.begin(); reciever != objects.end(); reciever++)
			{
				//4. if there is an intersection, determine if the surface is lit
				if ((*reciever)->intersects(r))
				{
					Point inter = (*reciever)->intersection(r);
					//std::cout << "Object-Ray Intersection: " << Utility::display(inter) << std::endl;
					Ray sr;
					std::vector<Light*>::iterator light;
					//	a) cast a shadow ray from intersection to each light
					//TODO: This is broken. pls fix me
					for (light = lights.begin();  light != lights.end() ; light++)
					{
						sr = Ray(inter, inter - (*light)->location());
						
						std::vector<Object*>::iterator occluder;
						//b) if there is no intersection, compute the pixel colour
						bool isShadowed = false;
						for (occluder = objects.begin(); occluder != objects.end(); occluder++)
						{
							if ((*occluder)->intersects(sr) == true)
							{
								isShadowed = true;
								break;
							}
						}
						//TODO: use the 4th colour channel to store the nearest depth buffer
						if (isShadowed == true)
						{
							//not in shadow, apply the full lighting model
							Colour colour = 256.0 * ((*reciever)->surface_colour(inter, *(*light), c->location()));
							//std::cout << "Output Colour: " << Utility::display(colour) << std::endl;
							set_lighting(*output_image, colour, pixel);
							colour = 256.0 * glm::clamp((*reciever)->ambient_colour() * (*light)->light_colour(), 0.0, 1.0);
							add_lighting(*output_image, colour, pixel);
						}
						else
						{
							//is in shadow, apply ambient only
							Colour colour = 256.0 * glm::clamp((*reciever)->ambient_colour() * (*light)->light_colour(), 0.0, 1.0);
							set_lighting(*output_image, colour, pixel);
							isShadowed = false;
						}
					}
				}
			}
		}
		
	}
	std::cout << "MESSAGE: Rendering Complete. Saving To file: ./output.bmp" << std::endl;
	output_image->save("./output.bmp");
	std::cout << "MESSAGE: Saving Complete. Exiting.." << std::endl;
	return 0;
}
void add_lighting(cimg_library::CImg<float>& image, const Colour& additive, const Point& pixel)
{
	image(pixel.x, pixel.y, 0, 0) +=additive.r;
	image(pixel.x, pixel.y, 0, 1) +=additive.g;
	image(pixel.x, pixel.y, 0, 2) +=additive.b;
	image(pixel.x, pixel.y, 0, 3) =pixel.z;
}
void set_lighting(cimg_library::CImg<float>& image, const Colour& base, const Point& pixel)
{
	image(pixel.x, pixel.y, 0, 0) =base.r;
	image(pixel.x, pixel.y, 0, 1) =base.g;
	image(pixel.x, pixel.y, 0, 2) =base.b;
	image(pixel.x, pixel.y, 0, 3) =pixel.z;
}
