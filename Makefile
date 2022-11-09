SERVER_DIR  = build/project/server
CLIENT_DIR  = build/project/client
TEST_DIR    = build/tests/tests
TEST_SERVER	= ${SERVER_DIR}/tests/server_tests
TEST_CLIENT = ${CLIENT_DIR}/tests/client_tests
.PHONY: all build rebuild check tests_all coverage clean linters valgrind run run_client run_server build_server
.PHONY:	build_client test test_server test_client test_all

all: clean format linters build test run

build: clean
	scripts/build.sh -DBUILD_ALL=true -DBUILD_TESTS=ON

rebuild: clean build

build_server: clean
	scripts/build.sh -DBUILD_SERVER=true

build_client: clean
	scripts/build.sh -DBUILD_CLIENT=true

run_server: build_server
	./${SERVER_DIR}/server

run_client: build_client
	./${CLIENT_DIR}/client

run: build
	./${SERVER_DIR}/server
	./${CLIENT_DIR}/client

test: build
	./${TEST_DIR}

test_server: build_server
	./${TEST_SERVER}

test_client: build_client
	./${TEST_CLIENT}

test_all:
	@make build
	@make test
	@make test_server
	@make test_client

coverage:
	scripts/coverage.sh ${TEST_DIR}
	scripts/coverage.sh ${TEST_SERVER}
	scripts/coverage.sh ${TEST_CLIENT}

format:
	scripts/format.sh project

valgrind:
	scripts/valgrind.sh ${TESTS_DIR} ${TEST_SERVER} ${TEST_CLIENT}

linters:
	scripts/linters.sh

clean:
	rm -rf build/
