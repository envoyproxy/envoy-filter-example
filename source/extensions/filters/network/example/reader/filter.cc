#include "extensions/filters/network/example/reader/filter.h"

#include "envoy/buffer/buffer.h"
#include "envoy/network/connection.h"

#include "common/common/assert.h"

namespace Envoy {
namespace Filter {

Network::FilterStatus ExampleReader::onData(Buffer::Instance& data, bool) {
  ENVOY_CONN_LOG(trace, "echo: got {} bytes", read_callbacks_->connection(), data.length());
  read_callbacks_->connection().write(data, false);
  ASSERT(0 == data.length());
  return Network::FilterStatus::StopIteration;
}

} // namespace Filter
} // namespace Envoy
