#include <string>

#include "extensions/filters/http/example/decoder/filter.h"

#include "server/config/network/http_connection_manager.h"

namespace Envoy {
namespace Http {

ExampleDecoderFilterConfig::ExampleDecoderFilterConfig(
    const io::example::http::filter::Decoder& proto_config)
    : key_(proto_config.key()), val_(proto_config.val()) {}

ExampleDecoderFilter::ExampleDecoderFilter(ExampleDecoderFilterConfigSharedPtr config)
    : config_(config) {}

ExampleDecoderFilter::~ExampleDecoderFilter() {}

void ExampleDecoderFilter::onDestroy() {}

const LowerCaseString ExampleDecoderFilter::headerKey() const {
  return LowerCaseString(config_->key());
}

const std::string ExampleDecoderFilter::headerValue() const { return config_->val(); }

FilterHeadersStatus ExampleDecoderFilter::decodeHeaders(HeaderMap& headers, bool) {
  // add a header
  headers.addCopy(headerKey(), headerValue());

  return FilterHeadersStatus::Continue;
}

FilterDataStatus ExampleDecoderFilter::decodeData(Buffer::Instance&, bool) {
  return FilterDataStatus::Continue;
}

FilterTrailersStatus ExampleDecoderFilter::decodeTrailers(HeaderMap&) {
  return FilterTrailersStatus::Continue;
}

void ExampleDecoderFilter::setDecoderFilterCallbacks(StreamDecoderFilterCallbacks& callbacks) {
  decoder_callbacks_ = &callbacks;
}

} // namespace Http
} // namespace Envoy