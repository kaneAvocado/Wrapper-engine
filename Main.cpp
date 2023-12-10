#include "Wrapper.h"
#include "Engine.h"
#include "Subject.h"

int main() {
	Subject subj;

	Wrapper wrapper(&subj, &Subject::test, { {"arg1", 0}, {"arg2", 0} });
	Wrapper* nullptr_wrapper = nullptr;

	Engine engine;

	// OK
	try {
		engine.register_command(&wrapper, "command1");
	}
	catch (std::exception& e) {
		std::cout << e.what() << std::endl;
		return 1;
	}

	// ОШИБКА: неверное имя команды
	try {
		engine.register_command(&wrapper, "");
	}
	catch (std::exception& e) {
		std::cout << e.what() << std::endl;
	}

	// ОШИБКА: wrapper равен nullptr
	try {
		engine.register_command(nullptr_wrapper, "command1");
	}
	catch (std::exception& e) {
		std::cout << e.what() << std::endl;
	}

	// OK: возвращает 1
	try {
		std::optional<int> check = engine.execute("command1", { {"arg1", 5}, {"arg2", 4} });

		if (check.has_value())
			std::cout << check.value() << std::endl;
		else
			std::cout << "Что-то пошло не так! Проверьте исключения выше." << std::endl;
	}
	catch (std::exception& e) {
		std::cout << e.what() << std::endl;
	}

	// ОШИБКА: неверное имя команды
	try {
		engine.execute("ccc", { {"arg1", 5}, {"arg2", 4} });
	}
	catch (std::exception& e) {
		std::cout << e.what() << std::endl;
	}

	// ОШИБКА: неверные имена аргументов
	try {
		engine.execute("command1", { {"a1", 5}, {"a2", 4} });
	}
	catch (std::exception& e) {
		std::cout << e.what() << std::endl;
	}

	// OK: возвращает -1
	try {
		std::optional<int> check = engine.execute("command1", { {"arg2", 5}, {"arg1", 4} });

		if (check.has_value())
			std::cout << check.value() << std::endl;
		else
			std::cout << "Что-то пошло не так! Проверьте исключения выше." << std::endl;
	}
	catch (std::exception& e) {
		std::cout << e.what() << std::endl;
	}

	return 0;
}
