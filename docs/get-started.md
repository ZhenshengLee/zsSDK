# 开始

## 构建

根据 `../scripts/bootstrap-ubuntu.sh`中的命令安装依赖项

```sh
mkdir build
cd ./build
cmake ..
```

若出现gradle构建错误，则先到`../extern/fastrtps/src/thirdparty/idl/`中执行

- 最终放弃使用java依赖项，fastrtpsgen也不用了

## 运行例子

```sh
# 由于fast-rtps没有安装到系统中，所以需要手动添加ldpath
source ./scripts/export-lib-path.sh
./build/bin/fasttrpsdemo1
```
