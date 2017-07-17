# Envoy filter example

This project demonstrates the linking of additional http filters with the Envoy binary.
A new filter `sample` which adds a http header is introduced.
Integration tests demonstrating the filter's end-to-end behavior are
also provided.

## Building

To build the Envoy static binary:

1. `git submodule update --init`
2. `bazel build //http-filter-example:envoy`

## Testing

To run the `sample` integration test:

`bazel test //http-filter-example:http_filter_integration_test`

## How it works

See the [network filter example](README.md#how-it-works).
