load("@rules_cc//cc:defs.bzl", "cc_library")

cc_library(
    name = "common",
    hdrs = glob([
        "src/**/*.h",
    ]),
    copts = ["-std=c++1z"],
    includes = ["src/"],
    visibility = ["//visibility:public"],
    deps = [
        "@com_github_bytespirit_idl//idl/common:status_cc_proto",
    ],
)
