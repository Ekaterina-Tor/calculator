#include "parser.h"

#include <cctype>      // для std::isspace, std::isdigit
#include <stdexcept>   // для std::runtime_error
#include <sstream>     // для std::stringstream

/// Вспомогательный прототипы функций для рекурсивного спуска
static double parseExpression(const std::string& s, size_t& pos);
static double parseTerm(const std::string& s, size_t& pos);
static double parseFactor(const std::string& s, size_t& pos);
static void skipWhitespace(const std::string& s, size_t& pos);

/// Основная функция evaluate:
double evaluate(const std::string& expr) {
    size_t pos = 0;  // Текущая позиция при разборе выражения
    double result = parseExpression(expr, pos);  // Начинаем разбирать как Expression
    skipWhitespace(expr, pos);
    // После разбора всего выражения pos должен указывать на конец строки
    if (pos != expr.length()) {
        throw std::runtime_error("Некорректный символ в выражении на позиции " + std::to_string(pos));
    }
    return result;
}

/// Пропуск пробельных символов (пробелы, табуляции и т.д.)
static void skipWhitespace(const std::string& s, size_t& pos) {
    while (pos < s.length() && std::isspace(static_cast<unsigned char>(s[pos]))) {
        ++pos;
    }
}

/// Разбор «выражения» (Expression ::= Term { ('+' | '-') Term })
static double parseExpression(const std::string& s, size_t& pos) {
    // Сначала разбираем левое слагаемое как Term
    double value = parseTerm(s, pos);

    while (true) {
        skipWhitespace(s, pos);
        if (pos >= s.length()) {
            break;  // конец строки — выходим
        }

        char op = s[pos];
        if (op != '+' && op != '-') {
            break;  // не '+' и не '-' — выходим на следующий уровень
        }
        ++pos;  // «съедаем» оператор

        // Разбираем следующее слагаемое
        double rhs = parseTerm(s, pos);
        if (op == '+') {
            value += rhs;  // "+" — складываем
        }
        else {
            value -= rhs;  // "-" — вычитаем
        }
    }
    return value;
}

/// Разбор «терма» (Term ::= Factor { ('*' | '/') Factor })
static double parseTerm(const std::string& s, size_t& pos) {
    // Сначала разбираем множитель
    double value = parseFactor(s, pos);

    while (true) {
        skipWhitespace(s, pos);
        if (pos >= s.length()) {
            break;  // конец строки — выходим
        }

        char op = s[pos];
        if (op != '*' && op != '/') {
            break;  // не '*' и не '/' — переходим вверх
        }
        ++pos;  // «съедаем» оператор

        // Разбираем следующий множитель
        double rhs = parseFactor(s, pos);
        if (op == '*') {
            value *= rhs;  // "*" — умножаем
        }
        else {
            if (rhs == 0.0) {
                throw std::runtime_error("Деление на ноль в позиции " + std::to_string(pos));
            }
            value /= rhs;  // "/" — делим
        }
    }
    return value;
}

/// Разбор «фактора» (Factor ::= number | '(' Expression ')')
static double parseFactor(const std::string& s, size_t& pos) {
    skipWhitespace(s, pos);
    if (pos >= s.length()) {
        throw std::runtime_error("Ожидался операнд, но достигнут конец строки");
    }

    // Если встречаем '(', то внутри парсируем Expression и ожидаем ')'
    if (s[pos] == '(') {
        ++pos;  // пропускаем '('
        double inner = parseExpression(s, pos);
        skipWhitespace(s, pos);
        if (pos >= s.length() || s[pos] != ')') {
            throw std::runtime_error("Отсутствует закрывающая скобка ')' на позиции " + std::to_string(pos));
        }
        ++pos;  // пропускаем ')'
        return inner;
    }

    // Иначе пытаемся считать число (целая или вещественная часть)
    size_t start = pos;
    bool hasDecimalPoint = false;  // флаг для точки в числе
    // Обрабатываем возможный знак перед числом
    if (s[pos] == '+' || s[pos] == '-') {
        ++pos;
    }
    // Основная часть: цифры и точка
    while (pos < s.length()) {
        char c = s[pos];
        if (c == '.') {
            if (hasDecimalPoint) break;  // вторая точка — выходим
            hasDecimalPoint = true;
            ++pos;
        }
        else if (std::isdigit(static_cast<unsigned char>(c))) {
            ++pos;
        }
        else {
            break;  // не цифра и не точка — конец числа
        }
    }
    if (start == pos || (pos == start + 1 && (s[start] == '+' || s[start] == '-'))) {
        // Не удалось распознать число
        throw std::runtime_error("Некорректное число, ожидается цифра на позиции " + std::to_string(start));
    }

    // Преобразуем подстроку в double
    double value = 0.0;
    try {
        std::string token = s.substr(start, pos - start);
        value = std::stod(token);
    }
    catch (const std::exception& e) {
        throw std::runtime_error("Ошибка преобразования в число на позиции " + std::to_string(start));
    }
    return value;
}
