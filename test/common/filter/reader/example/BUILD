licenses(["notice"])  # Apache 2

load(
    "@envoy//bazel:envoy_build_system.bzl",
    "envoy_cc_test",
)

envoy_cc_test(
    name = "filter_integration_test",
    srcs = ["filter_integration_test.cc"],
    data = [
        "//configs:v2",
    ],
    repository = "@envoy",
    deps = [
        "//source/common/filter/reader/example:filter_config",
        "@envoy//test/integration:http_integration_lib",
    ],
)
