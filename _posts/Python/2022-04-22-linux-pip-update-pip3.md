---
title: Linux Pip Update Pip3
date: 2022.04.22
categories: [Python]
tags: [linux, python]
---

Cannot find the higher version of libraries.

`WARNING: pip is being invoked by an old script wrapper. This will fail in a future version of pip`

Rebuild a symbol link. Or install `python-pip3`

```bash
$ sudo rm /usr/bin/pip
$ sudo ln -s /usr/bin/pip3 /usr/bin/pip
```