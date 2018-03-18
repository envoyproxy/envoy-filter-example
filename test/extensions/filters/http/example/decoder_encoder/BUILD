licenses(["notice"])  # Apache 2

load(
    "@envoy//bazel:envoy_build_system.bzl",
    "envoy_cc_test",
)

envoy_cc_test(
    name = "filter_test",
    srcs = ["filter_test.cc"],
    repository = "@envoy",
    deps = [
        "//source/extensions/filters/http/example/decoder_encoder:filter_lib",
        "@envoy//test/mocks/http:http_mocks",
    ],
)