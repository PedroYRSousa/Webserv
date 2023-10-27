#ifndef Exceptions_hpp
#define Exceptions_hpp

#include <stdexcept>

class LocationNotFoundException : public std::exception
{
public:
	const char *what() const throw()
	{
		return "Nenhuma Location encontrada para o path.";
	}
};

class ServerNotFound : public std::exception
{
public:
	const char *what() const throw()
	{
		return "Nenhum Server encontrado";
	}
};

class ResourceNotFound : public std::exception
{
public:
	const char *what() const throw()
	{
		return "O recurso solicitado não foi encontrado";
	}
};

class MethodNotAllowed : public std::exception
{
public:
	const char *what() const throw()
	{
		return "O método especificado não é permitido para o recurso selecionado";
	}
};

class PayloadTooLarge : public std::exception
{
public:
	const char *what() const throw()
	{
		return "O payload fornecido é maior que o suportado";
	}
};

class ForbiddenAccess : public std::exception
{
public:
	const char *what() const throw()
	{
		return "O recurso solicitado não foi encontrado";
	}
};

class IsNotADirectory : public std::exception
{
public:
	const char *what() const throw()
	{
		return "o Path fornecido não é um diretório";
	}
};

// Internal Errors

class InternalAccessError : public std::exception
{
public:
	const char *what() const throw()
	{
		return "Erro ao verificar permissões do arquivo.";
	}
};

class InternalDeleteError : public std::exception
{
public:
	const char *what() const throw()
	{
		return "Erro ao deletar arquivo";
	}
};

class InternalOpenError : public std::exception
{
public:
	const char *what() const throw()
	{
		return "Erro ao ler o arquivo.";
	}
};

class InternalOpenFile : public std::exception
{
public:
	const char *what() const throw()
	{
		return "Erro ao abrir o arquivo.";
	}
};

class ReadFileError : public std::exception
{
public:
	const char *what() const throw()
	{
		return "Erro ao abrir o arquivo.";
	}
};

class CgiMisconfiguration : public std::exception
{
public:
	const char *what() const throw()
	{
		return "O Cgi não foi configurado corretamente";
	}
};

class CgiScriptIsNotAValidPythonFile : public std::exception
{
public:
	const char *what() const throw()
	{
		return "O arquivo de CGI buscado não é um arquivo .py válido";
	}
};

#endif
