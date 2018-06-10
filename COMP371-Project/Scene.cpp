//
//  Scene.cpp
//  COMP371-Project
//
//  Created by Mathew Plouffe on 2018-06-10.
//  Copyright © 2018 Mathew Plouffe. All rights reserved.
//

#include "Scene.h"

Scene::Scene(void)
{
	//intentionally empty, allows for an explicit construct call
}
Scene::Scene(const char* filepath)
{
	construct(filepath);
}
void Scene::construct(const char* filepath)
{
	std::map<std::string, Entity*> ret;
	load_entities_from_file(filepath, ret);
	parse_entities(ret);
}
void Scene::load_entities_from_file(const char* filepath, std::map<std::string, Entity*>& entities)
{
	Utility::load_entities(entities, const_cast<char*>(filepath));
}
void Scene::parse_entities(std::map<std::string, Entity*> entities)
{

	if (entities.find("Camera") == entities.end())
	{
		std::cout << "ERROR: There is no camera in the scene, exiting.." <<std::endl;
		throw IOException("There is no camera in the provided scene. Cannot compute image without camera.");
	}
	camera = *dynamic_cast<Camera*>(entities.find("Camera")->second);
	std::map<std::string,Entity*>::iterator entity;
	std::cout << "MESSAGE: Entity roster:\n--------------------------" << std::endl;
	for (entity = entities.begin(); entity != entities.end(); entity++)
	{
		std::cout << entity->second->to_string() << std::endl;
		//if there is data still in the pointer, it is an object
		if (dynamic_cast<Object*>(entity->second) != NULL)
		{
			objects.push_back(dynamic_cast<Object*>(entity->second));
			//cannot be a light and an object, start next iteration
			continue;
		}
		//if there is data still in the pointer, it is a light
		if (dynamic_cast<Light*>(entity->second) !=  NULL)
		{
#ifndef AREA_LIGHTS
			lights.push_back(dynamic_cast<Light*>(entity->second));
			//cannot be a light and an object, start next iteration
			continue;
#else
			std::vector<Light*> transfer = dynamic_cast<AreaLight*>(entity->second)->return_lights();
			for (int i = 0; i < transfer.size(); i++)
			{
				lights.push_back(transfer[i]);
			}
#endif
		}
	}
	std::cout << "--------------------------" << std::endl;

}