#code-env/database
## 2023.11.07

## Basic commands

- Connect database

```bash
$ mysql -u root -p
$ mysql -p
```

- Query databases and tables

```sql
show databases;
use table_name;
show tables;
```

- Add user

```sql
CREATE USER 'newuser'@'localhost' IDENTIFIED BY 'password';
```

!!! Note `localhost'` means this user can only connect on localhost. If you want this user to connect from other host, you can use `%`

- Grant privileges

```sql
GRANT ALL PRIVILEGES ON database_name.* TO 'newuser'@'localhost';
GRANT ALL PRIVILEGES ON *.* TO 'newuser'@'localhost';
FLUSH PRIVILEGES; -- make change work
```

!!! Note `database_name.*` means all tables under database_name

- Delete User 

```sql
DROP USER 'olduser'@'oldhost';
```

- Protocol url

```
mysql://username:password@localhost/database
```

- Detailed describe

```sql
show full columns from post;
```
