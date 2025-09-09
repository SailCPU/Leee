#!/usr/bin/env python3
"""
Leee 机器人控制系统 - Python 接口
"""

import sys
import os
import json
import asyncio
import websockets
from typing import Dict, List, Optional, Any
import logging

# 配置日志
logging.basicConfig(level=logging.INFO)
logger = logging.getLogger(__name__)


class RobotInterface:
    """机器人控制接口"""

    def __init__(self, host: str = "localhost", port: int = 8080):
        """
        初始化机器人接口

        Args:
            host: 服务器主机地址
            port: 服务器端口
        """
        self.host = host
        self.port = port
        self.websocket = None
        self.connected = False
        self.robot_status = {}

    async def connect(self) -> bool:
        """
        连接到机器人服务器

        Returns:
            连接是否成功
        """
        try:
            uri = f"ws://{self.host}:{self.port}/robot"
            self.websocket = await websockets.connect(uri)
            self.connected = True
            logger.info(f"Connected to robot server at {uri}")
            return True
        except Exception as e:
            logger.error(f"Failed to connect to robot server: {e}")
            return False

    async def disconnect(self):
        """断开连接"""
        if self.websocket:
            await self.websocket.close()
            self.connected = False
            logger.info("Disconnected from robot server")

    async def send_command(self, command: str, params: Dict[str, Any] = None) -> Dict[str, Any]:
        """
        发送命令到机器人

        Args:
            command: 命令名称
            params: 命令参数

        Returns:
            命令执行结果
        """
        if not self.connected:
            return {"success": False, "error": "Not connected to robot server"}

        try:
            message = {
                "command": command,
                "params": params or {}
            }

            await self.websocket.send(json.dumps(message))
            response = await self.websocket.recv()
            result = json.loads(response)

            logger.info(f"Command '{command}' executed: {result}")
            return result

        except Exception as e:
            logger.error(f"Failed to send command '{command}': {e}")
            return {"success": False, "error": str(e)}

    async def get_status(self) -> Dict[str, Any]:
        """
        获取机器人状态

        Returns:
            机器人状态信息
        """
        return await self.send_command("get_status")

    async def move_to_position(self, x: float, y: float, z: float) -> Dict[str, Any]:
        """
        移动到指定位置

        Args:
            x: X坐标
            y: Y坐标
            z: Z坐标

        Returns:
            移动结果
        """
        params = {"x": x, "y": y, "z": z}
        return await self.send_command("move_to", params)

    async def execute_task(self, task_name: str, task_params: Dict[str, Any] = None) -> Dict[str, Any]:
        """
        执行任务

        Args:
            task_name: 任务名称
            task_params: 任务参数

        Returns:
            任务执行结果
        """
        params = {"task_name": task_name, "task_params": task_params or {}}
        return await self.send_command("execute_task", params)


class RobotController:
    """机器人控制器"""

    def __init__(self):
        self.interface = None
        self.tasks = []

    async def initialize(self, host: str = "localhost", port: int = 8080) -> bool:
        """
        初始化控制器

        Args:
            host: 服务器主机地址
            port: 服务器端口

        Returns:
            初始化是否成功
        """
        self.interface = RobotInterface(host, port)
        return await self.interface.connect()

    async def shutdown(self):
        """关闭控制器"""
        if self.interface:
            await self.interface.disconnect()

    async def get_robot_status(self) -> Dict[str, Any]:
        """
        获取机器人状态

        Returns:
            机器人状态
        """
        if not self.interface:
            return {"error": "Controller not initialized"}

        return await self.interface.get_status()

    async def perform_movement(self, movement_type: str, **kwargs) -> Dict[str, Any]:
        """
        执行移动操作

        Args:
            movement_type: 移动类型
            **kwargs: 移动参数

        Returns:
            移动结果
        """
        if not self.interface:
            return {"error": "Controller not initialized"}

        if movement_type == "position":
            return await self.interface.move_to_position(
                kwargs.get("x", 0),
                kwargs.get("y", 0),
                kwargs.get("z", 0)
            )
        else:
            return {"error": f"Unsupported movement type: {movement_type}"}

    async def run_task_sequence(self, tasks: List[Dict[str, Any]]) -> List[Dict[str, Any]]:
        """
        运行任务序列

        Args:
            tasks: 任务列表

        Returns:
            任务执行结果列表
        """
        results = []

        for task in tasks:
            task_name = task.get("name", "")
            task_params = task.get("params", {})

            logger.info(f"Executing task: {task_name}")
            result = await self.interface.execute_task(task_name, task_params)
            results.append(result)

            # 如果任务失败，可以选择停止执行
            if not result.get("success", False):
                logger.error(f"Task '{task_name}' failed: {result.get('error', 'Unknown error')}")
                break

        return results


# 全局控制器实例
_controller = None


async def get_controller() -> RobotController:
    """获取全局控制器实例"""
    global _controller
    if _controller is None:
        _controller = RobotController()
    return _controller


async def initialize_robot(host: str = "localhost", port: int = 8080) -> bool:
    """
    初始化机器人系统

    Args:
        host: 服务器主机地址
        port: 服务器端口

    Returns:
        初始化是否成功
    """
    controller = await get_controller()
    return await controller.initialize(host, port)


async def get_robot_status() -> Dict[str, Any]:
    """
    获取机器人状态

    Returns:
        机器人状态
    """
    controller = await get_controller()
    return await controller.get_robot_status()


async def move_robot(x: float, y: float, z: float) -> Dict[str, Any]:
    """
    移动机器人到指定位置

    Args:
        x: X坐标
        y: Y坐标
        z: Z坐标

    Returns:
        移动结果
    """
    controller = await get_controller()
    return await controller.perform_movement("position", x=x, y=y, z=z)


async def execute_robot_task(task_name: str, **kwargs) -> Dict[str, Any]:
    """
    执行机器人任务

    Args:
        task_name: 任务名称
        **kwargs: 任务参数

    Returns:
        任务执行结果
    """
    controller = await get_controller()
    return await controller.interface.execute_task(task_name, kwargs)


async def shutdown_robot():
    """关闭机器人系统"""
    controller = await get_controller()
    await controller.shutdown()


if __name__ == "__main__":
    # 示例使用
    async def main():
        print("Leee 机器人控制系统 - Python 接口示例")

        # 初始化
        if await initialize_robot():
            print("机器人系统初始化成功")

            # 获取状态
            status = await get_robot_status()
            print(f"机器人状态: {status}")

            # 执行移动
            result = await move_robot(10.0, 20.0, 30.0)
            print(f"移动结果: {result}")

            # 关闭系统
            await shutdown_robot()
            print("机器人系统已关闭")
        else:
            print("机器人系统初始化失败")

    # 运行示例
    asyncio.run(main())
