#!/usr/bin/env bash

THIS_PATH=$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)
THIS_NAME=$(basename "${BASH_SOURCE[0]%.*}")
INSTALL_PATH=${THIS_PATH}/${THIS_NAME}
mkdir -p ${INSTALL_PATH}

echo "Installing ${THIS_NAME} data ..."

pushd ${INSTALL_PATH}/ > /dev/null
curl --progress -LO --time-cond cacert.pem https://curl.haxx.se/ca/cacert.pem
echo "✅ ${INSTALL_PATH}/cacert.pem"
popd > /dev/null
