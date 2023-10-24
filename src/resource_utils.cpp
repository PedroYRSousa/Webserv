#include "lib.hpp"

void checkFileExist(std::string path)
{

	struct stat buffer;

	if (stat(path.c_str(), &buffer) != 0)
		throw ResourceNotFound();
}

bool isDirectory(std::string path)
{

	struct stat buffer;

	if (stat(path.c_str(), &buffer) == 0)
		return S_ISDIR(buffer.st_mode);
	return false;
}

void checkReadPermission(std::string path)
{

	// Verifica se o arquivo em 'path' tem permissão de leitura (R_OK)
	if (access(path.c_str(), R_OK) == 0)
	{
		return;
	}
	else
	{
		if (errno == EACCES)
			throw ForbiddenAccess();
		else
			throw InternalAccessError();
	}
}

std::string readFileContent(const std::string &path)
{

	std::ifstream file(path.c_str()); // Abre o arquivo

	if (!file.is_open())
	{
		throw InternalOpenError();
		return "";
	}

	std::ostringstream contentStream;
	contentStream << file.rdbuf(); // Lê o conteúdo do arquivo em um fluxo de string

	if (file.bad())
	{
		throw ReadFileError();
		return "";
	}

	return contentStream.str(); // Retorna o conteúdo do arquivo como uma string
}

std::string getFileExtension(std::string path)
{

	size_t period = path.rfind(".");
	if (period != std::string::npos)
		return path.substr(period + 1);
	return "";
}

std::map<std::string, std::string> getContentTypesTable()
{

	std::map<std::string, std::string> contentTypes;

	contentTypes["txt"] = "text/plain";
	contentTypes["html"] = "text/html";
	contentTypes["css"] = "text/css";
	contentTypes["js"] = "text/javascript";
	contentTypes["json"] = "application/json";
	contentTypes["xml"] = "application/xml";
	contentTypes["pdf"] = "application/pdf";
	contentTypes["zip"] = "application/zip";
	contentTypes["gzip"] = "application/gzip";
	contentTypes["tar"] = "application/x-tar";
	contentTypes["png"] = "image/png";
	contentTypes["jpg"] = "image/jpeg";
	contentTypes["jpeg"] = "image/jpeg";
	contentTypes["gif"] = "image/gif";
	contentTypes["svg"] = "image/svg+xml";
	contentTypes["ico"] = "image/x-icon";

	return contentTypes;
}

std::string getContentType(const std::string &path)
{

	std::map<std::string, std::string> contentTypes = getContentTypesTable();
	const std::string extension = getFileExtension(path);

	if (extension.empty())
		return "";

	std::map<std::string, std::string>::iterator it = contentTypes.find(extension);
	if (it != contentTypes.end())
		return it->second;
	else
		return "application/octet-stream"; // Tipo de conteúdo padrão para tipos de arquivo desconhecidos
}
