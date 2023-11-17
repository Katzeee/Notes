#linux/tool
## 2023.11.17

## `2>&1`

Let's break down `2>&1` in a simple way.

In Unix and Linux systems, every process has three streams opened:

-   Standard input (stdin), file descriptor number is 0
-   Standard output (stdout), file descriptor number is 1
-   Standard error (stderr), file descriptor number is 2

The `>` symbol is used for redirection. By default, it redirects the standard output (stdout).

`2>` is for redirecting stderr specifically.

Now coming to `2>&1`:

-   `2>` means "redirect stderr".
-   The `&` indicates what follows is a file descriptor and not a filename.
-   `1` represents stdout.

So, `2>&1` means "redirect stderr to the same location as stdout".

When you run a command like `command > output.txt 2>&1`, it:

1.  Redirects stdout to `output.txt` (`command > output.txt`)
2.  Then redirects stderr to the place where stdout points to, which is `output.txt` (`2>&1`)

So both stdout and stderr output ends up in `output.txt`.