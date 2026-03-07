# SmartLock

基于 CH32V30x RISC-V MCU 的嵌入式智能锁项目。

## 项目结构总览

| Demo | 功能模块 | 说明 |
|------|----------|------|
| **demo1** | 基础工程搭建 | CH32V30x 最小系统，GPIO 基本操作 |
| **demo2** | + LCD 显示驱动 | 集成 LCD 屏幕显示，字库与图片资源 |
| **demo3** | + 按键输入 + 定时器 | 矩阵键盘扫描，定时器中断管理 |
| **demo4** | + 音频播放 + UART 通信 | 语音提示功能，串口数据通信 |
| **demo5** | + AS608 指纹识别模块 | 指纹录入、识别与管理 |

## 各模块目录说明

```
demoX/
├── Core/           # RISC-V 内核相关文件
├── Debug/          # 调试支持
├── Driver/         # 外设驱动（LCD、按键、音频、指纹等）
├── Ld/             # 链接脚本
├── Peripheral/     # CH32V30x 标准外设库
│   ├── inc/        # 外设库头文件
│   └── src/        # 外设库源文件
├── Startup/        # 启动文件
└── User/           # 用户应用代码（main.c 等）
```

## 开发环境

- **MCU**: CH32V30x (RISC-V)
- **IDE**: MounRiver Studio
