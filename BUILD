load("@rules_cc//cc:defs.bzl", "cc_binary", "cc_import", "cc_library")

cc_binary(
    name = "main",
    srcs = glob(["src/**"]),
    deps = [
        ":freeglut",
        ":glew",
        ":FreeImage",
        ":glm"
    ]
)

cc_library(
    name = "freeglut",
    hdrs = glob(["include/GL/*.h"]),
    deps = [":freeglut_bin"],
    strip_include_prefix = "include"
)

cc_import(
    name = "freeglut_bin",
    interface_library = "lib/freeglut.lib",
    shared_library = "freeglut.dll"
)

cc_library(
    name = "glew",
    hdrs = glob(["include/GL/*.h"]),
    deps = [":glew_bin"],
    strip_include_prefix = "include"
)

cc_import(
    name = "glew_bin",
    interface_library = "lib/glew.lib",
    shared_library = "glew.dll"
)

cc_library(
    name = "FreeImage",
    hdrs = glob(["include/*.h"]),
    deps = [":FreeImage_bin"],
    strip_include_prefix = "include"
)

cc_import(
    name = "FreeImage_bin",
    interface_library = "lib/FreeImage.lib",
    shared_library = "FreeImage.dll"
)

cc_library(
  name= "glm",
  srcs=glob(["glm/**/*.cpp"]) + ["glm/detail/_fixes.hpp"],
  hdrs=glob(["glm/**/*.hpp"])+glob(["glm/**/*.h"]),
  textual_hdrs = glob(["glm/**/*.inl"]),
)