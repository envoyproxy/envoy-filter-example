#include <string>

#include "http_filter.h"

#include "common/protobuf/utility.h"
#include "envoy/registry/registry.h"

namespace Envoy {
namespace Server {
namespace Configuration {

class HttpSampleDecoderFilterConfig : public NamedHttpFilterConfigFactory {
public:
  HttpFilterFactoryCb createFilterFactory(const Json::Object&, const std::string&,
                                          FactoryContext&) override {
    return [](Http::FilterChainFactoryCallbacks& callbacks) -> void {
      callbacks.addStreamDecoderFilter(
          Http::StreamDecoderFilterSharedPtr{new Http::HttpSampleDecoderFilter()});
    };
  }

  HttpFilterFactoryCb createFilterFactoryFromProto(const Protobuf::Message&,
                                                   const std::string&,
                                                   FactoryContext&) override {
    return [](Http::FilterChainFactoryCallbacks& callbacks) -> void {
      callbacks.addStreamDecoderFilter(
          Http::StreamDecoderFilterSharedPtr{new Http::HttpSampleDecoderFilter()});
    };
  }

  /**
   *  Return the Protobuf Message that represents your config incase you have config proto
   */
  ProtobufTypes::MessagePtr createEmptyConfigProto() override {
    return ProtobufTypes::MessagePtr{new Envoy::ProtobufWkt::Empty()}; 
  }

  std::string name() override { return "sample"; }
};

/**
 * Static registration for this sample filter. @see RegisterFactory.
 */
static Registry::RegisterFactory<HttpSampleDecoderFilterConfig, NamedHttpFilterConfigFactory>
    register_;

} // Configuration
} // Server
} // Envoy
