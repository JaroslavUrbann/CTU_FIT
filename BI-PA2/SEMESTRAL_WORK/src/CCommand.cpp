//
// Created by jaroslav on 5/9/20.
//

#include "CCommand.h"

#include <utility>

CCommand::CCommand(std::string help,
					std::function<bool(const CInterface &, CGame &, const std::string &parameters)> execute,
					std::function<bool(const std::string &)> isValid)
					:m_Help(std::move(help)),m_Execute(std::move(execute)),m_IsValid(std::move(isValid)){}

std::string CCommand::Help() const{
	return m_Help;
}

bool CCommand::IsValid(const std::string &command){
	return m_IsValid(command);
}

bool CCommand::Execute(const CInterface &interface, CGame &game, const std::string& parameters){
	return m_Execute(interface, game, parameters);
}
