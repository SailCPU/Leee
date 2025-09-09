#!/bin/bash

# Leee 平台功能演示脚本
# 快速展示平台的核心功能和使用方法

set -e

# 颜色定义
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m'

# 日志函数
log_info() {
    echo -e "${BLUE}[演示]${NC} $1"
}

log_success() {
    echo -e "${GREEN}[成功]${NC} $1"
}

log_warn() {
    echo -e "${YELLOW}[提示]${NC} $1"
}

log_error() {
    echo -e "${RED}[错误]${NC} $1"
}

# 检查构建状态
check_build() {
    if [ ! -d "build" ]; then
        log_error "未找到构建目录，请先运行安装脚本: ./install.sh"
        exit 1
    fi

    if [ ! -f "build/Makefile" ]; then
        log_error "构建未完成，请先运行安装脚本: ./install.sh"
        exit 1
    fi
}

# 演示功能概览
show_overview() {
    echo
    echo "═══════════════════════════════════════════════════════════════"
    echo "                🎯 Leee 平台功能演示                          "
    echo "═══════════════════════════════════════════════════════════════"
    echo

    log_info "展示Leee平台的核心功能模块..."
    echo

    cd build
    make functional-overview 2>/dev/null || true
    cd ..
}

# 演示运动控制
demo_motion_control() {
    echo
    log_info "🤖 演示运动控制功能..."
    echo

    if [ -f "build/development-tools/examples/cpp/robot_kinematics_example" ]; then
        log_info "运行机器人运动学计算示例..."
        echo "─────────────────────────────────────"
        ./build/development-tools/examples/cpp/robot_kinematics_example
        echo "─────────────────────────────────────"
        log_success "运动学计算演示完成"
    else
        log_warn "运动控制示例未构建，请运行: ./install.sh"
    fi
}

# 演示人机界面
demo_human_interface() {
    echo
    log_info "👥 演示人机协作界面..."
    echo

    if [ -f "human-interface/web/app.py" ]; then
        log_info "启动Web监控界面..."
        log_info "访问地址: http://localhost:8080"
        log_warn "按 Ctrl+C 停止Web服务器"

        cd human-interface/web
        python3 app.py
        cd ../..
    else
        log_warn "Web界面文件不存在"
    fi
}

# 演示任务规划
demo_task_planning() {
    echo
    log_info "🎯 演示任务规划功能..."
    echo

    if [ -f "build/development-tools/examples/robotics/pick_and_place_demo.py" ]; then
        log_info "运行抓取放置任务演示..."
        echo "─────────────────────────────────────"
        python3 build/development-tools/examples/robotics/pick_and_place_demo.py
        echo "─────────────────────────────────────"
        log_success "任务规划演示完成"
    else
        log_warn "任务规划示例未构建"
    fi
}

# 演示系统管理
demo_system_management() {
    echo
    log_info "⚙️ 演示系统管理功能..."
    echo

    if [ -f "build/development-tools/test/unit/test_logger" ]; then
        log_info "运行日志系统测试..."
        ./build/development-tools/test/unit/test_logger
        log_success "日志系统测试完成"
    else
        log_warn "系统管理测试未构建"
    fi
}

# 演示开发工具
demo_development_tools() {
    echo
    log_info "🛠️ 演示开发工具集..."
    echo

    cd build

    # 运行测试
    if make run-tests 2>/dev/null; then
        log_success "测试套件执行完成"
    else
        log_warn "测试执行遇到问题，请检查构建配置"
    fi

    # 运行示例
    if make run-examples 2>/dev/null; then
        log_success "示例程序执行完成"
    else
        log_warn "示例执行遇到问题"
    fi

    cd ..
}

# 显示功能对比
show_feature_comparison() {
    echo
    log_info "📊 功能模块对比..."
    echo

    cat << 'EOF'
╔══════════════════════════════════════════════════════════════╗
║                     功能模块对比表                            ║
╠══════════════════════════════════════════════════════════════╣
║ 功能模块       │ 核心价值               │ 示例应用            ║
╠════════════════╪═══════════════════════╪═════════════════════╣
║ 运动控制       │ 精密定位，轨迹规划    │ 工业机器人，机械臂    ║
║ 智能感知       │ 状态监控，环境感知    │ 传感器融合，异常检测 ║
║ 任务规划       │ 智能编排，路径优化    │ 自动化生产线，AGV    ║
║ 人机协作       │ 直观界面，远程监控    │ 操作员控制台，移动端 ║
║ 系统管理       │ 配置管理，健康监控    │ 设备运维，故障诊断   ║
║ 工业集成       │ 标准协议，生产线连接  │ MES集成，SCADA系统   ║
║ 开发工具       │ 仿真测试，性能分析    │ 算法验证，调试工具   ║
╚══════════════════════════════════════════════════════════════╝
EOF
}

# 显示使用建议
show_recommendations() {
    echo
    log_info "💡 使用建议..."
    echo

    cat << 'EOF'
🎯 新手推荐使用路径：
   1. 先运行完整演示：./demo.sh --full
   2. 查看功能概览：make functional-overview
   3. 选择您的应用场景
   4. 根据需求定制构建配置
   5. 深入学习相关功能模块

🔧 开发者进阶路径：
   1. 阅读设计规范：design-specs/
   2. 运行单元测试：make run-tests
   3. 查看API文档：doc/
   4. 参与功能开发：development-tools/
   5. 贡献代码改进

📈 性能优化建议：
   - 使用Release构建获得最佳性能
   - 根据应用场景选择必要的功能模块
   - 定期运行测试确保功能稳定
   - 监控系统资源使用情况
EOF
}

# 解析命令行参数
parse_args() {
    case "${1:-}" in
        --motion-control)
            demo_motion_control
            ;;
        --human-interface)
            demo_human_interface
            ;;
        --task-planning)
            demo_task_planning
            ;;
        --system-management)
            demo_system_management
            ;;
        --development-tools)
            demo_development_tools
            ;;
        --full)
            run_full_demo
            ;;
        --help|-h)
            show_help
            ;;
        *)
            show_menu
            ;;
    esac
}

# 演示应用案例
demo_applications() {
    echo
    log_info "📦 应用案例演示..."
    echo

    if [ -d "applications" ]; then
        echo "可用的应用案例:"
        echo

        # 检查各个应用案例
        if [ -d "applications/industrial-robot" ]; then
            echo "🤖 工业机器人焊接应用"
            echo "   位置: applications/industrial-robot/"
            echo "   运行: cd applications/industrial-robot && cat README.md"
            echo
        fi

        if [ -d "applications/service-robot" ]; then
            echo "🏨 服务机器人应用"
            echo "   位置: applications/service-robot/"
            echo "   运行: cd applications/service-robot && cat README.md"
            echo
        fi

        if [ -d "applications/collaborative-robot" ]; then
            echo "🤝 协作机器人应用"
            echo "   位置: applications/collaborative-robot/"
            echo "   运行: cd applications/collaborative-robot && cat README.md"
            echo
        fi

        if [ -d "applications/automation-line" ]; then
            echo "🏭 自动化生产线应用"
            echo "   位置: applications/automation-line/"
            echo "   运行: cd applications/automation-line && cat README.md"
            echo
        fi

        log_success "应用案例展示完成"
    else
        log_warn "未找到应用案例目录"
    fi
}

# 演示工作空间模板
demo_workspace_templates() {
    echo
    log_info "🛠️ 工作空间模板演示..."
    echo

    if [ -d "workspace/templates" ]; then
        echo "可用的开发模板:"
        echo

        # 检查各个模板
        if [ -d "workspace/templates/basic-robot-app" ]; then
            echo "📚 基础机器人应用模板"
            echo "   位置: workspace/templates/basic-robot-app/"
            echo "   使用: cp -r workspace/templates/basic-robot-app my-app"
            echo
        fi

        if [ -d "workspace/templates/advanced-robot-app" ]; then
            echo "🚀 高级机器人应用模板"
            echo "   位置: workspace/templates/advanced-robot-app/"
            echo "   使用: cp -r workspace/templates/advanced-robot-app my-advanced-app"
            echo
        fi

        if [ -d "workspace/templates/industrial-app" ]; then
            echo "🏭 工业应用模板"
            echo "   位置: workspace/templates/industrial-app/"
            echo "   使用: cp -r workspace/templates/industrial-app my-industrial-app"
            echo
        fi

        log_success "工作空间模板展示完成"
    else
        log_warn "未找到工作空间模板目录"
    fi
}

# 显示菜单
show_menu() {
    echo
    echo "═══════════════════════════════════════════════════════════════"
    echo "                🎯 Leee 平台功能演示                          "
    echo "═══════════════════════════════════════════════════════════════"
    echo
    log_info "请选择演示内容:"
    echo
    echo "1) 🤖 运动控制演示"
    echo "2) 👥 人机界面演示"
    echo "3) 🎯 任务规划演示"
    echo "4) ⚙️ 系统管理演示"
    echo "5) 🛠️ 开发工具演示"
    echo "6) 📦 应用案例演示"
    echo "7) 🏗️ 工作空间模板演示"
    echo "8) 🎪 完整功能演示"
    echo "9) 📊 功能对比说明"
    echo "10) ❓ 帮助信息"
    echo
    read -p "请选择 (1-10): " choice

    case $choice in
        1) demo_motion_control ;;
        2) demo_human_interface ;;
        3) demo_task_planning ;;
        4) demo_system_management ;;
        5) demo_development_tools ;;
        6) demo_applications ;;
        7) demo_workspace_templates ;;
        8) run_full_demo ;;
        9) show_feature_comparison ;;
        10) show_help ;;
        *) log_warn "无效选择，请重新选择" ;;
    esac
}

# 运行完整演示
run_full_demo() {
    log_info "开始完整功能演示..."
    echo

    show_overview
    demo_motion_control
    demo_task_planning
    demo_system_management
    demo_development_tools
    show_feature_comparison
    show_recommendations

    echo
    log_success "完整演示完成！"
    echo
    log_info "🎉 感谢体验 Leee 智能机器人控制平台！"
    log_info "📧 如有问题，请查看文档或联系技术支持"
}

# 显示帮助
show_help() {
    cat << 'EOF'
Leee 平台演示脚本使用指南

用法:
    ./demo.sh [选项]

选项:
    --motion-control     运动控制功能演示
    --human-interface    人机界面功能演示
    --task-planning      任务规划功能演示
    --system-management  系统管理功能演示
    --development-tools  开发工具功能演示
    --full              完整功能演示
    --help, -h          显示此帮助信息

交互模式:
    ./demo.sh           进入交互式菜单选择演示内容

前提条件:
    - 已完成平台安装：./install.sh
    - 构建成功：make 命令执行成功

示例:
    # 运行完整演示
    ./demo.sh --full

    # 只演示运动控制
    ./demo.sh --motion-control

    # 进入交互模式
    ./demo.sh

更多信息:
    - 详细文档：README.md
    - 快速开始：GETTING_STARTED.md
    - 技术支持：doc/ 目录
EOF
}

# 主函数
main() {
    # 检查构建状态
    check_build

    # 解析命令行参数
    if [ $# -eq 0 ]; then
        show_menu
    else
        parse_args "$@"
    fi
}

# 检查是否直接运行脚本
if [[ "${BASH_SOURCE[0]}" == "${0}" ]]; then
    main "$@"
fi
