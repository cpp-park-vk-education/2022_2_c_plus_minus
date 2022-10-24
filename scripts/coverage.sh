#!/usr/bin/env bash
set -e
TESTS=${1}
DIR=$(dirname ${TESTS})

lcov -t ${TESTS} -o ${DIR}/coverage_report.info -c -d ${DIR}
	genhtml -o ${DIR}/coverage_report ${DIR}/coverage_report.info
