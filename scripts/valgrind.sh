#!/usr/bin/env bash

TEST_FILES=${@}
MAIN_TEST=${1}
VG_LOG=$(dirname ${1})".valgrind.log"
for ftest in ${TEST_FILES}; do chmod a+rwx ${ftest};done

NO_LOST_PATTERN="All heap blocks were freed -- no leaks are possible"
NO_ERROR_PATTERN="ERROR SUMMARY: 0 errors"

rm -f VG_LOG

valgrind --tool=memcheck --leak-check=summary --leak-check=full --track-origins=yes --log-file=${VG_LOG} ${TEST_FILES} || exit 1
NO_LOST=$(grep "${NO_LOST_PATTERN}" "${VG_LOG}")
NO_ERROR=$(grep "${NO_ERROR_PATTERN}" "${VG_LOG}")
if [ -z "${NO_LOST}" ] || [ -z "${NO_ERROR}" ]; then cat "${VG_LOG}"; exit 1; fi
echo " ***** SUCCESS MEMTEST *****"