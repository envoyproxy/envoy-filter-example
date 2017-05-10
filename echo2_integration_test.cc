#include "test/integration/integration.h"
#include "test/integration/utility.h"

namespace Lyft {
class Echo2IntegrationTest : public BaseIntegrationTest,
                             public testing::TestWithParam<Network::Address::IpVersion> {
public:
  /**
   * Initializer for an individual integration test.
   */
  void SetUp() override {
    createTestServer("echo2_server.json", GetParam(), {"echo"});
  }

  /**
   * Destructor for an individual integration test.
   */
  void TearDown() override {
    test_server_.reset();
  }
};

INSTANTIATE_TEST_CASE_P(IpVersions, Echo2IntegrationTest,
                        testing::ValuesIn(TestEnvironment::getIpVersionsForTest()));

TEST_P(Echo2IntegrationTest, Echo) {
  Buffer::OwnedImpl buffer("hello");
  std::string response;
  RawConnectionDriver connection(lookupPort("echo"), GetParam(), buffer,
                                 [&](Network::ClientConnection&, const Buffer::Instance& data)
                                     -> void {
                                       response.append(TestUtility::bufferToString(data));
                                       connection.close();
                                     });

  connection.run();
  EXPECT_EQ("hello", response);
}
} // Lyft
