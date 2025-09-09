/**
 * @file robot_kinematics_example.cpp
 * @brief 机器人运动学示例程序
 *
 * 演示如何使用Leee的运动学求解器进行正向和逆向运动学计算
 */

#include <iostream>
#include <iomanip>
#include <vector>
#include <Eigen/Dense>

#include "include/math/math_utils.h"
#include "include/kinematics/kinematics_solver.h"

int main() {
    std::cout << "Leee 机器人运动学示例" << std::endl;
    std::cout << "====================" << std::endl;

    // 创建6DOF机械臂的DH参数
    // 这里使用的是简化参数，实际应用中需要根据具体机械臂调整
    std::vector<double> link_lengths = {0.4, 0.3, 0.2, 0.1, 0.1, 0.05};

    // 创建运动学求解器
    leee::SixDOFManipulatorKinematics kinematics_solver(link_lengths);

    std::cout << "1. 正向运动学示例" << std::endl;
    std::cout << "------------------" << std::endl;

    // 设置关节角度（弧度）
    Eigen::VectorXd joint_angles(6);
    joint_angles << 0.1, 0.2, -0.1, 0.3, -0.2, 0.1;  // 示例角度

    std::cout << "关节角度: ";
    for (int i = 0; i < 6; i++) {
        std::cout << std::fixed << std::setprecision(3)
                  << joint_angles[i] * 180.0 / M_PI << "° ";
    }
    std::cout << std::endl;

    // 计算正向运动学
    Eigen::Matrix4d end_effector_pose = kinematics_solver.forwardKinematics(joint_angles);

    std::cout << "末端执行器位姿:" << std::endl;
    std::cout << end_effector_pose << std::endl;

    // 提取位置和姿态
    Eigen::Vector3d position = end_effector_pose.block<3, 1>(0, 3);
    std::cout << "位置: [" << position.transpose() << "] m" << std::endl;

    std::cout << "\n2. 逆向运动学示例" << std::endl;
    std::cout << "------------------" << std::endl;

    // 设置目标位姿
    Eigen::Matrix4d target_pose = Eigen::Matrix4d::Identity();
    target_pose.block<3, 1>(0, 3) = Eigen::Vector3d(0.5, 0.2, 0.3);  // 目标位置

    // 添加一些旋转（示例）
    Eigen::Matrix3d rotation;
    rotation = Eigen::AngleAxisd(0.1, Eigen::Vector3d::UnitZ()) *
               Eigen::AngleAxisd(0.2, Eigen::Vector3d::UnitY()) *
               Eigen::AngleAxisd(-0.1, Eigen::Vector3d::UnitX());
    target_pose.block<3, 3>(0, 0) = rotation;

    std::cout << "目标位姿:" << std::endl;
    std::cout << target_pose << std::endl;

    // 计算逆向运动学
    Eigen::VectorXd initial_guess = Eigen::VectorXd::Zero(6);  // 初始猜测
    Eigen::VectorXd solution = kinematics_solver.inverseKinematics(target_pose, initial_guess);

    if (solution.size() > 0) {
        std::cout << "求解成功!" << std::endl;
        std::cout << "关节角度解: ";
        for (int i = 0; i < 6; i++) {
            std::cout << std::fixed << std::setprecision(3)
                      << solution[i] * 180.0 / M_PI << "° ";
        }
        std::cout << std::endl;

        // 验证解
        Eigen::Matrix4d verification_pose = kinematics_solver.forwardKinematics(solution);
        std::cout << "验证位姿误差: "
                  << (verification_pose - target_pose).norm()
                  << std::endl;
    } else {
        std::cout << "求解失败!" << std::endl;
    }

    std::cout << "\n3. 雅可比矩阵计算示例" << std::endl;
    std::cout << "----------------------" << std::endl;

    // 计算雅可比矩阵
    Eigen::MatrixXd jacobian = kinematics_solver.jacobian(joint_angles);
    std::cout << "雅可比矩阵:" << std::endl;
    std::cout << jacobian << std::endl;

    // 计算关节速度到末端速度的映射
    Eigen::VectorXd joint_velocities(6);
    joint_velocities << 0.1, 0.05, -0.02, 0.08, -0.03, 0.01;  // 示例关节速度

    Eigen::VectorXd end_effector_velocity = jacobian * joint_velocities;
    std::cout << "关节速度: " << joint_velocities.transpose() << " rad/s" << std::endl;
    std::cout << "末端线速度: ["
              << end_effector_velocity.head<3>().transpose()
              << "] m/s" << std::endl;
    std::cout << "末端角速度: ["
              << end_effector_velocity.tail<3>().transpose()
              << "] rad/s" << std::endl;

    std::cout << "\n示例运行完成!" << std::endl;
    return 0;
}
