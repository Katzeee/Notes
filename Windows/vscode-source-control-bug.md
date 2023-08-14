#code-env/vscode #git #windows 
## 2022.07.14

Vscode cannot recognize the `.git` file in the folder. It says "the folder currently open doesn't have a git folder" (but it does have).

We can see the following output:

```bash
Looking for git in: C:/msys2/usr/bin/git.exe
Using git 2.27.0 from C:/msys2/usr/bin/git.exe
> git rev-parse --git-dir
> git rev-parse --git-dir
> git rev-parse --git-dir
> git rev-parse --git-dir
> git rev-parse --git-dir
> git rev-parse --git-dir
> git rev-parse --git-dir
```

> https://github.com/microsoft/vscode/issues/111504#issuecomment-763586175

Solution:

Since VSCode currently can't handle the output of msys2's git `rev-parse` output, the only temporary solutions appears to be indeed switching to a standalone (non-msys2) git version OR placing a wrapper around git.exe (msys2).

- 1. Place the following `git-wrap.bat` into git's directory:
  ```bat
  @echo off
  setlocal
  
  rem If you don't add path for msys2 into %PATH%, enable following line.
  rem set PATH=c:\msys64\usr\bin;%PATH%
  
  if "%1" equ "rev-parse" goto rev_parse
  git %*
  goto :eof
  :rev_parse
  for /f %%1 in ('git %*') do cygpath -wa %%1
  ```
- 2. Change the `settings.json` to include reference to `git-wrap.bat` instead of `git.exe`
  ```json
  "git.path": "C:/msys64/usr/bin/git-wrap.bat",
  ```