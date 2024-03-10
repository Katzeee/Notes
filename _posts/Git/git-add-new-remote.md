#git 
## 2022.10.13

## Add a new remote git repository for a local repository

- Check local repo

    ```bash
    $ git remote -v
    origin  git@github.com:keithnull/keithnull.github.io.git (fetch)
    origin  git@github.com:keithnull/keithnull.github.io.git (push)
    ```

- Method 1

    ```bash
    $ git remote add coding.net git@git.coding.net:KeithNull/keithnull.github.io.git
    # git remote add <name> <url>
    # add a new remote repo and name it <name>
    ```

    Now you will get one more pull address and one more push address like this:

    ```bash
    $ git remote -v
    coding.net      git@git.coding.net:KeithNull/keithnull.github.io.git (fetch)
    coding.net      git@git.coding.net:KeithNull/keithnull.github.io.git (push)
    origin  git@github.com:keithnull/keithnull.github.io.git (fetch)
    origin  git@github.com:keithnull/keithnull.github.io.git (push)       
    ```

    Every time you pull or push, you should specify which repo will you operate:

    ```bash
    $ git push origin master
    $ git pull coding.net master
    ```

- Method 2 (**RECOMMEND**)

    ```bash
    $ git remote set-url --add origin git@git.coding.net:KeithNull/keithnull.github.io.git
    # git remote set-url --add <name> <url>
    # add a new remote url to current remote repo named <name>
    ```

    Now we don't have multiple origin repos but a new address of the origin repo.

    ```bash
    $ git remote -v
    origin  git@github.com:keithnull/keithnull.github.io.git (fetch)
    origin  git@github.com:keithnull/keithnull.github.io.git (push)
    origin  git@git.coding.net:KeithNull/keithnull.github.io.git (push)
    ```

    All pulls and pushes are as the same.

    ```bash
    $ git pull
    $ git push
    ```
