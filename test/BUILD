cc_test(
    name = "status-test",
    srcs = ["status-test.cc"],
    copts = ["-Iexternal/gtest/include", "-std=c++1z"],
    deps = [
        "@com_google_googletest//:gtest_main",
        "//common:status",
    ],
)

cc_test(
    name = "context-test",
    srcs = ["context-test.cc"],
    copts = ["-Iexternal/gtest/include", "-std=c++1z"],
    deps = [
        "@com_google_googletest//:gtest_main",
        "//common:context",
    ],
)
