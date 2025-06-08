#include <iostream>     // для std::cout, std::cin
#include <string>       // для std::string
#include <stdexcept>    // для std::runtime_error
#include <Windows.h>    // для SetConsole

#include "parser.h"     // объявление функции evaluate

int main() {

    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    std::cout << "Калькулятор арифметических выражений\n";
    std::cout << "Введите выражение (можно использовать +, -, *, /, скобки): ";

    // Читаем всю строку целиком, с пробелами
    std::string input;
    std::getline(std::cin, input);

    try {
        // Вызываем функцию evaluate, чтобы вычислить результат
        double result = evaluate(input);

        // Выводим результат на экран
        std::cout << "Результат: " << result << std::endl;
    }
    catch (const std::runtime_error& e) {
        // Если где-то в процессе разбора/вычисления возникла ошибка — сообщаем пользователю
        std::cout << "Ошибка: " << e.what() << std::endl;
    }

    return 0;
}
