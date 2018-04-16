workspace(name = "envoy_filter_example")

# We need newer gRPC than Envoy has for ALTS, this could be removed once Envoy picks
# newer gRPC with ALTS support. (likely v1.11).
git_repository(
    name = "com_github_grpc_grpc",
    remote = "https://github.com/grpc/grpc.git",
    commit = "c50405364a194a0e4931153cbe329662d90530bc",  # Mar 28, 2018
)

local_repository(
    name = "envoy",
    path = "envoy",
)

load("@envoy//bazel:repositories.bzl", "envoy_dependencies")
load("@envoy//bazel:cc_configure.bzl", "cc_configure")

envoy_dependencies()

http_archive(
    name = "mixer",
    strip_prefix = "proxy-0.7.1",
    url = "https://github.com/istio/proxy/archive/0.7.1.zip",
)

load("@mixer//:repositories.bzl", "mixerapi_dependencies")
mixerapi_dependencies()

load("@envoy_api//bazel:repositories.bzl", "api_dependencies")
api_dependencies()

load("@io_bazel_rules_go//go:def.bzl", "go_rules_dependencies", "go_register_toolchains")
load("@com_lyft_protoc_gen_validate//bazel:go_proto_library.bzl", "go_proto_repositories")
go_proto_repositories(shared=0)
go_rules_dependencies()
go_register_toolchains()
load("@io_bazel_rules_go//proto:def.bzl", "proto_register_toolchains")
proto_register_toolchains()