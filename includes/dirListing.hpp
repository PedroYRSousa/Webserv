#ifndef dirListing_hpp
#define dirListing_hpp

#include "lib.hpp"

class DirListing
{
public:
	DirListing();
	DirListing(S_Request request, std::string root_directory);
	~DirListing();

	void addFile(std::string file_name);
	void makePage(S_Request request, std::string root_directory);
	std::string getPageString();

private:
	std::string _host;
	std::string _path;
	std::stringstream _page;
};

#endif
