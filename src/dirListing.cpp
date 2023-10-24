#include "dirListing.hpp"

DirListing::DirListing(){};

DirListing::DirListing(S_Request request, std::string root_directory)
{
	makePage(request, root_directory);
};

DirListing::~DirListing(){};

void DirListing::addFiles(std::list<dirent *> files)
{
	for (std::list<dirent *>::iterator it = files.begin(); it != files.end(); it++)
		addFile((*it)->d_name);
}

void DirListing::addFile(std::string file_name)
{
	_page
		<< "<p><a href=\"http://"
		<< _host
		<< _path << "/" << file_name << "\">"
		<< file_name << "</a></p>\n";
};

static bool sort(const dirent *a, const dirent *b)
{
	// Coloque '.' antes de '..'
	if (std::strcmp(a->d_name, ".") == 0)
		return true;
	if (std::strcmp(b->d_name, ".") == 0)
		return false;
	if (std::strcmp(a->d_name, "..") == 0)
		return true;
	if (std::strcmp(b->d_name, "..") == 0)
		return false;

	// Organize pastas antes de arquivos
	if (a->d_type == DT_DIR && b->d_type != DT_DIR)
		return true;
	if (a->d_type != DT_DIR && b->d_type == DT_DIR)
		return false;

	// Ordene alfabeticamente
	return std::strcmp(a->d_name, b->d_name) < 0;
}

void DirListing::makePage(S_Request request, std::string root_directory)
{
	DIR *folder;
	struct dirent *entry;
	std::string directory_path;

	_host = request.header_fields["host"];
	_path = request.path;

	if (_path.length() > 0 && _path[_path.length() - 1] == '/')
		_path = std::string(_path, 0, _path.length() - 1);

	if (_path.compare("/..") == 0 || _path.compare("/") == 0)
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

	std::list<dirent *> files;
	while ((entry = readdir(folder)))
	{
		std::string name(entry->d_name);

		if (directory_path.compare(root_directory) == 0 && name.compare("..") == 0)
			continue;

		if (name.compare(".") == 0)
			files.push_front(entry);
		else if (name.compare("..") == 0)
			files.push_front(entry);
		else
			files.push_back(entry);
	}

	files.sort(sort);
	addFiles(files);

	closedir(folder);

	_page << "</p></body></html>";
};

std::string DirListing::getPageString()
{
	return (_page.str());
}
