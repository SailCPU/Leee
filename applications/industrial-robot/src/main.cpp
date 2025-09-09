#include "welding_control.h"
#include <iostream>
#include <csignal>
#include <atomic>

// 全局变量用于信号处理
std::atomic<bool> running(true);

// 信号处理函数
void signalHandler(int signum) {
    std::cout << "\n收到信号 " << signum << "，正在停止程序..." << std::endl;
    running = false;
}

/**
 * @brief 主函数
 *
 * 演示工业机器人焊接应用的基本功能
 */
int main(int argc, char* argv[]) {
    // 设置信号处理
    signal(SIGINT, signalHandler);
    signal(SIGTERM, signalHandler);

    std::cout << "=== Leee 工业机器人焊接应用演示 ===" << std::endl;
    std::cout << "按 Ctrl+C 停止程序" << std::endl;
    std::cout << std::endl;

    try {
        // 创建焊接控制器
        auto welding_ctrl = std::make_unique<WeldingController>();

        // 初始化控制器
        std::string config_file = "config/welding_config.yaml";
        if (argc > 1) {
            config_file = argv[1];
        }

        std::cout << "正在初始化焊接控制器..." << std::endl;
        if (!welding_ctrl->initialize(config_file)) {
            std::cerr << "焊接控制器初始化失败" << std::endl;
            return 1;
        }
        std::cout << "焊接控制器初始化成功" << std::endl;

        // 创建焊接任务
        WeldingTask task;
        task.start_point = Eigen::Vector3d(100.0, 200.0, 50.0);
        task.end_point = Eigen::Vector3d(300.0, 200.0, 50.0);
        task.start_orientation = Eigen::Vector3d(0.0, 0.0, 0.0);
        task.end_orientation = Eigen::Vector3d(0.0, 0.0, 0.0);

        // 设置焊接参数
        task.params.process_type = "MIG";
        task.params.voltage = 24.0;
        task.params.current = 150.0;
        task.params.speed = 5.0;
        task.params.wire_feed_rate = 6.0;
        task.params.gas_flow_rate = 12.0;

        // 设置工件信息
        task.workpiece_material = "mild_steel";
        task.seam_thickness = 3.0;

        std::cout << "焊接任务参数:" << std::endl;
        std::cout << "  起始点: (" << task.start_point.x() << ", "
                  << task.start_point.y() << ", " << task.start_point.z() << ")" << std::endl;
        std::cout << "  终止点: (" << task.end_point.x() << ", "
                  << task.end_point.y() << ", " << task.end_point.z() << ")" << std::endl;
        std::cout << "  焊接工艺: " << task.params.process_type << std::endl;
        std::cout << "  电压: " << task.params.voltage << " V" << std::endl;
        std::cout << "  电流: " << task.params.current << " A" << std::endl;
        std::cout << "  速度: " << task.params.speed << " mm/s" << std::endl;
        std::cout << std::endl;

        // 执行焊接任务
        std::cout << "开始执行焊接任务..." << std::endl;
        if (!welding_ctrl->executeWeldingTask(task)) {
            std::cerr << "焊接任务执行失败" << std::endl;
            return 1;
        }

        // 监控焊接过程
        std::cout << "焊接过程监控:" << std::endl;
        while (welding_ctrl->isRunning() && running) {
            auto status = welding_ctrl->getCurrentStatus();

            std::cout << "\r当前位置: (" << status.current_position.x() << ", "
                      << status.current_position.y() << ", " << status.current_position.z() << ") | "
                      << "电压: " << status.current_voltage << " V | "
                      << "电流: " << status.current_current << " A | "
                      << "温度: " << status.temperature << " °C" << std::flush;

            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
        std::cout << std::endl;

        std::cout << "焊接任务完成！" << std::endl;

        // 显示最终状态
        auto final_status = welding_ctrl->getCurrentStatus();
        std::cout << "最终状态:" << std::endl;
        std::cout << "  运行状态: " << (final_status.is_running ? "运行中" : "已停止") << std::endl;
        std::cout << "  最终位置: (" << final_status.current_position.x() << ", "
                  << final_status.current_position.y() << ", " << final_status.current_position.z() << ")" << std::endl;
        if (!final_status.error_message.empty()) {
            std::cout << "  错误信息: " << final_status.error_message << std::endl;
        }

    } catch (const std::exception& e) {
        std::cerr << "程序异常: " << e.what() << std::endl;
        return 1;
    }

    std::cout << "程序正常退出" << std::endl;
    return 0;
}
