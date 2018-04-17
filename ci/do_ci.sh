#!/bin/bash -e

if [[ -f "${HOME:-/root}/.gitconfig" ]]; then
    mv "${HOME:-/root}/.gitconfig" "${HOME:-/root}/.gitconfig_save"
fi

function do_build () {
    bazel build -j 2 --verbose_failures=true //:envoy
}

function do_test() {
    bazel test -j 2 --test_output=all //:echo2_integration_test
}

case "$1" in
  build)
    do_build
  ;;
  test)
    do_test
  ;;
  *)
    echo "must be one of [build,test]"
    exit 1
  ;;
esac
