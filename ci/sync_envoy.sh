#!/bin/bash

set -e

echo "Updating Submodule..."
# Update submodule to latest Envoy SHA
ENVOY_SHA=$(git -C "$ENVOY_SRC_DIR" rev-parse HEAD)
CURRENT_SHA="$(git  ls-files -s envoy | cut -d' ' -f2)"
COMMITTER_NAME="envoy-update[bot]"
COMMITTER_EMAIL="envoy-update[bot]@users.noreply.github.com"


if [[ "$CURRENT_SHA" == "$ENVOY_SHA" ]]; then
    echo "Submodule already up to date (${ENVOY_SHA})"
    exit 0
fi

git submodule update --init
git -C envoy/ checkout "$ENVOY_SHA"

echo "Updating Workspace file."
sed -e "s|{ENVOY_SRC_DIR}|envoy|" "${ENVOY_SRC_DIR}/ci/WORKSPACE.filter.example" > "WORKSPACE"

echo "Committing, and Pushing..."
git config --global user.email "$COMMITTER_EMAIL"
git config --global user.name "$COMMITTER_NAME"
git commit -a -m "Update Envoy submodule to $ENVOY_SHA"
git push origin main
echo "Done"
