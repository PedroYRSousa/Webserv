#include "conn/Request.hpp"

// STATIC LOCAL
static std::vector<std::string> split(const std::string &s, char delimiter)
{
	std::vector<std::string> tokens;
	std::stringstream ss(s);
	std::string token;

	while (std::getline(ss, token, delimiter))
		tokens.push_back(token);

	return tokens;
}
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
static Error readFirstLine(Request *req, std::vector<std::string>::iterator *begin)
{
	std::vector<std::string> splitResult = split(trim(**begin), ' ');

	if (splitResult.size() < 3)
		return makeError("A requisicao possui um erro de formatacao");

	Error err = req->setMethod(splitResult[0]);
	if (err.status == ERROR)
		return err;
	req->setURI(splitResult[1]);
	req->setHTTPVersion(splitResult[2]);

	(*begin)++;
	return makeSuccess();
}
static void readHeaders(Request *req, std::vector<std::string>::iterator *begin, std::vector<std::string>::iterator end)
{
	std::vector<std::string> splitResult;

	while ((*begin) != end && (**begin).compare("\r") != 0 && (**begin).compare("") != 0)
	{
		splitResult = split(**begin, ':');
		if (splitResult.size() == 2)
			req->addHeader(splitResult[0], splitResult[1]);
		else if (splitResult.size() > 2)
		{
			std::string valueAux = "";
			for (size_t i = 1; i < splitResult.size(); i++)
				valueAux.append(splitResult[i] + ":");
			req->addHeader(splitResult[0], valueAux);
		}
		else
			req->addHeader(splitResult[0], "");
		(*begin)++;
	}
}
static void readBody(Request *req, std::vector<std::string>::iterator *begin, std::vector<std::string>::iterator end)
{
	while ((*begin) != end)
	{
		req->addInBody(trim(**begin));
		(*begin)++;
	}
}

// STATIC PUBLIC
Error Request::parser(std::string request, Request *out)
{
	Request req = Request();
	std::vector<std::string> splitResult = split(request, '\n');
	std::vector<std::string>::iterator begin = splitResult.begin();

	if (splitResult.size() <= 0)
		return makeError("A requisicao possui um erro de formatacao");

	req.raw = request;
	Error err = readFirstLine(&req, &begin);
	if (err.status == ERROR)
		return err;
	readHeaders(&req, &begin, splitResult.end());
	readBody(&req, &begin, splitResult.end());

	(*out) = req;
	return makeSuccess();
}
// STATIC PROTECTED
// STATIC PRIVATE
// PUBLIC
Request::Request()
{
	this->allowed_methods.insert("GET");
	this->allowed_methods.insert("POST");
}
Request::~Request()
{
}
void Request::setURI(std::string uri)
{
	this->uri = trim(uri);
}
Error Request::setMethod(std::string method)
{
	method = trim(method);

	if (this->allowed_methods.find(method) == this->allowed_methods.end())
		return makeError("Method nao permitido");

	this->method = method;

	return makeSuccess();
}
void Request::setHTTPVersion(std::string httpVersion)
{
	this->httpVersion = trim(httpVersion);
}
void Request::addHeader(std::string key, std::string value)
{
	key = trim(key);
	value = trim(value);

	if (this->headers.find(key) == this->headers.end())
		this->headers.insert(std::make_pair(key, value));
	else
		this->headers[key] = value;
}
void Request::addInBody(std::string bodyPart)
{
	this->body.append(trim(bodyPart));
}
std::string Request::getURI(void)
{
	return this->uri;
}
std::string Request::getMethod(void)
{
	return this->method;
}
std::string Request::getHTTPVersion(void)
{
	return this->httpVersion;
}
std::string Request::getBody(void)
{
	return this->body;
}
Error Request::getHeader(std::string key, std::string *value)
{
	if (this->headers.find(key) == this->headers.end())
		return makeError("Key nao encontrado em headers");

	(*value) = this->headers[key];
	return makeSuccess();
}
void Request::showRequest(void)
{
	std::cout << "method: " << this->method << std::endl;
	std::cout << "uri: " << this->uri << std::endl;
	std::cout << "httpVersion: " << this->httpVersion << std::endl;
	std::cout << std::endl
			  << "Headers: " << std::endl;
	for (std::map<std::string, std::string>::iterator it = this->headers.begin(); it != this->headers.end(); it++)
		std::cout << (*it).first << ":" << (*it).second << std::endl;
	std::cout << std::endl
			  << "body: " << this->body << std::endl
			  << std::endl;
}
// PROTECTED
// PRIVATE

#ifdef TEST_MODE
#include <gtest/gtest.h>
TEST(parseRequest, DeveRealizarOParser)
{
	try
	{
		Request req;
		std::string request = "GET /exemplo.html HTTP/1.1\nHost: www.exemplo.com\n\nTesteA";
		Error err = Request::parser(request, &req);
		EXPECT_EQ(err.status, SUCCESS);
		EXPECT_EQ(err.message, "");
		EXPECT_EQ(req.getMethod(), "GET");
		EXPECT_EQ(req.getURI(), "/exemplo.html");
		EXPECT_EQ(req.getHTTPVersion(), "HTTP/1.1");
		std::string value = "";
		err = req.getHeader("Host", &value);
		EXPECT_EQ(err.status, SUCCESS);
		EXPECT_EQ(err.message, "");
		EXPECT_EQ(value, "www.exemplo.com");
		value = "";
		err = req.getHeader("Host123", &value);
		EXPECT_EQ(err.status, ERROR);
		EXPECT_EQ(err.message, "Key nao encontrado em headers");
		EXPECT_EQ(value, "");
		EXPECT_EQ(req.getBody(), "TesteA");
	}
	catch (const std::exception &e)
	{
		std::cerr << e.what() << '\n';
	}
}
TEST(parseRequest, NãoDeveRealizarOParserMetodoNãoPermitido)
{
	try
	{
		Request req;
		std::string request = "HEAD /exemplo.html HTTP/1.1\nHost: www.exemplo.com\n\nTesteA";
		Error err = Request::parser(request, &req);
		EXPECT_EQ(err.status, ERROR);
		EXPECT_EQ(err.message, "Method nao permitido");

		request = "DELETE /exemplo.html HTTP/1.1\nHost: www.exemplo.com\n\nTesteA";
		err = Request::parser(request, &req);
		EXPECT_EQ(err.status, ERROR);
		EXPECT_EQ(err.message, "Method nao permitido");

		request = "OPTIONS /exemplo.html HTTP/1.1\nHost: www.exemplo.com\n\nTesteA";
		err = Request::parser(request, &req);
		EXPECT_EQ(err.status, ERROR);
		EXPECT_EQ(err.message, "Method nao permitido");
	}
	catch (const std::exception &e)
	{
		std::cerr << e.what() << '\n';
	}
}
TEST(parseRequest, NãoDeveRealizarOParserRequestInvalida)
{
	try
	{
		Request req;
		std::string request = "GET";
		Error err = Request::parser(request, &req);
		EXPECT_EQ(err.status, ERROR);
		EXPECT_EQ(err.message, "A requisicao possui um erro de formatacao");

		request = "GET /exemplo.html";
		err = Request::parser(request, &req);
		EXPECT_EQ(err.status, ERROR);
		EXPECT_EQ(err.message, "A requisicao possui um erro de formatacao");

		request = "";
		err = Request::parser(request, &req);
		EXPECT_EQ(err.status, ERROR);
		EXPECT_EQ(err.message, "A requisicao possui um erro de formatacao");
	}
	catch (const std::exception &e)
	{
		std::cerr << e.what() << '\n';
	}
}
#endif
