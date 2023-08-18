#code-env #python
## 2023.08.18

## Install pyenv on Ubuntu

To manage multiple versions of Python on Ubuntu, you can use `pyenv`. Here's how you can install and use it:

**1. Install dependencies:**

```bash
$ sudo apt update
$ sudo apt install -y build-essential libssl-dev zlib1g-dev libbz2-dev \
libreadline-dev libsqlite3-dev wget curl llvm libncurses5-dev libncursesw5-dev \
xz-utils tk-dev libffi-dev liblzma-dev python-openssl git

```

**2. Clone and install pyenv:**

```bash
$ git clone https://github.com/pyenv/pyenv.git ~/.local/.pyenv
$ echo 'export PYENV_ROOT="$HOME/.local/.pyenv"' >> ~/.bashrc
$ echo 'export PATH="$PYENV_ROOT/bin:$PATH"' >> ~/.bashrc
$ echo 'eval "$(pyenv init --path)"' >> ~/.bashrc
$ source ~/.bashrc
```

**3. Verify installation:**

```bash
$ pyenv --version
```

**4. Install desired Python versions:**

```bash
$ pyenv install 3.9.6
$ pyenv install 2.7.18
```

**5. Set global or local Python version:**

```bash
$ pyenv global 3.9.6  # Set the global Python version
$ pyenv local 2.7.18  # Set the Python version for the current directory
```

**6. Verify Python version:**

```bash
$ python --version
```

!!! Hint Mirror source

    Use this command to download from mirror.  
    `$ v=3.9.6; wget https://npm.taobao.org/mirrors/python/$v/Python-$v.tar.xz -P ~/.pyenv/cache/; pyenv install $v`

With `pyenv`, you can easily switch between different Python versions and manage them for different projects or system-wide.