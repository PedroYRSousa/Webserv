#include "lib.hpp"
#include "dirListing.hpp"

std::string intToString(int number)
{
	std::stringstream ss;
	ss << number;
	return ss.str();
}

void getResource(const S_Request &request, S_Response &response, const S_Location &location)
{

	std::string body;
	std::string extension;

	// build path

	std::string truePath = location.host_directory + request.path;

	// Verifica se existe e se é diretorio
	if (isDirectory(truePath))
	{

		std::cout << "é diretorio" << std::endl;
		// verifica config de DIRECTORY LISTING
		if (location.directory_listing)
		{
			DirListing dir(request, location.host_directory);
			response.body = dir.getPageString();

			// seta headers - fazer func
			response.header_fields["Content-Type"] = "text/html";
			if (body.empty())
				response.header_fields["Content-Length"] = intToString(response.body.size());
			response.status_code = 200;
		}
		else
		{
			if (!location.default_answer_directory.empty())
			{
				// um monte de coisa
			}
			// erro (qual?)
		}
		return;
	}

	// Verifica se é arquivo e se existe
	checkFileExist(truePath);

	// verifica se tem acesso ao arquivo
	// Erro de falta de acesso
	checkReadPermission(truePath);

	// le arquivo, seta headers e devolve com status 200
	response.body = readFileContent(truePath);
	response.header_fields["Content-Type"] = getContentType(truePath);
	if (body.empty())
		response.header_fields["Content-Length"] = intToString(response.body.size());
	response.status_code = 200;
}

void deleteResource(const S_Request &request, S_Response &response)
{

	(void)response;

	if (isDirectory(request.path))
		throw ForbiddenAccess();

	std::cout << "passou" << std::endl;

	checkFileExist(request.path);

	std::remove(request.path.c_str());

	std::cout << "deletou" << std::endl;
}
