#include "extensions/filters/http/example/encoder/filter.h"

#include "gtest/gtest.h"

namespace Envoy {
namespace Http {
class ExampleEncoderFilterTest : public testing::Test {
  void initializeFilter(const std::string& yaml) {
    io::example::http::filter::Encoder config;
    MessageUtil::loadFromYaml(yaml, config);
  }
};

TEST_F(ExampleEncoderFilterTest, Request) {
}

} // namespace Http
} // namespace Envoy