#include "lib.hpp"

void printList(std::vector<std::string> &list)
{
	for (std::size_t i = 0; i < list.size(); i++)
		std::cout << list[i] << " ";
}

void printLocation(int serveri, int index)
{
	std::cout << "location: \n"
			  << "#Path: " << servers[serveri].locations[index].location_path << "\n"
			  << "#Methods: " << servers[serveri].locations[index].accepted_methods << "\n"
			  << "#limit body size: " << servers[serveri].locations[index].body_size_limit << "\n"
			  << "#redirection: " << servers[serveri].locations[index].redirection << "\n"
			  << "#host_directory: " << servers[serveri].locations[index].host_directory << "\n"
			  << "#directory_listing: " << servers[serveri].locations[index].directory_listing << "\n"
			  << "#default_answer_directory: " << servers[serveri].locations[index].default_answer_directory << "\n"
			  << "#execute_cgi: " << servers[serveri].locations[index].accept_cgi << "\n"
			  << "#accept_files: " << servers[serveri].locations[index].accept_files << "\n"
			  << "#file_upload_location: " << servers[serveri].locations[index].file_upload_location << "\n";
}

void printServer(int index)
{
	std::cout << "Port: " << servers[index].port << '\n'
			  << "Names: ";
	printList(servers[index].server_name);
	std::cout << '\n';
	std::cout << "Erro Page Numbers: " << servers[index].custom_error.size() << '\n';
	for (std::size_t i = 0; i < servers[index].locations.size(); i++)
		printLocation(index, i);
	std::cout << "\n\n";
}

void printConfig()
{
	for (std::size_t i = 0; i < servers.size(); i++)
		printServer(i);
}
