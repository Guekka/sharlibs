{
  lib,
  python3,
  fetchFromGitHub,
}:
python3.pkgs.buildPythonApplication rec {
  pname = "misk";
  version = "0.8.1";
  pyproject = true;

  src = fetchFromGitHub {
    owner = "marzer";
    repo = "misk";
    rev = "v${version}";
    hash = "sha256-gEnZgjcA8hG4GIlRkb4dBg6BpTawgKH6LOzL83Sv5C0=";
  };

  nativeBuildInputs = [
    python3.pkgs.setuptools
    python3.pkgs.wheel
  ];

  propagatedBuildInputs = with python3.pkgs; [
    requests
  ];

  pythonImportsCheck = ["misk"];

  meta = with lib; {
    description = "Miscellaneous useful bits for Python 3";
    homepage = "https://github.com/marzer/misk";
    changelog = "https://github.com/marzer/misk/blob/${src.rev}/CHANGELOG.md";
    license = licenses.mit;
    maintainers = with maintainers; [];
    mainProgram = "misk";
  };
}
