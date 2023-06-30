#include <string>
#include <unordered_map>
#include <sstream>
#include <vector>

#include "http_filter.h"

#include "envoy/server/filter_config.h"

namespace Envoy {
namespace Http {

HttpSampleDecoderFilterConfig::HttpSampleDecoderFilterConfig(
    const sample::Decoder& proto_config)
    : key_(proto_config.key()), val_(proto_config.val()), extra_(stoi(proto_config.extra())) {}

HttpSampleDecoderFilter::HttpSampleDecoderFilter(HttpSampleDecoderFilterConfigSharedPtr config)
    : config_(config) {}

HttpSampleDecoderFilter::~HttpSampleDecoderFilter() {}

void HttpSampleDecoderFilter::onDestroy() {}

const LowerCaseString HttpSampleDecoderFilter::headerKey() const {
  return LowerCaseString(config_->key());
}

const std::string HttpSampleDecoderFilter::headerValue() const {
  return config_->val();
}

int HttpSampleDecoderFilter::headerExtra() const {
  return config_->extra();
}

FilterHeadersStatus HttpSampleDecoderFilter::decodeHeaders(RequestHeaderMap& headers, bool) {

  // parse config: key is header operation, value is a vector of arguments for the operation
  std::unordered_map<std::string, std::vector<std::string>> header_ops;
  std::string header_config = headerValue();

  // Find the position of the first space character
  size_t spacePos = header_config.find(' ');
  std::string operation = header_config.substr(0, spacePos);
  std::string args = header_config.substr(spacePos + 1);

  // Create a string stream to iterate over the arguments
  std::istringstream iss(args);
  std::vector<std::string> values;
  std::string value;

  // Split the rest of the string by space and store the values in a vector
  while (iss >> value) {
      values.push_back(value);
  }

  // Insert the key-value pair into header_ops
  header_ops[operation] = values;

  // perform header operations
  for (auto it = header_ops.begin(); it != header_ops.end(); ++it) {
        std::string op = it->first;

        if (op == "set-header") {
          headers.setCopy(LowerCaseString(header_ops[op][0]), header_ops[op][1]);
        }
        else {
          headers.addCopy(LowerCaseString("no"), "op"); 
        }
  }

  return FilterHeadersStatus::Continue;
}

FilterDataStatus HttpSampleDecoderFilter::decodeData(Buffer::Instance&, bool) {
  return FilterDataStatus::Continue;
}

void HttpSampleDecoderFilter::setDecoderFilterCallbacks(StreamDecoderFilterCallbacks& callbacks) {
  decoder_callbacks_ = &callbacks;
}

} // namespace Http
} // namespace Envoy
