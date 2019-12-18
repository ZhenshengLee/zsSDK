# ZSA-一个应用框架

## 程序文件组织

完全仿照微软的A4K项目, A4K全部改成ZSA, 意思是zsCppApp

本还想参照px4的firware，可惜比A4K复杂太多，不想看

### submodule问题

由于网络状况不佳，不使用submodule，直接复制依赖软件的release版本

## git问题

每次开机都要执行一次？双网卡问题？

```shell
ssh-add ~/.ssh/github
ssh -T git@github.com
```

## 打造基于多个传感器的算法服务

参考azure-kinect软件体系结构，打造基于astra和rplidar的算法服务，实现基于视觉的定位功能

- 多传感器的驱动和示例程序
- 删除多余代码
- 多线程是怎么走起来的
- 硬件换成astra相机
- 如何调试
- 怎么加入pcl处理
- 原型设计
- ROS驱动封装

## 通信中间件

采用fast-rtps

## 应用

### 简单rtps应用

一个采集传感器数据（cpu利用率，采集四个核），其余应用订阅该消息并显示出来，用key表示是哪一个核
