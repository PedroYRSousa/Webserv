#ifndef CONN_HPP
#define CONN_HPP

// Importes com ""
#include "Log.hpp"
#include "Error.hpp"

#include <map>
#include <set>
#include <vector>
#include <sstream>
#include <iostream>

// Defines
// #ifndef NAME
// #define NAME
// #endif // NAME

class Conn
{
public:
	Conn();
	virtual ~Conn();

	void setHTTPVersion(std::string method);
	void addHeader(std::string key, std::string value);
	void addInBody(std::string bodyPart);
	std::string getHTTPVersion(void);
	std::string getBody(void);
	Error getHeader(std::string key, std::string *value);

	virtual std::string dump(bool withBody) = 0;

protected:
	std::string raw;
	std::string httpVersion;
	std::map<std::string, std::string> headers;
	std::string body;

private:
};

#endif // CONN_HPP
