#git 
## 2024.02.19

## `git filter-branch`

`git filter-branch` can rewrite the git history, filter, rewrite, or rearrange.

It's not like `rebase`, it is used to rewrite the history, clean up some sensitive information, change author, commit content and so on without changing other commits' information, like commit time.

```bash
$ git filter-branch --env-filter '
if [ $GIT_COMMIT = <commit_hash> ]
then
    export GIT_AUTHOR_NAME="New Author Name"
    export GIT_AUTHOR_EMAIL="newemail@example.com"
fi
' --tag-name-filter cat -- --branches --tags
```

## `git filter-repo`

`git filter-repo` can also do like this.

```bash
$ git filter-repo --email-callback '
    if commit.committer_name == b"Old Author Name":
        commit.committer_name = b"New Author Name"
        commit.committer_email = b"newemail@example.com"
' --commit-callback '
    if commit.committer_name == b"Old Author Name":
        commit.committer_name = b"New Author Name"
        commit.committer_email = b"newemail@example.com"
'
```

## Difference

`git filter-branch`: shell

`git filter-repo`: python, more efficient, substitution of above one, **recommended**