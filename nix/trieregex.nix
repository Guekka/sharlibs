{
  lib,
  python3,
  fetchPypi,
}:
python3.pkgs.buildPythonApplication rec {
  pname = "trieregex";
  version = "1.0.0";
  pyproject = true;

  src = fetchPypi {
    inherit pname version;
    hash = "sha256-o03THQSqFp4ZiZcaMV/L1SQSYzDH8vnxaZGwqMkITq8=";
  };

  nativeBuildInputs = [
    python3.pkgs.setuptools
    python3.pkgs.wheel
  ];

  pythonImportsCheck = ["trieregex"];

  meta = with lib; {
    description = "Build trie-based regular expressions from large word lists";
    homepage = "https://pypi.org/project/trieregex/";
    license = licenses.mit;
    maintainers = with maintainers; [];
    mainProgram = "trieregex";
  };
}
