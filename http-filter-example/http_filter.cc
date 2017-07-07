#include <string>

#include "common/common/base64.h"
#include "common/common/logger.h"
#include "common/http/headers.h"
#include "common/http/utility.h"
#include "envoy/server/instance.h"
#include "envoy/ssl/connection.h"
#include "server/config/network/http_connection_manager.h"


namespace Envoy{
namespace Http{
namespace SampleFilter{

class Config {
 public:
  Config(const Json::Object&) {}
};
typedef std::shared_ptr<Config> ConfigPtr;

const LowerCaseString headerKey = LowerCaseString("via");
const std::string headerValue = "sample-filter";

class Instance : public Http::StreamDecoderFilter {
 public:
  Instance(ConfigPtr) {}
  ~Instance() {}

  // Http::StreamFilterBase
  void onDestroy() override {}

  // Http::StreamDecoderFilter
  FilterHeadersStatus decodeHeaders(HeaderMap& headers, bool) override {
    // add a header
    headers.addStatic(headerKey, headerValue);

    return FilterHeadersStatus::Continue;
  }

  FilterDataStatus decodeData(Buffer::Instance&, bool) override {
    return FilterDataStatus::Continue;
  }
  FilterTrailersStatus decodeTrailers(HeaderMap&) override {
    return FilterTrailersStatus::Continue;
  }
  void setDecoderFilterCallbacks(StreamDecoderFilterCallbacks& callbacks) override {
    decoder_callbacks_ = &callbacks;
  }

 private:
  StreamDecoderFilterCallbacks* decoder_callbacks_;
  ConfigPtr config_;
};

} // SampleFilter
} // Http

namespace Server{
namespace Configuration {

class SampleFilterConfig : public HttpFilterConfigFactory {
 public:
  HttpFilterFactoryCb tryCreateFilterFactory(
        HttpFilterType type,
        const std::string& name,
        const Json::Object& config,
        const std::string&,
        Server::Instance&) override {
    if (type != HttpFilterType::Decoder || name != "sample")
      return nullptr;

    Http::SampleFilter::ConfigPtr sample_config(
        new Http::SampleFilter::Config(config));

    return [sample_config](Http::FilterChainFactoryCallbacks& callbacks) -> void {
        callbacks.addStreamDecoderFilter(
            Http::StreamDecoderFilterSharedPtr{new Http::SampleFilter::Instance(sample_config)});
    };
  }
};

static RegisterHttpFilterConfigFactory<SampleFilterConfig> register_;

} // Configuration
} // Server
} // Envoy