# 协作机器人应用案例

## 概述

这是一个基于Leee平台的协作机器人应用案例，展示如何构建安全、高效的人机协作系统，实现力控操作、智能辅助和安全协作等功能。

## 功能特性

- 🤝 **安全协作**: 基于力传感器的安全协作控制
- 💪 **力控制**: 精确的力控和阻抗控制算法
- 👥 **智能辅助**: 学习用户意图的智能辅助功能
- 🛡️ **安全监控**: 多层次的安全监控和保护机制
- 🎯 **精确操作**: 亚毫米级定位精度和力控制精度

## 应用场景

### 装配作业
- 精密零件装配
- 力控拧紧操作
- 柔性物体操作
- 质量检测辅助

### 打磨抛光
- 自适应力控打磨
- 表面质量检测
- 路径规划优化
- 工艺参数调整

### 手术辅助
- 精确手术操作
- 力反馈控制
- 安全边界保护
- 术中导航

## 快速开始

### 1. 环境准备

```bash
# 确保Leee平台已安装
cd /path/to/leee
./install.sh

# 进入应用目录
cd applications/collaborative-robot
```

### 2. 配置协作机器人

```bash
# 编辑机器人配置文件
vim config/cobot_config.yaml

# 配置安全参数
safety:
  max_force: 50.0      # N
  max_torque: 10.0     # Nm
  safety_zones:
    - name: "human_area"
      radius: 0.5      # m

# 配置力控制参数
force_control:
  stiffness: [1000.0, 1000.0, 1000.0]  # N/m
  damping: [50.0, 50.0, 50.0]          # N/(m/s)
  force_limits: [20.0, 20.0, 20.0]     # N
```

### 3. 运行演示

```bash
# 编译应用
mkdir build && cd build
cmake ..
make

# 启动协作机器人
./collaborative_robot_demo
```

## 应用架构

```
collaborative-robot/
├── src/                 # 源代码
│   ├── force_controller.cpp      # 力控制器
│   ├── safety_monitor.cpp        # 安全监控器
│   ├── human_robot_interaction.cpp # 人机交互
│   ├── adaptive_control.cpp      # 自适应控制
│   └── main.cpp                 # 主程序
├── config/              # 配置文件
│   ├── cobot_config.yaml        # 协作机器人配置
│   ├── force_config.yaml        # 力控制配置
│   └── safety_config.yaml       # 安全配置
├── docs/                # 文档
│   ├── force_control_guide.md   # 力控制指南
│   ├── safety_manual.md         # 安全手册
│   └── programming_guide.md     # 编程指南
└── CMakeLists.txt       # 构建配置
```

## 核心组件

### 力控制系统

```cpp
#include <motion-control/force-control/force_controller.h>
#include <perception/force-sensing/force_sensor_interface.h>

class ForceControlSystem {
public:
    ForceControlSystem();
    bool initialize(const ForceControlConfig& config);
    bool setDesiredForce(const Eigen::Vector3d& force);
    bool setDesiredTorque(const Eigen::Vector3d& torque);
    ForceControlStatus getStatus();
    void updateControlLoop();

private:
    std::unique_ptr<leee::ForceController> force_controller_;
    std::unique_ptr<leee::ForceSensorInterface> force_sensor_;
    std::unique_ptr<leee::ImpedanceController> impedance_ctrl_;
    ForceControlConfig config_;
};
```

### 安全监控系统

```cpp
#include <system-management/safety/safety_monitor.h>
#include <perception/proximity-sensing/proximity_sensor.h>

class SafetyMonitor {
public:
    SafetyMonitor();
    bool initialize(const SafetyConfig& config);
    SafetyLevel assessSafetyLevel();
    bool detectHumanPresence();
    void triggerSafetyStop(SafetyEvent event);
    SafetyStatus getSafetyStatus();

private:
    std::unique_ptr<leee::ProximitySensor> proximity_sensor_;
    std::unique_ptr<leee::ForceSensor> force_sensor_;
    std::unique_ptr<leee::SafetyController> safety_controller_;
    SafetyConfig config_;
};
```

### 人机交互系统

```cpp
#include <human-interface/haptic-feedback/haptic_interface.h>
#include <task-planning/intent-recognition/intent_recognizer.h>

class HumanRobotInteraction {
public:
    HumanRobotInteraction();
    bool initialize();
    bool detectHumanIntent();
    void provideHapticFeedback(const Eigen::Vector3d& force);
    InteractionMode getCurrentMode();
    void switchMode(InteractionMode mode);

private:
    std::unique_ptr<leee::HapticInterface> haptic_interface_;
    std::unique_ptr<leee::IntentRecognizer> intent_recognizer_;
    std::unique_ptr<leee::GestureRecognition> gesture_recognizer_;
};
```

## 使用示例

### 力控制装配

```cpp
// 创建力控制系统
auto force_ctrl = std::make_unique<ForceControlSystem>();
force_ctrl->initialize(config);

// 设置期望力
Eigen::Vector3d desired_force(10.0, 0.0, 5.0);  // 10N X方向，5N Z方向
force_ctrl->setDesiredForce(desired_force);

// 执行力控制
while (assembly_in_progress) {
    force_ctrl->updateControlLoop();

    auto status = force_ctrl->getStatus();
    std::cout << "当前力: " << status.current_force.transpose() << std::endl;
    std::cout << "位置误差: " << status.position_error.transpose() << std::endl;

    std::this_thread::sleep_for(std::chrono::milliseconds(1));
}
```

### 安全协作

```cpp
// 创建安全监控系统
auto safety_monitor = std::make_unique<SafetyMonitor>();
safety_monitor->initialize(safety_config);

// 监控安全状态
while (collaboration_active) {
    auto safety_level = safety_monitor->assessSafetyLevel();

    switch (safety_level) {
        case SafetyLevel::SAFE:
            // 正常操作
            force_ctrl->setDesiredForce(desired_force);
            break;

        case SafetyLevel::WARNING:
            // 降低力
            force_ctrl->setDesiredForce(desired_force * 0.5);
            break;

        case SafetyLevel::DANGER:
            // 立即停止
            safety_monitor->triggerSafetyStop(SafetyEvent::HUMAN_CONTACT);
            break;
    }

    std::this_thread::sleep_for(std::chrono::milliseconds(10));
}
```

### 智能辅助

```cpp
// 创建人机交互系统
auto hri = std::make_unique<HumanRobotInteraction>();

// 检测用户意图
if (hri->detectHumanIntent()) {
    auto intent = hri->getCurrentMode();

    switch (intent) {
        case InteractionMode::GUIDED_LEAD:
            // 用户引导模式
            force_ctrl->enableGuidedMode();
            break;

        case InteractionMode::SHARED_CONTROL:
            // 共享控制模式
            force_ctrl->enableSharedControl();
            break;

        case InteractionMode::AUTONOMOUS:
            // 自主模式
            force_ctrl->enableAutonomousMode();
            break;
    }
}

// 提供力反馈
Eigen::Vector3d feedback_force = calculateFeedbackForce();
hri->provideHapticFeedback(feedback_force);
```

## 传感器配置

### 力传感器配置

```yaml
force_sensors:
  wrist_sensor:
    type: "6dof_force_torque"
    model: "ati_mini45"
    range:
      force: [-200.0, 200.0]    # N
      torque: [-10.0, 10.0]     # Nm
    resolution:
      force: 0.1                # N
      torque: 0.001             # Nm
    update_rate: 1000           # Hz

  finger_sensors:
    type: "tactile_array"
    model: "syn_touch"
    resolution: [16, 16]        # pixels
    sensitivity: 0.01           # N/pixel
    update_rate: 100            # Hz
```

### 接近传感器配置

```yaml
proximity_sensors:
  lidar:
    type: "2d_lidar"
    model: "rplidar_s2"
    range: 5.0                  # meters
    angle_resolution: 1.0       # degrees
    update_rate: 10             # Hz

  vision:
    type: "stereo_camera"
    model: "zed2"
    baseline: 0.12              # meters
    resolution: [1280, 720]
    fps: 15
    depth_range: [0.3, 5.0]     # meters
```

## 安全特性

- 🛡️ **力限控制**: 基于力传感器的动态力限制
- 🚨 **碰撞检测**: 多模态碰撞检测和预测
- 📢 **安全停止**: 分级安全停止机制
- 🔄 **自动恢复**: 安全事件后的自动恢复
- 📊 **实时监控**: 全面的安全状态监控

## 性能指标

- **力控制精度**: ±0.5N
- **位置精度**: ±0.1mm
- **响应时间**: <10ms
- **安全停止时间**: <100ms
- **连续工作时间**: 12小时

## 扩展功能

### 添加新的协作技能

```cpp
class CustomCollaborationSkill : public CollaborationSkill {
public:
    CustomCollaborationSkill();
    bool executeCollaborationTask(const Task& task) override;
    bool adaptToHumanBehavior(const HumanBehavior& behavior) override;
    std::string getSkillName() const override;
};
```

### 学习用户偏好

```cpp
class UserPreferenceLearning : public LearningSystem {
public:
    UserPreferenceLearning();
    bool learnFromInteraction(const InteractionData& data);
    bool predictUserIntent(const SensorData& sensor_data);
    void updateBehaviorModel(const UserFeedback& feedback);
};
```

## 技术支持

如需技术支持，请：

1. 查看[力控制指南](docs/force_control_guide.md)
2. 参考[安全手册](docs/safety_manual.md)
3. 查阅[编程指南](docs/programming_guide.md)
4. 提交问题到项目Issue
5. 联系技术支持团队

## 许可证

本应用案例遵循Leee平台的许可证协议。
