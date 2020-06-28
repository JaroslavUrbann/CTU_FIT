//
// Created by jaroslav on 5/9/20.
//

#include <sstream>
#include "CApplication.h"
#include "CCommand.h"
#include "CLoadingMenu.h"

CCommand HelpCommand(const std::vector<CCommand> &commands){
	return CCommand{
		"Available Commands:",
		[&](const CInterface &interface, CGame &game, const std::string &parameters){
			for (const auto &c: commands)
				interface.PrintHelp(c.Help());
			return true;
		},
		[](const std::string &command){
			return command == "help";
		}
	};
}

CCommand SaveCommand(){
	return CCommand{
		"\"save\" - Saves the game",
		[](const CInterface &interface, CGame &game, const std::string &parameters){
			interface.Message("Saving " + std::string(game.Save()?"successful!":"failed!"));
			return true;
		},
		[](const std::string &command){
			return command == "save";
		}
	};
}

CCommand StepCommand(){
	return CCommand{
			"enter - Plays one step of the game",
			[](const CInterface &interface, CGame &game, const std::string &parameters){
				game.Step();
				return true;
			},
			[](const std::string &command){
				return command.empty();
			}
	};
}

CCommand BuyCommand(){
	return CCommand{
			R"("buy [count] [attacker type] [entrance]" - Buys attackers, for example: "buy 30 a 1")",
			[](const CInterface &interface, CGame &game, const std::string &parameters){
				std::stringstream ss(parameters);
				int count;
				char attacker_type, entrance;
				std::string buy;
				// gets parameters
				ss >> buy >> count >> attacker_type >> entrance;
				bool success = game.BuyAttackers(count, attacker_type, entrance);
				// prints new state
				interface.Clear();
				game.Print(interface);
				interface.Message("Buying " + std::string(success?"successful!":"failed!"));
				return true;
			},
			[](const std::string &command){
				std::stringstream ss(command);
				int count;
				char attacker_type, entrance;
				std::string buy;
				return (ss >> buy >> count >> attacker_type >> entrance) && buy == "buy";
			}
	};
}

CCommand QuitCommand(){
	return CCommand{
		"\"quit\" - Quits the game (without saving)",
		[](const CInterface &interface, CGame &game, const std::string &parameters){
			return false;
		},
		[](const std::string &command){
			return command == "quit";
		}
	};
}

CApplication::CApplication(CInterface interface):m_Interface(interface){
	m_Commands.push_back(HelpCommand(m_Commands));
	m_Commands.push_back(SaveCommand());
	m_Commands.push_back(StepCommand());
	m_Commands.push_back(BuyCommand());
	m_Commands.push_back(QuitCommand());
}

int CApplication::Run(){
	CGame game;
	CLoadingMenu loadingMenu;
	std::string tmp;

	// checks if filename is valid
	const auto validFilename = [&](const std::string &filename){
		return game.Load(loadingMenu.GetSavedir() + "/" + filename);
	};

	// checks if command is valid
	const auto validCommand = [&](const std::string &command){
		for(auto c: m_Commands)
			if(c.IsValid(command))
				return true;
		return false;
	};

	// prints game state
	const auto refreshGame = [&](){
		return game.Print(m_Interface);
	};

	// prints loading menu
	const auto refreshLoadingMenu = [&](){
		return loadingMenu.Print(m_Interface);
	};

	m_Interface.Clear();
	refreshLoadingMenu();

	// save directory could not be opened
	if(!m_Interface.GetFile(validFilename, refreshLoadingMenu)){
		m_Interface.Clear();
		m_Interface.Message("Save files could not be loaded, press enter to close the game\n");
		getline(std::cin,tmp);
		return 0;
	}

	m_Interface.Clear();
	game.Print(m_Interface);

	// plays the game
	while (true){
		std::string command = m_Interface.GetCommand(validCommand, refreshGame);

		m_Interface.Clear();
		game.Print(m_Interface);

		for(auto c: m_Commands)
			if(c.IsValid(command))
				if(!c.Execute(m_Interface, game, command))
					return 0;

		// game ended
		if(game.GetStatus() != IN_PROGRESS)
			break;
	}

	// asks if user wants to play again
	do{
		m_Interface.Clear();
		game.Print(m_Interface);
		m_Interface.Message("You ");
		m_Interface.Message((game.GetStatus()==WON?"won":"lost"));
		m_Interface.Message(" the game!\nType \"play\" to play a new game or type anything else to exit\n");
	} while(getline(std::cin,tmp) && tmp.empty());
	return tmp == "play";
}
