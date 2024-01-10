{
  lib,
  doxygen,
  python3,
  misk,
  trieregex,
  fetchFromGitHub,
}:
python3.pkgs.buildPythonApplication rec {
  pname = "poxy";
  version = "0.15.0";
  pyproject = true;

  src = fetchFromGitHub {
    owner = "marzer";
    repo = "poxy";
    rev = "v${version}";
    hash = "sha256-fj/qJvuY0d6tTHA3MXL0HQXot6uKAOHQMxOHYmY2ces=";
  };

  nativeBuildInputs = [
    python3.pkgs.setuptools
    python3.pkgs.wheel
  ];

  propagatedBuildInputs = with python3.pkgs; [
    beautifulsoup4
    html5lib
    jinja2
    lxml
    misk
    pygments
    requests
    schema
    tomli
    trieregex
  ];

  pythonImportsCheck = ["poxy"];

  postInstall = ''
    wrapProgram $out/bin/poxy --set PATH ${lib.makeBinPath [doxygen]}
  '';

  meta = with lib; {
    description = "Documentation generator for C";
    homepage = "https://github.com/marzer/poxy";
    changelog = "https://github.com/marzer/poxy/blob/${src.rev}/CHANGELOG.md";
    license = licenses.mit;
    maintainers = with maintainers; [];
    mainProgram = "poxy";
  };
}
