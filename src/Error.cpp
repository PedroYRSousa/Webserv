#include "Error.hpp"

Error makeSucces()
{
	return {};
}

Error makeError(std::string message)
{
	Error err = {};

	err.message = message;
	err.status = ERROR;

	return err;
}
