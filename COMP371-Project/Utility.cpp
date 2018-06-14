//
//  Utility.cpp
//  COMP371-Project
//
//  Created by Mathew Plouffe on 2018-06-04.
//  Copyright © 2018 Mathew Plouffe. All rights reserved.
//

#include "Utility.h"
static const std::vector<std::string> light_property_ids =
{
	"amb",
	"dif",
	"spe",
};
static const std::vector<std::string> camera_property_ids =
{
	"f",
	"fov",
	"a",
};
static const std::vector<std::string> position_property_ids =
{
	"pos",
};
static const std::vector<std::string> colour_property_ids =
{
	"col",
};
static const std::vector<std::string> entity_ids =
{
	"sphere",
	"plane",
	"camera",
	"light",
};
void Utility::load_entities(std::map<std::string, Entity*>& entities, char* filepath)
{
	FILE *fp = fopen(filepath, "r");
	int counter = 0;
	if (fp == NULL)
	{
		std::stringstream error;
		error << "Utility::load_entities | line 43 | The file at path: " << filepath << " could not be opened or found";
		throw IOException(error.str());
	}
	char id[100];
	int result = -1;
	Colour ambient,specular,diffuse,light_colour;
	Point position;
	double shininess;
	int count;
	fscanf(fp, "%d\n", &count);
	std::cout << "MESSAGE: Loading " << count << " entities from file" << std::endl;
	
	while ((result = fscanf(fp, "%s\n", &id)) != EOF)
	{
		if (result != 1)
		{
			std::stringstream error;
			error << "Utility::load_entities | line 61 | The entity header: could not be read in from the file";
			throw IOException(error.str());
			return;
		}
		if (std::string(id).compare(entity_ids[0]) == 0)
		{
			std::cout << "MESSAGE: Loading a sphere entity from file.." << std::endl;
			double radius;
			load_position_properties(position, fp);
			load_single_param(radius,"rad:", fp);
			load_light_properties(ambient, diffuse, specular, fp);
			load_single_param(shininess, "shi:", fp);
			std::stringstream ss;
			ss << "Sphere" << counter++;
			entities[ss.str()] = new Sphere(position,
										  radius,
										  Material(ambient, diffuse, specular, shininess));
		}
		else if (std::string(id).compare(entity_ids[1]) == 0)
		{
			std::cout << "MESSAGE: Loading a plane entity from file.." << std::endl;
			Vector normal;
			load_vector_param(normal, "nor:", fp);
			load_position_properties(position, fp);
			load_light_properties(ambient, diffuse, specular, fp);
			load_single_param(shininess, "shi:", fp);
			std::stringstream ss;
			ss << "Plane" << counter++;
			entities[ss.str()] = new Plane(position,
											normal,
											Material(ambient, diffuse, specular, shininess));
		}
		else if (std::string(id).compare(entity_ids[2]) == 0)
		{
			std::cout << "MESSAGE: Loading a camera entity from file.." << std::endl;
			load_position_properties(position, fp);
			double f,fov,a;
			load_single_param(fov, "fov:", fp);
			load_single_param(f, "f:", fp);
			load_single_param(a, "a:", fp);
			std::stringstream ss;
			ss << "Camera";

			entities[ss.str()] = new Camera(f,
										  fov,
										  a,
										  position);
		}
		else if (std::string(id).compare(entity_ids[3]) == 0)
		{
			std::cout << "MESSAGE: Loading a light entity from file.." << std::endl;
			load_position_properties(position, fp);
			load_light_colour_properties(light_colour, fp);
			std::stringstream ss;
			ss << "Light" << counter++;
		#ifndef AREA_LIGHTS
			entities[ss.str()] = new Light(light_colour, position);
		#else
			entities[ss.str()] = new AreaLight(light_colour, position, 1.0);
		#endif
		}
		else
		{
			std::stringstream error;
			error << "Utility::load_entities | line 112 | The header: " << id << " does not exist in the file";
			throw IOException(error.str());
		}
	}
	
}
void Utility::load_light_properties(Colour& ambient, Colour& diffuse, Colour& specular, FILE *fp)
{
	double val1, val2, val3;
	char id[100];
	int result = -1;
	
	for (int i = 0 ; i < 3; i++)
	{
		result = fscanf(fp, "%s %lf %lf %lf\n", &id, &val1, &val2, &val3);
		if (result != 4)
		{
			std::stringstream error;
			error << "Utility::load_entities | line 130 | The light properties were not correctly loaded from file";
			throw IOException(error.str());
		}
		if (std::string(id).compare(light_property_ids[0] + ":") == 0)
		{
			ambient = Colour(val1, val2, val3);
		}
		else if (std::string(id).compare(light_property_ids[1] + ":") == 0)
		{
			diffuse = Colour(val1, val2, val3);
		}
		else if (std::string(id).compare(light_property_ids[2] + ":") == 0)
		{
			specular = Colour(val1, val2, val3);
		}
		else
		{
			std::stringstream error;
			error << "Utility::load_entities | line 148 | The light property " << id << " does not exist";
			throw IOException(error.str());
		}
	}
}
void Utility::load_position_properties(Point& location, FILE *fp)
{
	double val1, val2, val3;
	char id[100];
	int result = -1;

	result = fscanf(fp, "%s %lf %lf %lf\n", &id, &val1, &val2, &val3);
	if (result != 4)
	{
		std::stringstream error;
		error << "Utility::load_entities | line 163 | The position properties were not correctly loaded from file";
		throw IOException(error.str());
	}
	if (std::string(id).compare(position_property_ids[0] + ":") == 0)
	{
		location = Point(val1, val2, val3);
	}
	else
	{
		std::stringstream error;
		error << "Utility::load_entities | line 173 | The position property " << id << " does not exist";
		throw IOException(error.str());
	}
	
}
void Utility::load_camera_properties(double& focal, double& fov, double& aspect, FILE *fp)
{
	double val;
	char id[100];
	for (int i = 0 ; i < 3; i++)
	{
		int result = fscanf(fp, "%s: %lf\n", &id, &val);
		if (result != 2)
		{
			std::stringstream error;
			error << "Utility::load_entities | line 188 | The camera properties were not correctly loaded from file";
			throw IOException(error.str());
		}
		if (std::string(id).compare(camera_property_ids[0] + ":") == 0)
		{
			fov = val;
		}
		else if (std::string(id).compare(camera_property_ids[1] + ":") == 0)
		{
			focal = val;
		}
		else if (std::string(id).compare(camera_property_ids[2] + ":") == 0)
		{
			aspect = val;
		}
		else
		{
			std::stringstream error;
			error << "Utility::load_entities | line 206 | The camera property " << id << " does not exist";
			throw IOException(error.str());
		}
	}
}
void Utility::load_light_colour_properties(Colour& light_colour , FILE *fp)
{
	double val1, val2, val3;
	char id[100];
	int result = -1;
	result = fscanf(fp, "%s %lf %lf %lf\n", &id, &val1, &val2, &val3);
	
	if (result != 4)
	{
		std::stringstream error;
		error << "Utility::load_entities | line 221 | The camera properties were not correctly loaded from file";
		throw IOException(error.str());
	}
	if (std::string(id).compare(colour_property_ids[0] + ":") == 0)
	{
		light_colour = Colour(val1, val2, val3);
	}
	else
	{
		std::stringstream error;
		error << "Utility::load_entities | line 231 | The light colour property " << id << " does not exist";
		throw IOException(error.str());
	}
}
void Utility::load_single_param(double& param, char* identifier, FILE *fp)
{
	char id[100];
	double temp;
	int result = -1;
	result = fscanf(fp, "%s %lf\n", &id, &temp);
	
	if (result != 2)
	{
		std::stringstream error;
		error << "Utility::load_entities | line 245 | The single parameter properties were not correctly loaded from file";
		throw IOException(error.str());
	}
	if (std::string(id).compare(std::string(identifier)) == 0)
	{
		param = temp;
	}
	else
	{
		std::stringstream error;
		error << "Utility::load_entities | line 255 | The single parameter property " << id << " does not exist. Expected Identifier was: " << identifier;
		throw IOException(error.str());
	}
}
void Utility::load_vector_param(glm::dvec3& param, char* identifier, FILE *fp)
{
	char id[100];
	double val1, val2, val3;
	int result = -1;
	result = fscanf(fp, "%s %lf %lf %lf\n", &id, &val1, &val2, &val3);
	
	if (result != 4)
	{
		std::stringstream error;
		error << "Utility::load_entities | line 269 | The vector parameter properties were not correctly loaded from file";
		throw IOException(error.str());
	}
	if (std::string(id).compare(std::string(identifier)) == 0)
	{
		param = glm::vec3(val1, val2, val3);
	}
	else
	{
		std::stringstream error;
		error << "Utility::load_entities | line 279 | The single parameter property " << id << " does not exist. Expected Identifier was: " << identifier;
		throw IOException(error.str());
	}
}
bool Utility::almost_equals(const Point& p, const Point& q)
{
	Point result = p - q;
	result.x = std::abs(result.x);
	result.y = std::abs(result.y);
	result.z = std::abs(result.z);
	
	return (   (result.x < 0.001)
			&& (result.y < 0.001)
			&& (result.z < 0.001)
			);
}
glm::dvec3 Utility::pow(const glm::dvec3& vec, double power)
{
	glm::dvec3 vec1(0.0);

	vec1.x = powl(vec.x, power);
	vec1.y = powl(vec.y, power);
	vec1.z = powl(vec.z, power);
	
	return vec1;
}
