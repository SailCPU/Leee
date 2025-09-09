# 🚀 Leee 平台快速上手指南

## 您好，欢迎使用 Leee 智能机器人控制系统平台！

这个指南将帮助您在**5分钟内**开始使用Leee平台。无论您是机器人开发者、系统集成工程师还是研究人员，都能快速上手。

---

## 📋 目录

1. [快速预览](#快速预览)
2. [下载和安装](#下载和安装)
3. [选择您的使用场景](#选择您的使用场景)
4. [运行第一个示例](#运行第一个示例)
5. [访问Web界面](#访问Web界面)
6. [下一步学习](#下一步学习)

---

## 快速预览

Leee是一个**功能驱动**的机器人控制平台，专注于：

- 🤖 **运动控制**：精密伺服控制、轨迹规划
- 👁️ **智能感知**：状态监控、位置感知
- 🎯 **任务规划**：智能编排、路径优化
- 👥 **人机协作**：直观界面、远程监控
- 🏭 **工业集成**：标准协议、生产线连接

**核心优势：**
- 🎯 功能导向，需求驱动
- 🔧 模块化设计，按需选择
- 🌐 Web界面，开箱即用
- 📊 数据驱动，持续优化

---

## 下载和安装

### 方法1：Git克隆（推荐）

```bash
# 克隆项目
git clone https://github.com/leee-project/leee.git
cd leee

# 查看项目结构
ls -la
```

### 方法2：下载压缩包

```bash
# 下载最新版本
wget https://github.com/leee-project/leee/archive/main.zip
unzip main.zip
cd leee-main
```

---

## 选择您的使用场景

**请告诉我您的应用场景，我会为您定制配置：**

### 🎯 场景1：机器人应用开发
**适用人群：** 机器人软件开发者、算法工程师
**推荐配置：** 运动控制 + 人机界面 + 开发工具

### 🏭 场景2：工业自动化集成
**适用人群：** 系统集成工程师、生产线管理人员
**推荐配置：** 工业集成 + 系统管理 + 运动控制

### 🔬 场景3：机器人算法研究
**适用人群：** 研究人员、算法科学家
**推荐配置：** 全部功能模块 + 开发工具 + 测试框架

### 🤖 场景4：教育与培训
**适用人群：** 学生、教师、培训机构
**推荐配置：** 运动控制 + 人机界面 + 示例代码

---

## 运行第一个示例

让我们从一个简单的运动学计算示例开始：

### 步骤1：构建项目

```bash
# 创建构建目录
mkdir build
cd build

# 基础配置构建（推荐初次使用）
cmake .. \
  -DCMAKE_BUILD_TYPE=Release \
  -DBUILD_MOTION_CONTROL=ON \
  -DBUILD_HUMAN_INTERFACE=ON \
  -DBUILD_DEVELOPMENT_TOOLS=ON

# 编译
make -j4
```

### 步骤2：运行运动学示例

```bash
# 运行机器人运动学演示
./development-tools/examples/cpp/robot_kinematics_example
```

**您将看到：**
```
Leee 机器人运动学示例
====================
关节角度: 5.7° 11.5° 0.0° 17.2° 0.0° 5.7°
末端位置: [0.345, 0.123, 0.456] m
姿态角度: [2.1°, 5.3°, -1.2°]
雅可比矩阵条件数: 15.3
```

### 步骤3：运行Python示例

```bash
# 运行Python轨迹规划示例
cd ../development-tools/examples/python
python robot_control_example.py
```

---

## 访问Web界面

Leee提供了直观的Web监控界面，让您可以实时查看和控制机器人：

### 启动Web服务

```bash
# 启动Web服务器
cd human-interface/web
python app.py
```

### 访问界面

打开浏览器访问：**http://localhost:8080**

**界面功能：**
- 📊 **实时状态监控**：关节角度、末端位置、力矩数据
- 🎮 **远程控制面板**：直观的机器人控制界面
- 📈 **数据可视化**：图表展示和性能曲线
- ⚙️ **系统配置**：参数调整和功能设置
- 📝 **日志查看**：系统运行日志和错误信息

---

## 🎯 立即体验不同功能

### 功能模块速览

| 功能模块 | 立即体验命令 | 核心价值 |
|----------|-------------|----------|
| **运动控制** | `./examples/cpp/robot_kinematics_example` | 机器人精准运动控制 |
| **人机界面** | `cd human-interface/web && python app.py` | 直观监控和控制 |
| **任务规划** | `./examples/robotics/pick_and_place_demo.py` | 智能任务编排 |
| **系统管理** | `make run-tests` | 系统健康监控 |

### 快速功能验证

```bash
# 查看所有可用功能
make functional-overview

# 运行所有示例
make run-examples

# 验证构建完整性
make verify-capabilities
```

---

## 📈 下一步学习路径

### 🏆 入门级（推荐新手）

1. ✅ **完成基本安装和构建**
2. 📖 **阅读核心概念**：`README.md` 的"核心能力"部分
3. 🧪 **运行所有示例**：了解平台功能范围
4. 🎯 **选择应用场景**：确定您的具体使用需求

### 🚀 中级进阶

1. 📚 **深入学习**：查看`design-specs/`目录的架构文档
2. 🔧 **定制构建**：根据需求选择功能模块
3. 💻 **开发集成**：学习如何集成到您的项目
4. 📊 **性能调优**：优化系统性能和资源使用

### 🎓 高级应用

1. 🏗️ **架构扩展**：学习如何添加新的功能模块
2. 🔬 **算法定制**：定制专用的控制算法
3. 🌐 **系统集成**：集成到现有工业系统中
4. 📈 **性能优化**：高级性能分析和优化

---

## 💡 实用技巧

### 构建优化

```bash
# 快速构建（仅核心功能）
cmake .. -DBUILD_MOTION_CONTROL=ON -DBUILD_HUMAN_INTERFACE=ON

# 完整构建（所有功能）
cmake .. -DBUILD_MOTION_CONTROL=ON -DBUILD_PERCEPTION=ON \
         -DBUILD_TASK_PLANNING=ON -DBUILD_HUMAN_INTERFACE=ON \
         -DBUILD_SYSTEM_MANAGEMENT=ON -DBUILD_INDUSTRIAL_INTEGRATION=ON

# 调试构建
cmake .. -DCMAKE_BUILD_TYPE=Debug -DBUILD_DEVELOPMENT_TOOLS=ON
```

### 问题排查

```bash
# 查看构建状态
make functional-overview

# 运行测试验证
make run-tests

# 查看详细日志
tail -f logs/leee.log
```

---

## 🎉 恭喜！

您已经成功开始了Leee平台的旅程！

**现在您可以：**
- ✅ 运行机器人运动学计算
- ✅ 访问Web监控界面
- ✅ 理解功能导向的设计理念
- ✅ 选择适合您的应用场景

**接下来建议：**
1. 根据您的具体需求定制功能模块
2. 深入学习相关功能模块的文档
3. 开始您的第一个机器人应用开发

---

## 📞 获取帮助

遇到问题？我们随时为您提供帮助：

- 📖 **详细文档**：`README.md` 和 `doc/` 目录
- 🧪 **示例代码**：`development-tools/examples/` 目录
- 💬 **社区支持**：GitHub Issues 和 Discussions
- 📧 **技术咨询**：项目维护者联系方式

**祝您使用愉快！🎯**
