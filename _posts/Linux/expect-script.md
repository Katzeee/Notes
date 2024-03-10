#windows/tool #linux/tool
## 2023.09.07

> https://unix.stackexchange.com/questions/597113/how-can-i-use-regex-to-match-the-contents-which-is-end-with-by-using-expect

## Basic of expect script

Use expect script in bash file:

```bash
#!/bin/bash
expect << EOF
    spawn ssh xac@10.9.60.115
    send PS1=#\r
    expect #
    send "bash test.sh\r"
    expect {
    "\nLinking to GMPOC_MCU_MASTER.elf*ms*\n*End of build*" { exit 3; expect_after }
    # -re {} { exit 3; expect_before }
    # "#" { exit 2; expect_after }
    eof { exit 1 }
    }
EOF
    echo $?
```

!!! When mixing expect code into a shell script, it's best to use a heredoc.

Which means you should enclose your regex string with brackets.

```bash
expect {
    -re {.*(P|p)assword:} {send "$2\r"}
}
```


## Add a lock to your flow

```bash
$ notepad >.lock
```

This command will create a `.lock` file and prevent another program to open it again.