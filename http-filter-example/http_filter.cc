#include <string>

#include "http_filter.h"

#include "server/config/network/http_connection_manager.h"

namespace Envoy {
namespace Http {

const LowerCaseString HttpSampleDecoderFilter::headerKey = LowerCaseString("via");
const std::string HttpSampleDecoderFilter::headerValue = "sample-filter";

HttpSampleDecoderFilter::HttpSampleDecoderFilter() {}

HttpSampleDecoderFilter::~HttpSampleDecoderFilter() {}

void HttpSampleDecoderFilter::onDestroy() {}

FilterHeadersStatus HttpSampleDecoderFilter::decodeHeaders(HeaderMap& headers, bool) {
  // add a header
  headers.addStatic(headerKey, headerValue);

  return FilterHeadersStatus::Continue;
}

FilterDataStatus HttpSampleDecoderFilter::decodeData(Buffer::Instance&, bool) {
  return FilterDataStatus::Continue;
}

FilterTrailersStatus HttpSampleDecoderFilter::decodeTrailers(HeaderMap&) {
  return FilterTrailersStatus::Continue;
}

void HttpSampleDecoderFilter::setDecoderFilterCallbacks(StreamDecoderFilterCallbacks& callbacks) {
  decoder_callbacks_ = &callbacks;
}

} // Http
} // Envoy