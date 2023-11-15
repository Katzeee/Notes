#code-env #linux #nodejs
## 2023.08.16

## What is node, npm?

Node.js is an open-source JavaScript runtime environment outside a web browser.

NPM(Node Package Manager) is the default package manager for Node.js.

## Basic commands

!!! Hint Change the global node_modules directory

    ```bash
    $ mkdir ~/.npm-global
    $ npm config set prefix '~/.npm-global'
    $ export PATH=~/.npm-global/bin:$PATH
    ```


- Check version

  ```bash
  $ npm -v
  $ node -v
  ```

- Interactively create a package.json file:

  ```bash
  $ npm init
  ```

- Download a specific version of a package and add it to the list of dependencies in package.json:

  ```bash
  $ npm install {{module_name}}@{{version}}
  ```

- Download a package and add it to the list of dev dependencies in package.json:

  ```bash
  $ npm install {{module_name}} --save-dev
  ```

- Download a package and install it globally:

  ```bash
  $ npm install [--global|-g] {{module_name}}
  ```

- Uninstall a package and remove it from the list of dependencies in package.json:

  ```bash
  $ npm uninstall [-g] {{module_name}}
  ```

- Print a tree of locally installed dependencies:

  ```bash
  $ npm list
  ```

- List top-level globally installed modules:

  ```bash
  $ npm list --global --depth={{0}}
  ```

- Set mirror

  ```bash
  $ npm config set registry https://registry.npmmirror.com
  ```

## Node version control

Use nvm(Node Version Manager) to control the version of node.

```bash
$ curl -o- https://raw.githubusercontent.com/nvm-sh/nvm/v0.38.0/install.sh | bash
```

Add following to rc file.

```
export NVM_DIR="$HOME/.nvm"
[ -s "$NVM_DIR/nvm.sh" ] && \. "$NVM_DIR/nvm.sh"  # This loads nvm
[ -s "$NVM_DIR/bash_completion" ] && \. "$NVM_DIR/bash_completion"  # This loads nvm bash_completion
```

- Install a specific version of Node.js:

  ```bash
  $ nvm install {{node_version}}
  ```

- Use a specific version of Node.js in the current shell:

  ```bash
  $ nvm use {{node_version}}
  ```

- Set the default Node.js version:

  ```bash
  $ nvm alias default {{node_version}}
  ```

- List all available Node.js versions and highlight the default one:

  ```bash
  $ nvm list
  ```

- Uninstall a given Node.js version:

  ```bash
  $ nvm uninstall {{node_version}}
  ```

- Launch the REPL of a specific version of Node.js:

  ```bash
  $ nvm run {{node_version}} --version
  ```

- Execute a script in a specific version of Node.js:

  ```bash
  $ nvm exec {{node_version}} node {{app.js}}
  ```
