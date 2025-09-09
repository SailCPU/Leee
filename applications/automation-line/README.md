# 自动化生产线应用案例

## 概述

这是一个基于Leee平台的自动化生产线应用案例，展示如何构建完整的工业自动化生产线系统，实现物料搬运、质量检测、生产监控等功能。

## 功能特性

- 🔄 **生产线集成**: 完整的生产线控制和协调
- 📦 **物料搬运**: 智能物料识别和搬运系统
- 🔍 **质量检测**: 自动化质量检测和缺陷识别
- 📊 **生产监控**: 实时生产数据采集和分析
- ⚙️ **设备协调**: 多设备协同工作和调度优化

## 应用场景

### 装配生产线
- 自动零件装配
- 物料供应管理
- 质量检测集成
- 生产节拍控制

### 检测分拣线
- 视觉检测系统
- 智能分拣机器人
- 缺陷分类处理
- 数据统计分析

### 包装物流线
- 自动包装系统
- 物流配送优化
- 库存管理集成
- 追溯系统

## 快速开始

### 1. 环境准备

```bash
# 确保Leee平台已安装
cd /path/to/leee
./install.sh

# 进入应用目录
cd applications/automation-line
```

### 2. 配置生产线

```bash
# 编辑生产线配置文件
vim config/production_config.yaml

# 配置生产线参数
production_line:
  name: "assembly_line_001"
  stations: 5
  cycle_time: 30.0  # seconds
  max_capacity: 1000  # units/hour

# 配置设备协调
coordination:
  conveyor_speed: 0.5  # m/s
  robot_workspace: [[-1.0, 1.0], [-1.0, 1.0], [0.0, 1.0]]
  sensor_update_rate: 50  # Hz
```

### 3. 运行演示

```bash
# 编译应用
mkdir build && cd build
cmake ..
make

# 启动自动化生产线
./automation_line_demo
```

## 应用架构

```
automation-line/
├── src/                 # 源代码
│   ├── line_controller.cpp       # 生产线控制器
│   ├── material_handler.cpp      # 物料处理器
│   ├── quality_inspector.cpp     # 质量检测器
│   ├── production_monitor.cpp    # 生产监控器
│   └── main.cpp                 # 主程序
├── config/              # 配置文件
│   ├── production_config.yaml   # 生产线配置
│   ├── equipment_config.yaml    # 设备配置
│   └── quality_config.yaml      # 质量配置
├── docs/                # 文档
│   ├── line_integration.md      # 生产线集成指南
│   ├── quality_system.md        # 质量系统文档
│   └── monitoring_guide.md      # 监控指南
└── CMakeLists.txt       # 构建配置
```

## 核心组件

### 生产线控制器

```cpp
#include <industrial-integration/line-control/line_controller.h>
#include <task-planning/coordination/coordinator.h>

class ProductionLineController {
public:
    ProductionLineController();
    bool initialize(const ProductionConfig& config);
    bool startProduction();
    bool stopProduction();
    ProductionStatus getStatus();
    bool handleAlarm(const Alarm& alarm);

private:
    std::unique_ptr<leee::LineController> line_ctrl_;
    std::unique_ptr<leee::Coordinator> coordinator_;
    std::unique_ptr<leee::AlarmManager> alarm_mgr_;
    ProductionConfig config_;
};
```

### 物料处理系统

```cpp
#include <industrial-integration/material-handling/material_handler.h>
#include <perception/vision/object_recognition.h>

class MaterialHandlingSystem {
public:
    MaterialHandlingSystem();
    bool initialize();
    bool pickupMaterial(const MaterialInfo& material);
    bool placeMaterial(const Location& location);
    bool identifyMaterial(const Image& image);
    MaterialStatus getMaterialStatus();

private:
    std::unique_ptr<leee::MaterialHandler> handler_;
    std::unique_ptr<leee::ObjectRecognition> recognizer_;
    std::unique_ptr<leee::ConveyorController> conveyor_;
};
```

### 质量检测系统

```cpp
#include <industrial-integration/quality-control/quality_inspector.h>
#include <perception/vision/defect_detection.h>

class QualityInspectionSystem {
public:
    QualityInspectionSystem();
    bool initialize(const QualityConfig& config);
    InspectionResult inspectProduct(const Product& product);
    bool classifyDefect(const Defect& defect);
    QualityReport generateReport();
    void updateQualityModel(const TrainingData& data);

private:
    std::unique_ptr<leee::QualityInspector> inspector_;
    std::unique_ptr<leee::DefectDetection> defect_detector_;
    std::unique_ptr<leee::QualityDatabase> quality_db_;
};
```

## 使用示例

### 生产线控制

```cpp
// 创建生产线控制器
auto line_ctrl = std::make_unique<ProductionLineController>();
line_ctrl->initialize(production_config);

// 启动生产线
if (line_ctrl->startProduction()) {
    std::cout << "生产线启动成功" << std::endl;
}

// 监控生产线状态
while (line_ctrl->isRunning()) {
    auto status = line_ctrl->getStatus();
    std::cout << "生产节拍: " << status.cycle_time << " s" << std::endl;
    std::cout << "产量: " << status.total_produced << " 个" << std::endl;
    std::cout << "良品率: " << status.quality_rate * 100 << " %" << std::endl;

    std::this_thread::sleep_for(std::chrono::seconds(1));
}
```

### 物料搬运

```cpp
// 创建物料处理系统
auto material_handler = std::make_unique<MaterialHandlingSystem>();

// 识别物料
Image current_image = captureImage();
auto material_info = material_handler->identifyMaterial(current_image);

if (material_info.confidence > 0.8) {
    // 搬运物料到指定位置
    Location target_location = getProcessingLocation(material_info.type);
    material_handler->pickupMaterial(material_info);
    material_handler->placeMaterial(target_location);
} else {
    // 无法识别的物料，移到异常处理区
    material_handler->moveToExceptionArea();
}
```

### 质量检测

```cpp
// 创建质量检测系统
auto quality_system = std::make_unique<QualityInspectionSystem>();
quality_system->initialize(quality_config);

// 检测产品
Product current_product = getCurrentProduct();
InspectionResult result = quality_system->inspectProduct(current_product);

if (result.is_defective) {
    // 处理缺陷产品
    quality_system->classifyDefect(result.defect);
    moveToDefectArea(current_product);
} else {
    // 合格产品继续生产流程
    moveToNextStation(current_product);
}

// 生成质量报告
auto report = quality_system->generateReport();
saveQualityReport(report);
```

## 设备配置

### 机器人配置

```yaml
robots:
  pick_and_place_robot:
    type: "6dof_delta"
    workspace: [[-0.5, 0.5], [-0.5, 0.5], [0.0, 0.8]]
    payload: 5.0  # kg
    speed: 2.0    # m/s
    accuracy: 0.1  # mm

  assembly_robot:
    type: "collaborative_cobot"
    workspace: [[-0.8, 0.8], [-0.8, 0.8], [0.0, 1.2]]
    payload: 10.0
    force_control: true
    safety_rated: true
```

### 传感器配置

```yaml
sensors:
  vision_system:
    type: "smart_camera"
    model: "cognex_is5600"
    resolution: [1920, 1200]
    fps: 30
    lighting: "integrated_led"

  conveyor_sensor:
    type: "inductive_proximity"
    model: "turck_ni8u"
    sensing_range: 8.0  # mm
    update_rate: 100    # Hz

  force_sensor:
    type: "multi_axis"
    model: "ati_mini45"
    range: [-200.0, 200.0]  # N
    resolution: 0.1         # N
```

## 监控功能

- 📊 **实时仪表板**: 生产线状态实时显示
- 📈 **生产统计**: 产量、效率、质量数据统计
- 🚨 **报警系统**: 设备故障和质量问题的报警
- 📋 **维护提醒**: 基于数据预测的设备维护
- 📝 **追溯系统**: 产品全生命周期追溯

## 性能指标

- **生产节拍**: 30秒/件
- **检测精度**: 99.5%
- **可用性**: 99.8%
- **误检率**: <0.5%
- **漏检率**: <0.2%

## 扩展功能

### 集成MES系统

```cpp
class MESIntegration : public SystemIntegration {
public:
    MESIntegration();
    bool connectToMES(const std::string& endpoint);
    bool syncProductionData();
    bool submitQualityData(const QualityReport& report);
    bool receiveWorkOrders();
};
```

### 预测性维护

```cpp
class PredictiveMaintenance : public MaintenanceSystem {
public:
    PredictiveMaintenance();
    bool analyzeEquipmentHealth();
    bool predictFailure(const EquipmentData& data);
    MaintenanceSchedule generateSchedule();
    bool optimizeMaintenancePlan();
};
```

## 技术支持

如需技术支持，请：

1. 查看[生产线集成指南](docs/line_integration.md)
2. 参考[质量系统文档](docs/quality_system.md)
3. 查阅[监控指南](docs/monitoring_guide.md)
4. 提交问题到项目Issue
5. 联系技术支持团队

## 许可证

本应用案例遵循Leee平台的许可证协议。
