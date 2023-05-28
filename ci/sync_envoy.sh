#!/bin/bash

set -e

echo "Updating Submodule..."
# Update submodule to latest Envoy SHA
ENVOY_SHA=$(git -C "$ENVOY_SRC_DIR" rev-parse HEAD)
CURRENT_SHA="$(git  ls-files -s envoy | cut -d' ' -f2)"

if [[ "$CURRENT_SHA" == "$ENVOY_SHA" ]]; then
    echo "Submodule already up to date (${ENVOY_SHA})"
    exit 0
fi

git submodule update --init
git -C envoy/ checkout "$ENVOY_SHA"

echo "Updating Workspace file."
sed -e "s|{ENVOY_SRCDIR}|envoy|" "${ENVOY_SRCDIR}/ci/WORKSPACE.filter.example" > "WORKSPACE"

echo "Committing, and Pushing..."
git commit -a -m "Update Envoy submodule to $ENVOY_SHA"
git push origin main
echo "Done"
