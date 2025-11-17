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