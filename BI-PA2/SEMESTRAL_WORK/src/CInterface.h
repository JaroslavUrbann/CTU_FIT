//
// Created by jaroslav on 5/9/20.
//

#ifndef TOWER_ATTACK_CINTERFACE_H
#define TOWER_ATTACK_CINTERFACE_H

#include <string>
#include <iostream>
#include <functional>

class CInterface {
private:
	std::istream &m_In;
	std::ostream &m_Out;
public:
	CInterface(std::istream &in, std::ostream &out);
	void Clear() const;
	void Message(const std::string &message) const;
	void PrintHelp(const std::string &help) const;
	// gets a filename from the user
	bool GetFile(const std::function<bool(const std::string &)> &valid, const std::function<bool()> &refresh) const;
	// gets a command from the user
	std::string GetCommand(const std::function<bool(const std::string &)> &valid, const std::function<bool()> &refresh) const;

	void Message(const char &message) const;
};


#endif //TOWER_ATTACK_CINTERFACE_H
