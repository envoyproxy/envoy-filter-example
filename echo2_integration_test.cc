#include <type_traits>

#include "test/integration/integration.h"
#include "test/integration/utility.h"

namespace Envoy {
namespace {

// NoCreateConnectionDriver is used to provide "value" member regardless `createConnectionDriver`
// is a member function or else. The latter includes the case that there is no such member
// named `createConnectionDriver`.
template <typename, typename = std::true_type> struct NoCreateConnectionDriver : std::true_type {};

template <typename T>
struct NoCreateConnectionDriver<
    T, std::integral_constant<
           bool, std::is_member_function_pointer<decltype(&T::createConnectionDriver)>::value>>
    : std::false_type {};

// Extactly one "construct()" is enabled.
template <typename T, typename = std::enable_if_t<NoCreateConnectionDriver<T>::value>>
static std::unique_ptr<RawConnectionDriver>
construct(BaseIntegrationTest& base_test, uint32_t port, const std::string& initial_data,
          RawConnectionDriver::ReadCallback data_callback, Network::Address::IpVersion version) {
  UNREFERENCED_PARAMETER(base_test);
  Buffer::OwnedImpl buffer(initial_data);
  return std::make_unique<RawConnectionDriver>(port, buffer, data_callback, version);
}

template <typename T, typename = std::enable_if_t<!NoCreateConnectionDriver<T>::value>>
static std::unique_ptr<RawConnectionDriver>
construct(T& base_test, uint32_t port, const std::string& initial_data,
          RawConnectionDriver::ReadCallback data_callback, Network::Address::IpVersion version) {
  UNREFERENCED_PARAMETER(version);
  return base_test.createConnectionDriver(port, initial_data, std::move(data_callback));
}

} // namespace

class Echo2IntegrationTest : public BaseIntegrationTest,
                             public testing::TestWithParam<Network::Address::IpVersion> {

  std::string echoConfig() {
    return TestEnvironment::readFileToStringForTest(
        TestEnvironment::runfilesPath("echo2_server.yaml", "envoy_filter_example"));
  }

public:
  Echo2IntegrationTest() : BaseIntegrationTest(GetParam(), echoConfig()) {}
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

INSTANTIATE_TEST_SUITE_P(IpVersions, Echo2IntegrationTest,
                         testing::ValuesIn(TestEnvironment::getIpVersionsForTest()));

TEST_P(Echo2IntegrationTest, Echo) {
  std::string response;
  // auto connection = ConstructHelper<BaseIntegrationTest>::construct(
  auto connection = construct<BaseIntegrationTest>(
      *this, lookupPort("listener_0"), "hello",
      [&](Network::ClientConnection& conn, const Buffer::Instance& data) -> void {
        response.append(data.toString());
        conn.close(Network::ConnectionCloseType::FlushWrite);
      },
      version_);

  connection->run();
  EXPECT_EQ("hello", response);
}
} // namespace Envoy
