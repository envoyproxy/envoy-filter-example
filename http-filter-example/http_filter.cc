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

  // parse config
  std::unordered_map<std::string, std::vector<std::string>> header_ops;

  std::string header_config = headerValue();

  // Find the position of the first space character
  size_t spacePos = header_config.find(' ');

  // Extract the key and value substrings
  std::string key = header_config.substr(0, spacePos);
  std::string args = header_config.substr(spacePos + 1);

  // Create a string stream to iterate over the rest of the string
  std::istringstream iss(args);
  std::vector<std::string> values;
  std::string value;

  // Split the rest of the string by space and store the values in a vector
  while (iss >> value) {
      values.push_back(value);
  }

  // Insert the key-value pair into the result map
  header_ops[key] = values;

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

  // // add a header
  // headers.addCopy(headerKey(), headerValue());
  // headers.addCopy(headerKey(), headerExtra()); // adds to the same key
  // std::string extra_header = std::to_string(headerExtra());
  // headers.appendCopy(headerKey(), extra_header);
  // headers.addCopy(headerKey(), headerExtra()); 
  // headers.setCopy(headerKey(), extra_header);


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
