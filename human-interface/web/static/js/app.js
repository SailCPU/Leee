// Leee Web界面JavaScript应用

class LeeeWebInterface {
    constructor() {
        this.socket = null;
        this.robotState = {
            joint_angles: [0, 0, 0, 0, 0, 0],
            end_effector_pose: [0, 0, 0, 0, 0, 0],
            joint_velocities: [0, 0, 0, 0, 0, 0],
            joint_torques: [0, 0, 0, 0, 0, 0],
            is_connected: false,
            last_update: Date.now()
        };
        this.torqueChart = null;
        this.jointSliders = [];
        this.jointValues = [];

        this.init();
    }

    init() {
        this.connectWebSocket();
        this.setupUI();
        this.setupCharts();
        this.setupEventListeners();
        this.log('Leee Web界面初始化完成', 'info');
    }

    connectWebSocket() {
        this.socket = io();

        this.socket.on('connect', () => {
            this.log('WebSocket连接成功', 'success');
            this.updateConnectionStatus(true);
            this.socket.emit('subscribe_state');
        });

        this.socket.on('disconnect', () => {
            this.log('WebSocket连接断开', 'warning');
            this.updateConnectionStatus(false);
        });

        this.socket.on('state_update', (data) => {
            this.updateRobotState(data);
        });

        this.socket.on('command', (data) => {
            this.log(`收到命令: ${data.type}`, 'info');
        });

        this.socket.on('control_ack', (data) => {
            this.log(`控制确认: 关节${data.joint_id} -> ${data.angle.toFixed(3)}`, 'success');
        });

        this.socket.on('status', (data) => {
            this.log(`状态: ${data.message}`, 'info');
        });
    }

    setupUI() {
        // 创建关节角度显示
        this.createJointDisplays();

        // 创建关节控制滑块
        this.createJointControls();

        // 创建位姿显示
        this.createPoseDisplays();
    }

    createJointDisplays() {
        const jointAnglesDiv = document.getElementById('joint-angles');

        for (let i = 0; i < 6; i++) {
            const jointDiv = document.createElement('div');
            jointDiv.className = 'joint-item';
            jointDiv.innerHTML = `
                <span>关节 ${i + 1}:</span>
                <span id="joint-${i}-display">0.000°</span>
            `;
            jointAnglesDiv.appendChild(jointDiv);
        }
    }

    createJointControls() {
        const jointControlsDiv = document.getElementById('joint-controls');

        for (let i = 0; i < 6; i++) {
            const controlDiv = document.createElement('div');
            controlDiv.className = 'joint-control-item';
            controlDiv.innerHTML = `
                <label for="joint-${i}-slider">关节 ${i + 1}:</label>
                <input type="range" id="joint-${i}-slider" class="joint-slider"
                       min="-180" max="180" step="0.1" value="0">
                <div class="slider-value" id="joint-${i}-value">0.000°</div>
            `;
            jointControlsDiv.appendChild(controlDiv);

            // 存储滑块和值显示元素的引用
            this.jointSliders.push(document.getElementById(`joint-${i}-slider`));
            this.jointValues.push(document.getElementById(`joint-${i}-value`));

            // 添加事件监听器
            this.jointSliders[i].addEventListener('input', (e) => {
                const angle = parseFloat(e.target.value);
                this.jointValues[i].textContent = `${angle.toFixed(3)}°`;
            });

            this.jointSliders[i].addEventListener('change', (e) => {
                const angle = parseFloat(e.target.value);
                this.sendJointCommand(i, angle * Math.PI / 180); // 转换为弧度
            });
        }
    }

    createPoseDisplays() {
        const poseDiv = document.getElementById('end-effector-pose');
        const labels = ['X', 'Y', 'Z', 'Rx', 'Ry', 'Rz'];
        const units = ['m', 'm', 'm', '°', '°', '°'];

        for (let i = 0; i < 6; i++) {
            const poseItem = document.createElement('div');
            poseItem.className = 'pose-item';
            poseItem.innerHTML = `
                <span>${labels[i]}:</span>
                <span id="pose-${i}-display">0.000 ${units[i]}</span>
            `;
            poseDiv.appendChild(poseItem);
        }
    }

    setupCharts() {
        const ctx = document.getElementById('torque-chart').getContext('2d');
        this.torqueChart = new Chart(ctx, {
            type: 'line',
            data: {
                labels: ['关节1', '关节2', '关节3', '关节4', '关节5', '关节6'],
                datasets: [{
                    label: '关节力矩 (Nm)',
                    data: [0, 0, 0, 0, 0, 0],
                    borderColor: '#2563eb',
                    backgroundColor: 'rgba(37, 99, 235, 0.1)',
                    tension: 0.4,
                    fill: true
                }]
            },
            options: {
                responsive: true,
                maintainAspectRatio: false,
                scales: {
                    y: {
                        beginAtZero: true,
                        title: {
                            display: true,
                            text: '力矩 (Nm)'
                        }
                    }
                },
                plugins: {
                    legend: {
                        display: false
                    }
                }
            }
        });
    }

    setupEventListeners() {
        // 控制按钮事件
        document.getElementById('home-btn').addEventListener('click', () => {
            this.sendCommand('home', {});
        });

        document.getElementById('stop-btn').addEventListener('click', () => {
            this.sendCommand('stop', {});
        });

        document.getElementById('record-btn').addEventListener('click', (e) => {
            const isRecording = e.target.textContent === '停止录制';
            e.target.textContent = isRecording ? '开始录制' : '停止录制';
            this.sendCommand(isRecording ? 'stop_record' : 'start_record', {});
        });

        // 轨迹规划事件
        document.getElementById('move-to-btn').addEventListener('click', () => {
            const targetX = parseFloat(document.getElementById('target-x').value);
            const targetY = parseFloat(document.getElementById('target-y').value);
            const targetZ = parseFloat(document.getElementById('target-z').value);

            this.sendCommand('move_to', {
                position: [targetX, targetY, targetZ],
                orientation: [0, 0, 0] // 默认姿态
            });
        });

        // 配置按钮事件
        document.getElementById('save-config-btn').addEventListener('click', () => {
            this.saveConfiguration();
        });

        document.getElementById('load-config-btn').addEventListener('click', () => {
            this.loadConfiguration();
        });

        document.getElementById('reset-config-btn').addEventListener('click', () => {
            this.resetConfiguration();
        });
    }

    updateRobotState(state) {
        this.robotState = { ...this.robotState, ...state };
        this.robotState.last_update = Date.now();

        // 更新关节角度显示
        for (let i = 0; i < 6; i++) {
            const angle = this.robotState.joint_angles[i];
            const display = document.getElementById(`joint-${i}-display`);
            if (display) {
                display.textContent = `${(angle * 180 / Math.PI).toFixed(3)}°`;
            }

            // 更新滑块位置（如果不是用户正在控制）
            if (this.jointSliders[i] && !this.jointSliders[i].matches(':active')) {
                this.jointSliders[i].value = angle * 180 / Math.PI;
                this.jointValues[i].textContent = `${(angle * 180 / Math.PI).toFixed(3)}°`;
            }
        }

        // 更新位姿显示
        const poseLabels = ['X', 'Y', 'Z', 'Rx', 'Ry', 'Rz'];
        const poseUnits = ['m', 'm', 'm', '°', '°', '°'];

        for (let i = 0; i < 6; i++) {
            const value = this.robotState.end_effector_pose[i];
            const display = document.getElementById(`pose-${i}-display`);
            if (display) {
                display.textContent = `${value.toFixed(3)} ${poseUnits[i]}`;
            }
        }

        // 更新力矩图表
        if (this.torqueChart) {
            this.torqueChart.data.datasets[0].data = this.robotState.joint_torques;
            this.torqueChart.update();
        }

        // 更新最后更新时间
        const lastUpdateSpan = document.getElementById('last-update');
        if (lastUpdateSpan) {
            const now = new Date();
            lastUpdateSpan.textContent = `最后更新: ${now.toLocaleTimeString()}`;
        }
    }

    updateConnectionStatus(isConnected) {
        const statusSpan = document.getElementById('connection-status');
        if (statusSpan) {
            statusSpan.textContent = isConnected ? '在线' : '离线';
            statusSpan.className = isConnected ? 'status-online' : 'status-offline';
        }
    }

    sendJointCommand(jointId, angle) {
        if (this.socket && this.socket.connected) {
            this.socket.emit('joint_control', {
                joint_id: jointId,
                angle: angle
            });
        }
    }

    sendCommand(type, parameters) {
        if (this.socket && this.socket.connected) {
            this.socket.emit('command', {
                type: type,
                parameters: parameters
            });
            this.log(`发送命令: ${type}`, 'info');
        } else {
            this.log('无法发送命令：WebSocket未连接', 'error');
        }
    }

    async saveConfiguration() {
        try {
            const response = await fetch('/api/robot/config', {
                method: 'POST',
                headers: {
                    'Content-Type': 'application/json'
                },
                body: JSON.stringify({
                    control_mode: document.getElementById('control-mode').value,
                    max_velocity: parseFloat(document.getElementById('max-velocity').value),
                    max_acceleration: parseFloat(document.getElementById('max-acceleration').value)
                })
            });

            if (response.ok) {
                this.log('配置保存成功', 'success');
            } else {
                this.log('配置保存失败', 'error');
            }
        } catch (error) {
            this.log(`配置保存错误: ${error.message}`, 'error');
        }
    }

    async loadConfiguration() {
        try {
            const response = await fetch('/api/robot/config');
            const data = await response.json();

            if (data.status === 'success') {
                const config = data.data;
                document.getElementById('control-mode').value = config.control_mode;
                document.getElementById('max-velocity').value = config.velocity_limits[0];
                document.getElementById('max-acceleration').value = config.acceleration_limits[0];
                this.log('配置加载成功', 'success');
            } else {
                this.log('配置加载失败', 'error');
            }
        } catch (error) {
            this.log(`配置加载错误: ${error.message}`, 'error');
        }
    }

    resetConfiguration() {
        document.getElementById('control-mode').value = 'position';
        document.getElementById('max-velocity').value = '1.0';
        document.getElementById('max-acceleration').value = '0.5';
        this.log('配置已重置', 'info');
    }

    log(message, level = 'info') {
        const logContainer = document.getElementById('log-container');
        if (!logContainer) return;

        const logEntry = document.createElement('div');
        logEntry.className = `log-entry log-${level}`;

        const timestamp = new Date().toLocaleTimeString();
        logEntry.textContent = `[${timestamp}] ${message}`;

        logContainer.appendChild(logEntry);
        logContainer.scrollTop = logContainer.scrollHeight;

        // 限制日志条目数量
        while (logContainer.children.length > 100) {
            logContainer.removeChild(logContainer.firstChild);
        }
    }
}

// 页面加载完成后初始化应用
document.addEventListener('DOMContentLoaded', () => {
    window.leeeApp = new LeeeWebInterface();
});

// 页面卸载时清理资源
window.addEventListener('beforeunload', () => {
    if (window.leeeApp && window.leeeApp.socket) {
        window.leeeApp.socket.disconnect();
    }
});
