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
	std::mutex m;
	std::vector<std::thread*> threads;
	int x_min = 0;
	int step = static_cast<int>(output_image->get_image_width() / (1.0 * THREAD_MAX));
	for (int i = 0; i < THREAD_MAX; i++)
	{
		Point pixel_start	(x_min,			0, 0.0);
		Point pixel_end		(x_min + step,  output_image->get_image_height(), 0.0);
		std::stringstream ss;
		ss << "Thread: " << i;
		std::string identifier(ss.str());
		try
		{
			threads.push_back(
							  new std::thread (
										   [&output_image, &scene, pixel_start, pixel_end, identifier, &m]()
										   {
											   render_range(*output_image, *scene, pixel_start, pixel_end, identifier, m);
										   }
										   ));
		}
		catch (std::exception &ex)
		{
			std::cout << ex.what() << std::endl;
			return -1;
		}
		x_min+=step;
	}
		std::vector<std::thread*>::iterator thread;
		for (thread = threads.begin(); thread != threads.end(); thread++)
		{
			if ((*thread)->joinable() == true)
			{
				(*thread)->join();
			}
		}
#ifdef SAMPLE_RADIUS
	std::cout << "MESSAGE: Starting to apply Post-Rendering MSAA to image" <<std::endl;
	output_image->anti_alias(SAMPLE_RADIUS);
	std::cout << "\nMESSAGE: Post-Rendering MSAA complete." <<std::endl;
#endif
	output_image->normalise(std::numeric_limits<double>::max());
	
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
	ss  << "Current Image settings" << "\n--------------------";
	
#ifdef GAMMA
	ss << "\n\tImage Gamma: " << GAMMA;
#endif
#ifdef BASE_LIGHT_INTENSITY
	ss << "\n\tLight Brightness: " << BASE_LIGHT_INTENSITY;
#endif
	ss << "\nShadow Settings";
#ifdef DARK_FRAC
	ss << "\n\tDarkness: " << DARK_FRAC;
#endif
#ifdef BASE_FRAC
	ss << "\n\tBase Colour Fraction: " << BASE_FRAC;
#endif
	ss << "\n\tLighting Intensity Factor: " << GLOBAL_INTENSITY << "\nAnti-Aliasing Settings" << "\n\tSamples per pixel: " << MAX_RAYS;
#ifdef SAMPLE_RADIUS
	ss << "\n\tSampling Noise:" << NOISE_RANGE <<"\n\tMSAA Radius: " << SAMPLE_RADIUS;
#endif
	ss << "\n--------------------";
	return ss.str();
}
template <class T>
void render_range(Image<T>& image, const Scene& scene, const Point& pixel_start, const Point& pixel_end, const std::string identifier, std::mutex& m)
{
	m.lock();
	std::cout << "MESSAGE: Starting to Render Image on " << identifier << std::endl;
	std::cout << "MESSAGE: Rendering in Progress on " << identifier << std::endl;
	m.unlock();
	Point start(pixel_start.x - static_cast<int>(image.get_image_width() / 2.0), pixel_start.y - static_cast<int>(image.get_image_height() / 2.0), 0.0);
	Point end(pixel_end.x - static_cast<int>(image.get_image_width() / 2.0), pixel_end.y - static_cast<int>(image.get_image_height() / 2.0), 0.0);
	Vector direction;
	Point pixel;
	Ray r;
	Colour colour;
	Colour base;
	for (int x = start.x; x < end.x; x++)
	{
		for (int y = start.y; y < end.y; y++)
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
				direction = scene.scene_camera().location() - Point(x + x_noise, y + y_noise, scene.scene_camera().f_length());
				//std::cout << "Ray Direction: " << Utility::display(direction) << std::endl;
				r = Ray(scene.scene_camera().location(), direction);
				pixel = Point(x + image.get_image_width()/2,
							  y + image.get_image_height()/2,
							  scene.scene_camera().f_length());
				
				//3. determine if the ray intersects an object
				std::vector<Object*>::const_iterator receiver;
				for (receiver = (scene.objects).begin(); receiver != scene.objects.end(); receiver++)
				{
					//4. if there is an intersection, determine if the surface is lit
					if ((*receiver)->intersects(r))
					{
						Point inter = (*receiver)->intersection(r);
						//std::cout << "Object-Ray Intersection: " << Utility::display(inter) << std::endl;
						
						//4. a) if the object is closer to the camera than the currently stored depth, compute the colour
						if (image.test_depth_at(pixel, glm::distance(inter, scene.scene_camera().location())) == true)
						{
							image.set_depth_at(pixel, glm::distance(inter, scene.scene_camera().location()));
							
							//4. b) is the light going to the intersect obstructed by other objects in the scene?
							std::vector<Light*>::const_iterator light;
							Ray shadow_ray;
							colour = Colour();
							base = Colour();
							for (light = scene.lights.begin(); light != scene.lights.end(); light++)
							{
								shadow_ray = Ray(inter,(*light)->location() - inter);
								std::vector<Object*>::const_iterator occluder;
								bool is_shadowed = false;
								for (occluder = scene.objects.begin(); occluder != scene.objects.end(); occluder++)
								{
									if ((*occluder)->intersects(shadow_ray) == true && Utility::almost_equals((*occluder)->intersection(shadow_ray), inter) == false)
									{
										is_shadowed = true;
										break;
									}
								}
								if (is_shadowed == false)
								{
									Colour temp = (*receiver)->surface_colour(inter, *(*light), scene.scene_camera().location());
									colour += temp;
									temp = (*receiver)->ambient_colour();
									base += temp ;
								}
								else
								{
									Colour temp = ((*receiver)->ambient_colour() * (*light)->light_colour());
									base += temp ;
								}
								
							}
						}
					}
				}
				aggregate_colour_light	+= colour;
				aggregate_colour_base	+= base;
			}
			image.set_colour_at(pixel, aggregate_colour_light + aggregate_colour_base, MAX_RAYS);
		}
	}
	m.lock();
	std::cout << "MESSAGE: Rendering Complete on " << identifier << std::endl;
	m.unlock();
	return;
}
