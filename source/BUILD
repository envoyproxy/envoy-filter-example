package(default_visibility = ["//visibility:public"])

load(
    "@envoy//bazel:envoy_build_system.bzl",
    "envoy_cc_binary",
)
load("@envoy_api//bazel:api_build_system.bzl", "api_proto_library")

envoy_cc_binary(
    name = "envoy",
    repository = "@envoy",
    deps = [
        "//source/extensions/filters/http/example/decoder:filter_config",
        "//source/extensions/filters/http/example/decoder_encoder:filter_config",
        "//source/extensions/filters/http/example/encoder:filter_config",
        "//source/extensions/filters/network/example/reader:filter_config",
        "@envoy//source/exe:envoy_main_entry_lib",
    ],
)
