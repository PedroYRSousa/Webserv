#ifndef REQUEST_HPP
#define REQUEST_HPP

// Importes com ""
#include <Log.hpp>
#include <Error.hpp>

// Importes com <>
#include <map>
#include <set>
#include <vector>
#include <sstream>
#include <iostream>

// Defines
// #ifndef NAME
// #define NAME
// #endif // NAME

// Teste
// Teste:
// Teste:123

class Request
{
public:
	Request();
	~Request();

	static Error parser(std::string request, Request *out);

	void setURI(std::string method);
	Error setMethod(std::string method);
	void setHTTPVersion(std::string method);
	void addHeader(std::string key, std::string value);
	void addInBody(std::string bodyPart);
	std::string getURI(void);
	std::string getMethod(void);
	std::string getHTTPVersion(void);
	std::string getBody(void);
	Error getHeader(std::string key, std::string *value);
	void showRequest(void);

protected:
private:
	std::set<std::string> allowed_methods;

	std::string raw;
	std::string uri;
	std::string method;
	std::string httpVersion;
	std::map<std::string, std::string> headers;
	std::string body;
};

#endif // REQUEST_HPP
