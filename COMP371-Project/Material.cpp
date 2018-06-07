//
//  Material.cpp
//  COMP371-Project
//
//  Created by Mathew Plouffe on 2018-06-04.
//  Copyright © 2018 Mathew Plouffe. All rights reserved.
//

#include "Material.h"

Material::Material(void)
{
	ambient   = Colour(0.0f);
	diffuse   = Colour(0.0f);
	specular  = Colour(0.0f);
	shininess = 1.f;
}
Material::Material(Colour ambi, Colour diff, Colour spec, float shiny)
{
	ambient   = ambi;
	diffuse   = diff;
	specular  = spec;
	shininess = shiny;
}
Vector Material::reflect(const Vector& v, const Vector& n) const
{
	//reflects the vector about the provided normal
	return v - (2.0 * glm::dot(n,v) * n);
}
Colour Material::calculate_colour(const Vector& view, const Vector& normal, const Vector& light_direction, const Colour& light_colour) const
{
	//returns the colour of the light of the material, given the normal of the surface, the view direction and the light & colour
	//uses phong lighting to accomplish this
	Vector reflection = glm::normalize(reflect(-1.0 * light_direction,normal));
	
	Colour ret = light_colour * (
								 (std::max(0.0, glm::dot(normal, light_direction)) * this->diffuse) +
								 (pow(std::max(0.0, (double) glm::dot(view, reflection)), this->shininess) * this->specular) //+
								 //this->ambient
								 );
	//glm::clamp(ret.r, 0.0, 1.0);
	//glm::clamp(ret.g, 0.0, 1.0);
	//glm::clamp(ret.b, 0.0, 1.0);
	
	return ret;
}
