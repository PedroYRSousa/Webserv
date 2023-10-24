#include "dirListing.hpp"

DirListing::DirListing(){};

DirListing::DirListing(S_Request request, std::string root_directory)
{
	makePage(request, root_directory);
};

DirListing::~DirListing(){};

void DirListing::addFile(std::string file_name)
{
	_page << "<p><a href=\"http://"
		  << _host
		  << _path << "/" << file_name << "\">"
		  << file_name << "</a></p>\n";
};

void DirListing::makePage(S_Request request, std::string root_directory)
{
	DIR *folder;
	struct dirent *entry;
	std::string directory_path;

	_host = request.header_fields["host"];
	_path = request.path;
	if (_path == "/.." || _path == "/")
		_path = "";
	directory_path = root_directory + _path;

	folder = opendir(directory_path.c_str());
	if (folder == NULL)
	{
		// adicionar log e pensar melhor sistema de erro
		perror("Unable to read directory");
		exit(1);
	}
	_page << "<!DOCTYPE html><html><head><title>"
		  << _path << "</title></head><body><h1>INDEX</h1><p><hr>";

	while ((entry = readdir(folder)))
		addFile(entry->d_name);

	closedir(folder);

	_page << "</p></body></html>";
};

std::string DirListing::getPageString()
{
	return (_page.str());
}
