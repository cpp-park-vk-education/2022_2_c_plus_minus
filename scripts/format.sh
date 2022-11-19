#!/usr/bin/env bash

#!/bin/bash

DIR=${1}
NAMES="$(find ${DIR} | grep -E ".*(\.cpp|\.h|\.hpp)$")"
clang-format --verbose -i --style=file $NAMES