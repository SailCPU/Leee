#!/bin/bash

# Leee 项目部署脚本
# 支持多种部署环境和自动化部署

set -e  # 遇到错误立即退出

# 默认部署参数
DEPLOY_ENV=${DEPLOY_ENV:-development}
DEPLOY_DIR=${DEPLOY_DIR:-/opt/leee}
BACKUP_DIR=${BACKUP_DIR:-/opt/leee_backup}
SERVICE_NAME=${SERVICE_NAME:-leee}
WEB_PORT=${WEB_PORT:-8080}
ENABLE_WEB=${ENABLE_WEB:-true}

# 颜色输出
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

log_info() {
    echo -e "${BLUE}[INFO]${NC} $1"
}

log_success() {
    echo -e "${GREEN}[SUCCESS]${NC} $1"
}

log_warning() {
    echo -e "${YELLOW}[WARNING]${NC} $1"
}

log_error() {
    echo -e "${RED}[ERROR]${NC} $1"
}

# 显示帮助信息
show_help() {
    echo "Leee 项目部署脚本"
    echo ""
    echo "用法: $0 [选项]"
    echo ""
    echo "选项:"
    echo "  -e, --env ENV        部署环境 (development/production) [默认: development]"
    echo "  -d, --dir DIR        部署目录 [默认: /opt/leee]"
    echo "  -b, --backup DIR     备份目录 [默认: /opt/leee_backup]"
    echo "  -s, --service NAME   服务名称 [默认: leee]"
    echo "  -p, --port PORT      Web服务端口 [默认: 8080]"
    echo "  --no-web             禁用Web服务"
    echo "  --rollback           回滚到上一个版本"
    echo "  -h, --help           显示此帮助信息"
    echo ""
    echo "环境变量:"
    echo "  DEPLOY_ENV           部署环境"
    echo "  DEPLOY_DIR           部署目录"
    echo "  BACKUP_DIR           备份目录"
    echo "  SERVICE_NAME         服务名称"
    echo "  WEB_PORT             Web端口"
    echo "  ENABLE_WEB           是否启用Web服务"
}

# 解析命令行参数
parse_args() {
    ROLLBACK=false

    while [[ $# -gt 0 ]]; do
        case $1 in
            -e|--env)
                DEPLOY_ENV="$2"
                shift 2
                ;;
            -d|--dir)
                DEPLOY_DIR="$2"
                shift 2
                ;;
            -b|--backup)
                BACKUP_DIR="$2"
                shift 2
                ;;
            -s|--service)
                SERVICE_NAME="$2"
                shift 2
                ;;
            -p|--port)
                WEB_PORT="$2"
                shift 2
                ;;
            --no-web)
                ENABLE_WEB=false
                shift
                ;;
            --rollback)
                ROLLBACK=true
                shift
                ;;
            -h|--help)
                show_help
                exit 0
                ;;
            *)
                log_error "未知选项: $1"
                show_help
                exit 1
                ;;
        esac
    done
}

# 检查权限
check_permissions() {
    if [ "$EUID" -ne 0 ]; then
        log_warning "建议使用root权限运行部署脚本"
        log_warning "或者确保用户有足够的文件系统权限"
        read -p "是否继续? (y/N): " -n 1 -r
        echo
        if [[ ! $REPLY =~ ^[Yy]$ ]]; then
            exit 1
        fi
    fi
}

# 检查系统要求
check_system_requirements() {
    log_info "检查系统要求..."

    # 检查操作系统
    case "$(uname -s)" in
        Linux)
            log_info "检测到 Linux 系统"
            ;;
        Darwin)
            log_info "检测到 macOS 系统"
            ;;
        *)
            log_error "不支持的操作系统: $(uname -s)"
            exit 1
            ;;
    esac

    # 检查磁盘空间
    AVAILABLE_SPACE=$(df "$DEPLOY_DIR" | tail -1 | awk '{print $4}')
    if [ "$AVAILABLE_SPACE" -lt 1048576 ]; then  # 1GB in KB
        log_warning "可用磁盘空间不足 1GB"
    fi

    # 检查网络连接
    if ping -c 1 8.8.8.8 &> /dev/null; then
        log_info "网络连接正常"
    else
        log_warning "网络连接异常"
    fi

    log_success "系统要求检查完成"
}

# 备份现有安装
backup_existing() {
    if [ -d "$DEPLOY_DIR" ]; then
        log_info "备份现有安装..."

        TIMESTAMP=$(date +"%Y%m%d_%H%M%S")
        BACKUP_PATH="$BACKUP_DIR/leee_$TIMESTAMP"

        mkdir -p "$BACKUP_DIR"
        cp -r "$DEPLOY_DIR" "$BACKUP_PATH"

        log_success "备份完成: $BACKUP_PATH"
    fi
}

# 回滚到上一个版本
rollback_deployment() {
    log_info "开始回滚部署..."

    # 查找最新的备份
    LATEST_BACKUP=$(ls -td "$BACKUP_DIR"/leee_* 2>/dev/null | head -1)

    if [ -z "$LATEST_BACKUP" ]; then
        log_error "未找到可用的备份"
        exit 1
    fi

    log_info "回滚到: $LATEST_BACKUP"

    # 停止服务
    stop_service

    # 恢复备份
    rm -rf "$DEPLOY_DIR"
    cp -r "$LATEST_BACKUP" "$DEPLOY_DIR"

    # 重启服务
    start_service

    log_success "回滚完成"
}

# 创建部署目录结构
create_directories() {
    log_info "创建部署目录结构..."

    mkdir -p "$DEPLOY_DIR"/{bin,lib,include,share,config,log,data,web}
    mkdir -p "$DEPLOY_DIR/web"/{static,templates}

    log_success "目录结构创建完成"
}

# 复制文件
copy_files() {
    log_info "复制项目文件..."

    # 复制构建产物
    if [ -d "build" ]; then
        cp -r build/bin/* "$DEPLOY_DIR/bin/" 2>/dev/null || true
        cp -r build/lib/* "$DEPLOY_DIR/lib/" 2>/dev/null || true
    fi

    # 复制头文件
    if [ -d "include" ]; then
        cp -r include/* "$DEPLOY_DIR/include/"
    fi

    # 复制Python模块
    if [ -d "python" ]; then
        cp -r python "$DEPLOY_DIR/"
    fi

    # 复制Web界面
    if [ -d "web" ]; then
        cp -r web/* "$DEPLOY_DIR/web/"
    fi

    # 复制配置文件
    if [ -d "config" ]; then
        cp -r config/* "$DEPLOY_DIR/config/"
    fi

    log_success "文件复制完成"
}

# 创建配置文件
create_config() {
    log_info "创建配置文件..."

    CONFIG_FILE="$DEPLOY_DIR/config/leee.conf"

    cat > "$CONFIG_FILE" << EOF
# Leee 配置文件
# 自动生成于 $(date)

[general]
environment = $DEPLOY_ENV
log_level = info
log_file = $DEPLOY_DIR/log/leee.log

[web]
enabled = $ENABLE_WEB
port = $WEB_PORT
host = 0.0.0.0

[robot]
joint_limits_min = -180,-180,-180,-180,-180,-180
joint_limits_max = 180,180,180,180,180,180
velocity_limits = 100,100,100,100,100,100
acceleration_limits = 50,50,50,50,50,50

[network]
ros_enabled = false
ros_master_uri = http://localhost:11311
industrial_ethernet_enabled = false
realtime_protocol_enabled = false
EOF

    log_success "配置文件创建完成: $CONFIG_FILE"
}

# 创建systemd服务文件
create_service() {
    log_info "创建系统服务..."

    SERVICE_FILE="/etc/systemd/system/$SERVICE_NAME.service"

    cat > "$SERVICE_FILE" << EOF
[Unit]
Description=Leee Intelligent Robot Control System
After=network.target
Wants=network.target

[Service]
Type=simple
User=leee
Group=leee
WorkingDirectory=$DEPLOY_DIR
ExecStart=$DEPLOY_DIR/bin/Leee_main --config $DEPLOY_DIR/config/leee.conf
Restart=always
RestartSec=5
StandardOutput=journal
StandardError=journal
SyslogIdentifier=leee

[Install]
WantedBy=multi-user.target
EOF

    # 重新加载systemd配置
    systemctl daemon-reload

    log_success "系统服务创建完成: $SERVICE_FILE"
}

# 创建用户
create_user() {
    if ! id -u leee &>/dev/null; then
        log_info "创建 leee 用户..."
        useradd -r -s /bin/false leee
        log_success "用户创建完成"
    fi
}

# 设置权限
set_permissions() {
    log_info "设置文件权限..."

    chown -R leee:leee "$DEPLOY_DIR"
    chmod -R 755 "$DEPLOY_DIR/bin"
    chmod -R 644 "$DEPLOY_DIR/config"

    log_success "权限设置完成"
}

# 启动服务
start_service() {
    log_info "启动服务..."

    if [ "$ENABLE_WEB" = true ]; then
        # 启动Web服务
        cd "$DEPLOY_DIR/web"
        python3 app.py --port "$WEB_PORT" &
        WEB_PID=$!
        echo $WEB_PID > "$DEPLOY_DIR/web.pid"
        log_success "Web服务启动完成 (PID: $WEB_PID)"
    fi

    # 启动主服务
    systemctl start "$SERVICE_NAME"
    systemctl enable "$SERVICE_NAME"

    log_success "主服务启动完成"
}

# 停止服务
stop_service() {
    log_info "停止服务..."

    # 停止主服务
    systemctl stop "$SERVICE_NAME" 2>/dev/null || true

    # 停止Web服务
    if [ -f "$DEPLOY_DIR/web.pid" ]; then
        WEB_PID=$(cat "$DEPLOY_DIR/web.pid")
        kill "$WEB_PID" 2>/dev/null || true
        rm -f "$DEPLOY_DIR/web.pid"
        log_success "Web服务已停止"
    fi

    log_success "服务停止完成"
}

# 验证部署
verify_deployment() {
    log_info "验证部署..."

    # 检查文件是否存在
    if [ ! -f "$DEPLOY_DIR/bin/Leee_main" ]; then
        log_error "主程序文件不存在"
        return 1
    fi

    if [ "$ENABLE_WEB" = true ] && [ ! -f "$DEPLOY_DIR/web/app.py" ]; then
        log_error "Web应用文件不存在"
        return 1
    fi

    # 检查服务状态
    if ! systemctl is-active --quiet "$SERVICE_NAME"; then
        log_warning "主服务未运行"
    fi

    # 检查Web服务
    if [ "$ENABLE_WEB" = true ] && [ -f "$DEPLOY_DIR/web.pid" ]; then
        WEB_PID=$(cat "$DEPLOY_DIR/web.pid")
        if kill -0 "$WEB_PID" 2>/dev/null; then
            log_success "Web服务运行正常 (PID: $WEB_PID)"
        else
            log_warning "Web服务进程不存在"
        fi
    fi

    log_success "部署验证完成"
}

# 显示部署信息
show_deployment_info() {
    log_success "部署完成!"
    echo ""
    echo "部署信息:"
    echo "  环境: $DEPLOY_ENV"
    echo "  目录: $DEPLOY_DIR"
    echo "  服务: $SERVICE_NAME"
    echo "  Web端口: $WEB_PORT"
    echo ""
    echo "服务控制:"
    echo "  启动: systemctl start $SERVICE_NAME"
    echo "  停止: systemctl stop $SERVICE_NAME"
    echo "  重启: systemctl restart $SERVICE_NAME"
    echo "  状态: systemctl status $SERVICE_NAME"
    echo ""
    echo "Web界面: http://localhost:$WEB_PORT"

    if [ -d "$BACKUP_DIR" ]; then
        echo ""
        echo "备份位置: $BACKUP_DIR"
    fi
}

# 主函数
main() {
    log_info "开始部署 Leee 项目"
    log_info "部署环境: $DEPLOY_ENV"
    log_info "部署目录: $DEPLOY_DIR"

    parse_args "$@"

    if [ "$ROLLBACK" = true ]; then
        rollback_deployment
        exit 0
    fi

    check_permissions
    check_system_requirements
    backup_existing
    create_user
    create_directories
    copy_files
    create_config
    create_service
    set_permissions
    start_service
    verify_deployment
    show_deployment_info

    log_success "部署完成!"
}

# 执行主函数
main "$@"
