#include <string>

#include "http_filter.h"

#include "server/config/network/http_connection_manager.h"

namespace Envoy {
namespace Http {

HttpSampleDecoderFilter::HttpSampleDecoderFilter() {}

HttpSampleDecoderFilter::~HttpSampleDecoderFilter() {}

void HttpSampleDecoderFilter::onDestroy() {}

const LowerCaseString& HttpSampleDecoderFilter::headerKey() {
  static LowerCaseString* key = new LowerCaseString("via");
  return *key;
}

const std::string& HttpSampleDecoderFilter::headerValue() {
  static std::string* val = new std::string("sample-filter");
  return *val;
}

FilterHeadersStatus HttpSampleDecoderFilter::decodeHeaders(HeaderMap& headers, bool) {
  // add a header
  headers.addReference(headerKey(), headerValue());

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
