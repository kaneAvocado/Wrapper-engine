#pragma once
#include "Wrapper.h"

class Engine {
public:
    Engine() = default;
    ~Engine() = default;

    void register_command(Wrapper* wrapper, std::string const& commandName) {
        if (commandName == "") {
            throw std::exception("Invalid command name!");
            return;
        }

        if (wrapper == nullptr) {
            throw std::exception("Command is 'nullptr'!");
            return;
        }            

        if (commands.find(commandName) != commands.end()) {
            throw std::exception(("Command" + commandName + "is already exists").c_str());
            return;
        }           

        commands[commandName] = wrapper;
    }

    std::optional<int> execute(std::string const& commandName, args_vec args = {}) {
        if (commands.find(commandName) == commands.end()) {
            throw std::exception(("No such command" + commandName).c_str());
            return std::nullopt;
        }

        return commands[commandName]->execute(args);
    }

private:
    std::map<std::string, Wrapper*> commands;
};