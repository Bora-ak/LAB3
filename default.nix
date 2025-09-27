{ stdenv, cmake, protobuf, pkg-config }:

stdenv.mkDerivation {
  pname = "lab3";
  version = "1.0.0";
  src = ./.;

  nativeBuildInputs = [ cmake pkg-config ];
  buildInputs = [ protobuf ];

  cmakeFlags = [ "-DCMAKE_INSTALL_PREFIX=$out" ];
}
