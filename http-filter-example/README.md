# Envoy filter example

This project demonstrates the linking of additional HTTP filters with the Envoy binary.
A new filter `sample` which adds a HTTP header is introduced.
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

## How to write and use an HTTP filter

- The main task is to write a class that implements the interface
 [`Envoy::Http::StreamDecoderFilter`][StreamDecoderFilter] as in
 [`http_filter.h`](http_filter.h) and [`http_filter.cc`](http_filter.cc),
 which contains functions that handle http headers, data, and trailers.
 Note that this is an example of decoder filters, 
 and to write encoder filters or decoder/encoder filters
 you need to implement 
 [`Envoy::Http::StreamEncoderFilter`][StreamEncoderFilter] or
 [`Envoy::Http::StreamFilter`][StreamFilter] instead.
- You also need a class that implements 
 `Envoy::Server::Configuration::NamedHttpFilterConfigFactory`
 to enable the Envoy binary to find your filter,
 as in [`http_filter_config.cc`](http_filter_config.cc).
 It should be linked to the Envoy binary by modifying [`BUILD`][BUILD] file.
- Finally, you need to modify the Envoy config file to add your filter to the
 filter chain for a particular HTTP route configuration. For instance, if you
 wanted to change [the front-proxy example][front-envoy.yaml] to chain our
 `sample` filter, you'd need to modify its config to look like

```yaml
http_filters:
- name: sample          # before envoy.router because order matters!
  typed_config:
    "@type": type.googleapis.com/sample.Decoder
    key: Via
    val: sample-filter
- name: envoy.router
  typed_config: {}
```
or you can just use this demo yaml file [config.yaml](config.yaml).
- Run envoy with this cmd：
```sh
./bazel-bin/envoy --config-path ./http-filter-example/config.yaml -l trace
```
- Run an http server
```sh
python ./http-filter-example/httpserver.py
```
- Access the http server with curl cmd:
```sh
curl http://127.0.0.1:8080
```
You can find there is an kv string in the request header to the http server,
and the key value `Via` has become to `via`, you can see the request header 
in the http server stdout.
```sh
$ python httpserver.py 
Started httpserver on port  8081
127.0.0.1 - - [29/Sep/2020 11:30:03] "GET / HTTP/1.1" 200 -
host: 127.0.0.1:8080
user-agent: curl/7.58.0
accept: */*
x-forwarded-proto: http
x-request-id: b7a8d227-77a0-4985-a121-5e0f70bd16f2
via: sample-filter
x-envoy-expected-rq-timeout-ms: 15000
content-length: 0
```
- Start service and verify with shell script

1. `bash ./http-filter-example/start_service.sh`
2. `bash ./http-filter-example/verify.sh`

[StreamDecoderFilter]: https://github.com/envoyproxy/envoy/blob/b2610c84aeb1f75c804d67effcb40592d790e0f1/include/envoy/http/filter.h#L300
[StreamEncoderFilter]: https://github.com/envoyproxy/envoy/blob/b2610c84aeb1f75c804d67effcb40592d790e0f1/include/envoy/http/filter.h#L413
[StreamFilter]: https://github.com/envoyproxy/envoy/blob/b2610c84aeb1f75c804d67effcb40592d790e0f1/include/envoy/http/filter.h#L462
[BUILD]: https://github.com/envoyproxy/envoy-filter-example/blob/d76d3096c4cbd647d26b44b3f801c3afbc81d3e2/http-filter-example/BUILD#L15-L18
[front-envoy.yaml]: https://github.com/envoyproxy/envoy/blob/b2610c84aeb1f75c804d67effcb40592d790e0f1/examples/front-proxy/front-envoy.yaml#L28
