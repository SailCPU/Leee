#!/usr/bin/env python3
"""
Leee Web监控界面后端服务

提供RESTful API和WebSocket实时通信
"""

from flask import Flask, render_template, request, jsonify
from flask_socketio import SocketIO, emit
import json
import threading
import time
from typing import Dict, Any
import numpy as np

app = Flask(__name__,
            template_folder='templates',
            static_folder='static')
socketio = SocketIO(app, cors_allowed_origins="*")

# 全局状态存储
robot_state = {
    'joint_angles': [0.0] * 6,
    'end_effector_pose': [0.0] * 6,  # [x, y, z, rx, ry, rz]
    'joint_velocities': [0.0] * 6,
    'joint_torques': [0.0] * 6,
    'is_connected': False,
    'last_update': time.time()
}

# 控制命令队列
control_commands = []


@app.route('/')
def index():
    """主页面"""
    return render_template('index.html')


@app.route('/api/robot/state')
def get_robot_state():
    """获取机器人当前状态"""
    return jsonify({
        'status': 'success',
        'data': robot_state
    })


@app.route('/api/robot/command', methods=['POST'])
def send_command():
    """发送控制命令"""
    data = request.get_json()

    if not data:
        return jsonify({'status': 'error', 'message': 'No data provided'})

    command_type = data.get('type')
    parameters = data.get('parameters', {})

    # 添加命令到队列
    control_commands.append({
        'type': command_type,
        'parameters': parameters,
        'timestamp': time.time()
    })

    # 通过WebSocket广播命令
    socketio.emit('command', {
        'type': command_type,
        'parameters': parameters
    })

    return jsonify({
        'status': 'success',
        'message': f'Command {command_type} queued'
    })


@app.route('/api/robot/config', methods=['GET', 'POST'])
def robot_config():
    """机器人配置管理"""
    if request.method == 'GET':
        # 返回当前配置
        config = {
            'joint_limits': {
                'min': [-180] * 6,
                'max': [180] * 6
            },
            'velocity_limits': [100] * 6,
            'acceleration_limits': [50] * 6,
            'control_mode': 'position'
        }
        return jsonify({'status': 'success', 'data': config})

    elif request.method == 'POST':
        # 更新配置
        data = request.get_json()
        # TODO: 实现配置更新逻辑
        return jsonify({'status': 'success', 'message': 'Configuration updated'})


@socketio.on('connect')
def handle_connect():
    """WebSocket连接处理"""
    print('Client connected')
    emit('status', {'message': 'Connected to Leee Web Interface'})


@socketio.on('disconnect')
def handle_disconnect():
    """WebSocket断开连接处理"""
    print('Client disconnected')


@socketio.on('subscribe_state')
def handle_subscribe_state():
    """订阅状态更新"""
    emit('state_update', robot_state)


@socketio.on('joint_control')
def handle_joint_control(data):
    """关节控制"""
    joint_id = data.get('joint_id')
    angle = data.get('angle')

    if joint_id is not None and angle is not None:
        # 更新机器人状态
        robot_state['joint_angles'][joint_id] = angle

        # 发送确认
        emit('control_ack', {
            'joint_id': joint_id,
            'angle': angle,
            'status': 'success'
        })

        # 广播状态更新
        socketio.emit('state_update', robot_state)


def update_robot_state():
    """模拟机器人状态更新"""
    global robot_state

    while True:
        # 模拟关节角度变化
        for i in range(6):
            robot_state['joint_angles'][i] += np.random.normal(0, 0.01)
            robot_state['joint_angles'][i] = np.clip(robot_state['joint_angles'][i], -np.pi, np.pi)

        # 更新末端执行器位姿（简化计算）
        robot_state['end_effector_pose'] = [
            0.5 * np.cos(robot_state['joint_angles'][0]),
            0.5 * np.sin(robot_state['joint_angles'][0]),
            0.3 + 0.2 * np.sin(robot_state['joint_angles'][1]),
            0.0, 0.0, robot_state['joint_angles'][0]
        ]

        robot_state['last_update'] = time.time()

        # 通过WebSocket广播状态更新
        socketio.emit('state_update', robot_state)

        time.sleep(0.1)  # 10Hz更新


def process_control_commands():
    """处理控制命令"""
    global control_commands

    while True:
        if control_commands:
            command = control_commands.pop(0)
            print(f"Processing command: {command}")

            # TODO: 实现具体的控制逻辑
            # 这里可以调用C++后端的控制函数

        time.sleep(0.01)


if __name__ == '__main__':
    # 启动状态更新线程
    state_thread = threading.Thread(target=update_robot_state, daemon=True)
    state_thread.start()

    # 启动命令处理线程
    command_thread = threading.Thread(target=process_control_commands, daemon=True)
    command_thread.start()

    print("Starting Leee Web Interface...")
    socketio.run(app, host='0.0.0.0', port=8080, debug=True)
