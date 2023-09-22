#git
## 2023/09.17

### git use submodule

> https://stackoverflow.com/questions/1260748/how-do-i-remove-a-submodule

**Add a submodule**

```bash
$ cd <submodule-worspace>
$ git submodule add <url-to-submodule>
```

**Pull all submodule**
```bash
$ git submodule update --init --recursive
```

**Remove a submodule**

For newer version:

```bash
$ rm -rf .git/modules/<path-to-submodule>
$ git config --remove-section submodule.<path-to-submodule>
```

For older version:

1.  Delete the relevant section from the `.gitmodules` file.
2.  Stage the `.gitmodules` changes:  
    `git add .gitmodules`
3.  Delete the relevant section from `.git/config`.
4.  Remove the submodule files from the working tree and index:  
    `git rm --cached path_to_submodule` (no trailing slash).
5.  Remove the submodule's `.git` directory:  
    `rm -rf .git/modules/path_to_submodule`
6.  Commit the changes:  
    `git commit -m "Removed submodule <name>"`
7.  Delete the now untracked submodule files:  
    `rm -rf path_to_submodule`