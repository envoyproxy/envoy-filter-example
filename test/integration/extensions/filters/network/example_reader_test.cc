#include "test/integration/integration.h"
#include "test/integration/utility.h"

namespace Envoy {
class ExampleReaderIntegrationTest : public BaseIntegrationTest,
                                     public testing::TestWithParam<Network::Address::IpVersion> {

  std::string echoConfig() {
    return TestEnvironment::readFileToStringForTest(
        TestEnvironment::runfilesPath("test/fixtures/configs/v2/echo.yaml"));
  }

public:
  ExampleReaderIntegrationTest() : BaseIntegrationTest(GetParam(), echoConfig()) {}
  /**
   * Initializer for an individual integration test.
   */
  void SetUp() override { BaseIntegrationTest::initialize(); }

  /**
   * Destructor for an individual integration test.
   */
  void TearDown() override {
    test_server_.reset();
    fake_upstreams_.clear();
  }
};

INSTANTIATE_TEST_CASE_P(IpVersions, ExampleReaderIntegrationTest,
                        testing::ValuesIn(TestEnvironment::getIpVersionsForTest()));

TEST_P(ExampleReaderIntegrationTest, Echo) {
  Buffer::OwnedImpl buffer("hello");
  std::string response;
  RawConnectionDriver connection(
      lookupPort("listener_0"), buffer,
      [&](Network::ClientConnection&, const Buffer::Instance& data) -> void {
        response.append(TestUtility::bufferToString(data));
        connection.close();
      },
      GetParam());

  connection.run();
  EXPECT_EQ("hello", response);
}
} // namespace Envoy
