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
	std::cout << "MESSAGE:" << renderer_settings() << std::endl;
	Scene *scene = new Scene();
	std::cout << "MESSAGE: Loading entities from file" << std::endl;
	try
	{	//1. construct the scene from file
		//1. a) strip out the objects and lights from the entity list
		scene->construct("./scenes/scene5.txt");
	}
	catch (IOException &ex)
	{
		std::cout << ex.what() << std::endl;
		return -1;
	}
	Image<double>* output_image = new Image<double>(static_cast<long>(scene->scene_camera().image_width()), static_cast<long>(scene->scene_camera().image_height()), 4, scene->scene_camera().f_length());
	
	//2. 'cast' a ray through each pixel to the scene
	std::cout << "MESSAGE: Starting to Render Image" << std::endl;
	std::cout << "MESSAGE: Rendering in Progress" << std::flush;
	Vector direction;
	Point pixel;
	double pixels = output_image->get_image_width() * output_image->get_image_height();
	Ray r;
	Colour colour;
	Colour base;
	for (int x = -1 * output_image->get_image_width()/2 ; x < output_image->get_image_width()/2; x++)
	{
		for (int y = -1 * output_image->get_image_height()/2; y < output_image->get_image_height()/2; y++)
		{
			Colour aggregate_colour_base(0.0);
			Colour aggregate_colour_light(0.0);
			for (int i = 0; i < MAX_RAYS; i++)
			{
				int x_noise = NOISE_RANGE * (std::rand()/ RAND_MAX - 0.5);
				int y_noise = NOISE_RANGE * (std::rand()/ RAND_MAX - 0.5);
				colour  = Colour(0.0);
				base	= Colour(0.0);
				//starts at the camera position and exists from t[0,inf[ along the vector dir, which is the direction from camera to pixel
				//the pixel location is simply x,y,focal_length
				direction = scene->scene_camera().location() - Point(x + x_noise, y + y_noise, scene->scene_camera().f_length());
				//std::cout << "Ray Direction: " << Utility::display(direction) << std::endl;
				r = Ray(scene->scene_camera().location(), direction);
				pixel = Point(x + static_cast<int>(output_image->get_image_width()/2), y + static_cast<int>(output_image->get_image_height()/2), scene->scene_camera().f_length());
				
				//3. determine if the ray intersects an object
				std::vector<Object*>::iterator reciever;
				for (reciever = scene->objects.begin(); reciever != scene->objects.end(); reciever++)
				{
					//4. if there is an intersection, determine if the surface is lit
					if ((*reciever)->intersects(r))
					{
						Point inter = (*reciever)->intersection(r);
						//std::cout << "Object-Ray Intersection: " << Utility::display(inter) << std::endl;
						
						//4. a) if the object is closer to the camera than the currently stored depth, compute the colour
						if (output_image->test_depth_at(pixel, glm::distance(inter, scene->scene_camera().location())) == true)
						{
							output_image->set_depth_at(pixel, glm::distance(inter, scene->scene_camera().location()));
							
							//4. b) is the light going to the intersect obstructed by other objects in the scene?
							std::vector<Light*>::iterator light;
							Ray shadow_ray;
							base = (*reciever)->ambient_colour();
							colour = Colour();
							for (light = scene->lights.begin(); light != scene->lights.end(); light++)
							{
								shadow_ray = Ray(inter,(*light)->location() - inter);
								std::vector<Object*>::iterator occluder;
								bool is_shadowed = false;
								for (occluder = scene->objects.begin(); occluder != scene->objects.end(); occluder++)
								{
									if ((*occluder)->intersects(shadow_ray) == true && Utility::almost_equals((*occluder)->intersection(shadow_ray), inter) == false)
									{
										is_shadowed = true;
										break;
									}
								}
								if (is_shadowed == false)
								{
									colour  += Utility::pow(((*reciever)->surface_colour(inter, *(*light), scene->scene_camera().location())), 2.0);
									base	+= Utility::pow(base * (*light)->light_colour(), 2.0);
								}
								else
								{
									base	+=  Utility::pow(((1.0 - DARK_FRAC) * ( BASE_FRAC * (*reciever)->ambient_colour() + (1.0 - BASE_FRAC) * Colour(0.0, 0.0, 1.0))), 2);
								}
							}
							
						}
					}
				}
				aggregate_colour_base += base;
				aggregate_colour_light+= colour;
			}
			output_image->set_colour_at(pixel, aggregate_colour_base, aggregate_colour_light, MAX_RAYS);
			
			if ((std::abs(x) + std::abs(y) * output_image->get_image_height()) % (static_cast<long>(pixels * 0.05)) == 0 )
			{
				std::cout << "." << std::flush;
			}
		}
	}
	std::cout << "\nMESSAGE: Starting to apply Post-Rendering MSAA to image" <<std::endl;
	output_image->anti_alias(SAMPLE_RADIUS);
	output_image->normalise(std::numeric_limits<double>::max());
	std::cout << "\nMESSAGE: Post-Rendering MSAA complete." <<std::endl;
	std::cout << "MESSAGE: Rendering Complete. Saving To file: ./results/output.bmp" << std::endl;
	try
	{
		output_image->save_image_to_file("./results/output.bmp");
		int status1 = system("open -a /Applications/Preview.app ./scenes/scene5.bmp");
		int status  = system("open -a /Applications/Preview.app ./results/output.bmp");
		
		if (status == -1 || status1 == -1)
		{
			throw IOException("There was a problem opening the file.");
		}
	}
	catch (CImgException &ex)
	{
		std::cout << "ERROR: cannot save the image to file" << std::endl;
		return -1;
	}
	catch(IOException &ex)
	{
		std::cout << ex.what() << std::endl;
		return -1;
	}
	std::cout << "MESSAGE: Execution Successful. Exiting.." << std::endl;
	
	return 0;
}

std::string renderer_settings(void)
{
	std::stringstream ss;
	ss  << "Current Image settings" << "\n--------------------"
		<< "\n\tImage Gamma: " << GAMMA
		<< "\n\tLight Brightness: " << BASE_LIGHT_INTENSITY
		<< "\nShadow Settings" << "\n\tDarkness: " << DARK_FRAC << "\n\tBase Colour Fraction: " << BASE_FRAC << "\n\tLighting Intensity Factor: " << GLOBAL_INTENSITY
		<< "\nAnti-Aliasing Settings" << "\n\tSamples per pixel: " << MAX_RAYS << "\n\tSampling Noise:" << NOISE_RANGE <<"\n\tMSAA Radius: " << SAMPLE_RADIUS << "\n--------------------";
	return ss.str();
}
