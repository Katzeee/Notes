#git
## 2023.09.09

### git clone with proxy

> https://gist.github.com/evantoli/f8c23a37eb3558ab8765

```bash
$ git config --global http.proxy http://proxyUsername:proxyPassword@proxy.server.com:port
$ git config --global --unset http.proxy
```