2023.03.19
---

> https://stackoverflow.com/questions/70819007/can-not-use-clangd-to-read-linux-kernel-code

When I read the code of linux kernel, LSP doesn't work well. Here's the solution.

- Errors everywhere

Use `scripts/gen_compile_commands.py` to generate a file called `compile_commands.json` which is to tell clangd where to find those source files.

- Something like `Unkonwn argument: '-mno-fp-ret-in-387'`

This is because you compiles the kernel code using `gcc`, but you are using clangd as LSP, there are two workarounds.

  - Change the compiler

    ```bash
    $ make CC=clang defconfig
    $ make CC=clang
    ```

  - Tell clangd to ignore those flag

    Add these to `.clangd`

    ```bash
    Remove: 
      [-fconserve-stack, -fno-allow-store-data-races, -mfunction-return=thunk-extern, -mindirect-branch-cs-prefix, -mindirect-branch-register, -mindirect-branch=thunk-extern, -mskip-rax-setup, -mpreferred-stack-boundary=3, -mno-fp-ret-in-387]
    ```
