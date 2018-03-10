#include <string>

#include "common/filter/reader/example/filter.h"

#include "envoy/registry/registry.h"
#include "envoy/server/filter_config.h"

#include "source/common/filter/reader/example/filter.pb.h"

namespace Envoy {
namespace Server {
namespace Configuration {

/**
 * Config registration for the example filter. @see NamedNetworkFilterConfigFactory.
 */
class ExampleReaderConfigFactory : public NamedNetworkFilterConfigFactory {
public:
  NetworkFilterFactoryCb createFilterFactory(const Json::Object&, FactoryContext&) override {
    return [](Network::FilterManager& filter_manager) -> void {
      filter_manager.addReadFilter(Network::ReadFilterSharedPtr{new Filter::ExampleReader()});
    };
  }

  NetworkFilterFactoryCb createFilterFactoryFromProto(const Protobuf::Message&,
                                                      FactoryContext&) override {
    return [](Network::FilterManager& filter_manager) -> void {
      filter_manager.addReadFilter(Network::ReadFilterSharedPtr{new Filter::ExampleReader()});
    };
  }

  ProtobufTypes::MessagePtr createEmptyConfigProto() override {
    return ProtobufTypes::MessagePtr{new my::api::v1::net::filter::reader::Example()};
  }

  std::string name() override { return "my.net.filter.reader.example"; }
};

/**
 * Static registration for the example filter. @see RegisterFactory.
 */
static Registry::RegisterFactory<ExampleReaderConfigFactory, NamedNetworkFilterConfigFactory> registered_;

} // namespace Configuration
} // namespace Server
} // namespace Envoy
