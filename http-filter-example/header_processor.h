#pragma once

#include "source/extensions/filters/http/common/pass_through_filter.h"

#include <string>
#include <vector>

namespace Envoy {
namespace Http {

enum DynamicValue {
  DYN_URLP = 0,
  DYN_HDR = 1
};

class AclProcessor {
public:
  AclProcessor() {}
  ~AclProcessor() {}
  bool evaluateAcl(absl::string_view acl_expression);

private:
  bool isTrue_; // not needed?
};

class ConditionProcessor {
public:
  ConditionProcessor() {}
  ~ConditionProcessor() {}
  bool evaluateCondition(absl::string_view condition_expression);

private:
  std::vector<Envoy::Http::AclProcessor> acl_processors_; // is there a need to store each acl?
  bool isTrue_; // not needed? can directly return the result without storing it
};

class HeaderProcessor {
public:
  virtual ~HeaderProcessor() = default;
  virtual int parseOperation(absl::string_view operation_expression) = 0; // parses the arguments + condition, sends each acl to AclProcessor to be parsed
  virtual int evaluateOperation(RequestHeaderMap& headers) = 0;
  virtual int evaluateCondition(absl::string_view condition_expression) = 0;
  bool getCondition() const { return condition_; }
  void setCondition(bool result) { condition_ = result; }

protected:
  bool condition_;
};

class SetHeaderProcessor : public HeaderProcessor {
public:
  SetHeaderProcessor(bool isRequest, absl::string_view set_header_expression);
  virtual ~SetHeaderProcessor() {}
  virtual int parseOperation(absl::string_view operation_expression);
  virtual int evaluateOperation(RequestHeaderMap& headers);
  virtual int evaluateCondition(absl::string_view condition_expression); // TODO: will need to pass http-related metadata in order to evaluate dynamic values

  const std::string& getKey() const { return header_key_; }
  const std::vector<std::string>& getVals() const { return header_vals_; }
  void setKey(std::string key) { header_key_ = key; }
  void setVals(std::vector<std::string> vals) { header_vals_ = vals; }
  bool isHttpRequest() const { return isRequest_; } // possibly superfluous

private:
  std::string fetchDynamicValue(DynamicValue val); // TODO: fetch query params, header values, etc -- leaving implementation for later, possibly need to make a separate class

  bool isRequest_; // possibly superfluous: http filter will use this to determine whether to evaluate the operation in encodeHeaders or decodeHeaders
  std::string header_key_; // header key to set
  std::vector<std::string> header_vals_; // header values to set
};

class SetRequestPath : public HeaderProcessor {
public:
  SetRequestPath();
  virtual ~SetRequestPath() {}
  virtual int parseOperation(absl::string_view operation_expression);
  virtual int evaluateOperation(RequestHeaderMap& headers);
  virtual int evaluateCondition(absl::string_view condition_expression);

private:
  std::string path_; // path to set
};

} // namespace Http
} // namespace Envoy
