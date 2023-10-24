#include "lib.hpp"

S_Response run(S_Request request)
{
	S_Response response;
	std::string content_type;

	try
	{
		response.server_number = request.server_number;

		// Decidir a location
		S_Location location = findLocation(request.server_number, request.path);
		std::cout << "Location encontrada: " << location.location_path << std::endl;

		// Verifica se tem redirect
		if (hasRedirection(location))
		{
			response.status_code = 302;
			return response;
		}

		// verifica metodos
		checkIfMethodIsAllowed(request.method, location);

		// verifica se tem body_size_limit
		checkBodySizeLimit(location, request.body, request.method);

		// GET? POST? DELETE?
		if (request.method & GET)
			getResource(request, response, location);
		else
		{
			// Verifica se tem upload de files ativo na config
			// se nao tiver, erro de processamento
			if (request.method & POST)
				std::cout << "é POST" << std::endl; // log erro
			else if (request.method & DELETE)
				deleteResource(request, response);
		}
	}
	catch (const LocationNotFoundException &e)
	{
		response.status_code = 404;
		std::cout << e.what() << " " << request.path << std::endl; // log erro
		return response;
	}
	catch (const ServerNotFound &e)
	{
		response.status_code = 404;
		std::cout << e.what() << std::endl; // log erro
		return response;
	}
	catch (const MethodNotAllowed &e)
	{
		response.status_code = 405;
		std::cout << e.what() << std::endl; // log erro
		return response;
	}
	catch (const PayloadTooLarge &e)
	{
		response.status_code = 413;
		std::cout << e.what() << std::endl; // log erro
		return response;
	}
	catch (const ResourceNotFound &e)
	{
		response.status_code = 404;
		std::cout << e.what() << std::endl; // log erro
		return response;
	}
	catch (const ForbiddenAccess &e)
	{
		response.status_code = 403;
		std::cout << e.what() << std::endl; // log erro
		return response;
	}
	catch (const InternalAccessError &e)
	{
		response.status_code = 500;
		std::cout << e.what() << std::endl; // log erro
		return response;
	}
	catch (const InternalOpenError &e)
	{
		response.status_code = 404;
		std::cout << e.what() << std::endl; // log erro
		return response;
	}
	catch (const ReadFileError &e)
	{
		response.status_code = 500;
		std::cout << e.what() << std::endl; // log erro
		return response;
	}

	return response;
}
