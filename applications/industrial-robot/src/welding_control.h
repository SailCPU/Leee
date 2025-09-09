#ifndef WIELDING_CONTROL_H
#define WIELDING_CONTROL_H

#include <memory>
#include <string>
#include <Eigen/Dense>

#include <motion-control/kinematics/kinematics_solver.h>
#include <perception/state-monitoring/sensor_interface.h>
#include <system-management/configuration/config_manager.h>

// 焊接参数结构体
struct WeldingParameters {
    std::string process_type;  // MIG, TIG, etc.
    double voltage;           // 焊接电压 (V)
    double current;           // 焊接电流 (A)
    double speed;             // 焊接速度 (mm/s)
    double wire_feed_rate;    // 焊丝送进速度 (mm/s)
    double gas_flow_rate;     // 保护气体流量 (L/min)
};

// 焊接任务结构体
struct WeldingTask {
    Eigen::Vector3d start_point;
    Eigen::Vector3d end_point;
    Eigen::Vector3d start_orientation;
    Eigen::Vector3d end_orientation;
    WeldingParameters params;
    std::string workpiece_material;
    double seam_thickness;
};

// 焊接状态结构体
struct WeldingStatus {
    bool is_running;
    Eigen::Vector3d current_position;
    Eigen::Vector3d current_orientation;
    double current_voltage;
    double current_current;
    double arc_length;
    double temperature;
    std::string error_message;
};

/**
 * @brief 焊接控制类
 *
 * 提供完整的焊接控制功能，包括：
 * - 焊接参数设置和管理
 * - 焊接轨迹执行
 * - 实时状态监控
 * - 安全保护机制
 */
class WeldingController {
public:
    /**
     * @brief 构造函数
     */
    WeldingController();

    /**
     * @brief 析构函数
     */
    ~WeldingController();

    /**
     * @brief 初始化焊接控制器
     * @param config_file 配置文件路径
     * @return 初始化是否成功
     */
    bool initialize(const std::string& config_file);

    /**
     * @brief 执行焊接任务
     * @param task 焊接任务参数
     * @return 执行是否成功
     */
    bool executeWeldingTask(const WeldingTask& task);

    /**
     * @brief 停止焊接过程
     */
    void stopWelding();

    /**
     * @brief 获取当前焊接状态
     * @return 焊接状态信息
     */
    WeldingStatus getCurrentStatus() const;

    /**
     * @brief 检查是否正在焊接
     * @return 是否正在执行焊接任务
     */
    bool isRunning() const;

    /**
     * @brief 设置焊接参数
     * @param params 焊接参数
     */
    void setWeldingParameters(const WeldingParameters& params);

    /**
     * @brief 紧急停止
     */
    void emergencyStop();

private:
    // 运动控制相关
    std::unique_ptr<leee::KinematicsSolver> kinematics_solver_;
    std::unique_ptr<leee::TrajectoryPlanner> trajectory_planner_;

    // 感知和监控
    std::unique_ptr<leee::SensorInterface> sensor_interface_;
    std::unique_ptr<leee::ProcessMonitor> process_monitor_;

    // 系统管理
    std::unique_ptr<leee::ConfigManager> config_manager_;
    std::unique_ptr<leee::Logger> logger_;

    // 焊接设备接口
    std::unique_ptr<WeldingEquipmentInterface> welding_equipment_;

    // 内部状态
    WeldingStatus current_status_;
    WeldingParameters current_params_;
    bool is_initialized_;
    bool is_running_;

    /**
     * @brief 预处理焊接任务
     * @param task 输入任务
     * @return 处理后的任务
     */
    WeldingTask preprocessTask(const WeldingTask& task);

    /**
     * @brief 规划焊接轨迹
     * @param task 焊接任务
     * @return 轨迹规划结果
     */
    Trajectory planWeldingTrajectory(const WeldingTask& task);

    /**
     * @brief 执行轨迹
     * @param trajectory 焊接轨迹
     * @return 执行是否成功
     */
    bool executeTrajectory(const Trajectory& trajectory);

    /**
     * @brief 监控焊接过程
     */
    void monitorWeldingProcess();

    /**
     * @brief 处理焊接异常
     * @param error 异常信息
     */
    void handleWeldingError(const std::string& error);

    /**
     * @brief 安全检查
     * @return 是否安全
     */
    bool performSafetyCheck();
};

#endif // WIELDING_CONTROL_H
