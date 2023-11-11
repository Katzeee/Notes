#code-env #python
## 2023.08.18

## Install pyenv on Ubuntu

To manage multiple versions of Python on Ubuntu, you can use `pyenv`. Here's how you can install and use it:

**1. Installation:**

- Download source

```bash
$ curl https://pyenv.run | bash
```

Add to `~/.bashrc` or other `~/.profile`

```bash
$ export PYENV_ROOT="$HOME/.pyenv"
$ command -v pyenv >/dev/null || export PATH="$PYENV_ROOT/bin:$PATH"
$ eval "$(pyenv init -)"
```

- Set up build env:

```bash
$ sudo apt update; sudo apt install build-essential libssl-dev zlib1g-dev \
libbz2-dev libreadline-dev libsqlite3-dev curl \
libncursesw5-dev xz-utils tk-dev libxml2-dev libxmlsec1-dev libffi-dev liblzma-dev
```

**2. Verify installation:**

```bash
$ pyenv --version
```

**3. Install desired Python versions:**

```bash
$ pyenv install 3.9.6
$ pyenv install 2.7.18
```

!!! Hint Mirror source

    Use this command to download from mirror.  
    `$ v=3.9.6; wget https://npm.taobao.org/mirrors/python/$v/Python-$v.tar.xz -P ~/.pyenv/cache/; pyenv install $v`

**4. Set global or local Python version:**

```bash
$ pyenv global 3.9.6  # Set the global Python version
$ pyenv local 2.7.18  # Set the Python version for the current directory
```

**5. Verify Python version:**

```bash
$ python --version
```

With `pyenv`, you can easily switch between different Python versions and manage them for different projects or system-wide.

## Use with virtual envs

!!! Note 

**1. Create a virtual env**

```bash
$ pyenv virtualenv [3.6.2] my_env # version can be ignored
```

**2. (De)Activate virtual env**

```bash
$ pyenv activate my_env
$ pyenv deactivate my_env
```

**3. Check versions**

```bash
$ pyenv versions # show all python versions and venvs
$ pyenv virtualenvs # only show virtual envs
```