#include "extensions/filters/http/example/decoder_encoder/filter.h"

#include "gtest/gtest.h"

namespace Envoy {
namespace Http {
class ExampleDecoderEncoderFilterTest : public testing::Test {
  void initializeFilter(const std::string& yaml) {
    io::example::http::filter::DecoderEncoder config;
    MessageUtil::loadFromYaml(yaml, config);
  }
};

TEST_F(ExampleDecoderEncoderFilterTest, Request) {
}

} // namespace Http
} // namespace Envoy