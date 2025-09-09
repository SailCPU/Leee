#!/usr/bin/env python3
"""
抓取与放置任务演示

演示完整的机器人抓取与放置任务流程
"""

import numpy as np
import time
import sys
import os

# 添加项目路径
sys.path.append(os.path.join(os.path.dirname(__file__), '../..'))

from python.robotics.robot_kinematics import RobotKinematics, TrajectoryPlanner


class PickAndPlaceDemo:
    """抓取与放置任务演示类"""

    def __init__(self):
        # 初始化机器人模型
        dh_params = [
            (0.0, np.pi/2, 0.4, 0.0),    # 关节1
            (0.3, 0.0, 0.0, 0.0),        # 关节2
            (0.0, np.pi/2, 0.0, np.pi/2), # 关节3
            (0.0, -np.pi/2, 0.25, 0.0),  # 关节4
            (0.0, np.pi/2, 0.0, 0.0),    # 关节5
            (0.0, 0.0, 0.05, 0.0)        # 关节6
        ]

        self.robot = RobotKinematics(dh_params)
        self.planner = TrajectoryPlanner(max_velocity=0.5, max_acceleration=0.3)

        # 定义关键位置
        self.home_position = np.array([0.3, 0.0, 0.4])      # 家位置
        self.pick_approach = np.array([0.4, 0.2, 0.25])     # 抓取接近点
        self.pick_position = np.array([0.4, 0.2, 0.15])     # 抓取位置
        self.place_approach = np.array([0.4, -0.2, 0.25])   # 放置接近点
        self.place_position = np.array([0.4, -0.2, 0.15])   # 放置位置

        # 任务状态
        self.current_position = self.home_position.copy()
        self.gripper_closed = False

        print("Pick and Place 演示初始化完成")
        print(f"机器人DOF: {self.robot.num_joints}")
        print(f"工作空间: {self.home_position} (起始位置)")

    def move_to_position(self, target_position, orientation=None):
        """
        移动到指定位置

        Args:
            target_position: 目标位置 [x, y, z]
            orientation: 目标姿态 (可选)
        """
        print(".3f"
        # 构建目标位姿
        target_pose = np.eye(4)
        target_pose[:3, 3] = target_position

        if orientation is not None:
            target_pose[:3, :3] = orientation
        else:
            # 使用默认姿态
            target_pose[:3, :3] = np.eye(3)

        # 计算逆向运动学
        solution = self.robot.inverse_kinematics(target_pose)

        if solution is not None:
            print("  运动学求解成功")

            # 规划轨迹
            time_points, positions, velocities = self.planner.plan_trajectory(
                self.current_position, target_position, time_horizon=2.0
            )

            # 模拟执行轨迹
            for i, pos in enumerate(positions):
                self.current_position = pos
                if i % 10 == 0:  # 每10个点显示一次
                    print(".3f"                time.sleep(0.05)  # 模拟执行时间

            print(".3f"            return True
        else:
            print("  运动学求解失败")
            return False

    def control_gripper(self, close):
        """
        控制夹爪

        Args:
            close: True-关闭夹爪, False-张开夹爪
        """
        action = "关闭" if close else "张开"
        print(f"夹爪{action}...")

        # 模拟夹爪动作
        time.sleep(0.5)

        self.gripper_closed = close
        status = "闭合" if close else "张开"
        print(f"夹爪{status}")

    def execute_pick_and_place(self):
        """执行抓取与放置任务"""
        print("\n开始执行 Pick and Place 任务")
        print("=" * 40)

        # 步骤1: 从家位置移动到抓取接近点
        print("\n步骤1: 移动到抓取接近点")
        success = self.move_to_position(self.pick_approach)
        if not success:
            print("任务失败: 无法到达抓取接近点")
            return False

        # 步骤2: 下降到抓取位置
        print("\n步骤2: 下降到抓取位置")
        success = self.move_to_position(self.pick_position)
        if not success:
            print("任务失败: 无法到达抓取位置")
            return False

        # 步骤3: 关闭夹爪抓取物体
        print("\n步骤3: 抓取物体")
        self.control_gripper(close=True)

        # 步骤4: 抬升到接近点
        print("\n步骤4: 抬升到接近点")
        success = self.move_to_position(self.pick_approach)
        if not success:
            print("任务失败: 无法抬升")
            return False

        # 步骤5: 移动到放置接近点
        print("\n步骤5: 移动到放置接近点")
        success = self.move_to_position(self.place_approach)
        if not success:
            print("任务失败: 无法到达放置接近点")
            return False

        # 步骤6: 下降到放置位置
        print("\n步骤6: 下降到放置位置")
        success = self.move_to_position(self.place_position)
        if not success:
            print("任务失败: 无法到达放置位置")
            return False

        # 步骤7: 张开夹爪释放物体
        print("\n步骤7: 释放物体")
        self.control_gripper(close=False)

        # 步骤8: 抬升到接近点
        print("\n步骤8: 抬升到接近点")
        success = self.move_to_position(self.place_approach)
        if not success:
            print("任务失败: 无法抬升")
            return False

        # 步骤9: 返回家位置
        print("\n步骤9: 返回家位置")
        success = self.move_to_position(self.home_position)
        if not success:
            print("任务失败: 无法返回家位置")
            return False

        print("\n任务执行成功!")
        return True

    def run_multiple_cycles(self, num_cycles=3):
        """运行多个抓取放置周期"""
        print(f"\n开始运行 {num_cycles} 个抓取放置周期")
        print("=" * 50)

        success_count = 0

        for cycle in range(1, num_cycles + 1):
            print(f"\n=== 周期 {cycle}/{num_cycles} ===")

            try:
                if self.execute_pick_and_place():
                    success_count += 1
                    print(f"周期 {cycle}: 成功")
                else:
                    print(f"周期 {cycle}: 失败")

                # 周期间暂停
                if cycle < num_cycles:
                    print(f"等待2秒后开始下一个周期...")
                    time.sleep(2)

            except KeyboardInterrupt:
                print(f"\n用户在周期 {cycle} 中断任务")
                break
            except Exception as e:
                print(f"周期 {cycle} 发生错误: {e}")

        print("
执行完成统计:"        print(f"总周期数: {num_cycles}")
        print(f"成功周期数: {success_count}")
        print(".1f"
    def demonstrate_kinematics(self):
        """演示运动学计算"""
        print("\n运动学计算演示")
        print("=" * 20)

        # 设置一个测试关节配置
        test_angles = np.array([0.2, 0.3, -0.1, 0.4, -0.2, 0.1])

        print("测试关节角度 (度):", np.degrees(test_angles))

        # 正向运动学
        pose = self.robot.forward_kinematics(test_angles)
        position = pose[:3, 3]

        print(".3f"
        # 计算雅可比矩阵
        jacobian = self.robot.compute_jacobian(test_angles)
        print(f"雅可比矩阵形状: {jacobian.shape}")
        print(f"雅可比矩阵条件数: {np.linalg.cond(jacobian):.2f}")

        # 力矩计算（模拟）
        joint_torques = np.array([10, 8, 5, 3, 2, 1])  # Nm
        end_force = jacobian.T @ joint_torques
        print(f"关节力矩: {joint_torques} Nm")
        print(".1f"
    def interactive_mode(self):
        """交互模式"""
        print("\n进入交互模式")
        print("=" * 15)
        print("可用命令:")
        print("  p - 执行Pick and Place任务")
        print("  m - 移动到指定位置")
        print("  g - 控制夹爪 (open/close)")
        print("  k - 运动学计算演示")
        print("  s - 显示当前状态")
        print("  h - 返回家位置")
        print("  q - 退出")

        while True:
            try:
                cmd = input("\n命令> ").strip().lower()

                if cmd == 'q':
                    break
                elif cmd == 'p':
                    self.execute_pick_and_place()
                elif cmd == 'h':
                    self.move_to_position(self.home_position)
                elif cmd == 'k':
                    self.demonstrate_kinematics()
                elif cmd == 'g':
                    action = input("夹爪动作 (open/close): ").strip().lower()
                    if action in ['open', 'o']:
                        self.control_gripper(close=False)
                    elif action in ['close', 'c']:
                        self.control_gripper(close=True)
                    else:
                        print("无效动作")
                elif cmd == 's':
                    print(".3f"                          f"夹爪状态: {'闭合' if self.gripper_closed else '张开'}")
                elif cmd == 'm':
                    try:
                        x = float(input("X坐标: "))
                        y = float(input("Y坐标: "))
                        z = float(input("Z坐标: "))
                        target = np.array([x, y, z])
                        self.move_to_position(target)
                    except ValueError:
                        print("无效坐标")
                else:
                    print("未知命令")

            except KeyboardInterrupt:
                print("\n退出交互模式")
                break
            except Exception as e:
                print(f"错误: {e}")


def main():
    print("Leee Pick and Place 演示")
    print("========================")

    demo = PickAndPlaceDemo()

    # 显示菜单
    print("\n选择演示模式:")
    print("1. 单次 Pick and Place 任务")
    print("2. 多次周期执行")
    print("3. 运动学计算演示")
    print("4. 交互模式")

    try:
        choice = input("请选择 (1-4): ").strip()

        if choice == '1':
            demo.execute_pick_and_place()
        elif choice == '2':
            num_cycles = int(input("输入周期数 (默认3): ") or "3")
            demo.run_multiple_cycles(num_cycles)
        elif choice == '3':
            demo.demonstrate_kinematics()
        elif choice == '4':
            demo.interactive_mode()
        else:
            print("无效选择")

    except KeyboardInterrupt:
        print("\n用户中断")
    except Exception as e:
        print(f"错误: {e}")
        import traceback
        traceback.print_exc()

    print("\n演示结束")


if __name__ == "__main__":
    main()
