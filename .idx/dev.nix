{ pkgs, ... }: {
  channel = "stable-24.05";
  packages = [
    pkgs.zig
    pkgs.rustup
    pkgs.busybox
    pkgs.gcc
    pkgs.neovim
    pkgs.liburing
  ];
  env = { };
  idx = {
    extensions = [
      "pkief.material-icon-theme"
      "ziglang.vscode-zig"
      "rust-lang.rust"
      "tamasfe.even-better-toml"
    ];
    workspace = {
      onCreate = {
        install = "rustup default stable && rustup update && cargo run";
        default.openFiles = [
          "README.md"
        ];
      };
    };
  };
}