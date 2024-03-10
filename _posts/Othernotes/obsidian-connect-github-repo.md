#productivity/obsidian
## 2023.08.14

1.  Create a [personal access token](https://docs.github.com/en/authentication/keeping-your-account-and-data-secure/creating-a-personal-access-token#creating-a-personal-access-token-classic) from github ![create-pat-github.png](../.images/create-pat-github.png)

2.  Install the [Obsidian Git](https://github.com/denolehov/obsidian-git/wiki/Installation) community plugin

3.  Create a folder to store the repository. (e.g. `remote-blog/`). Set scopes to repo & expiration to no expiration

4.  Run the command (CMD/Ctrl + P): `Clone an existing remote repo` ![clone-repo-git-plugin.png](../.images/clone-repo-git-plugin.png)

5.  Paste the URL of the forked repository in the following format

```
https://<PERSONAL_ACCESS_TOKEN>@github.com/<USERNAME>/<REPO>.git
```

For example it might look like this:

```
https://ghp_XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX@github.com/ithinkwong/linked-blog-starter-md.git
```

7.  Then type in the folder you created in step 5 (e.g. `remote-blog/`)

8.  Restart Obsidian