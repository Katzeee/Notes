#linux/tool #compile #c-cpp 
## 2022.5.7

I met a undefined symbol error when I was running a c project.

```
../bin/test: symbol lookup error: ../bin/test: undefined symbol: g_thread_proxy
```

To find out the reason, use `ldd ../bin/test`, you can see which libs are linked to `test`, it will be easily find whether there is a link error.

```bash
~/rhs/YoloV5-LibTorch$ ldd ./bin/test 
	linux-vdso.so.1 (0x00007ffd82735000)
	libc10.so => /home/xac/local/libtorch/lib/libc10.so (0x00007f17636bf000)
	libdl.so.2 => /lib/x86_64-linux-gnu/libdl.so.2 (0x00007f17636a2000)
	libm.so.6 => /lib/x86_64-linux-gnu/libm.so.6 (0x00007f1763553000)
	libpthread.so.0 => /lib/x86_64-linux-gnu/libpthread.so.0 (0x00007f1763530000)
	librt.so.1 => /lib/x86_64-linux-gnu/librt.so.1 (0x00007f1763526000)
	libtorch.so => /home/xac/local/libtorch/lib/libtorch.so (0x00007f1763322000)
	libtorch_cuda.so => /home/xac/local/libtorch/lib/libtorch_cuda.so (0x00007f1763120000)
	libtorch_cuda_cpp.so => /home/xac/local/libtorch/lib/libtorch_cuda_cpp.so (0x00007f16f9ee2000)
	libtorch_cpu.so => /home/xac/local/libtorch/lib/libtorch_cpu.so (0x00007f16e1ea0000)
	libtorch_cuda_cu.so => /home/xac/local/libtorch/lib/libtorch_cuda_cu.so (0x00007f16a93b5000)
	libcudart.so.11.0 => /usr/local/cuda/lib64/libcudart.so.11.0 (0x00007f16a9111000)
	libnvToolsExt.so.1 => /usr/local/cuda/lib64/libnvToolsExt.so.1 (0x00007f16a8f05000)
	libstdc++.so.6 => /lib/x86_64-linux-gnu/libstdc++.so.6 (0x00007f16a8d23000)
	libc.so.6 => /lib/x86_64-linux-gnu/libc.so.6 (0x00007f16a8b31000)
	/lib64/ld-linux-x86-64.so.2 (0x00007f1764e63000)
	libgcc_s.so.1 => /lib/x86_64-linux-gnu/libgcc_s.so.1 (0x00007f16a8b16000)
	libgomp-52f2fd74.so.1 => /home/xac/local/libtorch/lib/libgomp-52f2fd74.so.1 (0x00007f16a88e3000)
	libc10_cuda.so => /home/xac/local/libtorch/lib/libc10_cuda.so (0x00007f16a85f3000)
	libcudart-a7b20f20.so.11.0 => /home/xac/local/libtorch/lib/libcudart-a7b20f20.so.11.0 (0x00007f16a8356000)
	libnvToolsExt-24de1d56.so.1 => /home/xac/local/libtorch/lib/libnvToolsExt-24de1d56.so.1 (0x00007f16a814c000)

```

then use `nm` and `grep` to find whether the symbol is in the `.so` file, like `nm /lib/x86_64-linux-gnu/libpthread.so. | grep pthread`