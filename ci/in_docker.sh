#!/bin/bash -eu

set -o pipefail

apt-get -qq update -y
apt-get -qq install --no-install-recommends gosu

USER_UID="$HOST_UID"
USER_GID="${HOST_GID:-${HOST_UID}}"

DOCKER_GROUP_ARGS+=(--gid "${USER_GID}")
DOCKER_USER_ARGS+=(--gid "${USER_GID}")

groupadd ${DOCKER_GROUP_ARGS[*]} -f envoygroup
useradd -o --uid ${USER_UID} ${DOCKER_USER_ARGS[*]} --no-create-home --home-dir /build envoybuild
usermod -a -G pcap envoybuild
chown envoybuild:envoygroup /build
chown envoybuild /proc/self/fd/2
sudo -EHs -u envoybuild bash -c 'cd /source $*'

cd /source

gosu envoybuild "$@"
