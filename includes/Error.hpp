#ifndef ERROR_HPP
#define ERROR_HPP

#include <iostream>

#ifndef ERROR
#define ERROR -1
#endif // ERROR

struct Error
{
	int status;
	std::string message;
};

Error makeSuccess();
Error makeError(const std::string &message);

#endif // ERROR_HPP
