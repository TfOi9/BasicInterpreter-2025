#pragma once

#include <memory>
#include <string>

#include "Expression.hpp"

class Program;
class VarState;

class Statement {
 public:
  explicit Statement(std::string source);
  virtual ~Statement() = default;

  virtual void execute(VarState& state, Program& program) const = 0;

  const std::string& text() const noexcept;

 private:
  std::string source_;
};

// TODO: Other statement types derived from Statement, e.g., GOTOStatement,
// LetStatement, etc.
int stringToInteger(std::string str);

class LetStatement : public Statement {
 public:
  LetStatement(std::string source);
  ~LetStatement() override;
  void execute(VarState& state, Program& program) const override;
  void set(const std::string& var, Expression* exp);
 private:
  std::string var_;
  Expression* exp_;
};

class PrintStatement : public Statement {
 public:
  PrintStatement(std::string source);
  ~PrintStatement() override;
  void execute(VarState& state, Program& program) const override;
  void set(Expression* exp);
 private:
  Expression* exp_;
};

class InputStatement : public Statement {
 public:
  InputStatement(std::string source);
  ~InputStatement() override;
  void execute(VarState& state, Program& program) const override;
  void set(const std::string& var);
 private:
  std::string var_;
};

class GotoStatement : public Statement {
 public:
  GotoStatement(std::string source);
  ~GotoStatement() override;
  void execute(VarState& state, Program& program) const override;
  void set(int toLine);
 private:
  int toLine_;
};

class IfStatement : public Statement {
 public:
  IfStatement(std::string source);
  ~IfStatement() override;
  void execute(VarState& state, Program& program) const override;
  void set(Expression* expl, char op, Expression* expr, int toLine);
 private:
  Expression* expl_;
  char op_;
  Expression* expr_;
  int toLine_;
};

class RemStatement : public Statement {
 public:
  RemStatement(std::string source);
  ~RemStatement() override;
  void execute(VarState& state, Program& program) const override;
};

class EndStatement : public Statement {
 public:
  EndStatement(std::string source);
  ~EndStatement() override;
  void execute(VarState& state, Program& program) const override;
};

class IndentStatement : public Statement {
 public:
  IndentStatement(std::string source);
  ~IndentStatement() override;
  void execute(VarState& state, Program& program) const override;
};

class DedentStatement : public Statement {
 public:
  DedentStatement(std::string source);
  ~DedentStatement() override;
  void execute(VarState& state, Program& program) const override;
};