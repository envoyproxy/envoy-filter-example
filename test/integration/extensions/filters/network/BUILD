licenses(["notice"])  # Apache 2

load(
    "@envoy//bazel:envoy_build_system.bzl",
    "envoy_cc_test",
)

envoy_cc_test(
    name = "filter_integration_test",
    srcs = ["example_reader_test.cc"],
    data = [
        "//test/fixtures/configs:v2",
    ],
    repository = "@envoy",
    deps = [
        "//source/extensions/filters/network/example/reader:filter_config",
        "@envoy//test/integration:http_integration_lib",
    ],
)
