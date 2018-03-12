#include "test/integration/http_integration.h"
#include "test/integration/utility.h"

namespace Envoy {
class ExampleEncoderFilterIntegrationTest
    : public HttpIntegrationTest,
      public testing::TestWithParam<Network::Address::IpVersion> {
public:
  ExampleEncoderFilterIntegrationTest()
      : HttpIntegrationTest(Http::CodecClient::Type::HTTP1, GetParam()) {}

  void SetUp() override { initialize(); }
  void TearDown() override { cleanupUpstreamAndDownstream(); }

  void initialize() override {
    config_helper_.addFilter(
        "{ name: io.example.http.filter.encoder, config: { key: via, val: encoder-example } }");
    HttpIntegrationTest::initialize();
    codec_client_ = makeHttpConnection(makeClientConnection((lookupPort("http"))));
  }
};

INSTANTIATE_TEST_CASE_P(IpVersions, ExampleEncoderFilterIntegrationTest,
                        testing::ValuesIn(TestEnvironment::getIpVersionsForTest()));

TEST_P(ExampleEncoderFilterIntegrationTest, Test1) {
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
  EXPECT_STREQ("encoder-example",
               response_->headers().get(Http::LowerCaseString("via"))->value().c_str());
}
} // namespace Envoy