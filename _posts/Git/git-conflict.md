#git 
## 2023.02.11

## Why

Recently, I am working with bustub. When I go through LAB3, I find my code version is late of the newest version of public code repository. 

I have made lots of change base the old version, but if I want to continue to finish the lab, I must pull the latest version of code which will make a lot of conflicts.

## Where is the confilt

After run `git pull --rebase`, you will notice git says 

```bash
➜  build git:(master) git pull public master --rebase                                     
From https://github.com/cmu-db/bustub
 * branch            master     -> FETCH_HEAD
Auto-merging CMakeLists.txt
CONFLICT (modify/delete): src/include/primer/p0_trie.h deleted in HEAD and modified in ca0179e (mutex init fail).  Version ca0179e (mutex init fail) of src/include/primer/p0_trie.h left in tree.
CONFLICT (modify/delete): test/primer/starter_trie_test.cpp deleted in HEAD and modified in ca0179e (mutex init fail).  Version ca0179e (mutex init fail) of test/primer/starter_trie_test.cpp left in tree.
error: could not apply ca0179e... mutex init fail
hint: Resolve all conflicts manually, mark them as resolved with
hint: "git add/rm <conflicted_files>", then run "git rebase --continue".
hint: You can instead skip this commit: run "git rebase --skip".
hint: To abort and get back to the state before "git rebase", run "git rebase --abort".
Could not apply ca0179e... mutex init fail
```

Then call `git status`, you will get the certain status of every file:

```bash
interactive rebase in progress; onto 49deaa1
Last command done (1 command done):
   pick ca0179e mutex init fail
Next commands to do (33 remaining commands):
   pick 6b03cab p0 all test passed
   pick aa60a9f passed all test and formated
  (use "git rebase --edit-todo" to view and edit)
You are currently rebasing branch 'master' on '49deaa1'.
  (fix conflicts and then run "git rebase --continue")
  (use "git rebase --skip" to skip this patch)
  (use "git rebase --abort" to check out the original branch)

Changes to be committed:
  (use "git restore --staged <file>..." to unstage)
        modified:   ../CMakeLists.txt

Unmerged paths:
  (use "git restore --staged <file>..." to unstage)
  (use "git add/rm <file>..." as appropriate to mark resolution)
        deleted by us:   ../src/include/primer/p0_trie.h
        deleted by us:   ../test/primer/starter_trie_test.cpp
```

Divide the output to two part: `Changes to be committed` which are the files without conflict on them, and `Unmerged paths` which are conflicted files.

## How to solve

Then conflicts are different based on the operations in local and remote. It can be easily distinguished by the hint.

- `deleted by us` (lazygit `DU`)

  Someone deleted this file on remote. (Maybe we modified it or not.)

  `git add` to keep, stage the change (lg `a`)

  `git rm` to discard

- `both modified` (lg `UU`)

  edit the conflict files to solve

  `HEAD` indicates the remote branch's change while another means the local change.
  
## Push conflit after local rebase

- Force push

  ```bash
  $ git push -f # cover all information of the remote, use local history instead
  $ git push --force-with-lease # Only if no conflit, cover the remote repo history
  ```

- Push to another branch
