#include "lib.hpp"

std::string buildFinalPath(S_Location location, S_Request request)
{

	// Verifica se há uma barra no final de host_directory e remove se existir
	if (!location.host_directory.empty() && location.host_directory[location.host_directory.size() - 1] == '/')
		location.host_directory.resize(location.host_directory.size() - 1);

	std::cout << location.host_directory << std::endl;
	std::cout << request.path << std::endl;
	std::string path = location.host_directory + request.path;
	std::cout << path << std::endl;
	return path;
}

bool isCGI(S_Location location, S_Request request)
{
	if (location.accept_cgi)
	{
		std::string extension = getFileExtension(request.path);
		if (extension == "py")
			return true;
	}
	return false;
}

void checkFileExist(std::string path)
{
	Log::debug << "Aqui 1" << Log::eof;
	Log::debug << path << Log::eof;
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

	Log::debug << "checkReadPermission aqui 1" << Log::eof;
	// Verifica se o arquivo em 'path' tem permissão de leitura (R_OK)
	if (access(path.c_str(), R_OK) == 0)
	{
		return;
	}
	else
	{
		if (errno == EACCES)
		{
			Log::debug << "checkReadPermission aqui 2" << Log::eof;
			throw ForbiddenAccess();
		}
		else
		{
			Log::debug << "checkReadPermission aqui 3" << Log::eof;
			throw InternalAccessError();
		}
	}
}

std::string readFileContent(const std::string &path)
{

	Log::debug << "Aqui 33" << Log::eof;
	Log::debug << path.c_str() << Log::eof;
	std::ifstream file(path.c_str()); // Abre o arquivo

	if (!file.is_open())
	{
		Log::debug << "Aqui 3" << Log::eof;
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

	Log::debug << "Aqui 4" << Log::eof;
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
