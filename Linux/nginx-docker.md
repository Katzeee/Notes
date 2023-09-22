#docker #front-end/nginx
## 2023.09.22

## Install docker

Just run this shell script[ubuntu]

> https://github.com/Katzeee/dot-files/blob/master/docker.sh

## Pull nginx image

```bash
$ docker pull nginx:latest
```

## Start an nginx container

**Start a smallest nginx**
```bash
$ docker run -d --name nginxtest -p 9091:80 -p 9092:443 nginx:latest
```

**Copy its config file**

```bash
$ docker cp $<container-id>:/etc/nginx/nginx.conf ~/server/nginx/nginx.conf
```

**Delete the container**

```bash
$ docker stop $<container-id> && docker rm $<container-id>
```

**Start a new container with `-v` flag**

```bash
$ docker run \
    -d \
    --name nginxtest \
    -v ~/server/nginx/www:/usr/share/nginx/html \
    -v ~/server/nginx/conf/nginx.conf:/etc/nginx/nginx.conf \
    -v ~/server/nginx/logs:/var/log/nginx \
    -p 9091:80 \
    -p 9092:443 \
    nginx:latest
```

**Try host a simple web page**

Put it at `~/server/nginx/www/index.html`

```html
<html>
    <head></head>
    <body>
        <h1>test</h1>
    </body>
</html>
```

