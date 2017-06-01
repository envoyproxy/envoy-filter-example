#include "envoy/network/connection.h"

#include "echo2.h"
#include "server/configuration_impl.h"

namespace Envoy {
namespace Server {
namespace Configuration {

/**
 * Config registration for the echo2 filter. @see NamedNetworkFilterConfigFactory.
 */
class Echo2ConfigFactory : public NamedNetworkFilterConfigFactory {
public:
  // NamedNetworkFilterConfigFactory
  NetworkFilterFactoryCb createFilterFactory(NetworkFilterType type, const Json::Object&,
                                             Server::Instance&) override {
    if (type != NetworkFilterType::Read) {
      throw EnvoyException(
          fmt::format("{} network filter must be configured as a read filter.", name()));
    }

    return [](Network::FilterManager& filter_manager) -> void {
      filter_manager.addReadFilter(Network::ReadFilterSharedPtr{new Filter::Echo2()});
    };
  }

  std::string name() override { return "echo2"; }
};

/**
 * Static registration for the echo2 filter. @see RegisterNamedNetworkFilterConfigFactory.
 */
static RegisterNamedNetworkFilterConfigFactory<Echo2ConfigFactory> registered_;

} // Configuration
} // Server
} // Envoy
