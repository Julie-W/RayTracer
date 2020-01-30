load("@rules_cc//cc:defs.bzl", "cc_binary")

cc_binary(
    name = "main",
    srcs = glob(["src/**"]),
    linkopts = ["user32.lib","gdi32.lib", "/SUBSYSTEM:WINDOWS"],
)