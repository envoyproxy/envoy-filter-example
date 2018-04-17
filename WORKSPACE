workspace(name = "envoy_filter_example")

local_repository(
    name = "envoy",
    path = "envoy",
)

load("@envoy//bazel:repositories.bzl", "envoy_dependencies")
load("@envoy//bazel:cc_configure.bzl", "cc_configure")

envoy_dependencies()

load("@envoy_api//bazel:repositories.bzl", "api_dependencies")
api_dependencies()

ISTIO_MIXER_SHA="d223f496ca5f15214869f5215cbf114c00012899"
http_archive(
    name = "mixer",
    strip_prefix = "proxy-"+ISTIO_MIXER_SHA,
    url = "https://github.com/istio/proxy/archive/"+ISTIO_MIXER_SHA+".zip",
)

load("@mixer//:repositories.bzl", "mixerapi_dependencies")
mixerapi_dependencies()
load("@mixer//:repositories.bzl", "boringssl_repositories")
boringssl_repositories(bind=True)

load("@io_bazel_rules_go//go:def.bzl", "go_rules_dependencies", "go_register_toolchains")
load("@com_lyft_protoc_gen_validate//bazel:go_proto_library.bzl", "go_proto_repositories")
go_proto_repositories(shared=0)
go_rules_dependencies()
go_register_toolchains()
load("@io_bazel_rules_go//proto:def.bzl", "proto_register_toolchains")
proto_register_toolchains()
