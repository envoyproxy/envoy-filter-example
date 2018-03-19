#pragma once

#include "envoy/network/filter.h"

#include "common/common/logger.h"

#include "source/extensions/filters/network/example/reader/filter.pb.h"

namespace Envoy {
namespace Filter {

/**
 * Implementation of a basic read, an echo filter.
 */
class ExampleReader : public Network::ReadFilter, Logger::Loggable<Logger::Id::filter> {
public:
  // Network::ReadFilter
  Network::FilterStatus onData(Buffer::Instance& data, bool end_stream) override;
  Network::FilterStatus onNewConnection() override { return Network::FilterStatus::Continue; }
  void initializeReadFilterCallbacks(Network::ReadFilterCallbacks& callbacks) override {
    read_callbacks_ = &callbacks;
  }

private:
  Network::ReadFilterCallbacks* read_callbacks_{};
};

} // namespace Filter
} // namespace Envoy
