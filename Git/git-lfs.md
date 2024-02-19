#git 
## 2024.02.19

## Install git lfs

1. Windows

    https://git-lfs.com/

2. Linux

    Use package manager like `apt`, `pacman`.

## Track your files use lfs

```bash
$ git lfs install # only need to run this once per user account 
```

```bash
$ git lfs track "*.png" # track all png files
$ git add .gitattributes # make sure this file been tracked
```

Then commit as usual

```bash
$ git add .
$ git commit -m "Use git lfs"
```

All `.png` files will be tracked as large file after this commit.

But this will not affect any previous commit or other branches.

If you want change the tracking mode in previous commit, you should checkout to that commit then use `git lfs migrate`.

!!! Warning This operation will affect the history of your repo.