# 2021.3.29

### 基本命令

```bash
$ git status #查看文件状态
$ git add . #添加所有文件到暂存区
$ git commit -m "message" #提交暂存区内容到本地仓库
$ git log #查看当前版本之前提交记录
$ git reflog #查看所有提交记录
$ git reset --hard 版本号 #回退到某个版本
$ git branch -v #查看分支
$ git branch 分支名 #创建分支
$ git checkout 分支名 #切换分支
$ git branch -a #查看所有分支
$ git merge 分支名 #将该分支合并到当前工作分支
```

# 2021.5.16

### 本地上传至github

```bash
$ git init
$ git add .
$ git remote add origin <url>
$ git pull
$ git push -u origin master -f #强制覆盖
```



# 2022.1.17

### git删除分支

先切换到别的分支
```bash
$ git branch -d name #删除本地分支
$ git push origin --delete name #删除远程分支
```
# 2022.1.17

### git分支改名字
```bash
$ git branch -m <oldname> <newname>
```
### ctags使用方法
```bash
ctags -R
```

### git删除分支
先切换到别的分支
```bash
$ git branch -d name #删除本地分支
$ git push origin --delete name #删除远程分支
```

# 2022.4.8
### git远程分支交互
```bash
# 将远程指定分支 拉取到 本地指定分支上
$ git pull origin <远程分支名>:<本地分支名>
# 将远程指定分支 拉取到 本地当前分支上：
$ git pull origin <远程分支名>
# 将与本地当前分支同名的远程分支拉取到本地当前分支上
$ git pull
# 将本地指定分支 推送到 远程指定分支上（注意：pull是远程在前本地在后，push相反）：
$ git push origin <本地指定分支名>:<远程指定分支名>
# 将本地指定分支 推送到 与本地指定分支同名的远程分支上（注意：pull是远程在前本地在后，push相反）：
$ git push origin <本地指定分支名>
# 将本地当前分支 推送到 与本地当前分支同名的远程分支上
$ git push
# 将本地分支与远程同名分支相关联
$ git push -u origin <本地分支名>
```

2022.10.13
---

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

## Push conflit after local rebase

```bash
$ git push -f # cover all information of the remote, use local history instead
$ git push --force-with-lease # Only if no conflit, cover the remote repo history
```

## Use git stash to stash your current changes

```bash
$ git stash # stash only stash files which are tracked, so use add first
$ git stash list
$ git stash pop 
$ git stash apply [--index]
$ git stash drop
```

2023.07.24
---

## checkout to remote branch and track it locally(i.e. create a local branch)

```bash
$ git checkout --track origin/games104/homework01-build 
```

or 

```bash
$ git switch games104/homework01-build
```

lazygit

- `c` for checkout by name
- you now have a detached HEAD. Press `n` to create a new branch here
