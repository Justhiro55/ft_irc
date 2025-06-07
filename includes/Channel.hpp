
#pragma once

#include "Client.hpp"
#include "vector"

class Channel {
	private:
		std::string name;
		std::vector<Client *> members;
};