2022.09.13
---

# INSTALL MYSQL ON ARCH LINUX

## Install

- Download

    ```bash
    $ yay -S mysql57
    ```

    After a very long period waiting for make, it will be installed successfully.

## Configuration

- Log file

    If mysql doesn't configure its own log, you should configure for it.

    ```bash
    $ sudo mkdir -p /var/log/mysql
    $ sudo chown mysql:mysql /var/log/mysql
    $ cd /var/log/mysql
    $ touch mysql-error.log
    ```

    Then add these to `my.cnf` which maybe located at `/etc/mysql`

    ```
    [mysqld]
    log-error=/var/log/mysql/mysql-error.log
    ```

- Initialize

    Before you can use mysql, there still are some necessary initialization need to be done.

    ```bash
    $ sudo mysqld -- initialize --user=mysql --basedir=/usr --datadir=/var/lib/mysql
    ```

    The last line of the output will like this:

    ```
    root@localhost: MA..2w8,_cKl 
    ```

    which means the password of the root user on localhost is `MA..2w8,_cKl`.

- Start service

    Now you should start the mysql service.

    ```bash
    $ systemctl enable mysqld # make mysql auto-start on boot
    $ systemctl start mysqld # start mysql
    $ system status mysqld # check the status
    ```

    Maybe you will get some error, you can inspect the details in the log file. 

    I got an error like `Can't create/write to file '/var/run/mysqld/mysqld.pid' (Errcode: 2 - No such file or directory)`.

    Solution:

    Just create the file or folder manually and change the group and user of it.

    ```bash
    $ mkdir -p /var/run/mysqld
    $ sudo chown mysql:mysql /var/run/mysqld
    ```