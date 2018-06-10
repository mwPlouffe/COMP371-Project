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
	Scene *scene = new Scene();
	std::cout << "MESSAGE: Loading entities from file" << std::endl;
	try
	{
		scene->construct("./scenes/scene5.txt");
	}
	catch (IOException &ex)
	{
		std::cout << ex.what() << std::endl;
		return -1;
	}
	Image<double>* output_image = new Image<double>(static_cast<long>(scene->scene_camera().image_width()), static_cast<long>(scene->scene_camera().image_height()), 4, scene->scene_camera().f_length());
	
	//a) strip out the objects and lights from the entity list

	//2. 'cast' a ray through each pixel to the scene
	std::cout << "MESSAGE: Starting to Render Image" << std::flush;
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
			//starts at the camera position and exists from t[0,inf[ along the vector dir, which is the direction from camera to pixel
			//the pixel location is simply x,y,focal_length
			direction = scene->scene_camera().location() - Point(x, y,scene->scene_camera().f_length());
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
						colour = Colour();
						base = (*reciever)->ambient_colour();
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
								colour +=  (*reciever)->surface_colour(inter, *(*light), scene->scene_camera().location());
							}
							else
							{
								double frac = 0.7;
								double dark = 0.9;
								base = dark * ( frac * base + (1.0 - frac) * Colour(0.0, 0.0, 0.0001));
							}
						}
						//colour = glm::clamp(base + colour, 0.0, 1.0);
						//colour *= std::numeric_limits<double>::max();
						output_image->set_colour_at(pixel, base, colour, 1);
					}
				}
				
			}
			if ((std::abs(x) + std::abs(y) * output_image->get_image_height()) % (static_cast<long>(pixels * 0.05)) == 0 )
			{
				std::cout << "." << std::flush;
			}
		}
	}
	std::cout << "\nMESSAGE: Rendering Complete. Saving To file: ./output.bmp" << std::endl;
	try
	{
		output_image->save_image_to_file("./output.bmp");
		int status = system("open -a /Applications/Preview.app ./output.bmp");
		
		if (status == -1)
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
		std::cout << "ERROR: "<< ex.what() << std::endl;
		return -1;
	}
	std::cout << "MESSAGE: Execution Successful. Exiting.." << std::endl;
	
	return 0;
}
