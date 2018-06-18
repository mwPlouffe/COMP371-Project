//
//  Material.h
//  COMP371-Project
//
//  Created by Mathew Plouffe on 2018-05-31.
//  Copyright © 2018 Mathew Plouffe. All rights reserved.
//

#ifndef Material_h
#define Material_h

#include "Libraries.h"
#include "Light.h"

#ifdef TRACER_UNIFORM
#define GLOBAL_INTENSITY 1.0
#elif defined(TRACER_RANDOM)
#define GLOBAL_INTENSITY 1.0
#else
#define GLOBAL_INTENSITY 1.0
#endif

class Material
{
	//this class implements the necessary logic for a material that can be lit according to the phong lighting model
	Colour ambient;
	Colour diffuse;
	Colour specular;
	float  shininess;
	
	Vector reflect(const Vector& v, const Vector& n) const;
	public:
	Material(void);
	Material(Colour ambi, Colour diff, Colour spec, float shiny);
	
	//calculate the colour based on the lighting
	Colour calculate_colour(const Vector& view, const Vector& normal, const Vector& light_direction, const Colour& light_colour) const;
	
	//calculate the colour based on the
	Colour calculate_shadow_colour(const Colour& light_colour) const;
	inline std::string to_string(void)
	{
		std::stringstream ss;
		ss << "Material Properties:\n" <<
			  "\t\tAmbient Colour: (" << ambient.r << ", " << ambient.g << ", " << ambient.b << ")\n" <<
			  "\t\tDiffuse Colour: (" << diffuse.r << ", " << diffuse.g << ", " << diffuse.b << ")\n" <<
			  "\t\tSpecular Colour: (" << specular.r << ", " << specular.g << ", " << specular.b << ")\n" <<
			  "\t\tShininess: " << shininess;
		return ss.str();
	}
	inline Colour ambient_colour(void) const
	{
		return ambient;
	}
};


#endif /* Material_h */
