#pragma once

#include <string>

#include "source/extensions/filters/http/common/pass_through_filter.h"

#include "http-filter-example/http_filter.pb.h"

namespace Envoy {
namespace Http {

class HttpSampleDecoderFilterConfig {
public:
  HttpSampleDecoderFilterConfig(const sample::Decoder& proto_config);

  const std::string& key() const { return key_; }
  const std::string& val() const { return val_; }
  int extra() const { return extra_; }

private:
  const std::string key_;
  const std::string val_;
  const int extra_ = 0;
};

using HttpSampleDecoderFilterConfigSharedPtr = std::shared_ptr<HttpSampleDecoderFilterConfig>;

class HttpSampleDecoderFilter : public PassThroughDecoderFilter {
public:
  HttpSampleDecoderFilter(HttpSampleDecoderFilterConfigSharedPtr);
  ~HttpSampleDecoderFilter();

  // Http::StreamFilterBase
  void onDestroy() override;

  // Http::StreamDecoderFilter
  FilterHeadersStatus decodeHeaders(RequestHeaderMap&, bool) override;
  FilterDataStatus decodeData(Buffer::Instance&, bool) override;
  void setDecoderFilterCallbacks(StreamDecoderFilterCallbacks&) override;

private:
  const HttpSampleDecoderFilterConfigSharedPtr config_;
  StreamDecoderFilterCallbacks* decoder_callbacks_;

  const LowerCaseString headerKey() const;
  const std::string headerValue() const;
  int headerExtra() const;
};

} // namespace Http
} // namespace Envoy
