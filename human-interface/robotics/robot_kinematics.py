#!/usr/bin/env python3
"""
机器人运动学模块

提供正向运动学、逆向运动学和轨迹规划功能
"""

import numpy as np
from typing import List, Tuple, Optional


class RobotKinematics:
    """机器人运动学求解器"""

    def __init__(self, dh_params: List[Tuple[float, float, float, float]]):
        """
        初始化机器人运动学求解器

        Args:
            dh_params: DH参数列表 [(a, alpha, d, theta), ...]
        """
        self.dh_params = dh_params
        self.num_joints = len(dh_params)

    def dh_transform(self, a: float, alpha: float, d: float, theta: float) -> np.ndarray:
        """计算DH变换矩阵"""
        ct = np.cos(theta)
        st = np.sin(theta)
        ca = np.cos(alpha)
        sa = np.sin(alpha)

        transform = np.array([
            [ct,    -st*ca,  st*sa,   a*ct],
            [st,     ct*ca, -ct*sa,   a*st],
            [0,      sa,     ca,      d   ],
            [0,      0,      0,       1   ]
        ])

        return transform

    def forward_kinematics(self, joint_angles: np.ndarray) -> np.ndarray:
        """
        正向运动学求解

        Args:
            joint_angles: 关节角度数组 [theta1, theta2, ..., thetaN]

        Returns:
            末端执行器位姿变换矩阵 (4x4)
        """
        if len(joint_angles) != self.num_joints:
            raise ValueError(f"Expected {self.num_joints} joint angles, got {len(joint_angles)}")

        transform = np.eye(4)

        for i, (a, alpha, d, theta_offset) in enumerate(self.dh_params):
            theta = joint_angles[i] + theta_offset
            link_transform = self.dh_transform(a, alpha, d, theta)
            transform = transform @ link_transform

        return transform

    def inverse_kinematics(self, target_pose: np.ndarray,
                          initial_guess: Optional[np.ndarray] = None) -> Optional[np.ndarray]:
        """
        逆向运动学求解（数值方法）

        Args:
            target_pose: 目标位姿变换矩阵 (4x4)
            initial_guess: 初始关节角度猜测

        Returns:
            关节角度解，如果收敛则返回，否则返回None
        """
        if initial_guess is None:
            initial_guess = np.zeros(self.num_joints)

        joint_angles = initial_guess.copy()
        max_iterations = 100
        tolerance = 1e-6
        learning_rate = 0.1

        for iteration in range(max_iterations):
            # 计算当前位姿
            current_pose = self.forward_kinematics(joint_angles)

            # 计算位姿误差
            pose_error = np.linalg.inv(current_pose) @ target_pose
            position_error = pose_error[:3, 3]
            orientation_error = 0.5 * np.array([
                pose_error[2, 1] - pose_error[1, 2],
                pose_error[0, 2] - pose_error[2, 0],
                pose_error[1, 0] - pose_error[0, 1]
            ])

            error_vector = np.concatenate([position_error, orientation_error])
            error_norm = np.linalg.norm(error_vector)

            if error_norm < tolerance:
                return joint_angles

            # 计算雅可比矩阵
            jacobian = self.compute_jacobian(joint_angles)

            # 更新关节角度
            try:
                delta_angles = np.linalg.pinv(jacobian) @ error_vector
                joint_angles += learning_rate * delta_angles
            except np.linalg.LinAlgError:
                return None

        return None

    def compute_jacobian(self, joint_angles: np.ndarray) -> np.ndarray:
        """
        计算雅可比矩阵（数值方法）

        Args:
            joint_angles: 当前关节角度

        Returns:
            雅可比矩阵 (6xN)
        """
        epsilon = 1e-6
        jacobian = np.zeros((6, self.num_joints))

        for i in range(self.num_joints):
            # 正扰动
            angles_plus = joint_angles.copy()
            angles_plus[i] += epsilon
            pose_plus = self.forward_kinematics(angles_plus)

            # 负扰动
            angles_minus = joint_angles.copy()
            angles_minus[i] -= epsilon
            pose_minus = self.forward_kinematics(angles_minus)

            # 计算差分
            diff = (pose_plus - pose_minus) / (2 * epsilon)

            # 提取线速度和角速度
            jacobian[:3, i] = diff[:3, 3]  # 线速度
            jacobian[3:, i] = 0.5 * np.array([
                diff[2, 1] - diff[1, 2],
                diff[0, 2] - diff[2, 0],
                diff[1, 0] - diff[0, 1]
            ])  # 角速度

        return jacobian


class TrajectoryPlanner:
    """轨迹规划器"""

    def __init__(self, max_velocity: float = 1.0, max_acceleration: float = 0.5):
        """
        初始化轨迹规划器

        Args:
            max_velocity: 最大速度
            max_acceleration: 最大加速度
        """
        self.max_velocity = max_velocity
        self.max_acceleration = max_acceleration

    def plan_trajectory(self, start_pos: np.ndarray, end_pos: np.ndarray,
                       start_vel: np.ndarray = None, end_vel: np.ndarray = None,
                       time_horizon: float = 1.0) -> Tuple[np.ndarray, np.ndarray, np.ndarray]:
        """
        规划轨迹（五次多项式）

        Args:
            start_pos: 起始位置
            end_pos: 结束位置
            start_vel: 起始速度
            end_vel: 结束速度
            time_horizon: 时间范围

        Returns:
            时间序列、位置序列、速度序列
        """
        if start_vel is None:
            start_vel = np.zeros_like(start_pos)
        if end_vel is None:
            end_vel = np.zeros_like(start_pos)

        # 五次多项式轨迹规划
        # s(t) = a0 + a1*t + a2*t^2 + a3*t^3 + a4*t^4 + a5*t^5
        # v(t) = a1 + 2*a2*t + 3*a3*t^2 + 4*a4*t^3 + 5*a5*t^4

        t0, tf = 0.0, time_horizon

        # 边界条件
        s0, sf = start_pos, end_pos
        v0, vf = start_vel, end_vel
        a0 = np.zeros_like(start_pos)  # 起始加速度为0
        af = np.zeros_like(start_pos)  # 结束加速度为0

        # 解五次多项式系数
        A = np.array([
            [1, t0, t0**2, t0**3, t0**4, t0**5],
            [0, 1, 2*t0, 3*t0**2, 4*t0**3, 5*t0**4],
            [0, 0, 2, 6*t0, 12*t0**2, 20*t0**3],
            [1, tf, tf**2, tf**3, tf**4, tf**5],
            [0, 1, 2*tf, 3*tf**2, 4*tf**3, 5*tf**4],
            [0, 0, 2, 6*tf, 12*tf**2, 20*tf**3]
        ])

        b = np.column_stack([s0, v0, a0, sf, vf, af])

        coefficients = np.linalg.solve(A, b)

        # 生成轨迹
        num_points = 100
        time_points = np.linspace(t0, tf, num_points)
        position_trajectory = np.zeros((num_points, len(start_pos)))
        velocity_trajectory = np.zeros((num_points, len(start_pos)))

        for i, t in enumerate(time_points):
            t_vec = np.array([1, t, t**2, t**3, t**4, t**5])
            position_trajectory[i] = coefficients.T @ t_vec

            t_vec_vel = np.array([0, 1, 2*t, 3*t**2, 4*t**3, 5*t**4])
            velocity_trajectory[i] = coefficients.T @ t_vec_vel

        return time_points, position_trajectory, velocity_trajectory
