package(default_visibility = ["//visibility:public"])

load(
    "@envoy//bazel:envoy_build_system.bzl",
    "envoy_cc_binary",
    "envoy_cc_library",
    "envoy_cc_test",
)

envoy_cc_binary(
    name = "envoy",
    repository = "@envoy",
    deps = [
        "@envoy//source/exe:envoy_main_entry_lib",
        ":http_filter_lib",
    ],
)

envoy_cc_library(
    name = "http_filter_lib",
    srcs = ["http_filter.cc"],
    repository = "@envoy",
    deps = [
        "@envoy//source/exe:envoy_common_lib",
    ],
)

envoy_cc_test(
    name = "http_filter_integration_test",
    srcs = ["http_filter_integration_test.cc"],
    data =  ["//http-filter-example:envoy.conf"],
    repository = "@envoy",
    deps = [
        ":http_filter_lib",
        "@envoy//test/integration:integration_lib",
    ],
)
