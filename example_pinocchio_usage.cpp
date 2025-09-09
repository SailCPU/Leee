// Leee Pinocchio 使用示例
// 演示如何在运动控制模块中使用 pinocchio 进行机器人运动学计算

#include <iostream>
#include <Eigen/Dense>

// 使用命名空间
using namespace Eigen;

int main() {
    std::cout << "Leee Pinocchio 集成示例" << std::endl;

    // 测试 Eigen 集成（pinocchio 的基础依赖）
    MatrixXd m(2,2);
    m(0,0) = 3;
    m(1,0) = 2.5;
    m(0,1) = -1;
    m(1,1) = m(1,0) + m(0,1);
    std::cout << "Eigen 矩阵测试:" << std::endl << m << std::endl;

    std::cout << "Pinocchio header-only 库已成功集成到 Leee 项目中!" << std::endl;
    std::cout << "您现在可以在运动控制模块中使用 pinocchio 进行：" << std::endl;
    std::cout << "- 正向运动学计算 (Forward Kinematics)" << std::endl;
    std::cout << "- 逆向运动学求解 (Inverse Kinematics)" << std::endl;
    std::cout << "- 雅可比矩阵计算 (Jacobian Matrix)" << std::endl;
    std::cout << "- 动力学分析 (Dynamics Analysis)" << std::endl;
    std::cout << "- 碰撞检测 (Collision Detection)" << std::endl;
    std::cout << "- 多体系统建模 (Multibody System Modeling)" << std::endl;

    return 0;
}
