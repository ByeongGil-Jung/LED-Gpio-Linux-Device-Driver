# LED-Gpio-Linux-Device-Driver

## 1. Command

(In /src directory)

### 1) Build the kernel source

```
make
```

### 2) Insert module & Make module

- Make sure that MAJOR_NUMBER (== 100) does not overlap with any other driver.

```
insmod my_device.ko
mknod /dev/my_device c 100 0
```

(In /run directory)

### 3) Build the application source

- Because it is based on the ARM board, it must be built using the ARM compiler.

```
arm-linux-gnueabihf-gcc run.c
```

## 2. Result

![result](https://github.com/ByeongGil-Jung/LED-Gpio-Linux-Device-Driver/blob/master/github/img/result.png)