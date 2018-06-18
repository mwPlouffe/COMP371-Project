//
//  ColourMixer.h
//  COMP371-Project
//
//  Created by Mathew Plouffe on 2018-06-16.
//  Copyright © 2018 Mathew Plouffe. All rights reserved.
//

#ifndef ColourMixer_h
#define ColourMixer_h

#include "Libraries.h"

typedef Colour (*const blend_func)(Colour);				//combines the provided colour with the blend function
typedef Colour (*const weight_func)(Colour, double);	//weights the colour provided, to be used with the average function

//used to store and process colours for more advanced colour processing
class ColourMixer
{
	//stores the colours as base and lighting colour separately
	private:
	std::vector<Colour> base_colours;
	std::vector<Colour> lighting_colours;
	
	public:
	ColourMixer(void);
	virtual ~ColourMixer(void) = default;
	
	//adding colours to the mixer
	void add_base(const Colour& base);
	void add_lighting(const Colour& lighting);
	void merge(const ColourMixer& cm);
	
	//generate the colours using different averaging techniques
	Colour simple_average(void);
	Colour square_average(void);
	
	void average(weight_func func, double weight, bool is_base);		//add a weighting to either set of colours
	void base_blend(blend_func func);									//blends all the base colours individually using the specified function;
	void clear(void);													//removes colours currently stored in the container
	
	//accessors and individual clear methods
	inline void clear_base(void)
	{
		base_colours = std::vector<Colour>();
	}
	inline void clear_lighting(void)
	{
		lighting_colours = std::vector<Colour>();
	}
	inline int base_size(void)
	{
		return base_colours.size();
	}
	inline int light_size(void)
	{
		return lighting_colours.size();
	}
};


#endif /* ColourMixer_h */
