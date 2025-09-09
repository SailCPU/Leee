#pragma once

/**
 * @file controller.h
 * @brief 机器人控制算法库
 */

#include <Eigen/Dense>
#include <memory>
#include <vector>

namespace leee {

/**
 * @brief 控制算法基类
 */
class Controller {
public:
    /**
     * @brief 计算控制输出
     * @param current_state 当前状态
     * @param desired_state 期望状态
     * @param dt 时间步长
     * @return 控制输出
     */
    virtual Eigen::VectorXd computeControl(const Eigen::VectorXd& current_state,
                                         const Eigen::VectorXd& desired_state,
                                         double dt) = 0;

    /**
     * @brief 重置控制器状态
     */
    virtual void reset() = 0;

    /**
     * @brief 设置控制器参数
     * @param params 参数向量
     */
    virtual void setParameters(const Eigen::VectorXd& params) = 0;

    /**
     * @brief 获取控制器参数
     * @return 参数向量
     */
    virtual Eigen::VectorXd getParameters() const = 0;

    virtual ~Controller() = default;
};

/**
 * @brief PID控制器
 */
class PIDController : public Controller {
private:
    Eigen::VectorXd kp_;        // 比例增益
    Eigen::VectorXd ki_;        // 积分增益
    Eigen::VectorXd kd_;        // 微分增益
    Eigen::VectorXd integral_;  // 积分项
    Eigen::VectorXd prev_error_; // 上一次误差
    Eigen::VectorXd integral_limits_; // 积分限幅
    double dt_;                 // 时间步长

public:
    /**
     * @brief 构造函数
     * @param kp 比例增益
     * @param ki 积分增益
     * @param kd 微分增益
     * @param integral_limits 积分限幅
     */
    PIDController(const Eigen::VectorXd& kp,
                  const Eigen::VectorXd& ki,
                  const Eigen::VectorXd& kd,
                  const Eigen::VectorXd& integral_limits = Eigen::VectorXd());

    Eigen::VectorXd computeControl(const Eigen::VectorXd& current_state,
                                 const Eigen::VectorXd& desired_state,
                                 double dt) override;

    void reset() override;
    void setParameters(const Eigen::VectorXd& params) override;
    Eigen::VectorXd getParameters() const override;
};

/**
 * @brief 阻抗控制器
 */
class ImpedanceController : public Controller {
private:
    Eigen::MatrixXd stiffness_;     // 刚度矩阵
    Eigen::MatrixXd damping_;       // 阻尼矩阵
    Eigen::VectorXd equilibrium_;   // 平衡位置
    double dt_;

public:
    /**
     * @brief 构造函数
     * @param stiffness 刚度矩阵
     * @param damping 阻尼矩阵
     * @param equilibrium 平衡位置
     */
    ImpedanceController(const Eigen::MatrixXd& stiffness,
                       const Eigen::MatrixXd& damping,
                       const Eigen::VectorXd& equilibrium);

    Eigen::VectorXd computeControl(const Eigen::VectorXd& current_state,
                                 const Eigen::VectorXd& desired_state,
                                 double dt) override;

    void reset() override;
    void setParameters(const Eigen::VectorXd& params) override;
    Eigen::VectorXd getParameters() const override;

    /**
     * @brief 设置刚度矩阵
     * @param stiffness 刚度矩阵
     */
    void setStiffness(const Eigen::MatrixXd& stiffness);

    /**
     * @brief 设置阻尼矩阵
     * @param damping 阻尼矩阵
     */
    void setDamping(const Eigen::MatrixXd& damping);
};

/**
 * @brief 自适应控制器
 */
class AdaptiveController : public Controller {
private:
    Eigen::MatrixXd adaptive_gain_;     // 自适应增益矩阵
    Eigen::VectorXd parameter_estimates_; // 参数估计
    Eigen::MatrixXd regression_matrix_;   // 回归矩阵
    Eigen::VectorXd error_integral_;     // 误差积分
    double gamma_;                       // 自适应率
    double dt_;

public:
    /**
     * @brief 构造函数
     * @param initial_params 初始参数估计
     * @param adaptive_gain 自适应增益
     * @param gamma 自适应率
     */
    AdaptiveController(const Eigen::VectorXd& initial_params,
                      const Eigen::MatrixXd& adaptive_gain,
                      double gamma);

    Eigen::VectorXd computeControl(const Eigen::VectorXd& current_state,
                                 const Eigen::VectorXd& desired_state,
                                 double dt) override;

    void reset() override;
    void setParameters(const Eigen::VectorXd& params) override;
    Eigen::VectorXd getParameters() const override;

    /**
     * @brief 获取当前参数估计
     * @return 参数估计向量
     */
    Eigen::VectorXd getParameterEstimates() const;
};

} // namespace leee
