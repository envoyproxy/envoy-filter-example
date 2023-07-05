#pragma once

#include <string>
#include <vector>

namespace Envoy {
namespace Http {

enum Operation {
  OP_SET_HEADER = 0,
  OP_SET_PATH = 1,
};

enum DynamicValue {
  DYN_URLP = 0,
  DYN_HDR = 1
};

class Condition {
public:
  Condition(std::string& condition); // make this a string_view
  ~Condition();

  int parseCondition(std::string& condition);
  int evaluateCondition();
};

class HeaderProcessor {
public:
  virtual int parseOperation(std::string& operation); // parses the arguments + condition, sends condition to its own class to be parsed
  virtual int evaluateOperation();
};

class SetHeaderProcessor : public HeaderProcessor {
public:
  SetHeaderProcessor(bool isRequest, std::string& set_header_expression);
  ~SetHeaderProcessor();

private:
  std::string fetchDynamicValue(DynamicValue val);

  bool isRequest_;
  std::string header_key_;
  std::vector<std::string> header_vals_;
  Envoy::Http::Condition condition_;
};

class SetRequestPath : public HeaderProcessor {
public:
  SetRequestPath();
  ~SetRequestPath();

private:
  std::string path_;
  Envoy::Http::Condition condition_;
};

} // namespace Http
} // namespace Envoy
