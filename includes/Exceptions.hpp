#ifndef Exceptions_hpp
#define Exceptions_hpp

#include <stdexcept>

class LocationNotFoundException: public std::exception {
public:
    const char* what() const throw() {
        return "Nenhuma Location encontrada para o path.";
    }
};

class ServerNotFound: public std::exception {
public:
    const char* what() const throw() {
        return "Nenhum Server encontrado";
    }
};

class ResourceNotFound: public std::exception {
public:
    const char* what() const throw() {
        return "O recurso solicitado não foi encontrado";
    }
};

class MethodNotAllowed: public std::exception {
public:
    const char* what() const throw() {
        return "O método especificado não é permitido para o recurso selecionado";
    }
};

class PayloadTooLarge: public std::exception {
public:
    const char* what() const throw() {
        return "O payload fornecido é maior que o suportado";
    }
};

class ForbiddenAccess: public std::exception {
public:
    const char* what() const throw() {
        return "O recurso solicitado não foi encontrado";
    }
};

// Internal Errors

class InternalAccessError: public std::exception {
public:
    const char* what() const throw() {
        return "Erro ao verificar permissões do arquivo.";
    }
};

class InternalOpenError: public std::exception {
public:
    const char* what() const throw() {
        return "Erro ao ler o arquivo.";
    }
};

class ReadFileError: public std::exception {
public:
    const char* what() const throw() {
        return "Erro ao abrir o arquivo.";
    }
};


#endif