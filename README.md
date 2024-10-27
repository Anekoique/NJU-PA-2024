# NJU-PA-2024

As an off-campus member, my own realization of NJU-ICS-PA 2024.

课程名称：南京大学 计算机系统基础 课程实验

课程主页：https://nju-projectn.github.io/ics-pa-gitbook/ics2024/index.html

课程目标：实现NEMU, 一款经过简化的全系统模拟器

master分支为完整项目实现，pas 分支为各 pa 任务的阶段实现

PA包括一个准备实验(配置实验环境)以及5部分连贯的实验内容:

- 图灵机与简易调试器
- 冯诺依曼计算机系统
- 批处理系统
- 分时多任务
- 程序性能优化

## 整体框架

```
NJU-PA-2024
├── abstract-machine   # 抽象计算机
├── am-kernels         # 基于抽象计算机开发的应用程序
├── fceux-am           # 红白机模拟器
├── init.sh            # 初始化脚本
├── Makefile           # 用于工程打包提交
├── nemu               # NEMU
├── PA_README.md       # 原始 README
└── README.md
```

## PA0 开发环境配置

target：基础环境配置, 包括系统安装+换源+基础工具安装与学习......

本人的基础环境：

OS：ubuntu 22.04

调试工具：GDB

编辑器：neovim

## PA1 最简单的计算机

target：

