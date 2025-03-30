#!/bin/bash -e

export NAME=http-filter-example

_curl () {
    local arg curl_command
    curl_command=(curl -s)
    if [[ ! "$*" =~ "-X" ]]; then
        curl_command+=(-X GET)
    fi
    for arg in "${@}"; do
        curl_command+=("$arg")
    done
    "${curl_command[@]}" || {
        echo "ERROR: curl (${curl_command[*]})" >&2
        return 1
    }
}

responds_with () {
    local expected
    expected="$1"
    shift
    _curl "${@}" | grep "$expected" || {
        echo "ERROR: curl expected (${*}): $expected" >&2
        return 1
    }
}

responds_with \
    "via: sample-filter" \
    "http://localhost:8080"
