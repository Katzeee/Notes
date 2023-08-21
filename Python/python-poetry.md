#code-env #python
## 2023.08.18


## Installation

- Linux

  ```python
  $ curl -sSL https://install.python-poetry.org | python3 -
  ```

- Windows

  ```powershell
  (Invoke-WebRequest -Uri https://install.python-poetry.org -UseBasicParsing).Content | py -
  ```

## Basic commands

- Create a new project

  ```bash
  $ poetry new demo
  ```

- Interactive init a project

  ```bash
  $ poetry init
  ```

- (De)activate the virtaul env

  !!! Note Poetry will defaultly creates a virtual environment in `{cache-dir}/virtualenvs`.

  ```bash
  $ poetry shell # activate
  $ exit # deactivate and exit nested shell
  $ deactivate # deactivate but don't exit nested shell
  ```

- Run a specific command in virtaul env

  ```bash
  $ poetry run <command> 
  ```

- Add dependencies

  ```bash
  $ poetry add [--group=dev] <package-name> # add a package to main[dev] group 
  ```

- Remove dependencies

  ```bash
  $ poetry remove [--group=dev] <package-name> # add a package to main[dev] group 
  ```

- Install dependencies

  ```bash
  $ poetry install
  ```