#include <string>

#include "extensions/filters/http/example/encoder/filter.h"

#include "server/config/network/http_connection_manager.h"

namespace Envoy {
namespace Http {

ExampleEncoderFilterConfig::ExampleEncoderFilterConfig(
    const io::example::http::filter::Encoder& proto_config)
    : key_(proto_config.key()), val_(proto_config.val()) {}

ExampleEncoderFilter::ExampleEncoderFilter(ExampleEncoderFilterConfigSharedPtr config)
    : config_(config) {}

ExampleEncoderFilter::~ExampleEncoderFilter() {}

void ExampleEncoderFilter::onDestroy() {}

const LowerCaseString ExampleEncoderFilter::headerKey() const {
  return LowerCaseString(config_->key());
}

const std::string ExampleEncoderFilter::headerValue() const { return config_->val(); }

FilterHeadersStatus ExampleEncoderFilter::encodeHeaders(HeaderMap& headers, bool) {
  // add a header
  headers.addCopy(headerKey(), headerValue());

  return FilterHeadersStatus::Continue;
}

FilterHeadersStatus ExampleEncoderFilter::encode100ContinueHeaders(HeaderMap&) {
  return FilterHeadersStatus::Continue;
}

FilterDataStatus ExampleEncoderFilter::encodeData(Buffer::Instance&, bool) {
  return FilterDataStatus::Continue;
}

FilterTrailersStatus ExampleEncoderFilter::encodeTrailers(HeaderMap&) {
  return FilterTrailersStatus::Continue;
}

void ExampleEncoderFilter::setEncoderFilterCallbacks(StreamEncoderFilterCallbacks& callbacks) {
  encoder_callbacks_ = &callbacks;
}

} // namespace Http
} // namespace Envoy