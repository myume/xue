{
  mkShell,
  cmake,
  clang-tools,
  meson,
  ninja,
  # clangStdenv
}:
mkShell.override {
  # stdenv = clangStdenv;
} {
  packages = [
    clang-tools
    cmake
    meson
    ninja
  ];
}
