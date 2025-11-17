#include <iostream>
#include <memory>
#include <string>

#include "../include/Lexer.hpp"
#include "../include/Parser.hpp"
#include "../include/Program.hpp"
#include "../include/Token.hpp"
#include "../include/utils/Error.hpp"

int main() {
  Lexer lexer;
  Parser parser;
  Program program;

  std::string line;
  while (std::getline(std::cin, line)) {
    if (line.empty()) {
      continue;
    }
    bool quitFlag = 0;
    try {
      // TODO: The main function.
      TokenStream stream = lexer.tokenize(line);
      const Token* tok = stream.peek();
      switch (tok->type) {
        case TokenType::HELP:
          // ...some help messages are supposed to be printed
          break;
        case TokenType::QUIT:
          quitFlag = 1;
          break;
        case TokenType::CLEAR:
          program.clear();
          break;
        case TokenType::LIST:
          program.list();
          break;
        case TokenType::RUN:
          program.run();
          break;
        default:
          ParsedLine pline = parser.parseLine(stream, line);
          if (pline.getLine().has_value()) {
            if (pline.getStatement() != nullptr) {
              program.addStmt(pline.getLine().value(), pline.fetchStatement());
            }
            else {
              program.removeStmt(pline.getLine().value());
            }
          }
          else {
            program.execute(pline.getStatement());
          }
      }
    } catch (const BasicError& e) {
      std::cout << e.message() << "\n";
    }
    if (quitFlag) {
      break;
    }
  }
  return 0;
}