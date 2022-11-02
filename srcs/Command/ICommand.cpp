#include "ICommand.hpp"
#include "Translator.hpp"

ICommand::ICommand(Translator* translator, CommandType type): translator(translator), type(type) {}

ICommand::~ICommand() {}

ICommand::CommandType ICommand::getType() const {
	return type;
}
