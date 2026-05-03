# StockTraderClient 股票交易客户端

## 项目介绍

StockTraderClient 是一个基于 Qt 框架开发的股票交易客户端应用，提供实时股票行情查看、交易操作、个人信息管理等功能。

## 功能特性

### 核心功能
- **行情查看**：实时股票报价、K线图分析、公司信息查看
- **交易功能**：股票买卖、订单管理、持仓查看
- **个人中心**：用户信息管理、账户资产查看、交易历史记录
- **用户管理**：支持登录/注册、个人信息编辑（包括用户名修改）

### 新增功能
- **实时行情功能**：实时价格显示、买卖盘口、多股票监控、市场指数、板块行情
- **历史数据功能**：K线图（支持多种周期）、交互式图表、成交量图、历史成交明细
- **基本面信息**：公司基本信息、财务数据、股本结构、公司公告
- **个人中心功能**：个人信息标签页、资产信息标签页、交易统计标签页、资金流水标签页

## 技术栈

- **开发框架**：Qt 6.9.1
- **编程语言**：C++
- **数据库**：SQL
- **网络通信**：Qt Network

## 项目结构

```
StockTraderClient/
├── head/            # 头文件
├── source/          # 源代码
├── ui/              # UI界面文件
├── build/           # 编译输出目录
├── resource/        # 资源文件
├── StockTraderClient.pro  # 项目配置文件
└── README.md        # 项目说明文件
```

## 主要模块

- **Client**：客户端核心类，负责与服务器通信
- **DataService**：数据服务类，处理市场数据
- **UserService**：用户服务类，处理用户相关操作
- **MainWindow**：主窗口，集成各功能模块
- **QuoteWidget**：行情报价组件
- **KLineWidget**：K线图组件
- **CompanyWidget**：公司信息组件
- **UserInfoWidget**：用户信息组件
- **EditUserInfoDialog**：用户信息编辑对话框

## 安装与运行

### 环境要求

- Qt 6.9.1 或更高版本
- C++17 兼容的编译器
- 支持的操作系统：Windows、Linux、macOS

### 编译步骤

1. 打开 Qt Creator，选择 "打开项目"，导航到 `StockTraderClient.pro` 文件
2. 选择合适的构建配置（Debug 或 Release）
3. 点击 "构建" 按钮编译项目
4. 编译完成后，运行应用程序

### 直接运行


### 数据模型
- `QuoteData`：实时行情数据结构
- `KLineData`：K线数据结构
- `CompanyInfo`：公司信息数据结构
- `IndexData`：指数数据结构
- `SectorData`：板块数据结构
- `UserInfo`：用户基本信息
- `AccountInfo`：账户资产信息
- `CashFlowRecord`：资金流水记录
- `TradingStats`：交易统计信息

### 核心组件
- `DataService`：数据服务类，负责数据生成和管理
- `QuoteWidget`：实时行情显示组件
- `KLineWidget`：K线图绘制组件
- `CompanyWidget`：公司信息显示组件
- `UserService`：用户数据管理服务
- `UserInfoWidget`：个人中心界面组件
- `EditUserInfoDialog`：用户信息编辑对话框

### 模拟数据
当前版本使用模拟数据，包括：
- 随机生成的股票价格和成交量
- 模拟的K线数据
- 虚拟的公司财务信息
- 模拟的市场指数和板块数据



