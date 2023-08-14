2023.05.20
Linux内核分析
---

# 汇编指令拾遗

- x86

  call 等价于 push + mov eip
  ret 等价于 pop eip

- arm64

  x29一般用作栈基址寄存器，x30用于保存跳转指令的下一条指令内存地址
  x0用于保存函数返回结果，x8用于保存子程序返回地址，传递系统调用号等

  bl 是 branch with link 跳转命令
  stp 是 store
  ldp 是 load
  msr 和 mrs 是 move special and register 即移动特殊寄存器

# 内核启动过程

- start_kernel

  先进行内存初始化，VFS初始化，调度初始化等，最终调用arch_call_rest_init()

- arch_call_rest_init又调用rest_init

- rest_init

  使用kernel_thread创建1号进程kernel_init，所有用户进程的祖先，会依次尝试运行（run_init_process，即execve）/sbin/init，/etc/init，/bin/init，/bin/sh

  使用kernel_thread创建2号进程kthreadd，所有内核进程的祖先

  - kernel_thread
  
    都是调用_do_fork完成了进程的创建

  最后执行了cpu_startup_entry，转为idle进程，无限循环运行的内核态

# 系统调用

  系统调用是利用trap主动进入内核态

- x86

  不论32位还是64位，都是eax传递系统调用号，使用寄存器

  CPU保存CPU执行的关键上下文（EBP，EFALG，EIP），硬件查表进入中断程序，此步32位和64位有区别

  - 32，压到内核栈，在中断描述表中找到表项

  - 64，MSR实现快速跳转，快速切换eip到系统调用处理入口
  
  entry_SYSCALL_64，内核保存现场SAVEALL，do_syscall_64执行中断服务程序，恢复现场，返回。

  iret 负责ip等寄存器

- arm64

  cpu将pc放入elr_el1，pstate放入spsr_el1，产生异常的原因放在esr_el1中。

  cpu查找vbar_el1（vector）找到中断向量入口地址，对于系统调用来说，里面再调用el0_sync，使用kernel_entry 0 保存现场，再跳到处理程序的入口el0_svc，里面再调用el0_svc_handler并ret_to_user返回系统调用（负责恢复现场）eret负责ip等寄存器

# VFS

超级块（struct super_block）：它描述一个已安装了的文件系统。

索引结点（struct inode）：它描述一个文件。

目录项（strcut dentry）：它描述文件系统的层次结构。一个完整路径的每个组成部分都是一个目录项。比如打开/home/edsionte/code/hello.c时，内核分别为“/”，"home/"，"edsionte/"，"code/"，"hello.c"创建相应的目录项。

文件（struct file）：它描述一个已被进程打开的文件。

# 进程创建

- 数据结构

task_struct 被用来描述进程

list_head双向链表把所有的task_struct穿起来

thread_struct 保存进程上下文中CPU相关的一些状态，最关键的就是sp和ip（高版本中ip直接保存在内核对堆栈了）

用户地址空间内存描述符mm_struct

signal_struct 信号相关

fs_struct 文件相关

- 进程创建

0号进程init_task的初始化是硬编码方式固定下来的

do_fork使用copy_process复制父进程，获取pid，调用wake_up_new_task将子进程加入就绪队列等待调度执行

copy_process调用

- dup_task_struct复制父进程描述符task_struct，

- copy_thread_tls初始化子进程内核栈，设置子进程ip等，里面有ret_from_fork，子进程的执行从ret_from_fork开始

  p->thread.ip = (unsigned long) ret_from_fork;

# 进程调度

通过schedule函数实现进程调度，两种情况，在中断时调用或内核线程主动调用

pick_next选择下一个待切换进程，context_switch负责进程上下文的切换

context_switch:

  地址空间切换，即mm，switch_mm切换CR3即页全局目录

  switch_to，切换栈与寄存器，switch_to会调用__switch_to_asm与体系结构强相关，就是压栈寄存器，保存旧进程的栈，恢复新进程栈，不显示处理ip寄存器，通过call和ret进行隐式处理，还可再考虑fork，子进程ret出去时正好时ret_addr

中断上下文和进程上下文的关键区别就是堆栈的切换方式，sp与ip寄存的切换分别是由CPU与内核完成的

# linux系统的一般执行过程

进程X->发生中断->CPU保存eip，esp，eflags等->跳转至中断处理程序->SAVE_ALL保存现场（进入X的内核态）->中断处理->schedule->switch_to->执行进程Y->RESTORE_ALL恢复现场（此时是Y的内核态）->iret(恢复eip等)->继续执行Y

# 设备驱动程序

cdev描述一个字符设备，cdev_add进行注册，file_operation控制，万物皆文件

# 虚拟化

虚拟内存：虚拟内存是一种将物理内存抽象为虚拟内存的技术，可以让每个程序都拥有独立的虚拟内存空间。虚拟内存可以提高系统的内存利用率，允许多个程序同时运行，并且可以防止程序之间相互干扰。在Linux系统中，虚拟内存是通过页表映射实现的。

容器虚拟化：容器虚拟化是一种基于操作系统级别的虚拟化方式，可以在Linux系统上运行多个容器，每个容器都运行在独立的命名空间中，可以共享操作系统内核。容器虚拟化可以提高系统的资源利用率，并且可以快速部署和管理应用程序。例如，Docker就是一种基于容器的虚拟化技术，可以在Linux系统上运行多个容器，每个容器都运行一个应用程序。

KVM虚拟化：KVM是一种基于硬件虚拟化的全虚拟化技术，可以在Linux系统上运行多个虚拟机。KVM可以提供更好的隔离性和安全性，每个虚拟机都拥有独立的操作系统和应用程序。KVM可以在虚拟机中运行各种操作系统，例如Windows、Linux等。

虚拟文件系统：虚拟文件系统是一种将不同的文件系统抽象为统一的文件系统接口的技术，可以让用户在不同的文件系统中访问文件和目录。在Linux系统中，虚拟文件系统是通过VFS（Virtual File System）实现的，它可以让用户访问各种文件系统，例如EXT4、NTFS等。