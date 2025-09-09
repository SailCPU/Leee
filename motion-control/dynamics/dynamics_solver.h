#pragma once

/**
 * @file dynamics_solver.h
 * @brief 机器人动力学求解器
 */

#include <Eigen/Dense>
#include <vector>

namespace leee {

/**
 * @brief 机器人动力学求解器
 */
class DynamicsSolver {
public:
    /**
     * @brief 计算关节力矩（逆动力学）
     * @param joint_angles 关节角度
     * @param joint_velocities 关节速度
     * @param joint_accelerations 关节加速度
     * @param external_forces 外力（可选）
     * @return 关节力矩向量
     */
    virtual Eigen::VectorXd inverseDynamics(const Eigen::VectorXd& joint_angles,
                                          const Eigen::VectorXd& joint_velocities,
                                          const Eigen::VectorXd& joint_accelerations,
                                          const Eigen::VectorXd& external_forces = Eigen::VectorXd()) = 0;

    /**
     * @brief 计算关节加速度（正向动力学）
     * @param joint_angles 关节角度
     * @param joint_velocities 关节速度
     * @param joint_torques 关节力矩
     * @param external_forces 外力（可选）
     * @return 关节加速度向量
     */
    virtual Eigen::VectorXd forwardDynamics(const Eigen::VectorXd& joint_angles,
                                          const Eigen::VectorXd& joint_velocities,
                                          const Eigen::VectorXd& joint_torques,
                                          const Eigen::VectorXd& external_forces = Eigen::VectorXd()) = 0;

    /**
     * @brief 计算质量矩阵
     * @param joint_angles 关节角度
     * @return 质量矩阵
     */
    virtual Eigen::MatrixXd massMatrix(const Eigen::VectorXd& joint_angles) = 0;

    /**
     * @brief 计算科里奥利力和向心力向量
     * @param joint_angles 关节角度
     * @param joint_velocities 关节速度
     * @return 科里奥利力和向心力向量
     */
    virtual Eigen::VectorXd coriolisCentrifugal(const Eigen::VectorXd& joint_angles,
                                               const Eigen::VectorXd& joint_velocities) = 0;

    /**
     * @brief 计算重力力矩向量
     * @param joint_angles 关节角度
     * @return 重力力矩向量
     */
    virtual Eigen::VectorXd gravityTorques(const Eigen::VectorXd& joint_angles) = 0;

    virtual ~DynamicsSolver() = default;
};

/**
 * @brief 基于牛顿-欧拉方法的动力学求解器
 */
class NewtonEulerDynamics : public DynamicsSolver {
private:
    struct Link {
        double mass;
        Eigen::Vector3d center_of_mass;
        Eigen::Matrix3d inertia_tensor;
        Eigen::Vector3d joint_axis;
    };

    std::vector<Link> links_;
    Eigen::Vector3d gravity_;

public:
    /**
     * @brief 构造函数
     * @param link_masses 连杆质量
     * @param link_coms 连杆质心位置
     * @param link_inertias 连杆惯性张量
     * @param joint_axes 关节轴方向
     * @param gravity 重力加速度向量
     */
    NewtonEulerDynamics(const std::vector<double>& link_masses,
                       const std::vector<Eigen::Vector3d>& link_coms,
                       const std::vector<Eigen::Matrix3d>& link_inertias,
                       const std::vector<Eigen::Vector3d>& joint_axes,
                       const Eigen::Vector3d& gravity = Eigen::Vector3d(0, 0, -9.81));

    Eigen::VectorXd inverseDynamics(const Eigen::VectorXd& joint_angles,
                                  const Eigen::VectorXd& joint_velocities,
                                  const Eigen::VectorXd& joint_accelerations,
                                  const Eigen::VectorXd& external_forces = Eigen::VectorXd()) override;

    Eigen::VectorXd forwardDynamics(const Eigen::VectorXd& joint_angles,
                                  const Eigen::VectorXd& joint_velocities,
                                  const Eigen::VectorXd& joint_torques,
                                  const Eigen::VectorXd& external_forces = Eigen::VectorXd()) override;

    Eigen::MatrixXd massMatrix(const Eigen::VectorXd& joint_angles) override;
    Eigen::VectorXd coriolisCentrifugal(const Eigen::VectorXd& joint_angles,
                                       const Eigen::VectorXd& joint_velocities) override;
    Eigen::VectorXd gravityTorques(const Eigen::VectorXd& joint_angles) override;
};

} // namespace leee
