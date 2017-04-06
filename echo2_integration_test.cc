#include "test/integration/integration.h"
#include "test/integration/utility.h"

class Echo2IntegrationTest : public BaseIntegrationTest, public testing::Test {
public:
  /**
   * Global initializer for all integration tests.
   */
  static void SetUpTestCase() {
    createTestServer("echo2_server.json", {"echo"});
  }

  /**
   * Global destructor for all integration tests.
   */
  static void TearDownTestCase() {
    test_server_.reset();
  }
};

TEST_F(Echo2IntegrationTest, Echo) {
  Buffer::OwnedImpl buffer("hello");
  std::string response;
  RawConnectionDriver connection(lookupPort("echo"), buffer,
                                 [&](Network::ClientConnection&, const Buffer::Instance& data)
                                     -> void {
                                       response.append(TestUtility::bufferToString(data));
                                       connection.close();
                                     });

  connection.run();
  EXPECT_EQ("hello", response);
}
