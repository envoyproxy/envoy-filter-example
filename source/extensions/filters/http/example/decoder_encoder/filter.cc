#include <string>

#include "extensions/filters/http/example/decoder_encoder/filter.h"

#include "server/config/network/http_connection_manager.h"

namespace Envoy {
namespace Http {

ExampleDecoderEncoderFilterConfig::ExampleDecoderEncoderFilterConfig(
    const io::example::http::filter::DecoderEncoder& proto_config)
    : key_(proto_config.key()), val_(proto_config.val()) {}

ExampleDecoderEncoderFilter::ExampleDecoderEncoderFilter(ExampleDecoderEncoderFilterConfigSharedPtr config)
    : config_(config) {}

ExampleDecoderEncoderFilter::~ExampleDecoderEncoderFilter() {}

void ExampleDecoderEncoderFilter::onDestroy() {}

const LowerCaseString ExampleDecoderEncoderFilter::headerKey() const {
  return LowerCaseString(config_->key());
}

const std::string ExampleDecoderEncoderFilter::headerValue() const { return config_->val(); }

FilterHeadersStatus ExampleDecoderEncoderFilter::decodeHeaders(HeaderMap& headers, bool) {
  // add a header
  headers.addCopy(headerKey(), headerValue());

  return FilterHeadersStatus::Continue;
}

FilterDataStatus ExampleDecoderEncoderFilter::decodeData(Buffer::Instance&, bool) {
  return FilterDataStatus::Continue;
}

FilterTrailersStatus ExampleDecoderEncoderFilter::decodeTrailers(HeaderMap&) {
  return FilterTrailersStatus::Continue;
}

void ExampleDecoderEncoderFilter::setDecoderFilterCallbacks(StreamDecoderFilterCallbacks& callbacks) {
  decoder_callbacks_ = &callbacks;
}

FilterHeadersStatus ExampleDecoderEncoderFilter::encodeHeaders(HeaderMap& headers, bool) {
  // add a header
  headers.addCopy(headerKey(), headerValue());

  return FilterHeadersStatus::Continue;
}

FilterHeadersStatus ExampleDecoderEncoderFilter::encode100ContinueHeaders(HeaderMap&) {
  return FilterHeadersStatus::Continue;
}

FilterDataStatus ExampleDecoderEncoderFilter::encodeData(Buffer::Instance&, bool) {
  return FilterDataStatus::Continue;
}

FilterTrailersStatus ExampleDecoderEncoderFilter::encodeTrailers(HeaderMap&) {
  return FilterTrailersStatus::Continue;
}

void ExampleDecoderEncoderFilter::setEncoderFilterCallbacks(StreamEncoderFilterCallbacks& callbacks) {
  encoder_callbacks_ = &callbacks;
}

} // namespace Http
} // namespace Envoy