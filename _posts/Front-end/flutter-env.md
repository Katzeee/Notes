#flutter #code-env 
## 2023.06.11

# Download flutter

> https://flutter.cn/docs/get-started/install/windows

- git

  need install git first

- unzip

  download the zip file then unzip it

  set env var(windows use GUI)

  ```bash
  $ export PATH=flutter\bin:$PATH
  $ export FLUTTER_STORAGE_BASE_URL=https://mirrors.tuna.tsinghua.edu.cn/flutter
  $ export PUB_HOSTED_URL=https://mirrors.tuna.tsinghua.edu.cn/dart-pub
  $ export FLUTTER_GIT_URL=https://mirrors.tuna.tsinghua.edu.cn/git/flutter-sdk.git
  ```

  `flutter\packages\flutter_tools\gradle\flutter.gradle`

- change the source

```java
buildscript {
    repositories {
        // google()
        // jcenter()
        maven { url 'https://maven.aliyun.com/repository/google' }
        maven { url 'https://download.flutter.io'}
        maven { url 'https://maven.aliyun.com/repository/gradle-plugin' }
        maven { url 'https://maven.aliyun.com/repository/jcenter' }
        maven { url 'https://maven.aliyun.com/repository/public' }
    }
    dependencies {
        classpath 'com.android.tools.build:gradle:3.5.0'
    }
}
...
class FlutterPlugin implements Plugin<Project> {
	// 此处也要修改其值
    private static final String MAVEN_REPO = "https://storage.flutter-io.cn/download.flutter.io";
...
```

# android sdk

I saved it in my quake drive.

```bash
$ export PATH=$PATH:C:\android-sdk\platform-tools
$ flutter doctor --android-licenses
```

# VS

Install `Desktop development with C++` package. (need windows sdk)

# Download JDK

> https://www.oracle.com/cn/java/technologies/downloads/#jdk17-windows

set env var(windows use GUI)

```bash
$ export JAVA_HOME=D:\Java\jdk-19.0.1
```

# scrcpy(for mirroring phone to computer)

# post settings and check

```bash
$ flutter doctor # check the installation
```

# For new project

You may fail to run the project on an android phone, try as follow

```java
    repositories {
        // google()
        // jcenter()
        maven { url 'https://maven.aliyun.com/repository/google' }
        maven { url 'https://maven.aliyun.com/repository/gradle-plugin' }
        maven { url 'https://download.flutter.io'}
        maven { url 'https://maven.aliyun.com/repository/jcenter' }
        maven { url 'https://maven.aliyun.com/repository/public' }
    }

    dependencies {
        classpath 'com.android.tools.build:gradle:3.5.0'
        classpath "org.jetbrains.kotlin:kotlin-gradle-plugin:$kotlin_version"
    }
}

allprojects {
    repositories {
        // google()
        // jcenter()
        maven { url 'https://maven.aliyun.com/repository/google' }
        maven { url 'https://maven.aliyun.com/repository/gradle-plugin' }
        maven { url 'https://download.flutter.io'}
        maven { url 'https://maven.aliyun.com/repository/jcenter' }
        maven { url 'https://maven.aliyun.com/repository/public' }
    }
}
...
```

or open `\flutter_application_test\android\gradle\wrapper`

download `distributionUrl=https\://services.gradle.org/distributions/gradle-7.5-all.zip` then change it to `distributionUrl=file\:///D:/Downloads/Compressed/gradle-7.5-all.zip`