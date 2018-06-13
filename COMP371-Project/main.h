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

#define cimg_display 0

#define THREAD_MAX 8
#define MAX_RAYS 9
//#define SAMPLE_RADIUS 1
#define NOISE_RANGE 0.0
//#define BASE_FRAC 0.9

//#define DARK_FRAC 0.9

#include <thread>
#include <mutex>

std::string renderer_settings(void);
template <class T>
void render_range(Image<T>& image, const Scene& scene, const Point& pixel_start, const Point& pixel_end, const std::string identifier, std::mutex& m);
#include "CImg.h"
#endif /* main_h */
