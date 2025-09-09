# 基础机器人应用模板

## 概述

这是一个基础的机器人应用开发模板，提供了Leee平台的基本框架和示例代码，帮助开发者快速上手机器人应用开发。

## 模板特性

- 🏗️ **标准项目结构**: 符合Leee平台规范的项目布局
- 📝 **示例代码**: 包含完整的运动控制和感知示例
- ⚙️ **配置模板**: 标准的配置文件模板
- 📚 **开发文档**: 详细的开发指南和API说明
- 🧪 **测试框架**: 基础的单元测试和集成测试

## 快速开始

### 1. 复制模板

```bash
# 从模板创建新项目
cp -r workspace/templates/basic-robot-app my-robot-app
cd my-robot-app

# 重命名项目
sed -i 's/basic_robot_app/my_robot_app/g' CMakeLists.txt
sed -i 's/BasicRobotApp/MyRobotApp/g' CMakeLists.txt
```

### 2. 配置项目

```bash
# 编辑项目配置
vim config/robot_config.yaml

# 修改机器人参数
robot:
  name: "my_custom_robot"
  type: "6dof_manipulator"
  workspace: "my_workspace"

# 调整运动参数
motion:
  max_velocity: 1.0      # m/s
  max_acceleration: 2.0  # m/s²
  position_tolerance: 0.001  # m
```

### 3. 构建和运行

```bash
# 创建构建目录
mkdir build && cd build

# 配置构建
cmake .. -DCMAKE_BUILD_TYPE=Release

# 编译项目
make -j4

# 运行应用
./basic_robot_app
```

## 项目结构

```
basic-robot-app/
├── CMakeLists.txt           # 构建配置
├── src/                     # 源代码目录
│   ├── main.cpp            # 主程序入口
│   ├── robot_controller.h  # 机器人控制器头文件
│   ├── robot_controller.cpp # 机器人控制器实现
│   ├── motion_planner.h    # 运动规划器头文件
│   ├── motion_planner.cpp  # 运动规划器实现
│   └── sensor_interface.h  # 传感器接口头文件
├── config/                  # 配置文件目录
│   ├── robot_config.yaml   # 机器人配置
│   ├── motion_config.yaml  # 运动配置
│   └── sensor_config.yaml  # 传感器配置
├── test/                    # 测试目录
│   ├── CMakeLists.txt      # 测试构建配置
│   ├── test_main.cpp       # 测试主文件
│   ├── test_robot_controller.cpp # 控制器测试
│   └── test_motion_planner.cpp   # 规划器测试
├── docs/                    # 文档目录
│   ├── api_reference.md    # API参考
│   ├── configuration_guide.md # 配置指南
│   └── development_notes.md   # 开发笔记
└── scripts/                 # 脚本目录
    ├── build.sh           # 构建脚本
    ├── run.sh            # 运行脚本
    └── test.sh           # 测试脚本
```

## 核心组件

### 机器人控制器

```cpp
#include <memory>
#include <string>
#include <Eigen/Dense>

#include <motion-control/kinematics/kinematics_solver.h>
#include <perception/state-monitoring/sensor_interface.h>

/**
 * @brief 机器人控制器类
 */
class RobotController {
public:
    /**
     * @brief 构造函数
     */
    RobotController();

    /**
     * @brief 析构函数
     */
    ~RobotController();

    /**
     * @brief 初始化控制器
     * @param config_file 配置文件路径
     * @return 初始化是否成功
     */
    bool initialize(const std::string& config_file);

    /**
     * @brief 移动到指定位置
     * @param target 目标位置
     * @return 移动是否成功
     */
    bool moveToPosition(const Eigen::Vector3d& target);

    /**
     * @brief 执行轨迹
     * @param trajectory 轨迹点序列
     * @return 执行是否成功
     */
    bool executeTrajectory(const std::vector<Eigen::Vector3d>& trajectory);

    /**
     * @brief 获取当前状态
     * @return 机器人当前状态
     */
    RobotState getCurrentState();

    /**
     * @brief 停止机器人
     */
    void stop();

private:
    std::unique_ptr<leee::KinematicsSolver> kinematics_solver_;
    std::unique_ptr<leee::MotionPlanner> motion_planner_;
    std::unique_ptr<leee::SensorInterface> sensor_interface_;

    RobotState current_state_;
    bool is_initialized_;
};
```

### 运动规划器

```cpp
#include <vector>
#include <Eigen/Dense>

#include <motion-control/trajectory-planning/trajectory_planner.h>

/**
 * @brief 运动规划器类
 */
class MotionPlanner {
public:
    /**
     * @brief 规划点到点轨迹
     * @param start 起始位置
     * @param end 结束位置
     * @return 轨迹点序列
     */
    std::vector<Eigen::Vector3d> planPointToPointTrajectory(
        const Eigen::Vector3d& start,
        const Eigen::Vector3d& end
    );

    /**
     * @brief 规划关节空间轨迹
     * @param start_joints 起始关节角度
     * @param end_joints 结束关节角度
     * @return 关节角度序列
     */
    std::vector<Eigen::VectorXd> planJointTrajectory(
        const Eigen::VectorXd& start_joints,
        const Eigen::VectorXd& end_joints
    );

    /**
     * @brief 检查轨迹可行性
     * @param trajectory 待检查轨迹
     * @return 轨迹是否可行
     */
    bool isTrajectoryFeasible(const std::vector<Eigen::Vector3d>& trajectory);

private:
    std::unique_ptr<leee::TrajectoryPlanner> trajectory_planner_;
    MotionConfig config_;
};
```

## 使用示例

### 基本运动控制

```cpp
#include "robot_controller.h"

int main() {
    // 创建机器人控制器
    auto robot_ctrl = std::make_unique<RobotController>();

    // 初始化
    if (!robot_ctrl->initialize("config/robot_config.yaml")) {
        std::cerr << "初始化失败" << std::endl;
        return 1;
    }

    // 移动到目标位置
    Eigen::Vector3d target(0.5, 0.3, 0.2);
    if (robot_ctrl->moveToPosition(target)) {
        std::cout << "移动成功" << std::endl;
    }

    // 获取当前状态
    auto state = robot_ctrl->getCurrentState();
    std::cout << "当前位置: " << state.position.transpose() << std::endl;

    return 0;
}
```

### 轨迹执行

```cpp
#include "motion_planner.h"

int main() {
    // 创建运动规划器
    auto planner = std::make_unique<MotionPlanner>();

    // 定义轨迹点
    std::vector<Eigen::Vector3d> waypoints;
    waypoints.push_back(Eigen::Vector3d(0.0, 0.0, 0.0));
    waypoints.push_back(Eigen::Vector3d(0.2, 0.1, 0.1));
    waypoints.push_back(Eigen::Vector3d(0.4, 0.2, 0.2));
    waypoints.push_back(Eigen::Vector3d(0.5, 0.3, 0.2));

    // 检查轨迹可行性
    if (planner->isTrajectoryFeasible(waypoints)) {
        // 执行轨迹
        robot_ctrl->executeTrajectory(waypoints);
    }

    return 0;
}
```

## 配置说明

### 机器人配置

```yaml
robot:
  name: "basic_robot"
  type: "6dof_manipulator"
  manufacturer: "Generic"

  # DH参数
  dh_parameters:
    a: [0.0, 0.5, 0.5, 0.0, 0.0, 0.0]
    alpha: [1.5708, 0.0, 0.0, 1.5708, -1.5708, 0.0]
    d: [0.2, 0.0, 0.0, 0.3, 0.0, 0.1]
    theta_offset: [0.0, 0.0, 0.0, 0.0, 0.0, 0.0]

  # 关节限制
  joint_limits:
    min: [-3.14159, -2.0944, -2.96706, -2.0944, -2.0944, -6.28319]
    max: [3.14159, 2.0944, 2.96706, 2.0944, 2.0944, 6.28319]
```

### 运动配置

```yaml
motion:
  max_velocity: 1.0        # m/s
  max_acceleration: 2.0    # m/s²
  max_jerk: 5.0           # m/s³

  # 控制参数
  position_tolerance: 0.001  # m
  orientation_tolerance: 0.01 # rad

  # 轨迹规划参数
  trajectory:
    time_step: 0.01        # s
    blend_radius: 0.01     # m
```

## 测试框架

### 单元测试

```cpp
#include <gtest/gtest.h>
#include "robot_controller.h"

class RobotControllerTest : public ::testing::Test {
protected:
    void SetUp() override {
        controller_ = std::make_unique<RobotController>();
        ASSERT_TRUE(controller_->initialize("config/test_config.yaml"));
    }

    std::unique_ptr<RobotController> controller_;
};

TEST_F(RobotControllerTest, Initialization) {
    EXPECT_TRUE(controller_->isInitialized());
}

TEST_F(RobotControllerTest, MoveToPosition) {
    Eigen::Vector3d target(0.1, 0.1, 0.1);
    EXPECT_TRUE(controller_->moveToPosition(target));

    auto state = controller_->getCurrentState();
    EXPECT_NEAR(state.position.x(), target.x(), 0.001);
}
```

### 运行测试

```bash
# 构建测试
cd build
make test

# 运行所有测试
ctest

# 运行特定测试
ctest -R RobotControllerTest
```

## 开发指南

### 添加新功能

1. **定义接口**: 在头文件中定义新的接口
2. **实现功能**: 在源文件中实现具体功能
3. **添加配置**: 在配置文件中添加相关参数
4. **编写测试**: 为新功能编写单元测试
5. **更新文档**: 更新API文档和使用说明

### 代码规范

- 使用C++17标准
- 遵循Google C++代码规范
- 使用智能指针管理资源
- 添加必要的注释和文档
- 使用异常处理错误情况

### 调试技巧

- 使用gdb进行调试
- 查看日志文件定位问题
- 使用Valgrind检查内存泄漏
- 运行单元测试验证功能

## 扩展模板

基于此基础模板，你可以：

1. **添加感知功能**: 集成摄像头、激光雷达等传感器
2. **实现任务规划**: 添加任务分解和规划能力
3. **集成人机界面**: 添加Web界面或移动端控制
4. **添加安全功能**: 实现碰撞检测和安全监控
5. **支持多机器人**: 实现多机器人协调控制

## 技术支持

- 📖 [API参考文档](docs/api_reference.md)
- 📋 [配置指南](docs/configuration_guide.md)
- 📝 [开发笔记](docs/development_notes.md)
- 💬 项目讨论区
- 📧 技术支持邮箱

## 许可证

本模板遵循Leee平台的许可证协议。
