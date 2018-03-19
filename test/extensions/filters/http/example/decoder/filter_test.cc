#include "extensions/filters/http/example/decoder/filter.h"

#include "gtest/gtest.h"

namespace Envoy {
namespace Http {
class ExampleDecoderFilterTest : public testing::Test {
  void initializeFilter(const std::string& yaml) {
    io::example::http::filter::Decoder config;
    MessageUtil::loadFromYaml(yaml, config);
  }
};

TEST_F(ExampleDecoderFilterTest, Request) {
}

} // namespace Http
} // namespace Envoy