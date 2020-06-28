//
// Created by jaroslav on 5/9/20.
//

#include "CInterface.h"
#include <string>
#include <iostream>

CInterface::CInterface(std::istream &in, std::ostream &out):m_In(in),m_Out(out){}

void CInterface::Message(const std::string &message) const{
	if(!(m_Out << message))
		throw std::runtime_error("Output cannot be written");
}

void CInterface::Message(const char &message) const{
	if(!(m_Out << message))
		throw std::runtime_error("Output cannot be written");
}

bool CInterface::GetFile(const std::function<bool(const std::string &)> &valid, const std::function<bool()> &refresh) const{
	std::string filename;
	while (true){
		getline(m_In, filename);

		Clear();
		if(!refresh())
			return false;

		if(!valid(filename)){
			if (!(m_Out << "Loading file: \"" << filename << "\" failed" << std::endl))
				throw std::runtime_error("Output cannot be written");
		}
		else return true;
	}
}

std::string CInterface::GetCommand(const std::function<bool(const std::string &)> &valid, const std::function<bool()> &refresh) const{
	std::string command;
	while (true){
		if (!(m_Out << "\nWrite a command:" << std::endl))
			throw std::runtime_error("Output cannot be written");

		getline(m_In, command);

		Clear();
		refresh();

		if(!valid(command)){
			if (!(m_Out << "Wrong command: \"" << command <<R"(" use "help")"))
				throw std::runtime_error("Output cannot be written");
		}
		else return command;
	}
}

void CInterface::PrintHelp(const std::string &help) const{
	if (!(m_Out << help << std::endl))
		throw std::runtime_error("Output cannot be written.");
}

void CInterface::Clear() const {
	Message("\033c");
}

