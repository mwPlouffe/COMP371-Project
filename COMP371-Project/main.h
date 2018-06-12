//
//  main.h
//  COMP371-Project
//
//  Created by Mathew Plouffe on 2018-06-05.
//  Copyright © 2018 Mathew Plouffe. All rights reserved.
//

#ifndef main_h
#define main_h

#include "Libraries.h"
#include "Utility.h"
#include "Image.h"
#include "Image.cpp"
#include "Scene.h"

#define MAX_RAYS 16
#define SAMPLE_RADIUS 1
#define DARK_FRAC 0.7
#define BASE_FRAC 0.4
#define NOISE_RANGE 1.0

#define cimg_display 0

std::string renderer_settings(void);
#include "CImg.h"
#endif /* main_h */
