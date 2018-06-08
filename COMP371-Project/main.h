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

#define cimg_display 0

#include "CImg.h"
void	add_lighting(cimg_library::CImg<double>& image, const Colour& additive, const Point& pixel);
void	set_lighting(cimg_library::CImg<double>& image, const Colour& base,		const Point& pixel);
bool	depth_test(const cimg_library::CImg<double>& image, const Point& pixel, double depth);
void	set_depth(cimg_library::CImg<double>& image, const Point& pixel, double depth);
bool	almost_equals(const Point& p, const Point& q);
#endif /* main_h */
