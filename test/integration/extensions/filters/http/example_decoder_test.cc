#include "test/integration/http_integration.h"
#include "test/integration/utility.h"

namespace Envoy {
class ExampleDecoderFilterIntegrationTest
    : public HttpIntegrationTest,
      public testing::TestWithParam<Network::Address::IpVersion> {
public:
  ExampleDecoderFilterIntegrationTest()
      : HttpIntegrationTest(Http::CodecClient::Type::HTTP1, GetParam()) {}

  void SetUp() override { initialize(); }

  void initialize() override {
    config_helper_.addFilter(
        "{ name: io.example.http.filter.decoder, config: { key: via, val: decoder-example } }");
    HttpIntegrationTest::initialize();
  }
};

INSTANTIATE_TEST_CASE_P(IpVersions, ExampleDecoderFilterIntegrationTest,
                        testing::ValuesIn(TestEnvironment::getIpVersionsForTest()));

TEST_P(ExampleDecoderFilterIntegrationTest, Test1) {
  Http::TestHeaderMapImpl headers{{":method", "GET"}, {":path", "/"}, {":authority", "host"}};

  IntegrationCodecClientPtr codec_client;
  FakeHttpConnectionPtr fake_upstream_connection;
  IntegrationStreamDecoderPtr response(new IntegrationStreamDecoder(*dispatcher_));
  FakeStreamPtr request_stream;

  codec_client = makeHttpConnection(lookupPort("http"));
  codec_client->makeHeaderOnlyRequest(headers, *response);
  fake_upstream_connection = fake_upstreams_[0]->waitForHttpConnection(*dispatcher_);
  request_stream = fake_upstream_connection->waitForNewStream(*dispatcher_);
  request_stream->waitForEndStream(*dispatcher_);
  response->waitForEndStream();

  EXPECT_STREQ("decoder-example",
               request_stream->headers().get(Http::LowerCaseString("via"))->value().c_str());

  codec_client->close();
}
} // namespace Envoy