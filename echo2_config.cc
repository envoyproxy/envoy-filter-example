#include <string>

#include "echo2.h"

#include "envoy/registry/registry.h"
#include "envoy/server/filter_config.h"

namespace Envoy {
namespace Server {
namespace Configuration {

/**
 * Config registration for the echo2 filter. @see NamedNetworkFilterConfigFactory.
 */
class Echo2ConfigFactory : public NamedNetworkFilterConfigFactory {
public:
  Network::FilterFactoryCb createFilterFactoryFromProto(const Protobuf::Message&,
                                                        FactoryContext&) override {
    return [](Network::FilterManager& filter_manager) -> void {
      filter_manager.addReadFilter(Network::ReadFilterSharedPtr{new Filter::Echo2()});
    };
  }

  ProtobufTypes::MessagePtr createEmptyConfigProto() override {
    return ProtobufTypes::MessagePtr{new Envoy::ProtobufWkt::Struct()};
  }

  std::string name() const override { return "echo2"; }

  bool isTerminalFilterByProto(const Protobuf::Message&, ServerFactoryContext&) override { return true; }
};

/**
 * Static registration for the echo2 filter. @see RegisterFactory.
 */
static Registry::RegisterFactory<Echo2ConfigFactory, NamedNetworkFilterConfigFactory> registered_;

} // namespace Configuration
} // namespace Server
} // namespace Envoy
