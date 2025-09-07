{
  description = "{{NAME}}";

  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs/nixos-25.05";
    flake-utils.url = "github:numtide/flake-utils";
  };

  outputs = {
    flake-utils,
    nixpkgs,
    ...
  }:
    flake-utils.lib.eachDefaultSystem (
      system: let
        pkgs = import nixpkgs {
          inherit system;
        };
      in {
        stdenv = pkgs.clangStdenv;
        devShells.default = with pkgs;
          mkShell {
            nativeBuildInputs = [
              clang_21
              ncurses
              cmake
              ninja
            ];

            shellHook = ''
              export CC=clang
              export CXX=clang++
            '';
          };
      }
    );
}
