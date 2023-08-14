#linux/tool 
## 2022.11.16

## pdftk

Reference:

> https://zhuanlan.zhihu.com/p/78255850

split and concatenate:

cat means concatenate, output derive the output file

```bash
$ pdftk pg1.pdf pg2.pdf pg3.pdf pg4.pdf pg5.pdf cat output OneDoc.pdf
$ pdftk main.pdf cat 1 2-4 6-end output x.pdf
```

