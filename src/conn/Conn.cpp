#include "conn/Conn.hpp"

// STATIC LOCAL
static std::string &rtrim(std::string &str)
{
	size_t end = str.find_last_not_of(" \t\n\r");
	if (end != std::string::npos)
	{
		str.erase(end + 1);
	}
	return str;
}
static std::string &ltrim(std::string &str)
{
	size_t start = str.find_first_not_of(" \t\n\r");
	if (start != std::string::npos)
	{
		str.erase(0, start);
	}
	return str;
}
static std::string &trim(std::string &str)
{
	return ltrim(rtrim(str));
}

// STATIC PUBLIC
// STATIC PROTECTED
// STATIC PRIVATE
// PUBLIC
Conn::Conn()
{
}
Conn::~Conn()
{
	this->headers.clear();
}

void Conn::setHTTPVersion(std::string httpVersion)
{
	this->httpVersion = trim(httpVersion);
}
void Conn::addHeader(std::string key, std::string value)
{
	key = trim(key);
	value = trim(value);

	if (this->headers.find(key) == this->headers.end())
		this->headers.insert(std::make_pair(key, value));
	else
		this->headers[key] = value;
}
void Conn::addInBody(std::string bodyPart)
{
	this->body.append(trim(bodyPart));
}
std::string Conn::getHTTPVersion(void)
{
	return this->httpVersion;
}
std::string Conn::getBody(void)
{
	return this->body;
}
Error Conn::getHeader(std::string key, std::string *value)
{
	if (this->headers.find(key) == this->headers.end())
		return makeError("Key nao encontrado em headers");

	(*value) = this->headers[key];
	return makeSuccess();
}
// PROTECTED
// PRIVATE
