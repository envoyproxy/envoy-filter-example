#!/bin/sh
python3 ./http-filter-example/httpserver.py&
./bazel-bin/envoy --config-path ./http-filter-example/config.yaml
