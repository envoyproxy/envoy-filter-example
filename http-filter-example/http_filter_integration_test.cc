#include "test/integration/http_integration.h"

namespace Envoy {
class HttpFilterSampleIntegrationTest : public HttpIntegrationTest,
                                        public testing::TestWithParam<Network::Address::IpVersion> {
public:
  HttpFilterSampleIntegrationTest()
      : HttpIntegrationTest(Http::CodecClient::Type::HTTP1, GetParam()) {}
  /**
   * Initializer for an individual integration test.
   */
  void SetUp() override { initialize(); }

  void initialize() override {
    config_helper_.prependFilter(
        "{ name: sample, typed_config: { \"@type\": type.googleapis.com/sample.Decoder, key: via, "
        "val: sample-filter } }");
    HttpIntegrationTest::initialize();
  }
};

INSTANTIATE_TEST_SUITE_P(IpVersions, HttpFilterSampleIntegrationTest,
                         testing::ValuesIn(TestEnvironment::getIpVersionsForTest()));

TEST_P(HttpFilterSampleIntegrationTest, Test1) {
  Http::TestRequestHeaderMapImpl headers{
      {":method", "GET"}, {":path", "/"}, {":authority", "host"}};
  Http::TestRequestHeaderMapImpl response_headers{
      {":status", "200"}};

  IntegrationCodecClientPtr codec_client;
  FakeHttpConnectionPtr fake_upstream_connection;
  FakeStreamPtr request_stream;

  codec_client = makeHttpConnection(lookupPort("http"));
  auto response = codec_client->makeHeaderOnlyRequest(headers);
  ASSERT_TRUE(fake_upstreams_[0]->waitForHttpConnection(*dispatcher_, fake_upstream_connection));
  ASSERT_TRUE(fake_upstream_connection->waitForNewStream(*dispatcher_, request_stream));
  ASSERT_TRUE(request_stream->waitForEndStream(*dispatcher_));
  request_stream->encodeHeaders(response_headers, true);
  ASSERT_TRUE(response->waitForEndStream());

  EXPECT_EQ(
      "sample-filter",
      request_stream->headers().get(Http::LowerCaseString("via"))[0]->value().getStringView());

  codec_client->close();
}
} // namespace Envoy
