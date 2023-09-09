#ifndef S_CONFIG_HPP
#define S_CONFIG_HPP

#include "Log.hpp"

#include <cstring>
#include <fstream>
#include <iostream>

#ifndef ERROR
#define ERROR -1
#endif // ERROR

class S_Config
{
public:
	static int readFile(std::string *readFileOut);

protected:
private:
	static S_Config _instance;

	static S_Config &getInstance(void);

	S_Config(void);
	~S_Config(void);
};

#endif // S_CONFIG_HPP
