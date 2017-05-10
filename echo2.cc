#include "echo2.h"

#include "envoy/buffer/buffer.h"
#include "envoy/network/connection.h"

#include "common/common/assert.h"

namespace Envoy {
namespace Filter {

Network::FilterStatus Echo2::onData(Buffer::Instance& data) {
  conn_log_trace("echo: got {} bytes", read_callbacks_->connection(), data.length());
  read_callbacks_->connection().write(data);
  ASSERT(0 == data.length());
  return Network::FilterStatus::StopIteration;
}

} // Filter
} // Envoy
