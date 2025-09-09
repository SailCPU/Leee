# 服务机器人应用案例

## 概述

这是一个基于Leee平台的服务机器人应用案例，展示如何构建智能服务机器人系统，实现人机交互、自主导航和任务执行等功能。

## 功能特性

- 🤖 **智能导航**: 基于SLAM和路径规划的自主导航能力
- 👥 **人机交互**: 语音识别、自然语言处理和多模态交互
- 🎯 **任务执行**: 灵活的任务规划和执行框架
- 👁️ **环境感知**: 多传感器融合的环境理解和物体识别
- 📱 **远程控制**: 移动端和Web端的远程监控和控制

## 应用场景

### 酒店服务
- 客房送物服务
- 行李搬运
- 前台接待辅助
- 清洁路径规划

### 医疗辅助
- 药品配送
- 医疗器械搬运
- 病人陪伴
- 消毒巡逻

### 商业服务
- 商品导购
- 库存管理
- 客户引导
- 安全巡逻

## 快速开始

### 1. 环境准备

```bash
# 确保Leee平台已安装
cd /path/to/leee
./install.sh

# 进入应用目录
cd applications/service-robot
```

### 2. 配置机器人

```bash
# 编辑机器人配置文件
vim config/robot_config.yaml

# 配置导航参数
navigation:
  map_resolution: 0.05  # 5cm
  max_velocity: 1.0     # m/s
  safety_margin: 0.3    # m

# 配置语音交互
speech:
  language: "zh-CN"
  wake_word: "小智"
  tts_engine: "baidu"
```

### 3. 运行演示

```bash
# 编译应用
mkdir build && cd build
cmake ..
make

# 启动服务机器人
./service_robot_demo
```

## 应用架构

```
service-robot/
├── src/                 # 源代码
│   ├── navigation_system.cpp    # 导航系统
│   ├── speech_interface.cpp     # 语音接口
│   ├── task_manager.cpp         # 任务管理器
│   ├── human_interaction.cpp    # 人机交互
│   └── main.cpp                # 主程序
├── config/              # 配置文件
│   ├── robot_config.yaml       # 机器人配置
│   ├── navigation_config.yaml  # 导航配置
│   └── speech_config.yaml      # 语音配置
├── docs/                # 文档
│   ├── navigation_guide.md     # 导航指南
│   ├── speech_api.md          # 语音API
│   └── task_system.md         # 任务系统
└── CMakeLists.txt       # 构建配置
```

## 核心组件

### 导航系统

```cpp
#include <motion-control/navigation/navigation_system.h>
#include <perception/environment-mapping/slam_interface.h>

class NavigationSystem {
public:
    NavigationSystem();
    bool initialize(const std::string& config_file);
    bool navigateTo(const Eigen::Vector3d& target);
    bool avoidObstacle(const Obstacle& obstacle);
    NavigationStatus getStatus();

private:
    std::unique_ptr<leee::SLAMInterface> slam_;
    std::unique_ptr<leee::PathPlanner> path_planner_;
    std::unique_ptr<leee::MotionController> motion_ctrl_;
};
```

### 人机交互系统

```cpp
#include <human-interface/speech/speech_recognition.h>
#include <human-interface/nlp/natural_language_processor.h>

class HumanInteractionSystem {
public:
    HumanInteractionSystem();
    bool processVoiceCommand(const std::string& audio_data);
    std::string generateResponse(const std::string& user_input);
    void showEmotion(const std::string& emotion);

private:
    std::unique_ptr<leee::SpeechRecognition> speech_recognizer_;
    std::unique_ptr<leee::NLPProcessor> nlp_processor_;
    std::unique_ptr<leee::EmotionDisplay> emotion_display_;
};
```

### 任务管理系统

```cpp
#include <task-planning/task_scheduler.h>
#include <task-planning/goal_manager.h>

class TaskManager {
public:
    TaskManager();
    bool addTask(const Task& task);
    bool executeTask(const std::string& task_id);
    TaskStatus getTaskStatus(const std::string& task_id);
    std::vector<Task> getPendingTasks();

private:
    std::unique_ptr<leee::TaskScheduler> scheduler_;
    std::unique_ptr<leee::GoalManager> goal_manager_;
    std::map<std::string, Task> active_tasks_;
};
```

## 使用示例

### 基本导航任务

```cpp
// 创建导航系统
auto nav_system = std::make_unique<NavigationSystem>();
nav_system->initialize("config/navigation_config.yaml");

// 导航到指定位置
Eigen::Vector3d target(5.0, 3.0, 0.0);
if (nav_system->navigateTo(target)) {
    std::cout << "开始导航到目标位置" << std::endl;
}

// 监控导航状态
while (nav_system->getStatus().is_navigating) {
    auto status = nav_system->getStatus();
    std::cout << "当前位置: (" << status.position.x()
              << ", " << status.position.y() << ")" << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
}
```

### 语音交互

```cpp
// 创建人机交互系统
auto interaction = std::make_unique<HumanInteractionSystem>();

// 处理语音命令
std::string audio_data = captureAudio();
std::string command = interaction->processVoiceCommand(audio_data);

// 生成回复
std::string response = interaction->generateResponse(command);
speakResponse(response);

// 显示表情
interaction->showEmotion("happy");
```

### 任务执行

```cpp
// 创建任务管理器
auto task_mgr = std::make_unique<TaskManager>();

// 创建送物任务
Task delivery_task;
delivery_task.id = "delivery_001";
delivery_task.type = "item_delivery";
delivery_task.target_location = Eigen::Vector3d(10.0, 5.0, 0.0);
delivery_task.item_name = "room_service";

// 添加并执行任务
task_mgr->addTask(delivery_task);
task_mgr->executeTask("delivery_001");

// 监控任务状态
while (true) {
    auto status = task_mgr->getTaskStatus("delivery_001");
    if (status == TaskStatus::COMPLETED) {
        std::cout << "任务完成！" << std::endl;
        break;
    }
    std::this_thread::sleep_for(std::chrono::seconds(1));
}
```

## 传感器配置

### 多传感器融合

```yaml
sensors:
  lidar:
    type: "2d_lidar"
    model: "rplidar_a3"
    range: 12.0  # meters
    angle_resolution: 1.0  # degrees
    update_rate: 10  # Hz

  camera:
    type: "rgb_d_camera"
    model: "realsense_d435"
    resolution: [640, 480]
    fps: 30
    depth_range: [0.1, 10.0]

  ultrasonic:
    type: "ultrasonic_array"
    sensors:
      - position: [0.2, 0.0, 0.1]
        direction: [1.0, 0.0, 0.0]
        range: [0.02, 4.0]
      - position: [-0.2, 0.0, 0.1]
        direction: [-1.0, 0.0, 0.0]
        range: [0.02, 4.0]
```

## 安全特性

- 🛡️ **碰撞检测**: 多层次的碰撞检测和避障
- 🚨 **紧急停止**: 支持多种紧急停止触发方式
- 📢 **语音警告**: 异常情况的语音提示
- 🔄 **自动恢复**: 故障后的自动恢复机制
- 📊 **状态监控**: 实时的系统状态监控

## 性能指标

- **导航精度**: ±5cm
- **响应时间**: <500ms
- **续航时间**: 8小时
- **载重能力**: 20kg
- **移动速度**: 0-1.2 m/s

## 扩展功能

### 添加新的服务技能

```cpp
class CustomServiceSkill : public ServiceSkill {
public:
    CustomServiceSkill();
    bool execute(const ServiceRequest& request) override;
    std::string getSkillName() const override;
};
```

### 集成商业系统

```cpp
class ERPIntegration : public SystemIntegration {
public:
    ERPIntegration();
    bool connectToERP(const std::string& endpoint);
    bool syncInventory();
    bool submitServiceRequest(const ServiceRequest& request);
};
```

## 技术支持

如需技术支持，请：

1. 查看[导航系统文档](docs/navigation_guide.md)
2. 参考[语音交互API](docs/speech_api.md)
3. 查阅[任务系统指南](docs/task_system.md)
4. 提交问题到项目Issue
5. 联系技术支持团队

## 许可证

本应用案例遵循Leee平台的许可证协议。
