#pragma once

#include <string>

#include "server/config/network/http_connection_manager.h"

#include "source/extensions/filters/http/example/encoder/filter.pb.h"

namespace Envoy {
namespace Http {

class ExampleEncoderFilterConfig {
public:
  ExampleEncoderFilterConfig(
      const io::example::http::filter::Encoder& proto_config);

  const std::string& key() const { return key_; }
  const std::string& val() const { return val_; }

private:
  const std::string key_;
  const std::string val_;
};

typedef std::shared_ptr<ExampleEncoderFilterConfig> ExampleEncoderFilterConfigSharedPtr;

class ExampleEncoderFilter : public StreamEncoderFilter {
public:
  ExampleEncoderFilter(ExampleEncoderFilterConfigSharedPtr);
  ~ExampleEncoderFilter();

  // Http::StreamFilterBase
  void onDestroy() override;

  // Http::StreamEncoderFilter
  FilterHeadersStatus encodeHeaders(HeaderMap&, bool) override;
  FilterHeadersStatus encode100ContinueHeaders(HeaderMap& headers) override;
  FilterDataStatus encodeData(Buffer::Instance&, bool) override;
  FilterTrailersStatus encodeTrailers(HeaderMap&) override;
  void setEncoderFilterCallbacks(StreamEncoderFilterCallbacks&) override;

private:
  const ExampleEncoderFilterConfigSharedPtr config_;
  StreamEncoderFilterCallbacks* encoder_callbacks_;

  const LowerCaseString headerKey() const;
  const std::string headerValue() const;
};

} // namespace Http
} // namespace Envoy