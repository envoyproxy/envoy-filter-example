#include <string>

#include "extensions/filters/network/example/reader/filter.h"

#include "envoy/registry/registry.h"
#include "envoy/server/filter_config.h"

#include "source/extensions/filters/network/example/reader/filter.pb.h"

namespace Envoy {
namespace Server {
namespace Configuration {

/**
 * Config registration for the example reader filter. @see NamedNetworkFilterConfigFactory.
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
    return ProtobufTypes::MessagePtr{new io::example::network::filter::Reader()};
  }

  std::string name() override { return "io.example.network.filter.reader"; }
};

/**
 * Static registration for the example reader filter. @see RegisterFactory.
 */
static Registry::RegisterFactory<ExampleReaderConfigFactory, NamedNetworkFilterConfigFactory>
    registered_;

} // namespace Configuration
} // namespace Server
} // namespace Envoy
