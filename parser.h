#ifndef PARSER_H
#define PARSER_H

#include <string>   // для std::string
#include <stdexcept> // для std::runtime_error

/// Функция evaluate:
/// - Принимает на вход строку expr, содержащую арифметическое выражение.
/// - Учёт возможных пробелов, операций +, -, *, / и скобок ( ).
/// - Возвращает результат вычисления выражения типа double.
/// - В случае некорректного выражения бросает исключение std::runtime_error.
double evaluate(const std::string& expr);

#endif // PARSER_H
