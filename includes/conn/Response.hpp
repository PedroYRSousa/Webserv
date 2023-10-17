#ifndef RESPONSE_HPP
#define RESPONSE_HPP

// Importes com ""
#include "Log.hpp"
#include "Conn.hpp"
#include "Error.hpp"

// Importes com <>

// Defines
// #ifndef NAME
// #define NAME
// #endif // NAME

class Response : public Conn
{
public:
	static std::map<int, std::string> statusString;

	Response();
	~Response();

	void setStatus(int status);
	int getStatus(void);
	std::string dump(bool withBody);

protected:
private:
	int status;
};

struct Response
{
	int statusCode;
	int serverNumber;
	std::string httpVersion;
	std::map<std::string, std::string> headerFields;
	std::string body;
};

#endif // RESPONSE_HPP
