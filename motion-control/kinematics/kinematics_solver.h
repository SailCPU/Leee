#pragma once

/**
 * @file kinematics_solver.h
 * @brief 机器人运动学求解器
 */

#include <Eigen/Dense>
#include <vector>

namespace leee {

/**
 * @brief 机器人运动学求解器
 */
class KinematicsSolver {
public:
    /**
     * @brief 正向运动学求解
     * @param joint_angles 关节角度向量
     * @return 末端执行器位姿（变换矩阵）
     */
    virtual Eigen::Matrix4d forwardKinematics(const Eigen::VectorXd& joint_angles) = 0;

    /**
     * @brief 逆向运动学求解
     * @param end_effector_pose 末端执行器目标位姿
     * @param initial_guess 初始关节角度猜测
     * @return 关节角度解向量
     */
    virtual Eigen::VectorXd inverseKinematics(const Eigen::Matrix4d& end_effector_pose,
                                            const Eigen::VectorXd& initial_guess) = 0;

    /**
     * @brief 计算雅可比矩阵
     * @param joint_angles 当前关节角度
     * @return 雅可比矩阵
     */
    virtual Eigen::MatrixXd jacobian(const Eigen::VectorXd& joint_angles) = 0;

    /**
     * @brief 检查关节角度是否在有效范围内
     * @param joint_angles 关节角度向量
     * @return 是否有效
     */
    virtual bool isValidJointAngles(const Eigen::VectorXd& joint_angles) const = 0;

    virtual ~KinematicsSolver() = default;
};

/**
 * @brief 6DOF机械臂运动学求解器
 */
class SixDOFManipulatorKinematics : public KinematicsSolver {
private:
    std::vector<double> link_lengths_;  // 连杆长度
    std::vector<Eigen::Vector3d> joint_axes_;  // 关节轴方向

public:
    /**
     * @brief 构造函数
     * @param link_lengths 连杆长度参数
     */
    explicit SixDOFManipulatorKinematics(const std::vector<double>& link_lengths);

    Eigen::Matrix4d forwardKinematics(const Eigen::VectorXd& joint_angles) override;
    Eigen::VectorXd inverseKinematics(const Eigen::Matrix4d& end_effector_pose,
                                    const Eigen::VectorXd& initial_guess) override;
    Eigen::MatrixXd jacobian(const Eigen::VectorXd& joint_angles) override;
    bool isValidJointAngles(const Eigen::VectorXd& joint_angles) const override;
};

} // namespace leee
