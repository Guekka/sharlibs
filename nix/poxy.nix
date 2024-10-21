{ lib
, doxygen
, python3
, misk
, trieregex
, fetchFromGitHub
}:

python3.pkgs.buildPythonApplication rec {
  pname = "poxy";
  version = "0.19.0";
  pyproject = true;

  build-system = [
    python3.pkgs.setuptools
    python3.pkgs.wheel
    python3.pkgs.distutils
  ];

  src = fetchFromGitHub {
    owner = "marzer";
    repo = "poxy";
    rev = "v${version}";
    hash = "sha256-f2Fb6GZwgED3QkdLSEkHrWYgItMSoyk1ziGxevj6gJo=";
  };

  dependencies = with python3.pkgs; [
    beautifulsoup4
    colorama
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
  dontCheckRuntimeDeps = true;

  postInstall = ''
    wrapProgram $out/bin/poxy --set PATH ${lib.makeBinPath [doxygen]}
  '';

  meta = with lib; {
    description = "Documentation generator for C";
    homepage = "https://github.com/marzer/poxy/";
    changelog = "https://github.com/marzer/poxy/blob/${src.rev}/CHANGELOG.md";
    license = licenses.mit;
    maintainers = [ ];
    mainProgram = "poxy";
  };
}
