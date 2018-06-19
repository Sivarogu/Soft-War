#!/bin/bash
set -euxo pipefail
root_dir=$(pwd)

cd ${root_dir}/server
make

cd ${root_dir}/lib-client
npm i

cd ${root_dir}/visualizer
npm i
