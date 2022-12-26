#!/usr/bin/env bash

set -o pipefail

function print_header() {
    echo -e "\n***** ${1} *****"
}

function check_log() {
    LOG=$( { ${1}; } 2>&1 )
    STATUS=$?
    echo "$LOG"
    if echo "$LOG" | grep -q -E "${2}"
    then
        exit 1
    fi

    if [ $STATUS -ne 0 ]
    then
        exit $STATUS
    fi
}

#print_header "RUN cppcheck"
#check_log "cppcheck ./project --enable=all --inconclusive --error-exitcode=1 -Iproject/server/include -Iproject/client/include --suppress=missingIncludeSystem" "\(information\)"

#print_header "RUN clang-tidy"
#check_log "clang-tidy project/server/*.cpp project/client/*.cpp -warnings-as-errors=* -extra-arg=-std=c++17 -- -I project/client/include -Iproject/server/include" "Error (?:reading|while processing)"

#print_header "RUN cpplint"
#check_log "cpplint ./project/server/src/*.cpp ./project/server/*.cpp  ./project/server/include/*.h
#        ./project/client/src/*.cpp ./project/client/*.cpp  ./project/client/include/*.h ./tests/*.cpp" "Can't open for reading"

print_header "SUCCESS"
