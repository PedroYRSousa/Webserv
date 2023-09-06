#ifndef S_SCHEDULE_HPP
#define S_SCHEDULE_HPP

#include <map>
#include <cstdlib>

#include "Server.hpp"

class S_Schedule
{
public:
  static S_Schedule &getInstance(void);
  static void start(int argc, char **argv);
  static void loop(void);
  static void end(void);
  static void stop(void);

protected:
private:
  static S_Schedule _instance;

  bool toContinue;
  std::map<int, Server> servers;

  S_Schedule(void);
  ~S_Schedule(void);
  size_t addServer(int port, Server server);
};

#endif // S_SCHEDULE_HPP
