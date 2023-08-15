#git
## 2023.08.15

Reference:

> https://stackoverflow.com/questions/23207791/git-rebase-conflict-who-is-head
>
> https://git-scm.com/docs/git-rebase
>
> https://git-scm.com/docs/git-merge

## Explaination of commands

```git
$ git checkout topic
$ git rebase master
```

**Rebase to master means reapply commits on topic to master(result to several commits)**

```git
$ git checkout master
$ git merge topic
```

**Merge from master means to replay commits on topic on master(record the result to one commit)**


## What does git rebase do?

When merging (including rebasing, which is a special case of repeated "merging" internally), there are two "heads" (two specific branch-tips) involved. Let's call these `your-branch` and `origin/their-branch`:

```
              G - H --------      <-- HEAD=your-branch
            /               \
... - E - F                   M   <-- desired merge commit [requires manual merge]
            \               /
              I - J - K - L       <-- origin/their-branch
```

This point is commonly (and unsurprisingly) confusing, although when labeled like this it's clear enough.

Making it worse, though, git uses `--ours` and `--theirs` to refer to the two head commits during a merge, with "ours" being the one you were on (commit `H`) when you ran `git merge`, and "theirs" being, well, theirs (commit `L`). But when you're doing a rebase, the two heads are reversed, so that "ours" is the head you're rebasing on-to—i.e., their updated code—and "theirs" is the commit you're currently rebasing, i.e., your own code.

**This is because rebase actually uses a series of cherry-pick operations.** You start with much the same picture:

```
              G - H           <-- HEAD=your-branch
            /
... - E - F
            \
              I - J - K - L   <-- origin/their-branch
```

What git needs to do here is to _copy_ the effect of commits `G` and `H`, i.e., `git cherry-pick` commit `G`, then do it again with commit `H`. But to do that, git has to _switch_ to commit `L` first, internally (using "detached HEAD" mode):

```
              G - H           <-- your-branch
            /
... - E - F
            \
              I - J - K - L   <-- HEAD, origin/their-branch
```

Now it can start the rebase operation by comparing the trees for commits `F` and `G` (to see what you changed), then comparing `F` vs `L` (to see if some of your work is already in `L`) and taking any changes not already in `L` and add it. This is a "merge" operation, internally.

```
              G - H           <-- your-branch
            /
... - E - F                   G'   <-- HEAD
            \               /
              I - J - K - L   <-- origin/their-branch
```

If the merge does not go well, `HEAD` is still left at commit `L` (because commit `G'` does not yet exist). Thus, yes, `HEAD` is the head of their development branch—at least, it is right now.

Once the copy of `G` exists, though, `HEAD` moves to `G'` and git attempts to copy the changes from `H`, in the same manner (diff `G` vs `H`, then diff `F` vs `G'`, and merge the results):

```
              G - H           <-- your-branch
            /
... - E - F                   G' - H'   <-- HEAD
            \               /
              I - J - K - L   <-- origin/their-branch
```

Again, if the merge fails and needs help, you're left with `HEAD` pointing to `G'` instead of `H'` as `H'` does not yet exist.

Once the merges all succeed and commits `G'` and `H'` _do_ exist, git removes the label `your-branch` from commit `H`, and makes it point to commit `H'` instead:

```
              G - H
            /
... - E - F                   G' - H'   <-- HEAD=your-branch
            \               /
              I - J - K - L   <-- origin/their-branch
```

and you are now rebased and `HEAD` is once again what you would expect. But during the rebase, `HEAD` is either their branch-tip (commit `L`), or one of the new commits copied and appended past their branch-tip; and `--ours` means the branch being grown at the end of `L` while `--theirs` means the commit being copied-from (`G` or `H` above).

(This is basically git exposing the raw mechanism of _how_ it does what it does, which happens rather a lot in git.)