---
title: Linux Symlink
date: 2022.05.21
categories: [Linux]
tags: [linux/tool]
---

## Do not symlink with relative path

If you use a relative path in symlink, the path deduction starts at where you call the symlink, not at where the `ln` command ran. So use absolute path in making symlink.