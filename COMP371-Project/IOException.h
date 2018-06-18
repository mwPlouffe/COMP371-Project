//
//  IOException.h
//  COMP371-Project
//
//  Created by Mathew Plouffe on 2018-06-05.
//  Copyright © 2018 Mathew Plouffe. All rights reserved.
//

#ifndef IOException_h
#define IOException_h

#include <exception>
#include <string>

//simple IO exception class for use when doing file IO

class IOException : public std::exception
{
	protected:
	std::string message;
	public:
	IOException(void);
	IOException(std::string message);
	
	virtual ~IOException(void) = default;
	virtual std::string what(void);
};


#endif /* IOException_h */
