//
//  Utility.h
//  COMP371-Project
//
//  Created by Mathew Plouffe on 2018-05-31.
//  Copyright © 2018 Mathew Plouffe. All rights reserved.
//

#ifndef Utility_h
#define Utility_h

#include "Entity.h"
#include "Sphere.h"
#include "Light.h"
#include "AreaLight.h"
#include "Camera.h"
#include "Plane.h"

#include <fstream>
#include <sstream>
#include <regex>
#include <string>

//#define AREA_LIGHTS
#define RADIUS 10.0

//provides useful utilities for the other classes
struct Utility
{
	static void load_entities(std::map<std::string,Entity*>& entities, char* filepath);
	static inline std::string display(const glm::dvec3& vector)
	{
		std::stringstream ss;
		ss << "Vector: (" << vector.x << ", " << vector.y << ", " << vector.z << ")";
		return ss.str();
	}
	static bool almost_equals(const Point& p, const Point& q);
	static glm::dvec3 pow(const glm::dvec3& vec, double power);
	static Colour simple_average(const Colour& c1, const Colour& c2, int count);
	static Colour square_average(const Colour& c1, const Colour& c2, int count);
	private:
	static void load_light_properties(Colour& ambient, Colour& diffuse, Colour& specular, FILE *fp);
	static void load_position_properties(Point& location, FILE *fp);
	static void load_camera_properties(double& focal, double& fov, double& aspect, FILE *fp);
	static void load_light_colour_properties(Colour& light_colour , FILE *fp);
	static void load_single_param(double& param, char* identifier, FILE *fp);
	static void load_vector_param(glm::dvec3& param, char* identifier, FILE *fp);
};

#endif /* Utility_h */
