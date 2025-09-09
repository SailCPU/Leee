#pragma once

/**
 * @file math_utils.h
 * @brief 机器人学数学基础运算库
 */

#include <Eigen/Dense>
#include <vector>

namespace leee {

/**
 * @brief 向量和矩阵运算工具类
 */
class MathUtils {
public:
    /**
     * @brief 计算向量范数
     * @param vec 输入向量
     * @return 向量范数
     */
    static double norm(const Eigen::VectorXd& vec);

    /**
     * @brief 计算向量点积
     * @param a 向量a
     * @param b 向量b
     * @return 点积结果
     */
    static double dot(const Eigen::VectorXd& a, const Eigen::VectorXd& b);

    /**
     * @brief 计算向量叉积（3D）
     * @param a 向量a
     * @param b 向量b
     * @return 叉积结果
     */
    static Eigen::Vector3d cross(const Eigen::Vector3d& a, const Eigen::Vector3d& b);

    /**
     * @brief 创建旋转矩阵
     * @param axis 旋转轴
     * @param angle 旋转角度（弧度）
     * @return 旋转矩阵
     */
    static Eigen::Matrix3d rotationMatrix(const Eigen::Vector3d& axis, double angle);

    /**
     * @brief 创建变换矩阵
     * @param rotation 旋转矩阵
     * @param translation 平移向量
     * @return 变换矩阵
     */
    static Eigen::Matrix4d transformationMatrix(const Eigen::Matrix3d& rotation,
                                               const Eigen::Vector3d& translation);
};

} // namespace leee
