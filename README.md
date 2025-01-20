# NJU-PA-2024

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

Environment：

OS：ubuntu 22.04

调试工具：GDB

编辑器：neovim

从十月底开始到期末周断断续续不到两个月时间，pa4剩下个尾巴

pa不仅带来了计算机系统的宏观视角，也指导了我们解决问题的方法论

做的有点赶，缺少了解决bug和思考问题的notes，工具的利用和code的实现都还有问题

这是一个值得二周目的硬核lab 希望学完os后带着新的视角来重新体验pa 
