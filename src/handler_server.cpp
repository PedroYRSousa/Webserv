#include "lib.hpp"

// S_Response run(S_Request request)
//{
//	S_Response response;
//	std::string content_type;

//	try
//	{
//		response.server_number = request.server_number;

//		// Decidir a location
//		S_Location location = findLocation(request.server_number, request.path);
//		std::cout << "Location encontrada: " << location.location_path << std::endl;

//		// Verifica se tem redirect
//		if (hasRedirection(location))
//		{
//			response.status_code = 302;
//			return response;
//		}

//		// verifica metodos
//		checkIfMethodIsAllowed(request.method, location);

//		// verifica se tem body_size_limit
//		checkBodySizeLimit(location, request.body, request.method);

//		// GET? POST? DELETE?
//		if (request.method & GET)
//			getResource(request, response, location);
//		else
//		{
//			// Verifica se tem upload de files ativo na config
//			// se nao tiver, erro de processamento
//			if (request.method & POST)
//				std::cout << "é POST" << std::endl; // log erro
//			else if (request.method & DELETE)
//				deleteResource(request, response);
//		}
//	}
//	catch (const LocationNotFoundException &e)
//	{
//		response.status_code = 404;
//		std::cout << e.what() << " " << request.path << std::endl; // log erro
//		return response;
//	}
//	catch (const ServerNotFound &e)
//	{
//		response.status_code = 404;
//		std::cout << e.what() << std::endl; // log erro
//		return response;
//	}
//	catch (const MethodNotAllowed &e)
//	{
//		response.status_code = 405;
//		std::cout << e.what() << std::endl; // log erro
//		return response;
//	}
//	catch (const PayloadTooLarge &e)
//	{
//		response.status_code = 413;
//		std::cout << e.what() << std::endl; // log erro
//		return response;
//	}
//	catch (const ResourceNotFound &e)
//	{
//		response.status_code = 404;
//		std::cout << e.what() << std::endl; // log erro
//		return response;
//	}
//	catch (const ForbiddenAccess &e)
//	{
//		response.status_code = 403;
//		std::cout << e.what() << std::endl; // log erro
//		return response;
//	}
//	catch (const InternalAccessError &e)
//	{
//		response.status_code = 500;
//		std::cout << e.what() << std::endl; // log erro
//		return response;
//	}
//	catch (const InternalOpenError &e)
//	{
//		response.status_code = 404;
//		std::cout << e.what() << std::endl; // log erro
//		return response;
//	}
//	catch (const ReadFileError &e)
//	{
//		response.status_code = 500;
//		std::cout << e.what() << std::endl; // log erro
//		return response;
//	}

//	return response;
//}

static std::vector<std::string> getEnvs(S_Request request, std::string root)
{
	std::string method = "";
	std::vector<std::string> envs(5);
	if (request.method == POST)
		method = "POST";
	if (request.method == GET)
		method = "GET";
	if (request.method == DELETE)
		method = "DELETE";

	envs[0] = "REQUEST_METHOD=" + method;
	envs[1] = "QUERY_STRING=" + request.queryString;
	envs[2] = "CONTENT_TYPE=text/html";
	envs[3] = "CONTENT_LENGTH=" + request.body.length();
	// if (headers.count("user-agent") > 0)
	// envs[5] = "HTTP_USER_AGENT=" + headers["user-agent"];
	envs[4] = "DOCUMENT_ROOT=" + root;
	// envs[7] = "SCRIPT_FILENAME=" + resource.substr(1);
	// envs[8] = "SCRIPT_NAME=" + resource.substr(resource.find_last_of('/') + 1);
	// envs[9] = "REDIRECT_STATUS=200";
	// envs[10] = "REQUEST_BODY=" + request.getBody();

	return envs;
}
static std::string getPath(S_Request request, std::string root)
{
	return root + request.path;
}

static void cgiExec(int _pipe[2], S_Request request, std::string root)
{
	std::vector<std::string> envs = getEnvs(request, root);
	std::string path = "/usr/bin/python3";
	std::string tst = getPath(request, root);

	if (access(path.c_str(), X_OK) < 0)
	{
		std::cerr << std::string(strerror(errno)) << std::endl;
		throw std::exception();
	} // Erro pois não tem acesso // 403

	char const *argv[] = {path.c_str(), tst.c_str(), NULL};
	char const *envp[] = {envs[0].c_str(), envs[1].c_str(), envs[2].c_str(), envs[3].c_str(), envs[4].c_str(), NULL};

	std::cout << "root: |" << root << "|" << std::endl;
	std::cout << "path: |" << path << "|" << std::endl;
	std::cout << "tst: |" << tst << "|" << std::endl;
	if (dup2(_pipe[0], STDIN_FILENO) < 0)
	{
		throw std::exception();
	} // Erro no dup
	if (close(_pipe[0]) < 0)
	{
		throw std::exception();
	} // Erro no close
	if (dup2(_pipe[1], STDOUT_FILENO) < 0)
	{
		throw std::exception();
	} // Erro no dup
	if (close(_pipe[1]) < 0)
	{
		throw std::exception();
	} // Erro no close

	if (execve(path.c_str(), (char *const *)argv, (char *const *)envp) < 0)
	{
		std::cerr << std::string(strerror(errno)) << std::endl;
		throw std::exception();
	}
}
static S_Response cgiSchedule(int _pid, int _pipe[2], S_Request request, std::string root)
{
	int ret = 1;
	S_Response res;
	size_t timeout = 1000;

	if (close(_pipe[1]) < 0)
	{
		throw std::exception();
	} // Erro no close
	pid_t pid = _pid;
	struct timeval startTime;
	gettimeofday(&startTime, NULL);

	while (true)
	{
		pid_t result = waitpid(pid, NULL, WNOHANG);
		if (result == ERROR)
		{
			{
				throw std::exception();
			} // Erro no waitpid
		}

		if (result != 0)
			break;

		struct timeval currentTime;
		gettimeofday(&currentTime, NULL);

		unsigned int elapsedTime = (currentTime.tv_sec - startTime.tv_sec) * 1000 + (currentTime.tv_usec - startTime.tv_usec) / 1000;
		if (elapsedTime >= timeout)
		{
			kill(pid, SIGTERM);
			{
				std::cerr << "Timeout" << std::endl;
				throw std::exception();
			} // Request timeout 408
		}
		usleep(1000);
	}

	int const bufsize = 1048576; // 1 Mega
	char buffer[bufsize + 1];
	while (ret)
	{
		ret = read(_pipe[0], buffer, bufsize);
		if (ret < 0)
		{
			perror("cgi read");
			{
				throw std::exception();
			}
		}
		buffer[ret] = '\0';

		res.body.append(std::string(buffer, ret));
	}

	return res;
}
S_Response runCGI(S_Request request, std::string root)
{
	int _pid = 0;
	int _pipe[2];
	S_Response response;

	try
	{
		// Incia o pipe
		if (pipe(_pipe) == ERROR)
		{
			throw std::exception();
		} // Erro na geração do pipe

		if (request.method == POST)
			write(_pipe[1], request.body.c_str(), request.body.length());

		_pid = fork();
		if (_pid == 0)
			cgiExec(_pipe, request, root);
		else if (_pid > 0)
		{
			// Pai
			response = cgiSchedule(_pid, _pipe, request, root);
		}
		else
		{
			throw std::exception();
			// Erro na geração do PID
		}

		response.server_number = request.server_number;
	}
	catch (const std::exception &e)
	{
		response.status_code = 418;
		std::cerr << e.what() << '\n';
	}

	return response;
}

int main(void)
{
	std::string root = "/home/yago/Webserv/teste";
	S_Request req;

	req.body = "";
	req.method = GET;
	req.path = "/main.py";
	req.queryString = "";
	req.server_number = 0;

	std::cout << "root: |" << root << "|" << std::endl;
	std::cout << "req.path: |" << req.path << "|" << std::endl;
	S_Response res = runCGI(req, root);

	std::cout << "res.body: |" << res.body << "|" << std::endl;

	// char const *argv[] = {"/usr/bin/python3", "/home/yago/Webserv/teste/main.py", NULL};
	// char const *envp[] = {NULL};
	// if (execve("/usr/bin/python3", (char *const *)argv, (char *const *)envp) < 0)
	//{
	//	std::cout << std::string(strerror(errno)) << std::endl;
	// }
}
