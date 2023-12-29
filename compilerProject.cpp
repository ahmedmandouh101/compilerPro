#include <iostream>
#include <cstdlib> 
#include <cctype>
#include <string>




enum TokenType {
    NUMBER,
    PLUS,
    MINUS,
    MULTIPLY,
    DIVIDE,
    LPAREN,
    RPAREN,
    END
};



struct Token {
    TokenType type;
    std::string value;
};




class Lexer {
public:
    Lexer(const std::string& input) : input(input), currentPos(0) {}
	

    Token getNextToken() {
        while (currentPos < input.length()) {
            char currentChar = input[currentPos];

            if (std::isdigit(currentChar)) {
                return processNumber();
            }
            
            if (std::isspace(currentChar)) {
            // Skip whitespace characters
            currentPos++;
            continue;
        }

            switch (currentChar) {
                case '+':
                    currentPos++;
                    return {PLUS, "+"};
                case '-':
                    currentPos++;
                    return {MINUS, "-"};
                case '*':
                    currentPos++;
                    return {MULTIPLY, "*"};
                case '/':
                    currentPos++;
                    return {DIVIDE, "/"};
                case '(':
                    currentPos++;
                    return {LPAREN, "("};
                case ')':
                    currentPos++;
                    return {RPAREN, ")"};
                default:
                    std::cerr << "Error: Unexpected character '" << currentChar << "' at position " << currentPos << std::endl;
                    exit(EXIT_FAILURE);
            }
        }

        return {END, ""};
    }

private:

    Token processNumber() {
        std::string number;
        while (currentPos < input.length() && (std::isdigit(input[currentPos]) || input[currentPos] == '.')) {
            number += input[currentPos++];
        }
        return {NUMBER, number};
    }

    std::string input;
    size_t currentPos;
};



class Parser {
public:
    Parser(const std::string& input) : lexer(input), currentToken(lexer.getNextToken()) {}
	

    double parse() {
        return parseExpression();
    }

private:

    double parseExpression() {
        double result = parseTerm();

        while (currentToken.type == PLUS || currentToken.type == MINUS) {
            Token op = currentToken;
            if (op.type == PLUS) {
                match(PLUS);
                result += parseTerm();
            } else if (op.type == MINUS) {
                match(MINUS);
                result -= parseTerm();
            }
        }

        return result;
    }

    double parseTerm() {
        double result = parseFactor();

        while (currentToken.type == MULTIPLY || currentToken.type == DIVIDE) {
            Token op = currentToken;
            if (op.type == MULTIPLY) {
                match(MULTIPLY);
                result *= parseFactor();
            } else if (op.type == DIVIDE) {
                match(DIVIDE);
                double divisor = parseFactor();
                if (divisor != 0) {
                    result /= divisor;
                } else {
                    std::cerr << "Error: Division by zero." << std::endl;
                    exit(EXIT_FAILURE);
                }
            }
        }

        return result;
    }

    double parseFactor() {
        if (currentToken.type == NUMBER) {
            double value = std::atof(currentToken.value.c_str());
            match(NUMBER);
            return value;
        } else if (currentToken.type == LPAREN) {
            match(LPAREN);
            double result = parseExpression();
            match(RPAREN);
            return result;
        } else {
            std::cerr << "Error: Unexpected token '" << currentToken.value << "' of type " << currentToken.type << std::endl;
            exit(EXIT_FAILURE);
        }
    }
    


    void match(TokenType expectedType) {
        if (currentToken.type == expectedType) {
            currentToken = lexer.getNextToken();
        } else {
            std::cerr << "Error: Expected token of type " << expectedType << ", but got token of type " << currentToken.type << std::endl;
            exit(EXIT_FAILURE);
        }
    }

    Lexer lexer;
    Token currentToken;
};


int main() {
    std::string input = "3 + 4 * ( 2 - 1 ) / 5";
    Parser parser(input);
    double result = parser.parse();
    std::cout << "Result: " << result << std::endl;

    return 0;
}


