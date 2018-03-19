#pragma once

#include <string>

#include "server/config/network/http_connection_manager.h"

#include "source/extensions/filters/http/example/decoder_encoder/filter.pb.h"

namespace Envoy {
namespace Http {

class ExampleDecoderEncoderFilterConfig {
public:
  ExampleDecoderEncoderFilterConfig(
      const io::example::http::filter::DecoderEncoder& proto_config);

  const std::string& key() const { return key_; }
  const std::string& val() const { return val_; }

private:
  const std::string key_;
  const std::string val_;
};

typedef std::shared_ptr<ExampleDecoderEncoderFilterConfig> ExampleDecoderEncoderFilterConfigSharedPtr;

class ExampleDecoderEncoderFilter : public StreamFilter {
public:
  ExampleDecoderEncoderFilter(ExampleDecoderEncoderFilterConfigSharedPtr);
  ~ExampleDecoderEncoderFilter();

  // Http::StreamFilterBase
  void onDestroy() override;

  // Http::StreamDecoderFilter
  FilterHeadersStatus decodeHeaders(HeaderMap&, bool) override;
  FilterDataStatus decodeData(Buffer::Instance&, bool) override;
  FilterTrailersStatus decodeTrailers(HeaderMap&) override;
  void setDecoderFilterCallbacks(StreamDecoderFilterCallbacks&) override;

  // Http::StreamEncoderFilter
  FilterHeadersStatus encodeHeaders(HeaderMap&, bool) override;
  FilterHeadersStatus encode100ContinueHeaders(HeaderMap& headers) override;
  FilterDataStatus encodeData(Buffer::Instance&, bool) override;
  FilterTrailersStatus encodeTrailers(HeaderMap&) override;
  void setEncoderFilterCallbacks(StreamEncoderFilterCallbacks&) override;

private:
  const ExampleDecoderEncoderFilterConfigSharedPtr config_;
  StreamDecoderFilterCallbacks* decoder_callbacks_;
  StreamEncoderFilterCallbacks* encoder_callbacks_;

  const LowerCaseString headerKey() const;
  const std::string headerValue() const;
};

} // namespace Http
} // namespace Envoy