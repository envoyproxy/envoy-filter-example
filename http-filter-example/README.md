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

See the [network filter example](../README.md#how-it-works).

## How to custom it

- The main task is to write a class 
 that implements the interface `Envoy::Http::StreamDecoderFilter`,
 as in [`http_filter.h`](http_filter.h) and [`http_filter.cc`](http_filter.cc),
 which contains functions that handle http headers, data, and trailers.
- You also need a class that implements 
 `Envoy::Server::Configuration::NamedHttpFilterConfigFactory`
 to enable the Envoy binary to find your filter,
 as in [`http_filter_config.cc`](http_filter_config.cc).
- Finally, you need to modify the [Envoy config file](envoy.conf#L33-L37)
 to add your filter to Envoy.
 

