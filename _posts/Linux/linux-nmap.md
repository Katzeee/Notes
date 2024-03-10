#linux/tool
## 2022.6.1

## nmap

> https://zhuanlan.zhihu.com/p/343709343

- Scan specified ip address
  ```bash
  $ nmap 192.168.1.0/24
  ```
  Add `-vv` option to get detailed information.
  ```bash
  $ nmap -vv 192.168.1.0/24
  ```

- Scan specified ip address and port
  ```bash
  $ nmap -p1-22 192.168.1.0/24 # scan the 192.168.1.0/24 of the port number from 1 to 24
  $ nmap -p22,80 192.168.1.0/24 # scan the 192.168.1.0/24 of the port number 22 and 80 
  ```

- Detect the operating-system of the specified ip address
  ```bash
  $ nmap -O 192.168.1.1
  ```