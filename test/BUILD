load("@rules_cc//cc:defs.bzl", "cc_test")

cc_test(
    name = "status-test",
    srcs = ["status-test.cc"],
    copts = [
        "-Iexternal/gtest/include",
        "-std=c++1z",
    ],
    deps = [
        "//:common",
        "@com_google_googletest//:gtest_main",
    ],
)

cc_test(
    name = "context-test",
    srcs = ["context-test.cc"],
    copts = [
        "-Iexternal/gtest/include",
        "-std=c++1z",
    ],
    deps = [
        "//:common",
        "@com_google_googletest//:gtest_main",
    ],
)
