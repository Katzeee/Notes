#python #code-env
## 2023.08.19

## Use venv with `pyproject.toml` and `pyright`

> https://github.com/microsoft/pyright/blob/main/docs/configuration.md

Just set two varibles:

```toml
[tool.pyright]
venvPath = "/home/xac/Documents/test-plugins/"
venv = ".venv"
```

venvPath [path, optional]: Path to a directory containing one or more subdirectories, each of which contains a virtual environment. When used in conjunction with a venv setting (see below), pyright will search for imports in the virtual environment’s site-packages directory rather than the paths specified by the default Python interpreter. **If you are working on a project with other developers, it is best not to specify this setting in the config file, since this path will typically differ for each developer.** Instead, it can be specified on the command line or in a per-user setting.

venv [string, optional]: Used in conjunction with the venvPath, specifies the virtual environment to use.