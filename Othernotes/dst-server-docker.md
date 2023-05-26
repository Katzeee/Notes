2023.05.27
---

Host a dst-server via docker!

Just clone `https://github.com/Katzeee/docker-dst-server`

Build the image from yourself `docker build . -t dst-server:latest` if you can't pull it from docker-hub.

Put the game data like this

```
➜  ~ tree ~/.klei -L 4
.klei
`-- DoNotStarveTogether
    |-- Agreements
    |   `-- DoNotStarveTogether
    |       `-- agreements.ini
    `-- DoNotStarveTogether
        `-- Cluster_1
            |-- Caves
            |-- Master
            |-- adminlist.txt
            |-- blocklist.txt
            |-- cluster.ini
            |-- cluster_token.txt
            |-- mods
            `-- whitelist.txt
8 directories, 6 files
```

Then run `docker run -v ${HOME}/.klei/DoNotStarveTogether:/data -p 10999-11000:10999-11000/udp -p 12346-12347:12346-12347/udp -e "DST_SERVER_ARCH=amd64" -it jamesits/dst-server:latest`.

I don't know why docker compose doesn't work!!!


