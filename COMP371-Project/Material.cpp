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
	shininess = 1.0;
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
	return (2.0 * glm::dot(n,v) * n) - v;
}
Colour Material::calculate_colour(const Vector& view, const Vector& normal, const Vector& light_direction, const Colour& light_colour) const
{
	//returns the colour of the light of the material, given the normal of the surface, the view direction and the light & colour
	//uses phong lighting to accomplish this
	Vector reflection = glm::normalize(reflect(light_direction,normal));
	
	Colour diff = std::max(0.0, glm::dot(normal, light_direction) ) * this->diffuse;
	Colour spec = pow( std::max(0.0, (double) glm::dot(view, reflection)), this->shininess ) * this->specular;

	//treat the ambient colour as the base colour of the material, ie do not apply lighting effects to it
	return glm::clamp((light_colour * GLOBAL_INTENSITY * (this->ambient + diff + spec)), 0.0, 1.0);
}
Colour Material::calculate_shadow_colour(const Colour& light_colour) const
{
	//blends the shadow colouring (blue) with the light and material colours to generate better shadows
	//Colour shadows = 0.99 * (this->ambient * light_colour) + 0.01 * (Colour(0.0, 0.0, 1.0));
	//return glm::clamp(shadows, 0.0, 1.0);
	return (this->ambient * light_colour);
}