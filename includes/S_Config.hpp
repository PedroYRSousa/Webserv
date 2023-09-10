#ifndef S_CONFIG_HPP
#define S_CONFIG_HPP

#include "Log.hpp"
#include "Error.hpp"

#include <cstring>
#include <fstream>
#include <iostream>

class S_Config
{
public:
	static Error readFile(std::string filePath);

protected:
private:
	static S_Config _instance;

	static S_Config &getInstance(void);

	S_Config(void);
	~S_Config(void);
};

#endif // S_CONFIG_HPP
