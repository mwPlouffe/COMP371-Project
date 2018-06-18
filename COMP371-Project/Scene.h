//
//  Scene.h
//  COMP371-Project
//
//  Created by Mathew Plouffe on 2018-06-10.
//  Copyright © 2018 Mathew Plouffe. All rights reserved.
//

#ifndef Scene_h
#define Scene_h

#include "Libraries.h"
#include "Utility.h"
#include "Image.h"

//loads and stores the important data of the scene, to be passed around to the threads: the camera, the objects and the lights.
class Scene
{
	protected:
	Camera					camera;
	
	public:
	std::vector<Light*>		lights;
	std::vector<Object*>	objects;
	Scene(void);
	Scene(const char* filepath);
	
	//constructs the scene from the file
	void construct(const char* filepath);
	inline Camera scene_camera(void) const
	{
		return camera;
	}
	private:
	//helper functions for the construct function
	void load_entities_from_file(const char* filepath, std::map<std::string, Entity*>& entities);
	void parse_entities(std::map<std::string, Entity*> entities);
};


#endif /* Scene_h */
