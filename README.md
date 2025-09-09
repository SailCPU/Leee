# Leee - 智能机器人控制系统平台

[![License](https://img.shields.io/badge/License-Unlicense-blue.svg)](LICENSE)
[![AI-Powered](https://img.shields.io/badge/AI--Powered-Yes-blue.svg)]()
[![Cloud-Native](https://img.shields.io/badge/Cloud--Native-Yes-green.svg)]()
[![Industry-4.0](https://img.shields.io/badge/Industry--4.0-Ready-orange.svg)]()

Leee是专为**智能制造和机器人应用**打造的控制系统平台，专注于**机器人运动控制**、**智能感知**、**任务规划**和**人机协作**四大核心能力。通过AI驱动的开发模式，提供从原型设计到生产部署的完整解决方案。

## ⚡ 立即体验

**下载项目后，运行以下命令开始您的Leee之旅：**

```bash
# 项目概览（推荐先运行）
./project-overview.sh

# 一键安装
./install.sh

# 功能演示
./demo.sh --full

# 快速上手指南
cat GETTING_STARTED.md
```

## 🤖 AI驱动开发理念

Leee项目开创性地采用**AI Agent主导开发**的新模式：

- 🎯 **AI Agent优先**: AI Agent负责代码生成、测试编写和优化
- 🚀 **智能协作**: 人机协同，优势互补
- 📊 **数据驱动**: 基于反馈的持续优化
- 🔄 **敏捷迭代**: 快速响应需求变化

## ✨ 核心能力

### 🤖 运动控制 (Motion Control)
- 🎯 **精密定位**: 亚毫米级定位精度，支持多轴协同运动
- 📈 **轨迹规划**: 智能路径规划，优化运动效率和安全性
- 🎛️ **伺服控制**: 高级控制算法，支持力控和位置混合控制
- ⚡ **实时性能**: 微秒级响应，支持高速运动控制
- 🔄 **自适应调节**: 动态参数调整，适应不同负载条件

### 👁️ 智能感知 (Intelligent Perception)
- 📊 **状态监控**: 实时关节状态、力矩、温度监测
- 🎯 **位置感知**: 高精度位置和姿态检测
- 🔍 **环境感知**: 传感器数据融合和环境理解
- 📈 **性能分析**: 运动性能评估和优化建议
- ⚠️ **异常检测**: 智能故障诊断和预警机制

### 🎯 任务规划 (Task Planning)
- 📋 **任务编排**: 可视化任务流程设计和执行
- 🔄 **路径优化**: 多目标路径规划和避障算法
- 🎪 **场景适配**: 灵活的任务模板和参数配置
- 📊 **效率优化**: 自动化生产节拍优化
- 🔄 **动态调度**: 实时任务优先级调整和资源分配

### 👥 人机协作 (Human-Machine Collaboration)
- 🌐 **远程监控**: 云端实时监控和数据分析
- 🎮 **直观控制**: 图形化控制界面，支持远程操作
- 📱 **移动访问**: 响应式设计，支持移动设备管理
- 📊 **数据可视化**: 实时图表和3D可视化展示
- 🔗 **集成接口**: 标准API，支持第三方系统集成

### 🏭 智能制造 (Smart Manufacturing)
- 🔄 **生产线集成**: 标准工业协议支持，易于集成
- 📈 **生产优化**: 数据驱动的生产效率提升
- 🔧 **维护预测**: 基于数据的设备维护预测
- 📋 **质量控制**: 自动化质量检测和过程控制
- 📊 **生产分析**: 详细的生产数据分析和报告

### 🚀 平台特性
- 🌐 **云原生架构**: 支持云端部署和边缘计算
- 🔧 **即插即用**: 模块化设计，快速部署验证
- 📊 **数据驱动**: 全面的数据采集和分析能力
- 🔄 **持续进化**: AI驱动的系统优化和功能扩展
- 🛡️ **企业级安全**: 工业级安全标准和数据保护

## 📚 主要组件

### 功能模块

| 能力域 | 功能模块 | 核心价值 | 状态 |
|--------|----------|----------|------|
| **运动控制** | 精密伺服 | 亚毫米级定位，实时运动控制 | ✅ 稳定 |
| | 轨迹规划 | 智能路径优化，避障算法 | 🚧 开发中 |
| | 力控制 | 阻抗控制，柔性操作 | 🚧 开发中 |
| **智能感知** | 状态监控 | 实时状态检测，性能分析 | ✅ 稳定 |
| | 位置感知 | 高精度定位，姿态检测 | 🚧 开发中 |
| | 异常检测 | 智能诊断，故障预警 | 🚧 开发中 |
| **任务规划** | 任务编排 | 可视化流程设计 | 🚧 开发中 |
| | 路径优化 | 多目标规划，效率优化 | 🚧 开发中 |
| | 动态调度 | 实时任务调整 | 🚧 开发中 |
| **人机协作** | 远程监控 | 云端监控界面 | 🚧 开发中 |
| | 直观控制 | 图形化操作面板 | ✅ 稳定 |
| | 数据可视化 | 实时图表展示 | 🚧 开发中 |
| **系统管理** | 配置管理 | 参数服务器，动态配置 | ✅ 稳定 |
| | 日志系统 | 结构化日志，远程收集 | ✅ 稳定 |
| | 健康监控 | 系统状态监控 | 🚧 开发中 |

### 技术生态

Leee构建在成熟的技术生态之上，确保系统的稳定性、性能和扩展性：

| 技术领域 | 解决方案 | 核心价值 | 成熟度 |
|----------|----------|----------|--------|
| **数学计算** | Eigen数学库 | 高性能矩阵运算，运动学计算基础 | ⭐⭐⭐⭐⭐ |
| **网络通信** | Asio异步框架 | 实时通信协议，工业以太网支持 | ⭐⭐⭐⭐⭐ |
| **数据处理** | JSON处理库 | 配置管理，数据序列化 | ⭐⭐⭐⭐⭐ |
| **系统集成** | ROS生态 | 机器人操作系统标准接口 | ⭐⭐⭐⭐⭐ |
| **用户界面** | Web技术栈 | 跨平台图形界面，响应式设计 | ⭐⭐⭐⭐⭐ |
| **开发工具** | 现代化工具链 | 自动化测试，性能分析 | ⭐⭐⭐⭐⭐ |

## 👥 人机协作界面

Leee提供直观的人机协作界面，支持远程监控、实时控制和数据分析，让操作人员能够高效、安全地管理机器人系统。

### 🎯 核心功能



**实时监控面板**
- 📊 **状态总览**: 机器人运行状态、关键指标一目了然
- 🎯 **精密控制**: 直观的控制界面，支持精确操作
- 📈 **性能图表**: 实时数据可视化，趋势分析
- ⚠️ **智能预警**: 异常状态自动报警，及时响应
- 📱 **移动访问**: 支持手机、平板等移动设备

**远程操作中心**
- 🌐 **云端连接**: 安全的远程访问和控制
- 🎮 **直观操作**: 拖拽式界面设计，降低学习成本
- 🔄 **实时同步**: 毫秒级数据同步，确保操作准确性
- 📋 **任务管理**: 可视化任务编排和执行监控
- 🔐 **权限控制**: 分级权限管理，确保操作安全

**数据分析平台**
- 📊 **历史数据**: 完整的运行数据记录和分析
- 📈 **性能报告**: 自动化生成的生产效率报告
- 🔍 **故障分析**: 智能故障诊断和解决方案建议
- 📋 **优化建议**: 基于数据的系统优化建议

### 🚀 部署方式

**云端部署**
```bash
# 一键部署到云服务器
leee deploy --cloud --domain your-domain.com
```

**本地部署**
```bash
# 本地快速启动
leee start --local
```

**容器部署**
```bash
# Docker容器部署
docker run -p 8080:8080 leee/interface:latest
```

## 🚀 快速开始

### 系统要求

- **操作系统**: Windows 10+, Ubuntu 18.04+, macOS 10.15+
- **内存**: 至少4GB RAM，推荐8GB+
- **存储**: 至少10GB可用空间
- **网络**: 稳定的互联网连接（云端功能需要）
- **浏览器**: Chrome 80+, Firefox 75+, Safari 13+ (Web界面)

## 🚀 快速开始 - 5分钟上手

### 场景1: 我是机器人应用开发者

**如果您想快速开发机器人应用：**

```bash
# 方法1：一键安装（推荐）
git clone https://github.com/leee-project/leee.git
cd leee
./install.sh  # 交互式安装脚本

# 方法2：手动构建
mkdir build && cd build
cmake .. -DBUILD_MOTION_CONTROL=ON -DBUILD_HUMAN_INTERFACE=ON
make -j4

# 立即体验
./demo.sh --motion-control  # 运行运动控制演示
./demo.sh --full           # 运行完整功能演示
```

**5分钟内您就能体验：**
- 🤖 机器人运动学计算演示
- 🎯 轨迹规划可视化
- 🌐 Web监控界面预览
- 📊 功能模块概览

### 场景2: 我是系统集成工程师

**如果您想集成到现有生产线：**

```bash
# 选择您需要的功能模块
cmake .. \
  -DBUILD_INDUSTRIAL_INTEGRATION=ON \
  -DBUILD_SYSTEM_MANAGEMENT=ON \
  -DBUILD_HUMAN_INTERFACE=ON

# 构建并测试
make -j4
make verify-capabilities
```

**您将获得：**
- 🔗 标准工业协议支持
- 📊 生产线监控仪表板
- ⚙️ 设备状态管理界面

### 场景3: 我是机器人研究人员

**如果您想进行机器人算法研究：**

```bash
# 启用所有功能进行全面研究
cmake .. \
  -DBUILD_MOTION_CONTROL=ON \
  -DBUILD_PERCEPTION=ON \
  -DBUILD_TASK_PLANNING=ON \
  -DBUILD_DEVELOPMENT_TOOLS=ON

# 运行研究工具集
make functional-overview  # 查看所有可用功能
make run-examples         # 运行所有示例
```

**您将拥有：**
- 🧪 完整的算法实验环境
- 📈 性能分析和优化工具
- 🔬 研究级别的仿真支持

## 📖 详细使用指南

### 第一步：选择您的应用场景

Leee平台支持多种应用场景，请根据您的需求选择相应的功能模块：

| 应用场景 | 推荐功能模块 | 核心价值 |
|----------|-------------|----------|
| **工业机器人** | 运动控制 + 工业集成 + 系统管理 | 提高生产效率和设备可靠性 |
| **服务机器人** | 运动控制 + 感知 + 任务规划 + 人机界面 | 增强交互能力和环境适应性 |
| **协作机器人** | 运动控制 + 感知 + 力控制 + 人机界面 | 安全高效的人机协作 |
| **移动机器人** | 运动控制 + 感知 + 任务规划 + 导航 | 自主移动和路径规划 |
| **教育培训** | 运动控制 + 人机界面 + 开发工具 | 机器人技术学习和实验 |

### 第二步：定制您的构建配置

**基础配置（推荐新手使用）：**
```bash
cmake .. \
  -DCMAKE_BUILD_TYPE=Release \
  -DBUILD_MOTION_CONTROL=ON \
  -DBUILD_HUMAN_INTERFACE=ON \
  -DBUILD_SYSTEM_MANAGEMENT=ON
```

**高级配置（按需定制）：**
```bash
cmake .. \
  -DCMAKE_BUILD_TYPE=Release \
  -DBUILD_MOTION_CONTROL=ON \
  -DBUILD_PERCEPTION=ON \
  -DBUILD_TASK_PLANNING=ON \
  -DBUILD_HUMAN_INTERFACE=ON \
  -DBUILD_INDUSTRIAL_INTEGRATION=ON \
  -DBUILD_DEVELOPMENT_TOOLS=ON \
  -DBUILD_TESTING=ON
```

### 第三步：功能模块使用示例

#### 运动控制模块使用

```cpp
// 包含运动控制头文件
#include <motion-control/kinematics/kinematics_solver.h>
#include <motion-control/control/controller.h>

// 创建运动学求解器
auto kinematics = std::make_unique<leee::SixDOFManipulatorKinematics>(link_lengths);

// 正向运动学计算
Eigen::VectorXd joint_angles(6);
joint_angles << 0.1, 0.2, 0.0, 0.3, 0.0, 0.1;
auto pose = kinematics->forwardKinematics(joint_angles);

// PID控制器使用
auto pid_controller = std::make_unique<leee::PIDController>(kp, ki, kd);
auto control_output = pid_controller->computeControl(current_state, desired_state, dt);
```

#### 人机界面模块使用

```python
# 启动Web监控界面
from human_interface.web.app import LeeeWebInterface

app = LeeeWebInterface()
app.start_server(host='0.0.0.0', port=8080)

# 或者使用Python API
from human_interface.robotics.robot_kinematics import RobotKinematics

robot = RobotKinematics(dh_params)
trajectory = robot.plan_trajectory(start_pos, end_pos)
```

### 第四步：集成到您的项目

**CMake项目集成：**
```cmake
# 在您的CMakeLists.txt中
find_package(Leee REQUIRED)

# 链接所需的功能模块
target_link_libraries(your_app
    Leee::motion-control
    Leee::human-interface
    Leee::system-management
)
```

**Python项目集成：**
```python
# 在您的Python项目中
import sys
sys.path.append('/path/to/leee')

from motion_control.kinematics import KinematicsSolver
from human_interface.web import WebInterface
```

### 第五步：运行和监控

**启动Web监控界面：**
```bash
# 启动Web服务器
cd human-interface/web
python app.py

# 访问 http://localhost:8080 查看监控界面
```

**运行测试验证：**
```bash
# 运行所有测试
make run-tests

# 运行特定功能模块的测试
cd development-tools/test
./run-tests.sh motion-control
```

### 第六步：性能调优和扩展

**性能监控：**
```bash
# 查看系统性能指标
make performance-report

# 监控特定功能模块性能
./development-tools/scripts/monitor.sh motion-control
```

**功能扩展：**
```bash
# 添加新的功能模块
mkdir my-custom-module
# 按照功能导向原则设计模块

# 集成到构建系统中
# 编辑主CMakeLists.txt添加新模块
```

## 🎯 常见问题解答

### Q: 我应该选择哪些功能模块？
A: 根据您的具体应用场景选择：
- **初次使用**：建议从运动控制 + 人机界面开始
- **工业应用**：添加工业集成和系统管理
- **研究开发**：启用所有功能模块进行全面探索

### Q: 如何评估功能模块的效果？
A: 平台提供了多种验证方式：
- 运行示例程序验证基本功能
- 使用Web界面实时监控运行状态
- 查看性能报告分析系统表现
- 运行测试套件确保功能稳定性

### Q: 支持哪些编程语言？
A: 平台采用功能导向设计，技术实现透明化：
- **C++**：高性能核心算法实现
- **Python**：灵活的应用开发和集成
- **Web技术栈**：用户界面和监控功能

### Q: 如何获得技术支持？
A: 
- 📖 查看详细文档：`doc/` 目录
- 🧪 运行示例代码：`development-tools/examples/`
- 💬 社区讨论：GitHub Issues 和 Discussions
- 📧 技术咨询：项目维护者联系方式

### CMake集成

```cmake
# 添加Leee到项目
add_subdirectory(path/to/leee)

# 或者包含第三方库
include(path/to/leee/capability-foundations/third_party.cmake)

# 链接库
target_link_libraries(your_target PRIVATE leee)
```

## 📖 文档

- [API文档](doc/) - 详细的API参考文档
- [设计规范](design-specs/) - 架构设计和编码规范
- [示例代码](examples/) - 实际使用示例
- [测试报告](test/) - 完整的测试套件

## 📁 项目结构

```
Leee/
├── motion-control/         # 运动控制模块
│   ├── servo-control/     # 精密伺服控制
│   ├── trajectory-planning/ # 轨迹规划
│   ├── force-control/     # 力控制
│   ├── math/              # 数学运算库
│   ├── kinematics/        # 运动学计算
│   ├── dynamics/          # 动力学计算
│   └── control/           # 控制算法
├── perception/            # 智能感知模块
│   ├── state-monitoring/  # 状态监控
│   ├── position-sensing/  # 位置感知
│   └── anomaly-detection/ # 异常检测
├── task-planning/         # 任务规划模块
│   ├── task-orchestration/ # 任务编排
│   ├── path-optimization/ # 路径优化
│   └── dynamic-scheduling/ # 动态调度
├── human-interface/       # 人机协作界面
│   ├── remote-monitoring/ # 远程监控
│   ├── intuitive-control/ # 直观控制
│   ├── data-visualization/ # 数据可视化
│   ├── robotics/          # 机器人算法
│   ├── web/               # Web界面
│   └── utils/             # 界面工具
├── system-management/     # 系统管理模块
│   ├── configuration/     # 配置管理
│   ├── logging/           # 日志系统
│   └── health-monitoring/ # 健康监控
├── industrial-integration/ # 工业集成模块
│   ├── protocol-adapters/ # 协议适配器
│   ├── production-lines/  # 生产线集成
│   └── quality-control/   # 质量控制
├── development-tools/     # 开发工具
│   ├── simulation/        # 仿真工具
│   ├── testing/           # 测试工具
│   ├── debugging/         # 调试工具
│   ├── examples/          # 示例代码
│   └── scripts/           # 自动化脚本
├── capability-foundations/ # 能力基础技术库
├── build/                 # 构建输出目录
├── cmake-build-debug/     # 调试构建目录
├── design-specs/          # 设计规范文档
└── doc/                   # 文档生成文件
```

### 🏗️ 功能模块说明

| 功能域 | 核心价值 | 包含模块 |
|--------|----------|----------|
| **运动控制** | 实现机器人精准、高效的运动能力 | 伺服控制、轨迹规划、力控制、运动学计算 |
| **智能感知** | 实时感知机器人状态和环境 | 状态监控、位置感知、异常检测 |
| **任务规划** | 智能编排和优化机器人任务 | 任务编排、路径优化、动态调度 |
| **人机协作** | 提供直观易用的操作界面 | 远程监控、直观控制、数据可视化 |
| **系统管理** | 确保系统稳定可靠运行 | 配置管理、日志系统、健康监控 |
| **工业集成** | 无缝对接工业生产环境 | 协议适配、生产线集成、质量控制 |
| **开发工具** | 加速开发和验证过程 | 仿真测试、调试工具、示例代码 |

**💡 提示：运行 `./project-overview.sh` 获取更详细的项目状态和功能说明**

## 🛠️ 开发环境搭建

### 环境准备

**推荐配置**
- **操作系统**: Ubuntu 20.04+ 或 Windows 10+
- **内存**: 8GB RAM 以上
- **存储**: 20GB 可用空间
- **网络**: 稳定的互联网连接

### 快速开始

**1. 克隆项目**
```bash
git clone https://github.com/leee-project/leee.git
cd leee
```

**2. 一键启动**
```bash
# 使用开发工具自动配置环境
./development-tools/scripts/setup-dev-environment.sh

# 或者使用包管理器
pip install leee-dev-tools
leee setup --development
```

**3. 验证安装**
```bash
# 运行系统自检
leee check --all

# 启动开发界面
leee start --dev
```

### 功能模块开发

**运动控制开发**
```bash
# 进入运动控制模块
cd motion-control

# 运行模块测试
./run-tests.sh servo-control
```

**界面开发**
```bash
# 启动Web开发服务器
cd human-interface/web
npm run dev
# 或
python app.py --dev
```

## 🎯 智能开发模式

Leee采用**功能驱动、AI赋能**的创新开发模式，专注于业务价值而非技术实现：

### 🧠 智能开发理念

**功能优先，技术透明**
- 🎯 **需求导向**: 从机器人应用场景出发，定义功能需求
- 🔄 **快速原型**: AI辅助快速生成功能原型和验证方案
- 📊 **数据驱动**: 基于实际使用数据持续优化功能表现
- 👥 **协作开发**: 人机协作，人负责业务创新，AI负责技术实现

**质量保障体系**
- 🔍 **智能测试**: AI生成全面的功能测试用例和边界条件
- 📈 **性能监控**: 持续监控功能性能和用户体验指标
- 🛡️ **安全验证**: 自动化安全漏洞扫描和风险评估
- 📋 **文档生成**: AI自动生成功能说明和使用指南

### 🔄 持续交付流程

**敏捷开发模式**
- 📝 **功能规划**: 基于用户需求规划功能模块和优先级
- 🚀 **快速迭代**: 小步快跑，支持功能快速上线和反馈
- 🔧 **自动化部署**: 一键部署到测试、生产环境的完整流程
- 📊 **效果评估**: 量化功能效果，数据驱动的优化决策

**DevOps一体化**
- 🏗️ **环境统一**: 开发、测试、生产环境配置一致性
- 🔄 **持续集成**: 代码提交自动触发功能验证和集成测试
- 📦 **功能管理**: 功能模块的版本控制和依赖管理
- 📈 **监控告警**: 生产环境功能状态实时监控和异常告警

### 📋 功能生态标准

Leee选择技术组件的标准完全基于功能价值和业务需求：

**功能完备性**
- 🎯 **功能匹配**: 组件功能必须直接支持机器人应用场景
- 🔧 **集成便利**: 易于集成到现有功能模块中
- 📈 **扩展性**: 支持未来功能扩展和定制化需求
- 🔄 **兼容性**: 与现有功能模块无缝协作

**质量与可靠性**
- 🛡️ **稳定性**: 经过生产环境验证，具备高可用性
- 📊 **性能**: 满足机器人系统的实时性和精度要求
- 🔍 **可维护性**: 代码质量高，易于理解和维护
- 📋 **文档完善**: 详细的使用文档和示例代码

**业务价值导向**
- 💰 **成本效益**: 开源免费，降低总体拥有成本
- 🌍 **生态成熟**: 拥有活跃的社区和完善的支持体系
- 🔄 **持续更新**: 保持功能更新，适配新技术发展趋势
- 🎯 **应用验证**: 在机器人领域有成功应用案例

## 🤝 功能贡献

欢迎为Leee贡献功能！我们重视**业务价值贡献**，鼓励从用户需求出发的功能创新：

### 🎯 功能贡献指南

**需求驱动贡献**
1. **识别需求**: 发现机器人应用中的实际问题和需求
2. **功能设计**: 基于需求设计具体的功能解决方案
3. **原型验证**: 快速构建功能原型进行验证
4. **迭代优化**: 基于反馈数据持续优化功能表现

**贡献类型**
- 🔧 **功能增强**: 为现有模块添加新功能
- 🎯 **场景适配**: 针对特定应用场景定制功能
- 📊 **性能优化**: 提升功能性能和用户体验
- 🐛 **问题修复**: 解决实际使用中的功能问题
- 📚 **文档完善**: 改进功能说明和使用指南

### 📋 贡献流程

**1. 功能规划**
```bash
# 创建功能分支
git checkout -b feature/your-feature-name

# 功能需求文档
docs/features/your-feature.md
```

**2. 功能实现**
```bash
# 在相应功能模块中实现
# 例如：运动控制功能
motion-control/servo-control/
```

**3. 功能验证**
```bash
# 运行功能测试
./development-tools/testing/run-tests.sh --feature your-feature

# 性能基准测试
./development-tools/testing/benchmark.sh your-feature
```

**4. 提交贡献**
```bash
# 提交功能代码
git add .
git commit -m "feat: 添加[功能名称]功能

- 解决的问题：[描述]
- 实现的功能：[功能列表]
- 测试验证：[测试结果]"

# 创建Pull Request
git push origin feature/your-feature-name
```

### 🏆 贡献标准

**功能价值标准**
- 🎯 **用户价值**: 功能必须解决实际用户问题
- 📊 **可测量**: 功能效果可量化评估
- 🔧 **易用性**: 功能接口直观，易于集成使用
- 🛡️ **可靠性**: 功能稳定可靠，满足生产环境要求

## 📄 许可证

本项目采用 [Unlicense](LICENSE) 许可证 - 详见 [LICENSE](LICENSE) 文件。

**第三方库许可证**:
- 各第三方库保持其原始许可证
- 所有许可证都经过兼容性验证
- 详见各库的 `metadata.json` 文件

## 👥 作者

**Sail** - *项目维护者*  
📧 sailcpu@gmail.com  
🏢 个人项目

## 🙏 致谢

- 感谢所有贡献者的辛勤工作
- 感谢第三方库的维护者提供优秀工具
- 感谢开源社区的持续支持

## 📊 项目状态

[![Build Status](https://img.shields.io/badge/build-passing-brightgreen.svg)]()
[![Test Coverage](https://img.shields.io/badge/coverage-85%25-yellow.svg)]()
[![Documentation](https://img.shields.io/badge/docs-generated-blue.svg)]()

---

*"功能驱动，价值导向 - Leee致力于为机器人应用提供最实用的功能解决方案"*

## 🙌 致谢

- **机器人学研究者**: 感谢机器人学领域的先驱者们奠定了理论基础
- **AI技术的贡献者**: 感谢AI技术的进步使智能机器人开发成为可能
- **开源社区**: 感谢所有第三方库的维护者提供优秀工具
- **机器人开发者**: 感谢所有为项目贡献代码和机器人应用案例的开发者

⭐ 如果Leee帮助你构建了更好的机器人系统，请给它一个星标！

---

*🤖 本文档体现了Leee作为智能机器人控制系统开发平台的定位，由AI Agent辅助完善*
