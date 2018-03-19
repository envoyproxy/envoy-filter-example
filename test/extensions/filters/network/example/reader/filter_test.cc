#include "extensions/filters/network/example/reader/filter.h"

#include "common/protobuf/utility.h"

#include "gtest/gtest.h"

namespace Envoy {
namespace Http {
class ExampleReaderFilterTest : public testing::Test {
  void initializeFilter(const std::string& yaml) {
    io::example::network::filter::Reader config;
    MessageUtil::loadFromYaml(yaml, config);
  }
};

TEST_F(ExampleReaderFilterTest, Request) {
}

} // namespace Http
} // namespace Envoy