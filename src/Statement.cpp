#include "../include/Statement.hpp"

#include <iostream>
#include <limits>
#include <sstream>
#include <string>
#include <utility>

#include "../include/Program.hpp"
#include "../include/VarState.hpp"
#include "../include/utils/Error.hpp"

int stringToInteger(std::string str) {
    std::istringstream stream(str);
    int value;
    stream >> value;
    if (!stream.eof()) stream >> std::ws;
    if (stream.fail() || !stream.eof()) {
        BasicError("INVALID NUMBER");
    }
    return value;
}

Statement::Statement(std::string source) : source_(std::move(source)) {}

const std::string& Statement::text() const noexcept { return source_; }

// TODO: Imply interfaces declared in the Statement.hpp.
LetStatement::LetStatement(std::string source) : Statement(source), exp_(nullptr) {}

LetStatement::~LetStatement() {
    if (exp_ != nullptr) {
        delete exp_;
    }
}

void LetStatement::set(const std::string& var, Expression* exp) {
    var_ = var;
    if (exp_ != nullptr) {
        delete exp_;
    }
    exp_ = exp;
}

void LetStatement::execute(VarState& state, Program& program) const {
    int val;
    try {
        val = exp_->evaluate(state);
    }
    catch (...) {
        val = 0;
    }
    state.setValue(var_, val);
}

PrintStatement::PrintStatement(std::string source) : Statement(source), exp_(nullptr) {}

PrintStatement::~PrintStatement() {
    if (exp_ != nullptr) {
        delete exp_;
    }
}

void PrintStatement::set(Expression* exp) {
    if (exp_ != nullptr) {
        delete exp_;
    }
    exp_ = exp;
}

void PrintStatement::execute(VarState &state, Program &program) const {
    int val;
    try {
        val = exp_->evaluate(state);
    }
    catch (...) {
        throw;
    }
    std::cout << val << std::endl;
}

InputStatement::InputStatement(std::string source) : Statement(source) {}

InputStatement::~InputStatement() {}

void InputStatement::set(const std::string& var) {
    var_ = var;
}

void InputStatement::execute(VarState &state, Program &program) const {
    std::string str;
    while (1) {
        int val = 0;
        bool valid = 1;
        std::cout << " ? ";
        std::getline(std::cin, str);
        try {
            val = stringToInteger(str);
        }
        catch (...) {
            valid = 0;
            str.clear();
            std::cout << "INVALID NUMBER" << std::endl;
        }
        if (valid) {
            state.setValue(var_, val);
            break;
        }
    }
}

GotoStatement::GotoStatement(std::string source) : Statement(source) {}

GotoStatement::~GotoStatement() {}

void GotoStatement::set(int toLine) {
    toLine_ = toLine;
}

void GotoStatement::execute(VarState& state, Program& program) const {
    if (program.hasLine(toLine_)) {
        program.changePC(toLine_);
    }
    else {
        BasicError("LINE NUMBER ERROR");
    }
}