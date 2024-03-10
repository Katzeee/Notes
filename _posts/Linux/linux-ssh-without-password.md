#code-env #linux/tool
## 2023.08.19

## ssh login without password

1. Create ssh key for both computer

    ```bash
    $ ssh-keygen -t rsa # press enter thrice
    ```

2. Add self to known_hosts 

    ```bash
    $ ssh-copy-id <username>@<ip-addr>
    ```

    !!! Danger If you want login B without password on host A, you should run this command on A!


