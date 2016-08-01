#!/bin/bash

set -e
cd "$(dirname -- "$0")"
rm -rf doc/api/html
mkdir -p doc/api/html
jusdapidoc -lang C++ -verbose doc/api/src doc/api/html
