#include <string>

#include "extensions/filters/http/example/encoder/filter.h"

#include "common/config/json_utility.h"
#include "envoy/registry/registry.h"

#include "source/extensions/filters/http/example/encoder/filter.pb.h"
#include "source/extensions/filters/http/example/encoder/filter.pb.validate.h"

namespace Envoy {
namespace Server {
namespace Configuration {

class ExampleEncoderFilterConfig : public NamedHttpFilterConfigFactory {
public:
  HttpFilterFactoryCb createFilterFactory(const Json::Object& json_config, const std::string&,
                                          FactoryContext& context) override {

    io::example::http::filter::Encoder proto_config;
    translate(json_config, proto_config);

    return createFilter(proto_config, context);
  }

  HttpFilterFactoryCb createFilterFactoryFromProto(const Protobuf::Message& proto_config,
                                                   const std::string&,
                                                   FactoryContext& context) override {

    return createFilter(Envoy::MessageUtil::downcastAndValidate<
                            const io::example::http::filter::Encoder&>(proto_config),
                        context);
  }

  ProtobufTypes::MessagePtr createEmptyConfigProto() override {
    return ProtobufTypes::MessagePtr{new io::example::http::filter::Encoder()};
  }

  std::string name() override { return "io.example.http.filter.encoder"; }

private:
  HttpFilterFactoryCb
  createFilter(const io::example::http::filter::Encoder& proto_config,
               FactoryContext& context) {

    Http::ExampleEncoderFilterConfigSharedPtr config =
        std::make_shared<Http::ExampleEncoderFilterConfig>(
            Http::ExampleEncoderFilterConfig(proto_config));

    return [&context, config](Http::FilterChainFactoryCallbacks& callbacks) -> void {
      auto filter = new Http::ExampleEncoderFilter(config);
      callbacks.addStreamEncoderFilter(Http::StreamEncoderFilterSharedPtr{filter});
    };
  }

  void
  translate(const Json::Object& json_config,
                                   io::example::http::filter::Encoder& proto_config) {

    // normally we want to validate the json_config againts a defined
    // json-schema here.
    JSON_UTIL_SET_STRING(json_config, proto_config, key);
    JSON_UTIL_SET_STRING(json_config, proto_config, val);
  }
};

static Registry::RegisterFactory<ExampleEncoderFilterConfig, NamedHttpFilterConfigFactory>
    register_;

} // namespace Configuration
} // namespace Server
} // namespace Envoy