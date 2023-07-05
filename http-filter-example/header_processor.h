#pragma once

#include "source/extensions/filters/http/common/pass_through_filter.h"

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

class ConditionProcessor {
public:
  ConditionProcessor(std::string& condition_expression); // make this a string_view
  ~ConditionProcessor() {}
  bool evaluateCondition();

private:
  int parseCondition(std::string& condition_expression);
  bool isTrue_;
};

class HeaderProcessor {
public:
  virtual int parseOperation(std::string& operation) = 0; // parses the arguments + condition, sends condition to its own class to be parsed
  virtual int evaluateOperation(RequestHeaderMap& headers) = 0;
  virtual int evaluateConditions(std::string& conditions_expression) = 0;
  const bool getCondition() const { return condition_; }
  void setCondition(bool result) { condition_ = result; }

private:
  std::vector<Envoy::Http::ConditionProcessor> condition_processors_;
  bool condition_;
};

class SetHeaderProcessor : public HeaderProcessor {
public:
  SetHeaderProcessor(bool isRequest, std::string& set_header_expression);
  ~SetHeaderProcessor() {}
  virtual int parseOperation(std::string& operation);
  virtual int evaluateOperation(RequestHeaderMap& headers);
  virtual int evaluateConditions(std::string& conditions_expression); // pass http-related metadata
  const std::string& getKey() const { return header_key_; }
  const std::vector<std::string> getVals() const { return header_vals_; } // TODO pass by reference instead
  const bool isHttpRequest() const { return isRequest_; }

private:
  std::string fetchDynamicValue(DynamicValue val); // TODO brainstorm implementation

  bool isRequest_; // http filter will use this to determine whether to evaluate the operation in encodeHeaders or decodeHeaders
  std::string header_key_;
  std::vector<std::string> header_vals_;
};

class SetRequestPath : public HeaderProcessor {
public:
  SetRequestPath();
  ~SetRequestPath() {}
  virtual int parseOperation(std::string& operation);
  virtual int evaluateOperation(RequestHeaderMap& headers);
  virtual int evaluateConditions(std::string& conditions_expression);

private:
  std::string path_;
};

} // namespace Http
} // namespace Envoy
