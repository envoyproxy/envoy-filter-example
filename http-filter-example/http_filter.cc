#include <string>

#include "http_filter.h"

#include "envoy/server/filter_config.h"

#include "iostream"

namespace Envoy {
namespace Http {

HttpSampleDecoderFilterConfig::HttpSampleDecoderFilterConfig(
    const sample::Decoder& proto_config)
    : key_(proto_config.key()), val_(proto_config.val()) {}

HttpSampleDecoderFilter::HttpSampleDecoderFilter(HttpSampleDecoderFilterConfigSharedPtr config)
    : config_(config), cluster_header_("") {}

HttpSampleDecoderFilter::~HttpSampleDecoderFilter() {}

void HttpSampleDecoderFilter::onDestroy() {}

const LowerCaseString HttpSampleDecoderFilter::headerKey() const {
  return LowerCaseString(config_->key());
}

const std::string HttpSampleDecoderFilter::headerValue() const {
  return config_->val();
}

const std::string HttpSampleDecoderFilter::readClusterHeader() const {
  return "routing_destination";
}

FilterHeadersStatus HttpSampleDecoderFilter::decodeHeaders(RequestHeaderMap& headers, bool) {
  std::cout << "BEGIN: decodeHeaders" << std::endl;

  // read header from shared memory and store in `cluster_header_`
  cluster_header_ = readClusterHeader();

  // add a header
  headers.addCopy(headerKey(), headerValue());
  // ENVOY_LOG(trace, "key: {}, value: {}", config_->key(), config_->val());
  std::cout <<"key: " << config_->key() << "value: " << config_->val() << std::endl;

  return FilterHeadersStatus::Continue;
}

FilterDataStatus HttpSampleDecoderFilter::decodeData(Buffer::Instance& data, bool) {
  // ENVOY_LOG(trace, "request body: {}", data.toString());
  std::cout << "BEGIN: decodeData" << std::endl;
  std::cout << "request body: " << data.toString() << std::endl;
  return FilterDataStatus::Continue;
}

void HttpSampleDecoderFilter::setDecoderFilterCallbacks(StreamDecoderFilterCallbacks& callbacks) {
  decoder_callbacks_ = &callbacks;
}

} // namespace Http
} // namespace Envoy
