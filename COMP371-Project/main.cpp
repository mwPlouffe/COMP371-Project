//
//  main.cpp
//  COMP371-Project
//
//  Created by Mathew Plouffe on 2018-05-31.
//  Copyright © 2018 Mathew Plouffe. All rights reserved.
//
#include "main.h"

using namespace cimg_library;

int main(int argc, char* argv[])
{
	argc = 3;
	//read in a file path to be processed
	if (argc != 3)
	{
		std::cout << "ERROR: please provide only one output and one input path at a time" << std::endl;
		return -1;
	}
	//std::string fp(argv[1]);
	//std::string save_fp(argv[2]);
	std::string fp("./scenes/scene5.txt");
	std::string save_fp("./results/scene5-gen.bmp");
	
	std::cout << "MESSAGE:" << renderer_settings() << std::endl;
	Scene *scene = new Scene();
	std::cout << "MESSAGE: Loading entities from file" << std::endl;
	try
	{	//1. construct the scene from file
		//1. a) strip out the objects and lights from the entity list
		scene->construct(fp.c_str());
	}
	catch (IOException &ex)
	{
		std::cout << ex.what() << std::endl;
		return -1;
	}
	//create an image object from the dimensions specified
	Image<double>* output_image = new Image<double>(static_cast<long>(scene->scene_camera().image_width()), static_cast<long>(scene->scene_camera().image_height()), 4, scene->scene_camera().f_length());
	
	//threading support - need a lock and a std::vector of threads
	std::mutex m;
	std::vector<std::thread*> threads;
	int x_min = 0;
	
	//create the step size based upon the number of threads
	int step = static_cast<int>(output_image->get_image_width() / (1.0 * THREAD_MAX));
	for (int i = 0; i < THREAD_MAX; i++)
	{
		//correct the last thread to ensure the entire image is traced
		if (i == THREAD_MAX -1 && x_min + step < output_image->get_image_width())
		{
			step = output_image->get_image_width() - x_min;
		}
		//specify the start and end points for thread
		Point pixel_start	(x_min,			0,									0.0);
		Point pixel_end		(x_min + step,  output_image->get_image_height(),	0.0);
		std::stringstream ss;
		ss << "Thread: " << i;
		std::string identifier(ss.str());
		//generate the thread with a lambda function that calls the render-range function
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
		//catch any threading exceptions
		catch (std::exception &ex)
		{
			std::cout << ex.what() << std::endl;
			return -1;
		}
		x_min+=step;
	}
	//wait for the threads to complete
		std::vector<std::thread*>::iterator thread;
		for (thread = threads.begin(); thread != threads.end(); thread++)
		{
			if ((*thread)->joinable() == true)
			{
				(*thread)->join();
			}
		}
#ifdef SAMPLE_RADIUS
	//post processing MSAA, generally disabled
	std::cout << "MESSAGE: Starting to apply Post-Rendering MSAA to image" <<std::endl;
	output_image->anti_alias(SAMPLE_RADIUS);
	std::cout << "\nMESSAGE: Post-Rendering MSAA complete." <<std::endl;
#endif
	//normalise to the type of data stored in the image
	output_image->normalise(std::numeric_limits<double>::max());
	
	std::cout << "MESSAGE: Rendering Complete. Saving To file: " << save_fp << std::endl;
	//save the file and open the file.
	try
	{
		output_image->save_image_to_file(save_fp);
		
		//this is janky and shouldn't be done
//		int status  = system("open -a /Applications/Preview.app ./results/output.bmp");
//		
//		if (status == -1)
//		{
//			throw IOException("There was a problem opening the file.");
//		}
	}
	//catch any exceptions generated in the process.
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
//the renderer range function, lists any and all active preprocessor directives
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
	ss << "\n\tLighting Intensity Factor: " << GLOBAL_INTENSITY << "\nAnti-Aliasing Settings" << "\n\tSamples per pixel: " << MAX_RAYS << "\n\tSampling Noise:" << NOISE_RANGE;
#ifdef SAMPLE_RADIUS
	ss <<"\n\tMSAA Radius: " << SAMPLE_RADIUS;
#endif
	ss << "\n--------------------";
	return ss.str();
}
//the rendering function for each range
template <class T>
void render_range(Image<T>& image, const Scene& scene, const Point& pixel_start, const Point& pixel_end, const std::string identifier, std::mutex& m)
{
	//be sure to lock the shared resources, otherwise chaose ensues.
	m.lock();
	std::cout << "MESSAGE: Rendering in Progress on " << identifier << std::endl;
	m.unlock();
	
	//take the image out of world coordinates and into image coordinates
	Point start(pixel_start.x - static_cast<int>(image.get_image_width() / 2.0), pixel_start.y - static_cast<int>(image.get_image_height() / 2.0), 0.0);
	Point end(pixel_end.x - static_cast<int>(image.get_image_width() / 2.0), pixel_end.y - static_cast<int>(image.get_image_height() / 2.0), 0.0);
	Point pixel;
	//create a tracer for the scene
	Tracer t(MAX_RAYS, NOISE_RANGE, scene, image);
	for (int x = start.x; x < end.x; x++)
	{
		for (int y = start.y; y < end.y; y++)
		{
			//generate pixel to be passed to the tracer
			pixel = Point(x, y, scene.scene_camera().f_length());
			
			//trace the image using the appropriate AA algorithm
#ifdef TRACER_UNIFORM
			t.trace_uniform(pixel);
#elif defined(TRACER_RANDOM)
			t.trace_random(pixel);
#endif
		}
	}
	m.lock();
	std::cout << "MESSAGE: Rendering Complete on " << identifier << std::endl;
	m.unlock();
	return;
}
