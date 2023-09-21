#linux/tool
## 2023.09.21

## Use `xargs` with no arg means `xargs echo`

```bash
$ echo "F1 F2" | xargs
# is equal to
$ echo "F1 F2" | xargs echo
```

## `-n` to pass values group by number

```bash
$ echo -n "F1 F2" | xargs -n1 echo
```

!!! echo -n to ignore the `\n` at end

## `-D` to specify the delimiter

```bash
$ echo -n "F1#F2" | xargs -n1 -d"#"
```


## `-I replace-str` to place the output at wherever you want

```bash
$ find . -name "*.h" | xargs -n1 -I{} cp {} target_dir
```

## `-p` to interact with the every output of xargs 

```bash
$ echo -n1 -p "F1 F2" | xargs
```

## `-t` to echo every command

```bash
$ echo -n1 -p "F1 F2" | xargs
```

## Use nested shell command

```bash
$ echo -n1 -p "F1 F2" | xargs -I{} sh -c cp {} $(echo pre{} | cut -d" " -f1)
```