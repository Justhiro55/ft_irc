#pragma once

#include "AbstractCommand.hpp"

class Nick : public AbstractCommand {
	public:
		Nick();
		~Nick();

		void executeCmd();
};

class Pass : public AbstractCommand {

	public:
		Pass();
		~Pass();
		
		void executeCmd();
};
