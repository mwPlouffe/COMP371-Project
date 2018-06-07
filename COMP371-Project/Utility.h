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
#include "Camera.h"
#include "Plane.h"


#include <fstream>
#include <sstream>
#include <regex>
#include <string>

struct Utility
{
	static void load_entities(std::map<std::string,Entity*>& entities, char* filepath);
	static inline std::string display(const glm::dvec3& vector)
	{
		std::stringstream ss;
		ss << "Vector: (" << vector.x << ", " << vector.y << ", " << vector.z << ")";
		return ss.str();
	}
	
	private:
	static void load_light_properties(Colour& ambient, Colour& diffuse, Colour& specular, FILE *fp);
	static void load_position_properties(Point& location, FILE *fp);
	static void load_camera_properties(double& focal, double& fov, double& aspect, FILE *fp);
	static void load_light_colour_properties(Colour& light_colour , FILE *fp);
	static void load_single_param(double& param, char* identifier, FILE *fp);
	static void load_vector_param(glm::dvec3& param, char* identifier, FILE *fp);

};

#endif /* Utility_h */
