# 一、安装 MSYS2

```
https://www.msys2.org/

```

# 二、安装 GTK4

```
GTK4 不支持UCRT64，这里要用非UCRT64的环境
echo $MSYSTEM 输出必须是MINGW64

pacman -S mingw-w64-x86_64-gtk4
pacman -S mingw-w64-x86_64-toolchain base-devel

# 如果下载出现问题 ，清除旧包
rm -rf /var/cache/pacman/pkg/*
# 刷新库
pacman -Syy
# 设置代理
export http_proxy=http://127.0.0.1:99
export https_proxy=http://127.0.0.1:99

# 查看是否安装完成
gcc --version
pkg-config --modversion gtk4
```

# 四、添加环境变量到 PATH

```
D:\Programs\msys64\usr\bin
D:\Programs\msys64\mingw64\bin
D:\Programs\msys64\ucrt64\bin
```
