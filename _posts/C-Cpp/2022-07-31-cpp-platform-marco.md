---
title: Cpp Platform Marco
date: 2022.07.31
categories: [C-Cpp]
tags: [c-cpp]
---

> https://stackoverflow.com/questions/4605842/how-to-identify-platform-compiler-from-preprocessor-macros

- For Mac OS:

`#ifdef __APPLE__`

- For MingW on Windows:

`#ifdef __MINGW32__`

- For Linux:

`#ifdef __linux__`

