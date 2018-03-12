licenses(["notice"])  # Apache 2

load(
    "@envoy//bazel:envoy_build_system.bzl",
    "envoy_cc_test",
)

envoy_cc_test(
    name = "filter_integration_test",
    srcs = [
        "example_decoder_encoder_test.cc",
        "example_decoder_test.cc",
        "example_encoder_test.cc",
    ],
    repository = "@envoy",
    deps = [
        "//source/extensions/filters/http/example/decoder:filter_config",
        "//source/extensions/filters/http/example/decoder_encoder:filter_config",
        "//source/extensions/filters/http/example/encoder:filter_config",
        "@envoy//test/integration:http_integration_lib",
    ],
)
