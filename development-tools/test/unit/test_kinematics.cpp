/**
 * @file test_kinematics.cpp
 * @brief 运动学模块单元测试
 */

#include <gtest/gtest.h>
#include <Eigen/Dense>
#include "include/kinematics/kinematics_solver.h"
#include "include/math/math_utils.h"

class KinematicsTest : public ::testing::Test {
protected:
    void SetUp() override {
        // 设置6DOF机械臂参数用于测试
        std::vector<double> link_lengths = {0.4, 0.3, 0.2, 0.1, 0.1, 0.05};
        kinematics_solver_ = std::make_unique<leee::SixDOFManipulatorKinematics>(link_lengths);
    }

    std::unique_ptr<leee::SixDOFManipulatorKinematics> kinematics_solver_;
};

TEST_F(KinematicsTest, ForwardKinematicsIdentity) {
    // 测试零关节角度的正向运动学
    Eigen::VectorXd zero_angles = Eigen::VectorXd::Zero(6);
    Eigen::Matrix4d pose = kinematics_solver_->forwardKinematics(zero_angles);

    // 在零角度时，末端应该在机械臂伸展位置
    Eigen::Vector3d expected_position(0.4 + 0.3 + 0.2 + 0.1 + 0.1 + 0.05,
                                     0.0, 0.0);

    EXPECT_NEAR(pose(0, 3), expected_position.x(), 1e-6);
    EXPECT_NEAR(pose(1, 3), expected_position.y(), 1e-6);
    EXPECT_NEAR(pose(2, 3), expected_position.z(), 1e-6);
}

TEST_F(KinematicsTest, ForwardKinematicsKnownConfiguration) {
    // 测试已知关节配置的正向运动学
    Eigen::VectorXd angles(6);
    angles << 0.0, 0.0, 0.0, 0.0, 0.0, 0.0;  // 所有关节为0

    Eigen::Matrix4d pose = kinematics_solver_->forwardKinematics(angles);

    // 验证旋转矩阵为单位矩阵
    Eigen::Matrix3d rotation = pose.block<3, 3>(0, 0);
    Eigen::Matrix3d identity = Eigen::Matrix3d::Identity();

    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            EXPECT_NEAR(rotation(i, j), identity(i, j), 1e-6);
        }
    }
}

TEST_F(KinematicsTest, JacobianMatrixDimensions) {
    // 测试雅可比矩阵维度
    Eigen::VectorXd angles = Eigen::VectorXd::Zero(6);
    Eigen::MatrixXd jacobian = kinematics_solver_->jacobian(angles);

    EXPECT_EQ(jacobian.rows(), 6);  // 6DOF
    EXPECT_EQ(jacobian.cols(), 6);  // 6个关节
}

TEST_F(KinematicsTest, JacobianFiniteDifference) {
    // 使用有限差分验证雅可比矩阵
    Eigen::VectorXd angles = Eigen::VectorXd::Random(6) * 0.1;  // 小角度
    Eigen::MatrixXd jacobian = kinematics_solver_->jacobian(angles);

    double epsilon = 1e-6;
    Eigen::MatrixXd numerical_jacobian(6, 6);

    for (int j = 0; j < 6; ++j) {
        Eigen::VectorXd angles_plus = angles;
        Eigen::VectorXd angles_minus = angles;

        angles_plus[j] += epsilon;
        angles_minus[j] -= epsilon;

        Eigen::Matrix4d pose_plus = kinematics_solver_->forwardKinematics(angles_plus);
        Eigen::Matrix4d pose_minus = kinematics_solver_->forwardKinematics(angles_minus);

        Eigen::Matrix4d diff = pose_plus * pose_minus.inverse();

        // 提取线速度
        Eigen::Vector3d linear_vel = diff.block<3, 1>(0, 3) / (2 * epsilon);
        numerical_jacobian.col(j).head<3>() = linear_vel;

        // 提取角速度
        Eigen::Matrix3d rotation_diff = diff.block<3, 3>(0, 0);
        Eigen::Vector3d angular_vel = Eigen::Vector3d(
            rotation_diff(2, 1), rotation_diff(0, 2), rotation_diff(1, 0)
        ) / (2 * epsilon);
        numerical_jacobian.col(j).tail<3>() = angular_vel;
    }

    // 比较解析雅可比和数值雅可比
    Eigen::MatrixXd diff = jacobian - numerical_jacobian;
    double max_error = diff.cwiseAbs().maxCoeff();
    EXPECT_LT(max_error, 1e-3);  // 允许一定的数值误差
}

TEST_F(KinematicsTest, JointLimits) {
    // 测试关节角度有效性检查
    Eigen::VectorXd valid_angles = Eigen::VectorXd::Zero(6);
    EXPECT_TRUE(kinematics_solver_->isValidJointAngles(valid_angles));

    Eigen::VectorXd invalid_angles(6);
    invalid_angles << 0.0, 0.0, 0.0, 0.0, 0.0, M_PI * 2;  // 超出范围
    EXPECT_FALSE(kinematics_solver_->isValidJointAngles(invalid_angles));
}

TEST_F(KinematicsTest, InverseKinematicsConvergence) {
    // 测试逆向运动学收敛性
    Eigen::VectorXd true_angles = Eigen::VectorXd::Random(6) * 0.5;
    Eigen::Matrix4d target_pose = kinematics_solver_->forwardKinematics(true_angles);

    Eigen::VectorXd initial_guess = Eigen::VectorXd::Zero(6);
    Eigen::VectorXd solution = kinematics_solver_->inverseKinematics(target_pose, initial_guess);

    ASSERT_TRUE(solution.size() > 0);  // 应该有解

    // 验证解的正确性
    Eigen::Matrix4d verification_pose = kinematics_solver_->forwardKinematics(solution);
    Eigen::Matrix4d pose_error = verification_pose.inverse() * target_pose;

    double position_error = pose_error.block<3, 1>(0, 3).norm();
    EXPECT_LT(position_error, 1e-3);

    // 姿态误差（简化检查）
    double orientation_error = (pose_error.block<3, 3>(0, 0) - Eigen::Matrix3d::Identity()).norm();
    EXPECT_LT(orientation_error, 1e-2);
}

TEST(MathUtilsTest, VectorNorm) {
    Eigen::Vector3d vec(3.0, 4.0, 0.0);
    double norm = leee::MathUtils::norm(vec);
    EXPECT_NEAR(norm, 5.0, 1e-6);
}

TEST(MathUtilsTest, VectorDotProduct) {
    Eigen::Vector3d a(1.0, 2.0, 3.0);
    Eigen::Vector3d b(4.0, 5.0, 6.0);
    double dot = leee::MathUtils::dot(a, b);
    EXPECT_NEAR(dot, 32.0, 1e-6);
}

TEST(MathUtilsTest, VectorCrossProduct) {
    Eigen::Vector3d a(1.0, 0.0, 0.0);
    Eigen::Vector3d b(0.0, 1.0, 0.0);
    Eigen::Vector3d cross = leee::MathUtils::cross(a, b);
    Eigen::Vector3d expected(0.0, 0.0, 1.0);

    for (int i = 0; i < 3; ++i) {
        EXPECT_NEAR(cross[i], expected[i], 1e-6);
    }
}

TEST(MathUtilsTest, RotationMatrix) {
    Eigen::Vector3d axis(0.0, 0.0, 1.0);  // Z轴
    double angle = M_PI / 2;  // 90度

    Eigen::Matrix3d rotation = leee::MathUtils::rotationMatrix(axis, angle);

    // 90度绕Z轴旋转矩阵
    Eigen::Matrix3d expected;
    expected << 0, -1, 0,
                1,  0, 0,
                0,  0, 1;

    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            EXPECT_NEAR(rotation(i, j), expected(i, j), 1e-6);
        }
    }
}

TEST(MathUtilsTest, TransformationMatrix) {
    Eigen::Matrix3d rotation = Eigen::Matrix3d::Identity();
    Eigen::Vector3d translation(1.0, 2.0, 3.0);

    Eigen::Matrix4d transform = leee::MathUtils::transformationMatrix(rotation, translation);

    EXPECT_NEAR(transform(0, 3), 1.0, 1e-6);
    EXPECT_NEAR(transform(1, 3), 2.0, 1e-6);
    EXPECT_NEAR(transform(2, 3), 3.0, 1e-6);
    EXPECT_NEAR(transform(3, 3), 1.0, 1e-6);
}
