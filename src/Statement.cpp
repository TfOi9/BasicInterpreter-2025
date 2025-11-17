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
    int val;
    try {
        size_t pos;
        val = std::stoi(str, &pos);
        // 检查是否整个字符串都被解析
        if (pos != str.length()) {
            throw BasicError("INT LITERAL OVERFLOW");
        }
    }
    catch (const std::out_of_range&) {
        throw BasicError("INT LITERAL OVERFLOW");
    }
    catch (const std::invalid_argument&) {
        throw BasicError("SYNTAX ERROR");
    }
    return val;
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
        throw;
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
        throw BasicError("LINE NUMBER ERROR");
    }
}

IfStatement::IfStatement(std::string source) : Statement(source), expl_(nullptr), expr_(nullptr) {}

IfStatement::~IfStatement() {
    if (expl_ != nullptr) {
        delete expl_;
    }
    if (expr_ != nullptr) {
        delete expr_;
    }
}

void IfStatement::set(Expression *expl, char op, Expression *expr, int toLine) {
    if (expl_ != nullptr) {
        delete expl_;
    }
    if (expr_ != nullptr) {
        delete expr_;
    }
    expl_ = expl;
    expr_ = expr;
    op_ = op;
    toLine_ = toLine;
}

void IfStatement::execute(VarState &state, Program &program) const {
    int vall, valr;
    vall = expl_->evaluate(state);
    valr = expr_->evaluate(state);
    bool satisfied;
    switch (op_) {
        case '<':
            satisfied =  vall < valr;
            break;
        case '>':
            satisfied = vall > valr;
            break;
        case '=':
            satisfied = vall == valr;
            break;
        default:
            satisfied = 0;
    }
    if (satisfied) {
        if (program.hasLine(toLine_)) {
            program.changePC(toLine_);
        }
        else {
            BasicError("LINE NUMBER ERROR");
        }
    }
}

RemStatement::RemStatement(std::string source) : Statement(source) {}

RemStatement::~RemStatement() {}

void RemStatement::execute(VarState &state, Program &program) const {}

EndStatement::EndStatement(std::string source) : Statement(source) {}

EndStatement::~EndStatement() {}

void EndStatement::execute(VarState &state, Program &program) const {
    program.programEnd();
}

IndentStatement::IndentStatement(std::string source) : Statement(source) {}

IndentStatement::~IndentStatement() {}

void IndentStatement::execute(VarState &state, Program &program) const {
    state.indent();
}

DedentStatement::DedentStatement(std::string source) : Statement(source) {}

DedentStatement::~DedentStatement() {}

void DedentStatement::execute(VarState &state, Program &program) const {
    state.dedent();
}