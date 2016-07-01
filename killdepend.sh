#!/bin/bash

find {modules,data}/*/src -name 'modinfo.[ch]pp' -exec rm -v {} \;
find tools/*/src -name depend.cpp -exec rm -v {} \;
