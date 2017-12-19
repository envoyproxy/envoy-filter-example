#include "test/integration/integration.h"
#include "test/integration/utility.h"

namespace Envoy {
class Echo2IntegrationTest : public BaseIntegrationTest,
                             public testing::TestWithParam<Network::Address::IpVersion> {

// TODO(alyssawilk): make this a general util.
std::string readFileToStringForTest(const std::string& filename) {
  std::ifstream file(filename);
  if (file.fail()) {
    std::cerr << "failed to open: " << filename << std::endl;
    RELEASE_ASSERT(false);
  }

  std::stringstream file_string_stream;
  file_string_stream << file.rdbuf();
  return file_string_stream.str();
}

std::string echoConfig() {
  return readFileToStringForTest(TestEnvironment::runfilesPath("echo2_server.yaml");
}

public:
  Echo2IntegrationTest() : BaseIntegrationTest(GetParam(), echoConfig()) {}

  // Called once by the gtest framework before any EchoIntegrationTests are run.
  static void SetUpTestCase() {
  }

  /**
   * Initializer for an individual integration test.
   */
  void SetUp() override {
    named_ports_ = {{"echo"}};
    BaseIntegrationTest::initialize();
  }

  /**
   * Destructor for an individual integration test.
   */
  void TearDown() override {
    test_server_.reset();
    fake_upstreams_.clear();
  }
};

INSTANTIATE_TEST_CASE_P(IpVersions, Echo2IntegrationTest,
                        testing::ValuesIn(TestEnvironment::getIpVersionsForTest()));

TEST_P(Echo2IntegrationTest, Echo) {
  Buffer::OwnedImpl buffer("hello");
  std::string response;
  RawConnectionDriver connection(lookupPort("echo"), buffer,
                                 [&](Network::ClientConnection&, const Buffer::Instance& data)
                                     -> void {
                                       response.append(TestUtility::bufferToString(data));
                                       connection.close();
                                     }, GetParam());

  connection.run();
  EXPECT_EQ("hello", response);
}
} // Envoy
