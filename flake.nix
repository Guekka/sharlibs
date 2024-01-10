{
  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs/nixos-unstable";
    systems.url = "github:nix-systems/default";
  };

  outputs = {
    self,
    nixpkgs,
    devenv,
    systems,
    ...
  } @ inputs: let
    forEachSystem = nixpkgs.lib.genAttrs (import systems);
  in {
    devShells =
      forEachSystem
      (system: let
        pkgs = nixpkgs.legacyPackages.${system};
        misk = pkgs.callPackage ./nix/misk.nix {};
        trieregex = pkgs.callPackage ./nix/trieregex.nix {};
        poxy = pkgs.callPackage ./nix/poxy.nix {inherit misk trieregex;};
      in {
        default = pkgs.mkShell {
          packages = [
            poxy
            pkgs.cmake
            pkgs.ninja
            pkgs.pkg-config
            pkgs.gcc
            pkgs.vcpkg
            pkgs.libgccjit
          ];

          shellHook = ''
            export VCPKG_ROOT=$(realpath $(dirname $(readlink -f $(type -p vcpkg)))/../share/vcpkg)
          '';
        };
      });
  };
}
