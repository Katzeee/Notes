#linux 
## 2022.4.24

不小心删了`/tmp`文件夹，创建命令如下

```bash
sudo mkdir -m 1777 /tmp

# drwxrwxrwt表示1777,t表示sticky bit(防删除位),
# 任何人都可以在里面创建文件, 但是只能删除自己创建的文件(root除外).
```