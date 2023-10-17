#ifndef SCHEDULE_HPP
#define SCHEDULE_HPP

#include "Log.hpp"
#include "Error.hpp"
#include "socket/Server.hpp"
#include "socket/Client.hpp"

#include <map>
#include <vector>
#include <poll.h>

// Defines
// #ifndef NAME
// #define NAME
// #endif // NAME

class Schedule
{
public:
	static Error start(std::string filePathConfig);
	static Error loop(void);
	static Error end(void);
	static void stop(void);

	static void addSocket(Socket *s);
	static void removeSocket(Socket *s);

protected:
private:
	Schedule();
	~Schedule();

	static Schedule _instance;
	static Error handleServer(struct pollfd *poll, Server *server);
	static Error handleClient(struct pollfd *poll, Client *client);
	static Error acceptConnection(struct pollfd *poll, Server *server);
	static Error readClient(struct pollfd *poll, Client *client);
	static Error writeClient(struct pollfd *poll, Client *client);

	int timeout;
	bool toContinue;
	std::map<int, Socket *> sockets;
	std::vector<pollfd> polls;
};

#endif // SCHEDULE_HPP
