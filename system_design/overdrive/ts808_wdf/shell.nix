{ pkgs ? import <nixpkgs> { } }:
pkgs.mkShell {
  buildInputs = [
    pkgs.gnumake
    pkgs.gcc
    pkgs.ffmpeg
    pkgs.python3Packages.numpy
    pkgs.python3Packages.scipy
    pkgs.python3Packages.matplotlib
    pkgs.sox
  ];
}
