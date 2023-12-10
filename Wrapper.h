#pragma once
#include <iostream>
#include <map>
#include <vector>
#include <functional>
#include <string>
#include <optional>

using args_vec = std::vector<std::pair<std::string, int>>;
auto checkArgNames = [](args_vec args) {
	for (int i = 0; i < args.size(); i++)
		for (int j = i + 1; j < args.size(); j++)
			if (args[i].first == args[j].first)
				return false;
	return true;
};

class args_t {
public:
	args_t(args_vec args) {
		for (auto const& arg : args) {
			argsNames.push_back(arg.first);
			defaultValues.push_back(arg.second);
			argsMap.insert(arg);
		}
	}

	void setNewValues(args_vec args) {
		if (args.size() > argsMap.size())
			throw std::exception("Too many arguements");
		if (!checkArgNames(args))
			throw std::exception("Identical arguement names");

		for (auto const& arg : args) 
			argsMap[arg.first] = arg.second;
	}

	std::vector<int> getValues() {
		std::vector<int> vec;
		for (auto& item : argsMap)
			vec.push_back(item.second);
		return vec;
	}

private:
	std::vector<std::string> argsNames;
	std::vector<int> defaultValues;
	std::map<std::string, int> argsMap;
};

class Wrapper {
public:
	Wrapper() = delete;

	template<typename Subj, typename... Args>
	Wrapper(Subj* s, int(Subj::*method)(Args...), args_vec const& args) {
		if (!s || !method)
			throw std::exception("Unexpected 'nullptr'");
		if (!checkArgNames(args))
			throw std::exception("Identical arguement names");
		if (args.size() != sizeof...(Args))
			throw std::exception("Unexpected amount of arguements");

		_args = std::make_shared<args_t>(args);
		_method = [this, s, method](std::vector<int> const& commandArgs) {
			return callMethod(s, method, commandArgs, std::make_index_sequence<sizeof...(Args)>{});
		};
	}

	std::optional<int> execute(args_vec args) {
		try {
			_args->setNewValues(args);
		}
		catch (std::exception& e) {
			std::cout << e.what() << std::endl;
			return std::nullopt;
		}

		return _method(_args->getValues());
	}

private:
	std::shared_ptr<args_t> _args = nullptr;
	std::function<int(std::vector<int> const& methodArgs)> _method;

	template<typename Subj, typename Method, size_t... Index>
	int callMethod(Subj* s, Method method, std::vector<int> const& commandArgs, std::index_sequence<Index...>) {
		return (s->*method)(commandArgs[Index]...);
	}
};
