#include "Error.hpp"

Error makeSucces()
{
	Error err;
	err.message = "";
	err.status = 0;

	return err;
}

Error makeError(const std::string &message)
{
	Error err;

	err.message = message;
	err.status = ERROR;

	return err;
}
