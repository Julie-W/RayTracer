load("@rules_cc//cc:defs.bzl", "cc_binary", "cc_library")

cc_binary(
    name = "main",
    srcs = glob(["src/**"]),
    copts = ["/std:c++17", "/W4"],
    linkopts = ["user32.lib","gdi32.lib", "/SUBSYSTEM:WINDOWS", "/SUBSYSTEM:CONSOLE"],
    deps = [
        ":vecmath"
    ]
)

cc_library(
    name = "vecmath",
    hdrs = glob(["include/vecmath/*.h"]),
    srcs = glob(["include/vecmath/*.cpp"]),
    includes = ["include/vecmath"]
)