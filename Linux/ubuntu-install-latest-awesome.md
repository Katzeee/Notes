#linux/ubuntu #window-manager/awesome
## 2023.08.25

## Build from source

```bash
$ sudo cp /etc/apt/sources.list /etc/apt/sources.list.bk
$ sudo sed -Ei 's/^# deb-src /deb-src /' /etc/apt/sources.list
$ sudo apt-get update
$ sudo apt install libxcb-shape0-dev libxcb-xfixes0-dev
$ sudo apt build-dep awesome
$ git clone https:// github.com/awesomewm/awesome
$ cd awesome && make package -j14 && cd build
$ sudo apt install *.deb
```