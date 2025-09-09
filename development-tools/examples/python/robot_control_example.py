#!/usr/bin/env python3
"""
机器人控制示例程序

演示如何使用Leee进行机器人运动控制
"""

import numpy as np
import time
import sys
import os

# 添加项目路径
sys.path.append(os.path.join(os.path.dirname(__file__), '../..'))

from python.robotics.robot_kinematics import RobotKinematics, TrajectoryPlanner


def main():
    print("Leee 机器人控制示例")
    print("====================")

    # 1. 创建机器人运动学模型
    print("\n1. 初始化机器人运动学模型")
    print("-" * 30)

    # 6DOF机械臂的DH参数 (a, alpha, d, theta)
    dh_params = [
        (0.0, np.pi/2, 0.4, 0.0),    # 关节1
        (0.3, 0.0, 0.0, 0.0),        # 关节2
        (0.0, np.pi/2, 0.0, np.pi/2), # 关节3
        (0.0, -np.pi/2, 0.25, 0.0),  # 关节4
        (0.0, np.pi/2, 0.0, 0.0),    # 关节5
        (0.0, 0.0, 0.05, 0.0)        # 关节6
    ]

    robot = RobotKinematics(dh_params)
    print(f"机器人DOF: {robot.num_joints}")
    print("DH参数已设置")

    # 2. 正向运动学演示
    print("\n2. 正向运动学演示")
    print("-" * 20)

    # 设置关节角度
    joint_angles = np.array([0.1, 0.2, -0.1, 0.3, -0.2, 0.1])
    print(f"关节角度: {np.degrees(joint_angles)} 度")

    # 计算正向运动学
    pose = robot.forward_kinematics(joint_angles)
    position = pose[:3, 3]
    orientation = pose[:3, :3]

    print(f"末端位置: [{position[0]:.3f}, {position[1]:.3f}, {position[2]:.3f}] m")

    # 计算姿态角（Z-Y-X欧拉角）
    euler_angles = rotation_matrix_to_euler_angles(orientation)
    print(f"姿态角度: [{np.degrees(euler_angles[0]):.1f}, {np.degrees(euler_angles[1]):.1f}, {np.degrees(euler_angles[2]):.1f}] 度")

    # 3. 逆向运动学演示
    print("\n3. 逆向运动学演示")
    print("-" * 20)

    # 设置目标位置
    target_position = np.array([0.4, 0.2, 0.3])
    target_orientation = euler_angles_to_rotation_matrix([0.1, 0.2, -0.1])

    # 构建目标位姿
    target_pose = np.eye(4)
    target_pose[:3, :3] = target_orientation
    target_pose[:3, 3] = target_position

    print(f"目标位置: [{target_position[0]:.3f}, {target_position[1]:.3f}, {target_position[2]:.3f}] m")

    # 计算逆向运动学
    initial_guess = np.zeros(6)
    solution = robot.inverse_kinematics(target_pose, initial_guess)

    if solution is not None:
        print("逆向运动学求解成功!")
        print(f"关节角度解: {np.degrees(solution)} 度")

        # 验证解
        verification_pose = robot.forward_kinematics(solution)
        error = np.linalg.norm(verification_pose - target_pose)
        print(".6f")
    else:
        print("逆向运动学求解失败")

    # 4. 轨迹规划演示
    print("\n4. 轨迹规划演示")
    print("-" * 18)

    planner = TrajectoryPlanner(max_velocity=0.5, max_acceleration=0.2)

    start_pos = np.array([0.3, 0.0, 0.2])
    end_pos = np.array([0.5, 0.3, 0.4])

    print(f"起始位置: {start_pos}")
    print(f"结束位置: {end_pos}")

    # 规划轨迹
    time_points, positions, velocities = planner.plan_trajectory(
        start_pos, end_pos,
        time_horizon=2.0
    )

    print(f"轨迹时间: {time_points[0]:.1f} - {time_points[-1]:.1f} s")
    print(f"轨迹点数: {len(time_points)}")

    # 显示轨迹的一些关键点
    key_indices = [0, len(time_points)//4, len(time_points)//2, 3*len(time_points)//4, -1]
    print("\n轨迹关键点:")
    print("时间(s)  位置(m)                    速度(m/s)")
    print("-" * 50)
    for idx in key_indices:
        pos = positions[idx]
        vel = velocities[idx]
        print("6.2f")

    # 5. 实时仿真演示
    print("\n5. 实时控制仿真")
    print("-" * 18)

    print("开始关节角度跟踪演示...")

    # 创建一个简单的正弦轨迹
    simulation_time = 5.0
    dt = 0.1
    current_time = 0.0

    target_angles = np.zeros(6)
    current_angles = np.zeros(6)

    while current_time < simulation_time:
        # 更新目标角度（正弦波）
        for i in range(6):
            target_angles[i] = 0.5 * np.sin(2 * np.pi * 0.2 * current_time + i * np.pi / 3)

        # 简单的PD控制（模拟）
        kp = 2.0
        kd = 0.1

        error = target_angles - current_angles
        control_torque = kp * error - kd * 0  # 假设速度为0

        # 更新当前角度（简单的积分）
        current_angles += control_torque * dt

        # 显示当前状态
        if int(current_time * 10) % 5 == 0:  # 每0.5秒显示一次
            print("5.1f"
                  "6.2f"
                  "6.2f")

        current_time += dt
        time.sleep(dt * 0.1)  # 加速仿真

    print("\n仿真完成!")

    print("\n示例运行完成!")
    return True


def rotation_matrix_to_euler_angles(R):
    """
    将旋转矩阵转换为Z-Y-X欧拉角
    """
    sy = np.sqrt(R[0, 0] * R[0, 0] + R[1, 0] * R[1, 0])

    singular = sy < 1e-6

    if not singular:
        x = np.arctan2(R[2, 1], R[2, 2])
        y = np.arctan2(-R[2, 0], sy)
        z = np.arctan2(R[1, 0], R[0, 0])
    else:
        x = np.arctan2(-R[1, 2], R[1, 1])
        y = np.arctan2(-R[2, 0], sy)
        z = 0

    return np.array([x, y, z])


def euler_angles_to_rotation_matrix(euler_angles):
    """
    将Z-Y-X欧拉角转换为旋转矩阵
    """
    x, y, z = euler_angles

    Rx = np.array([[1, 0, 0],
                   [0, np.cos(x), -np.sin(x)],
                   [0, np.sin(x), np.cos(x)]])

    Ry = np.array([[np.cos(y), 0, np.sin(y)],
                   [0, 1, 0],
                   [-np.sin(y), 0, np.cos(y)]])

    Rz = np.array([[np.cos(z), -np.sin(z), 0],
                   [np.sin(z), np.cos(z), 0],
                   [0, 0, 1]])

    return Rz @ Ry @ Rx


if __name__ == "__main__":
    try:
        main()
    except KeyboardInterrupt:
        print("\n用户中断")
    except Exception as e:
        print(f"错误: {e}")
        import traceback
        traceback.print_exc()
