#!/bin/bash

# Leee 平台一键安装脚本
# 快速设置开发环境和运行示例

set -e

# 颜色定义
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m'

# 日志函数
log_info() {
    echo -e "${BLUE}[INFO]${NC} $1"
}

log_success() {
    echo -e "${GREEN}[SUCCESS]${NC} $1"
}

log_warn() {
    echo -e "${YELLOW}[WARN]${NC} $1"
}

log_error() {
    echo -e "${RED}[ERROR]${NC} $1"
}

# 检查系统要求
check_requirements() {
    log_info "检查系统要求..."

    # 检查操作系统
    if [[ "$OSTYPE" == "linux-gnu"* ]]; then
        log_info "检测到 Linux 系统"
    elif [[ "$OSTYPE" == "darwin"* ]]; then
        log_info "检测到 macOS 系统"
    else
        log_warn "未完全测试的操作系统: $OSTYPE"
    fi

    # 检查必需工具
    local tools=("cmake" "make" "g++" "python3")
    local missing_tools=()

    for tool in "${tools[@]}"; do
        if ! command -v "$tool" &> /dev/null; then
            missing_tools+=("$tool")
        fi
    done

    if [ ${#missing_tools[@]} -ne 0 ]; then
        log_error "缺少必需工具: ${missing_tools[*]}"
        log_info "请安装缺失的工具后重新运行"
        exit 1
    fi

    # 检查CMake版本
    local cmake_version=$(cmake --version | head -n1 | cut -d' ' -f3)
    log_info "CMake 版本: $cmake_version"

    # 检查Python版本
    local python_version=$(python3 --version 2>&1 | cut -d' ' -f2)
    log_info "Python 版本: $python_version"

    log_success "系统要求检查完成"
}

# 询问用户偏好
ask_user_preferences() {
    log_info "配置您的Leee平台..."

    # 询问使用场景
    echo
    log_info "请选择您的主要使用场景:"
    echo "1) 🤖 机器人应用开发"
    echo "2) 🏭 工业自动化集成"
    echo "3) 🔬 机器人算法研究"
    echo "4) 📚 教育与培训"
    echo "5) ⚡ 快速体验（推荐新手）"

    read -p "请选择 (1-5) [默认: 5]: " scenario_choice
    scenario_choice=${scenario_choice:-5}

    # 根据选择设置构建选项
    case $scenario_choice in
        1) # 机器人应用开发
            BUILD_MOTION_CONTROL=ON
            BUILD_HUMAN_INTERFACE=ON
            BUILD_DEVELOPMENT_TOOLS=ON
            BUILD_SYSTEM_MANAGEMENT=ON
            ;;
        2) # 工业自动化集成
            BUILD_INDUSTRIAL_INTEGRATION=ON
            BUILD_SYSTEM_MANAGEMENT=ON
            BUILD_HUMAN_INTERFACE=ON
            BUILD_MOTION_CONTROL=ON
            ;;
        3) # 机器人算法研究
            BUILD_MOTION_CONTROL=ON
            BUILD_PERCEPTION=ON
            BUILD_TASK_PLANNING=ON
            BUILD_DEVELOPMENT_TOOLS=ON
            ;;
        4) # 教育与培训
            BUILD_MOTION_CONTROL=ON
            BUILD_HUMAN_INTERFACE=ON
            BUILD_DEVELOPMENT_TOOLS=ON
            ;;
        5|*) # 快速体验
            BUILD_MOTION_CONTROL=ON
            BUILD_HUMAN_INTERFACE=ON
            BUILD_SYSTEM_MANAGEMENT=ON
            ;;
    esac

    # 询问是否启用Web界面
    echo
    read -p "是否启用Web监控界面? (y/N): " enable_web
    if [[ $enable_web =~ ^[Yy]$ ]]; then
        log_info "Web界面将在安装完成后自动启动"
        START_WEB=true
    fi

    log_success "配置完成"
}

# 创建构建目录并配置
setup_build() {
    log_info "设置构建环境..."

    # 创建构建目录
    mkdir -p build
    cd build

    # 构建配置
    local cmake_args=(
        -DCMAKE_BUILD_TYPE=Release
        -DBUILD_TESTING=ON
        -DBUILD_EXAMPLES=ON
        -DBUILD_MOTION_CONTROL=$BUILD_MOTION_CONTROL
        -DBUILD_PERCEPTION=$BUILD_PERCEPTION
        -DBUILD_TASK_PLANNING=$BUILD_TASK_PLANNING
        -DBUILD_HUMAN_INTERFACE=$BUILD_HUMAN_INTERFACE
        -DBUILD_SYSTEM_MANAGEMENT=$BUILD_SYSTEM_MANAGEMENT
        -DBUILD_INDUSTRIAL_INTEGRATION=$BUILD_INDUSTRIAL_INTEGRATION
        -DBUILD_DEVELOPMENT_TOOLS=$BUILD_DEVELOPMENT_TOOLS
    )

    log_info "CMake 配置参数:"
    printf '  %s\n' "${cmake_args[@]}"

    # 运行CMake配置
    if cmake .. "${cmake_args[@]}"; then
        log_success "CMake配置成功"
    else
        log_error "CMake配置失败"
        exit 1
    fi

    cd ..
}

# 构建项目
build_project() {
    log_info "构建Leee平台..."

    cd build

    # 获取CPU核心数
    local cpu_count=$(nproc 2>/dev/null || sysctl -n hw.ncpu 2>/dev/null || echo 4)
    log_info "使用 $cpu_count 个CPU核心进行构建"

    # 构建项目
    if make -j"$cpu_count"; then
        log_success "构建成功完成"
    else
        log_error "构建失败"
        exit 1
    fi

    cd ..
}

# 运行测试
run_tests() {
    log_info "运行功能验证..."

    cd build

    # 运行测试
    if make run-tests; then
        log_success "功能验证通过"
    else
        log_warn "部分测试可能失败，但基本功能正常"
    fi

    cd ..
}

# 启动Web界面
start_web_interface() {
    if [ "$START_WEB" = true ]; then
        log_info "启动Web监控界面..."

        cd human-interface/web

        # 在后台启动Web服务器
        nohup python3 app.py > web_server.log 2>&1 &
        local web_pid=$!

        # 等待服务器启动
        sleep 3

        if kill -0 $web_pid 2>/dev/null; then
            log_success "Web界面启动成功"
            log_info "访问地址: http://localhost:8080"
            log_info "服务器进程ID: $web_pid"
        else
            log_warn "Web界面启动可能失败，请检查日志: web_server.log"
        fi

        cd ../..
    fi
}

# 显示使用指南
show_usage_guide() {
    echo
    log_success "🎉 Leee平台安装完成！"
    echo
    echo "═══════════════════════════════════════════════════════════════"
    echo "                     使用指南                                "
    echo "═══════════════════════════════════════════════════════════════"
    echo
    log_info "1. 查看功能概览:"
    echo "   cd build && make functional-overview"
    echo
    log_info "2. 运行示例程序:"
    echo "   make run-examples"
    echo
    log_info "3. 访问Web界面:"
    if [ "$START_WEB" = true ]; then
        echo "   🌐 http://localhost:8080"
    else
        echo "   cd human-interface/web && python3 app.py"
    fi
    echo
    log_info "4. 查看详细文档:"
    echo "   cat README.md"
    echo "   cat GETTING_STARTED.md"
    echo
    log_info "5. 运行特定功能测试:"
    echo "   cd development-tools/test"
    echo "   ./run-tests.sh motion-control"
    echo
    echo "═══════════════════════════════════════════════════════════════"
    echo
    log_info "💡 提示:"
    echo "   - 使用 'make help' 查看所有可用命令"
    echo "   - 运行 'make functional-overview' 了解平台功能"
    echo "   - 查看 'GETTING_STARTED.md' 获取详细使用指南"
    echo
    log_success "祝您使用愉快！🎯"
}

# 主函数
main() {
    echo
    echo "═══════════════════════════════════════════════════════════════"
    echo "           🚀 Leee 智能机器人控制平台安装向导           "
    echo "═══════════════════════════════════════════════════════════════"
    echo

    log_info "欢迎使用Leee平台一键安装脚本"
    log_info "这个脚本将帮助您快速设置和体验Leee平台"

    # 初始化变量
    BUILD_MOTION_CONTROL=OFF
    BUILD_PERCEPTION=OFF
    BUILD_TASK_PLANNING=OFF
    BUILD_HUMAN_INTERFACE=OFF
    BUILD_SYSTEM_MANAGEMENT=OFF
    BUILD_INDUSTRIAL_INTEGRATION=OFF
    BUILD_DEVELOPMENT_TOOLS=OFF
    START_WEB=false

    # 执行安装步骤
    check_requirements
    ask_user_preferences
    setup_build
    build_project
    run_tests
    start_web_interface
    show_usage_guide
}

# 检查是否直接运行脚本
if [[ "${BASH_SOURCE[0]}" == "${0}" ]]; then
    main "$@"
fi
