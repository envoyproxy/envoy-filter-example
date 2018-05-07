#include <string>

#include "http_filter.h"

#include "common/config/json_utility.h"
#include "envoy/registry/registry.h"

#include "http-filter-example/http_filter.pb.h"
#include "http-filter-example/http_filter.pb.validate.h"

namespace Envoy {
namespace Server {
namespace Configuration {

class HttpSampleDecoderFilterConfig : public NamedHttpFilterConfigFactory {
public:
  Http::FilterFactoryCb createFilterFactory(const Json::Object& json_config, const std::string&,
                                            FactoryContext& context) override {

    sample::Decoder proto_config;
    translateHttpSampleDecoderFilter(json_config, proto_config);

    return createFilter(proto_config, context);
  }

  Http::FilterFactoryCb createFilterFactoryFromProto(const Protobuf::Message& proto_config,
                                                     const std::string&,
                                                     FactoryContext& context) override {

    return createFilter(
        Envoy::MessageUtil::downcastAndValidate<const sample::Decoder&>(proto_config), context);
  }

  /**
   *  Return the Protobuf Message that represents your config incase you have config proto
   */
  ProtobufTypes::MessagePtr createEmptyConfigProto() override {
    return ProtobufTypes::MessagePtr{new sample::Decoder()};
  }

  std::string name() override { return "sample"; }

private:
  Http::FilterFactoryCb createFilter(const sample::Decoder& proto_config, FactoryContext&) {
    Http::HttpSampleDecoderFilterConfigSharedPtr config =
        std::make_shared<Http::HttpSampleDecoderFilterConfig>(
            Http::HttpSampleDecoderFilterConfig(proto_config));

    return [config](Http::FilterChainFactoryCallbacks& callbacks) -> void {
      auto filter = new Http::HttpSampleDecoderFilter(config);
      callbacks.addStreamDecoderFilter(Http::StreamDecoderFilterSharedPtr{filter});
    };
  }

  void translateHttpSampleDecoderFilter(const Json::Object& json_config,
                                        sample::Decoder& proto_config) {

    // normally we want to validate the json_config againts a defined json-schema here.
    JSON_UTIL_SET_STRING(json_config, proto_config, key);
    JSON_UTIL_SET_STRING(json_config, proto_config, val);
  }
};

/**
 * Static registration for this sample filter. @see RegisterFactory.
 */
static Registry::RegisterFactory<HttpSampleDecoderFilterConfig, NamedHttpFilterConfigFactory>
    register_;

} // namespace Configuration
} // namespace Server
} // namespace Envoy
