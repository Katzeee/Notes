#productivity/zotero
## 2022.5.4

## sci-hub

查找文献pdf

首选项->高级->常规->设置编辑器，搜索extensions.zotero.findPDFs.resolvers

加入

```json
{
    "name":"Sci-Hub",
    "method":"GET",
    "url":"https://sci-hub.ren/{doi}",
    "mode":"html",
    "selector":"#pdf",
    "attribute":"src",
    "automatic":true
}
```

## 茉莉花（jasminum）

识别中文文献

工具->插件->从文件安装，首选项->茉莉花，将添加中文PDF选项打勾，之后在非官方中文翻译器里面更新全部。

## delitemwithatt

插入参考文献时，可能会出现英文文献采用“等”，而不是etal，该插件可以将语言字段设为en，并且可以合并作者姓和名。

## zotfile

对pdf文件进行重命名