#include "common/grpc/codec.h"
#include "common/grpc/common.h"
#include "api/filter/network/http_connection_manager.pb.h"

#include "test/integration/http_integration.h"
#include "test/integration/utility.h"

#include "gtest/gtest.h"

namespace Envoy {
class HttpFilterSamplev2IntegrationTest : public HttpIntegrationTest,
                                        public testing::TestWithParam<Network::Address::IpVersion> {
public:
  HttpFilterSamplev2IntegrationTest() : HttpIntegrationTest(Http::CodecClient::Type::HTTP1, GetParam()) {}
  /**
   * Initializer for an individual integration test.
   */
 void SetUp() override {
    HttpIntegrationTest::SetUp();
    initialize();
  }

  void createUpstreams() override {
    HttpIntegrationTest::createUpstreams();
    fake_upstreams_.emplace_back(new FakeUpstream(0, FakeHttpConnection::Type::HTTP2, version_));
    ports_.push_back(fake_upstreams_.back()->localAddress()->ip()->port());
  }

  void initialize() override {
    config_helper_.addFilter(
        "{ name: sample, config: {} }");
    config_helper_.addConfigModifier([](envoy::api::v2::Bootstrap& bootstrap) {
      auto* sample_cluster = bootstrap.mutable_static_resources()->add_clusters();
      sample_cluster->MergeFrom(bootstrap.static_resources().clusters()[0]);
      sample_cluster->set_name("service1");
      sample_cluster->mutable_http2_protocol_options();
    });
    config_helper_.addConfigModifier(
        [](envoy::api::v2::filter::network::HttpConnectionManager &hcm) {
          hcm.mutable_route_config()
              ->mutable_virtual_hosts(0)
              ->mutable_routes(0)
              ->mutable_route();

        });
    named_ports_ = {"http"};
    HttpIntegrationTest::initialize();
  }


  void cleanup() {
    if (fake_upstream_connection_ != nullptr) {
      fake_upstream_connection_->close();
      fake_upstream_connection_->waitForDisconnect();
    }
  }
};

INSTANTIATE_TEST_CASE_P(IpVersions, HttpFilterSamplev2IntegrationTest,
                        testing::ValuesIn(TestEnvironment::getIpVersionsForTest()));

TEST_P(HttpFilterSamplev2IntegrationTest, Test1) {
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

  EXPECT_STREQ("sample-filter",
               request_stream->headers().get(Http::LowerCaseString("via"))->value().c_str());

  codec_client->close();
}
} // Envoy
