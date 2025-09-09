#include "welding_control.h"
#include <iostream>
#include <thread>
#include <chrono>
#include <cmath>

WeldingController::WeldingController()
    : is_initialized_(false), is_running_(false) {
    // 初始化状态
    current_status_.is_running = false;
    current_status_.current_voltage = 0.0;
    current_status_.current_current = 0.0;
    current_status_.arc_length = 0.0;
    current_status_.temperature = 25.0;
}

WeldingController::~WeldingController() {
    if (is_running_) {
        stopWelding();
    }
}

bool WeldingController::initialize(const std::string& config_file) {
    try {
        // 初始化配置管理器
        config_manager_ = std::make_unique<leee::ConfigManager>();
        if (!config_manager_->loadConfig(config_file)) {
            std::cerr << "Failed to load configuration file: " << config_file << std::endl;
            return false;
        }

        // 初始化运动学求解器
        kinematics_solver_ = std::make_unique<leee::KinematicsSolver>();
        if (!kinematics_solver_->initialize()) {
            std::cerr << "Failed to initialize kinematics solver" << std::endl;
            return false;
        }

        // 初始化轨迹规划器
        trajectory_planner_ = std::make_unique<leee::TrajectoryPlanner>();
        if (!trajectory_planner_->initialize()) {
            std::cerr << "Failed to initialize trajectory planner" << std::endl;
            return false;
        }

        // 初始化传感器接口
        sensor_interface_ = std::make_unique<leee::SensorInterface>();
        if (!sensor_interface_->initialize()) {
            std::cerr << "Failed to initialize sensor interface" << std::endl;
            return false;
        }

        // 初始化过程监控器
        process_monitor_ = std::make_unique<leee::ProcessMonitor>();
        if (!process_monitor_->initialize()) {
            std::cerr << "Failed to initialize process monitor" << std::endl;
            return false;
        }

        // 初始化日志系统
        logger_ = std::make_unique<leee::Logger>();
        logger_->setLogLevel(leee::LogLevel::INFO);

        // 初始化焊接设备接口
        welding_equipment_ = std::make_unique<WeldingEquipmentInterface>();
        if (!welding_equipment_->connect()) {
            std::cerr << "Failed to connect to welding equipment" << std::endl;
            return false;
        }

        logger_->info("Welding controller initialized successfully");
        is_initialized_ = true;
        return true;

    } catch (const std::exception& e) {
        std::cerr << "Exception during initialization: " << e.what() << std::endl;
        return false;
    }
}

bool WeldingController::executeWeldingTask(const WeldingTask& task) {
    if (!is_initialized_) {
        logger_->error("Controller not initialized");
        return false;
    }

    if (is_running_) {
        logger_->error("Welding task already running");
        return false;
    }

    try {
        logger_->info("Starting welding task execution");

        // 安全检查
        if (!performSafetyCheck()) {
            logger_->error("Safety check failed");
            return false;
        }

        // 预处理任务
        WeldingTask processed_task = preprocessTask(task);

        // 设置焊接参数
        setWeldingParameters(processed_task.params);

        // 规划焊接轨迹
        Trajectory trajectory = planWeldingTrajectory(processed_task);
        if (trajectory.points.empty()) {
            logger_->error("Failed to plan welding trajectory");
            return false;
        }

        // 开始监控线程
        is_running_ = true;
        std::thread monitor_thread(&WeldingController::monitorWeldingProcess, this);

        // 执行轨迹
        bool success = executeTrajectory(trajectory);

        // 等待监控线程结束
        is_running_ = false;
        monitor_thread.join();

        if (success) {
            logger_->info("Welding task completed successfully");
            return true;
        } else {
            logger_->error("Welding task failed");
            return false;
        }

    } catch (const std::exception& e) {
        logger_->error("Exception during welding task execution: " + std::string(e.what()));
        is_running_ = false;
        return false;
    }
}

void WeldingController::stopWelding() {
    if (!is_running_) {
        return;
    }

    logger_->info("Stopping welding process");
    is_running_ = false;

    // 停止焊接设备
    if (welding_equipment_) {
        welding_equipment_->stop();
    }

    // 停止机器人运动
    if (trajectory_planner_) {
        trajectory_planner_->stop();
    }

    current_status_.is_running = false;
    logger_->info("Welding process stopped");
}

WeldingStatus WeldingController::getCurrentStatus() const {
    return current_status_;
}

bool WeldingController::isRunning() const {
    return is_running_;
}

void WeldingController::setWeldingParameters(const WeldingParameters& params) {
    current_params_ = params;

    // 设置焊接设备参数
    if (welding_equipment_) {
        welding_equipment_->setVoltage(params.voltage);
        welding_equipment_->setCurrent(params.current);
        welding_equipment_->setWireFeedRate(params.wire_feed_rate);
        welding_equipment_->setGasFlowRate(params.gas_flow_rate);
    }

    logger_->info("Welding parameters updated");
}

void WeldingController::emergencyStop() {
    logger_->error("Emergency stop activated");

    // 立即停止所有操作
    stopWelding();

    // 执行紧急停止程序
    if (trajectory_planner_) {
        trajectory_planner_->emergencyStop();
    }

    // 断开焊接电源
    if (welding_equipment_) {
        welding_equipment_->emergencyStop();
    }

    current_status_.error_message = "Emergency stop activated";
}

WeldingTask WeldingController::preprocessTask(const WeldingTask& task) {
    WeldingTask processed = task;

    // 根据材料调整焊接参数
    if (task.workpiece_material == "stainless_steel") {
        processed.params.gas_flow_rate = 15.0;  // 提高保护气体流量
    } else if (task.workpiece_material == "aluminum") {
        processed.params.voltage *= 1.1;  // 铝材需要稍高电压
        processed.params.wire_feed_rate *= 0.9;  // 降低送丝速度
    }

    // 根据焊缝厚度调整参数
    if (task.seam_thickness > 5.0) {
        processed.params.current *= 1.2;  // 厚板增加电流
        processed.params.speed *= 0.8;    // 降低焊接速度
    }

    logger_->info("Task preprocessing completed");
    return processed;
}

Trajectory WeldingController::planWeldingTrajectory(const WeldingTask& task) {
    Trajectory trajectory;

    // 使用轨迹规划器规划焊接路径
    trajectory = trajectory_planner_->planWeldingTrajectory(
        task.start_point,
        task.end_point,
        task.start_orientation,
        task.end_orientation,
        task.params.speed
    );

    // 添加焊接工艺参数
    trajectory.welding_params = task.params;

    logger_->info("Welding trajectory planned with " +
                 std::to_string(trajectory.points.size()) + " points");

    return trajectory;
}

bool WeldingController::executeTrajectory(const Trajectory& trajectory) {
    try {
        // 移动到起始位置
        Eigen::VectorXd start_joint_angles = kinematics_solver_->inverseKinematics(
            trajectory.points[0].position,
            trajectory.points[0].orientation
        );

        if (!trajectory_planner_->moveToPosition(start_joint_angles)) {
            logger_->error("Failed to move to start position");
            return false;
        }

        // 开始焊接
        welding_equipment_->start();

        // 执行焊接轨迹
        for (size_t i = 0; i < trajectory.points.size() && is_running_; ++i) {
            const auto& point = trajectory.points[i];

            // 计算关节角度
            Eigen::VectorXd joint_angles = kinematics_solver_->inverseKinematics(
                point.position,
                point.orientation
            );

            // 执行运动
            if (!trajectory_planner_->moveToPosition(joint_angles)) {
                logger_->error("Failed to execute trajectory point " + std::to_string(i));
                welding_equipment_->stop();
                return false;
            }

            // 更新当前位置
            current_status_.current_position = point.position;
            current_status_.current_orientation = point.orientation;

            // 小延迟以控制焊接速度
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }

        // 停止焊接
        welding_equipment_->stop();

        logger_->info("Trajectory execution completed");
        return true;

    } catch (const std::exception& e) {
        logger_->error("Exception during trajectory execution: " + std::string(e.what()));
        welding_equipment_->stop();
        return false;
    }
}

void WeldingController::monitorWeldingProcess() {
    logger_->info("Starting welding process monitoring");

    while (is_running_) {
        try {
            // 读取传感器数据
            auto sensor_data = sensor_interface_->readSensors();

            // 更新焊接状态
            current_status_.current_voltage = sensor_data.voltage;
            current_status_.current_current = sensor_data.current;
            current_status_.arc_length = sensor_data.arc_length;
            current_status_.temperature = sensor_data.temperature;

            // 检查焊接质量
            if (std::abs(current_status_.current_voltage - current_params_.voltage) > 2.0) {
                logger_->warn("Voltage deviation detected: " +
                             std::to_string(current_status_.current_voltage));
            }

            if (current_status_.temperature > 80.0) {
                logger_->warn("High temperature detected: " +
                             std::to_string(current_status_.temperature) + "°C");
                // 采取降温措施
                welding_equipment_->reduceCurrent(0.9);
            }

            // 监控间隔
            std::this_thread::sleep_for(std::chrono::milliseconds(50));

        } catch (const std::exception& e) {
            logger_->error("Exception in monitoring thread: " + std::string(e.what()));
            break;
        }
    }

    logger_->info("Welding process monitoring stopped");
}

void WeldingController::handleWeldingError(const std::string& error) {
    logger_->error("Welding error: " + error);
    current_status_.error_message = error;

    // 执行错误恢复程序
    stopWelding();
}

bool WeldingController::performSafetyCheck() {
    // 检查机器人状态
    if (!kinematics_solver_->isCalibrated()) {
        logger_->error("Robot not calibrated");
        return false;
    }

    // 检查焊接设备状态
    if (!welding_equipment_->isReady()) {
        logger_->error("Welding equipment not ready");
        return false;
    }

    // 检查传感器状态
    if (!sensor_interface_->isConnected()) {
        logger_->error("Sensors not connected");
        return false;
    }

    // 检查工作空间安全
    auto robot_position = kinematics_solver_->getCurrentPosition();
    if (robot_position.norm() > 1000.0) {  // 超出安全工作范围
        logger_->error("Robot outside safe workspace");
        return false;
    }

    logger_->info("Safety check passed");
    return true;
}
