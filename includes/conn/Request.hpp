#ifndef REQUEST_HPP
#define REQUEST_HPP

#include "Log.hpp"
#include "Conn.hpp"
#include "Error.hpp"

struct Request
{
	int serverNumber;
	std::string method;
	std::string path;
	std::string httpVersion;
	std::map<std::string, std::string> headerFields;
	std::string body;
};

class Request : public Conn
{
public:
	Request();
	~Request();

	static Error parser(std::string request, Request *out);

	void setURI(std::string method);
	Error setMethod(std::string method);
	std::string getURI(void);
	std::string getMethod(void);
	Error getHeader(std::string key, std::string *value);

	std::string dump(bool withBody);

protected:
private:
	std::set<std::string> allowed_methods;

	std::string uri;
	std::string method;
};

#endif // REQUEST_HPP
