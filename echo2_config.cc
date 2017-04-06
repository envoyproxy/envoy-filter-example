#include "envoy/network/connection.h"

#include "echo2.h"
#include "server/configuration_impl.h"

namespace Server {
namespace Configuration {

/**
 * Config registration for the echo filter. @see NetworkFilterConfigFactory.
 */
class Echo2ConfigFactory : public NetworkFilterConfigFactory {
public:
  // NetworkFilterConfigFactory
  NetworkFilterFactoryCb tryCreateFilterFactory(NetworkFilterType type, const std::string& name,
                                                const Json::Object&, Server::Instance&) {
    if (type != NetworkFilterType::Read || name != "echo2") {
      return nullptr;
    }

    return [](Network::FilterManager& filter_manager)
        -> void { filter_manager.addReadFilter(Network::ReadFilterSharedPtr{new Filter::Echo2()}); };
  }
};

/**
 * Static registration for the echo filter. @see RegisterNetworkFilterConfigFactory.
 */
static RegisterNetworkFilterConfigFactory<Echo2ConfigFactory> registered_;

} // Configuration
} // Server
