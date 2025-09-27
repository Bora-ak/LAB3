{
  description = "LAB3: UDP + Protobuf (server/client)";

  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs/nixos-23.05";
    utils.url   = "github:numtide/flake-utils";
  };

  outputs = { self, nixpkgs, utils }:
    utils.lib.eachDefaultSystem (system:
      let
        pkgs = import nixpkgs { inherit system; };
        lab3Pkg = pkgs.callPackage ./default.nix { };
      in {
        packages.default = lab3Pkg;

        # Legacy package set with overlay exposing `lab3` (for cross compile cmd below)
        legacyPackages = import nixpkgs {
          inherit system;
          overlays = [
            (final: _: { lab3 = final.callPackage ./default.nix { }; })
          ];
        };
      });
}
