.PHONY: build test

build:
	bazel build //:common

test:
	bazel test //test:*
