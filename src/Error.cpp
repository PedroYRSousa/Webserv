#include "Error.hpp"

Error makeError(std::string message)
{
	Error err = {};

	err.message = message;
	if (!message.empty())
		err.status = ERROR;

	return err;
}
