#linux #proxy
## 2022.5.4

## **[ubuntu]**

下载完ssr-electron之后没有办法直接在ubuntu使用，查看log文件发现是执行python命令时出错，缺少软链接

执行`ln -s /usr/local/python3 /usr/local/python`

之后发现依赖库没安装

执行`sudo apt install python-openssl`

## **[arch]**

```bash
2022-05-13 14:25:12:264 [info] run command: %s python /home/xac/.config/electron-ssr/shadowsocksr/shadowsocks/local.py -s shallaz4.dnsflix.com -p 566 -k ****** -m chacha20-ietf -O auth_aes128_md5 -G 7515:-2Wt64CqdUoX]3 -o plain -b 127.0.0.1 -l 1080
2022-05-13 14:25:12:292 [info] Traceback (most recent call last):
2022-05-13 14:25:12:292 [info]   File "/home/xac/.config/electron-ssr/shadowsocksr/shadowsocks/local.py", line 31, in <module>
2022-05-13 14:25:12:292 [info]     from shadowsocks import shell, daemon, eventloop, tcprelay, udprelay, asyncdns
2022-05-13 14:25:12:292 [info]   File "/home/xac/.config/electron-ssr/shadowsocksr/shadowsocks/../shadowsocks/shell.py", line 26, in <module>
2022-05-13 14:25:12:292 [info]     from shadowsocks.common import to_bytes, to_str, IPNetwork, PortRange
2022-05-13 14:25:12:293 [info]   File "/home/xac/.config/electron-ssr/shadowsocksr/shadowsocks/../shadowsocks/common.py", line 27, in <module>
2022-05-13 14:25:12:293 [info]     from shadowsocks import lru_cache
2022-05-13 14:25:12:293 [info]   File "/home/xac/.config/electron-ssr/shadowsocksr/shadowsocks/../shadowsocks/lru_cache.py", line 44, in <module>
2022-05-13 14:25:12:293 [info]     class LRUCache(collections.MutableMapping):
2022-05-13 14:25:12:293 [info] AttributeError: module 'collections' has no attribute 'MutableMapping'
2022-05-13 14:25:12:293 [info] 
2022-05-13 14:25:12:296 [error] Python SSR quit with code: 1
```

This is caused by high version of python, `arch` is installed with python3.10, in which the definition of `collections.MutableMapping` has been changed 

change the 44th line in `"/home/xac/.config/electron-ssr/shadowsocksr/shadowsocks/../shadowsocks/lru_cache.py" ` from `collections.MutableMapping` to `collections.abc.MutableMapping`

## 2022.5.16

**Double-click** the node!!!