# Envoy filter example

This project demonstrates the linking of additional filters with the Envoy binary.
A new filter `my.net.filter.reader.example` is introduced, identical modulo renaming to the existing
[`echo`](https://github.com/envoyproxy/envoy/blob/master/source/common/filter/echo.h)
filter. Integration tests demonstrating the filter's end-to-end behavior are
also provided.

For an example of additional HTTP filters, see [here](http-filter-example).

## Building

To build the Envoy static binary:

```
$ bazel build //source:envoy
```

## Testing

To run the integration tests:

```
$ bazel test --test_output=all //test/...
```

To run the regular Envoy tests from this project:

```
$ bazel test @envoy//test/...
```

## How it works

The [Envoy repository](https://github.com/envoyproxy/envoy/) will be downloaded as a .zip via bazel's `http_archive`.
The [`WORKSPACE`](WORKSPACE) file maps the `@envoy` repository to this local path.

The [`source/BUILD`](source/BUILD) file introduces a new Envoy static binary target, `envoy`,
that links together the new filters and `@envoy//source/exe:envoy_main_lib`. The
filter examples register themselves during the static initialization phase of the
Envoy binary as a new filter.
