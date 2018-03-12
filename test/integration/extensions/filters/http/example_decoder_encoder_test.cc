#include "test/integration/http_integration.h"
#include "test/integration/utility.h"

namespace Envoy {
class ExampleDecoderEncoderFilterIntegrationTest
    : public HttpIntegrationTest,
      public testing::TestWithParam<Network::Address::IpVersion> {
public:
  ExampleDecoderEncoderFilterIntegrationTest()
      : HttpIntegrationTest(Http::CodecClient::Type::HTTP1, GetParam()) {}

  void SetUp() override { initialize(); }

  void initialize() override {
    config_helper_.addFilter(
        "{ name: io.example.http.filter.decoder_encoder, config: { key: via, val: example-decoder-encoder } }");
    HttpIntegrationTest::initialize();
  }
};

INSTANTIATE_TEST_CASE_P(IpVersions, ExampleDecoderEncoderFilterIntegrationTest,
                        testing::ValuesIn(TestEnvironment::getIpVersionsForTest()));

TEST_P(ExampleDecoderEncoderFilterIntegrationTest, Test1) {
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

  EXPECT_STREQ("example-decoder-encoder",
               request_stream->headers().get(Http::LowerCaseString("via"))->value().c_str());

  codec_client->close();
}

TEST_P(ExampleDecoderEncoderFilterIntegrationTest, Test2) {
  codec_client_ = makeHttpConnection(makeClientConnection((lookupPort("http"))));

  Http::TestHeaderMapImpl request_headers{
      {":method", "GET"}, {":path", "/"}, {":authority", "host"}};

  Http::TestHeaderMapImpl response_headers{{":status", "200"},
                                           {"content-type", "application/json"}};

  sendRequestAndWaitForResponse(request_headers, 0, response_headers, 0);

  EXPECT_TRUE(upstream_request_->complete());
  EXPECT_EQ(0U, upstream_request_->bodyLength());
  EXPECT_TRUE(response_->complete());
  EXPECT_STREQ("200", response_->headers().Status()->value().c_str());
  EXPECT_EQ(0U, response_->body().size());
  EXPECT_NE(response_->headers().get(Http::LowerCaseString("via")), nullptr);
  EXPECT_STREQ("example-decoder-encoder",
               response_->headers().get(Http::LowerCaseString("via"))->value().c_str());
}
} // namespace Envoy