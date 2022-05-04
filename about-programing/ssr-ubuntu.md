# 2022.5.4

下载完ssr-electron之后没有办法直接在ubuntu使用，查看log文件发现是执行python命令时出错，缺少软链接

执行`ln -s /usr/local/python3 /usr/local/python`

之后发现依赖库没安装

执行`sudo apt install python-openssl`