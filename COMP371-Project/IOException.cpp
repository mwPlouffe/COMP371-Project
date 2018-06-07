//
//  IOException.cpp
//  COMP371-Project
//
//  Created by Mathew Plouffe on 2018-06-05.
//  Copyright © 2018 Mathew Plouffe. All rights reserved.
//

#include "IOException.h"

IOException::IOException(void)
{
	message = "ERROR: A problem occurred when performing operations of a file";
}
IOException::IOException(std::string message)
{
	this->message = "ERROR: " + message;
}
std::string IOException::what(void)
{
	return message;
}