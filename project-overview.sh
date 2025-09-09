#!/bin/bash

# Leee 项目概览脚本
# 显示项目状态、功能模块和使用指南

set -e

# 颜色定义
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
MAGENTA='\033[0;35m'
CYAN='\033[0;36m'
NC='\033[0m'

# 日志函数
log_info() {
    echo -e "${BLUE}[概览]${NC} $1"
}

log_success() {
    echo -e "${GREEN}[✓]${NC} $1"
}

log_warn() {
    echo -e "${YELLOW}[!]${NC} $1"
}

log_error() {
    echo -e "${RED}[✗]${NC} $1"
}

log_title() {
    echo -e "${MAGENTA}$1${NC}"
}

log_header() {
    echo -e "${CYAN}$1${NC}"
}

# 显示项目信息
show_project_info() {
    echo
    log_title "╔═══════════════════════════════════════════════════════════════╗"
    log_title "║                    Leee 智能机器人控制平台                     ║"
    log_title "╚═══════════════════════════════════════════════════════════════╝"
    echo

    log_info "项目定位：功能驱动的智能机器人控制系统平台"
    log_info "核心能力：运动控制、智能感知、任务规划、人机协作"
    echo
}

# 检查项目状态
check_project_status() {
    log_header "📊 项目状态检查"
    echo

    # 检查构建状态
    if [ -d "build" ] && [ -f "build/Makefile" ]; then
        log_success "构建环境已配置"
    else
        log_warn "构建环境未配置，建议运行: ./install.sh"
    fi

    # 检查功能模块
    local modules=("motion-control" "perception" "task-planning" "human-interface" "system-management" "industrial-integration" "development-tools")
    local available_modules=0

    echo
    log_info "功能模块状态:"
    for module in "${modules[@]}"; do
        if [ -d "$module" ]; then
            echo -e "  ${GREEN}✓${NC} $module"
            ((available_modules++))
        else
            echo -e "  ${RED}✗${NC} $module"
        fi
    done

    echo
    log_info "可用功能模块: $available_modules/${#modules[@]}"

    # 检查应用案例
    echo
    log_info "应用案例状态:"
    local applications=("industrial-robot" "service-robot" "collaborative-robot" "automation-line")
    local available_apps=0

    for app in "${applications[@]}"; do
        if [ -d "applications/$app" ]; then
            echo -e "  ${GREEN}✓${NC} $app"
            ((available_apps++))
        else
            echo -e "  ${RED}✗${NC} $app"
        fi
    done

    echo
    log_info "可用应用案例: $available_apps/${#applications[@]}"

    # 检查工作空间模板
    echo
    log_info "工作空间模板状态:"
    local templates=("basic-robot-app" "advanced-robot-app" "industrial-app")
    local available_templates=0

    for template in "${templates[@]}"; do
        if [ -d "workspace/templates/$template" ]; then
            echo -e "  ${GREEN}✓${NC} $template"
            ((available_templates++))
        else
            echo -e "  ${RED}✗${NC} $template"
        fi
    done

    echo
    log_info "可用工作空间模板: $available_templates/${#templates[@]}"

    # 检查脚本
    echo
    log_info "工具脚本状态:"
    local scripts=("install.sh" "demo.sh" "development-tools/scripts/build.sh")
    for script in "${scripts[@]}"; do
        if [ -f "$script" ] && [ -x "$script" ]; then
            log_success "$(basename "$script")"
        else
            log_warn "$(basename "$script") (未找到或无执行权限)"
        fi
    done
}

# 显示功能模块详情
show_functional_modules() {
    log_header "🎯 核心功能模块"
    echo

    cat << 'EOF'
┌─────────────────┬─────────────────────────────────────┬─────────────────────┐
│   功能模块      │          核心价值                   │     示例应用        │
├─────────────────┼─────────────────────────────────────┼─────────────────────┤
│ 🤖 运动控制     │ 精密定位，轨迹规划，实时控制       │ 工业机器人，机械臂   │
│ 👁️ 智能感知     │ 状态监控，位置感知，异常检测       │ 传感器融合，安全监控 │
│ 🎯 任务规划     │ 智能编排，路径优化，动态调度       │ 自动化生产线，AGV   │
│ 👥 人机协作     │ 直观界面，远程监控，数据可视化     │ 操作员控制台，移动端 │
│ ⚙️ 系统管理     │ 配置管理，日志系统，健康监控       │ 设备运维，故障诊断   │
│ 🏭 工业集成     │ 标准协议，生产线连接，质量控制     │ MES集成，SCADA系统   │
│ 🛠️ 开发工具     │ 仿真测试，性能分析，调试工具       │ 算法验证，系统调优   │
└─────────────────┴─────────────────────────────────────┴─────────────────────┘
EOF
}

# 显示快速开始指南
show_quick_start() {
    log_header "🚀 快速开始指南"
    echo

    cat << 'EOF'
1️⃣ 下载项目
   git clone https://github.com/leee-project/leee.git
   cd leee

2️⃣ 一键安装
   ./install.sh

3️⃣ 功能体验
   ./demo.sh --full

4️⃣ 开发环境
   make functional-overview  # 查看功能概览
   make run-examples        # 运行所有示例
EOF
    echo
}

# 显示使用场景
show_use_cases() {
    log_header "🎪 应用场景推荐"
    echo

    cat << 'EOF'
🎯 新手开发者：
   建议从运动控制 + 人机界面开始
   命令: cmake .. -DBUILD_MOTION_CONTROL=ON -DBUILD_HUMAN_INTERFACE=ON

🏭 工业集成工程师：
   推荐工业集成 + 系统管理 + 运动控制
   命令: cmake .. -DBUILD_INDUSTRIAL_INTEGRATION=ON -DBUILD_SYSTEM_MANAGEMENT=ON

🔬 算法研究人员：
   建议启用所有功能模块进行全面研究
   命令: cmake .. -DBUILD_ALL=ON

📚 教育培训：
   推荐运动控制 + 人机界面 + 开发工具
   命令: cmake .. -DBUILD_MOTION_CONTROL=ON -DBUILD_HUMAN_INTERFACE=ON -DBUILD_DEVELOPMENT_TOOLS=ON
EOF
    echo
}

# 显示开发工具
show_development_tools() {
    log_header "🛠️ 开发工具集"
    echo

    cat << 'EOF'
构建工具:
  ./install.sh           # 一键安装脚本
  ./demo.sh             # 功能演示脚本
  make functional-overview  # 功能模块概览
  make verify-capabilities  # 能力验证

开发辅助:
  make run-tests        # 运行测试套件
  make run-examples     # 运行示例程序
  make performance-report  # 性能分析报告

文档资源:
  README.md             # 详细使用指南
  GETTING_STARTED.md    # 快速上手指南
  design-specs/         # 设计规范文档
  doc/                  # API文档
EOF
    echo
}

# 显示技术架构
show_technical_architecture() {
    log_header "🏗️ 技术架构"
    echo

    cat << 'EOF'
功能导向设计:
  ├── motion-control/       # 运动控制模块
  ├── perception/          # 智能感知模块
  ├── task-planning/       # 任务规划模块
  ├── human-interface/     # 人机协作界面
  ├── system-management/   # 系统管理模块
  ├── industrial-integration/ # 工业集成模块
  └── development-tools/   # 开发工具集

技术生态:
  ├── capability-foundations/ # 能力基础技术库
  │   ├── Eigen            # 数学计算
  │   ├── Asio            # 网络通信
  │   ├── WebSocket++     # 实时通信
  │   └── doctest         # 测试框架
  └── CMake构建系统       # 跨平台构建
EOF
    echo
}

# 显示帮助信息
show_help_info() {
    log_header "📞 获取帮助"
    echo

    cat << 'EOF'
📖 文档资源:
  • README.md - 完整使用指南
  • GETTING_STARTED.md - 5分钟快速上手
  • design-specs/ - 架构设计规范
  • doc/ - API参考文档

💬 社区支持:
  • GitHub Issues - 问题反馈
  • GitHub Discussions - 技术讨论
  • 项目Wiki - 详细教程

🎯 学习路径:
  1. 运行 ./demo.sh --full 体验完整功能
  2. 阅读 GETTING_STARTED.md 了解基础概念
  3. 根据应用场景选择功能模块
  4. 深入学习相关技术文档
  5. 参与社区讨论和贡献

🔧 故障排除:
  • 构建问题: 检查CMake版本和依赖
  • 运行问题: 查看日志文件 logs/leee.log
  • 功能问题: 运行 make run-tests 验证功能
EOF
    echo
}

# 主函数
main() {
    show_project_info
    check_project_status
    show_functional_modules
    show_quick_start
    show_use_cases
    show_development_tools
    show_technical_architecture
    show_help_info

    echo
    log_title "═══════════════════════════════════════════════════════════════"
    log_success "项目概览完成！准备开始您的Leee之旅吧！🎯"
    log_title "═══════════════════════════════════════════════════════════════"
    echo

    # 提供交互式选项
    echo "💡 接下来您可以："
    echo "   1) 运行 ./install.sh 开始安装"
    echo "   2) 运行 ./demo.sh 体验功能"
    echo "   3) 探索 ./applications/ 应用案例"
    echo "   4) 查看 ./workspace/templates/ 开发模板"
    echo "   5) 阅读 README.md 了解详情"
    echo "   6) 查看 GETTING_STARTED.md 快速上手"
    echo
    read -p "需要我帮您执行某个操作吗? (y/N): " choice
    if [[ $choice =~ ^[Yy]$ ]]; then
        echo
        echo "1) 一键安装: ./install.sh"
        echo "2) 功能演示: ./demo.sh"
        echo "3) 构建项目: mkdir build && cd build && cmake .. && make"
        echo "4) 查看应用案例: ls -la applications/"
        echo "5) 查看工作空间模板: ls -la workspace/templates/"
        echo
        read -p "请选择 (1-5): " action
        case $action in
            1) ./install.sh ;;
            2) ./demo.sh ;;
            3)
                mkdir -p build
                cd build
                cmake ..
                make -j4
                cd ..
                ;;
            4) echo "应用案例:" && ls -la applications/ ;;
            5) echo "工作空间模板:" && ls -la workspace/templates/ ;;
        esac
    fi
}

# 检查是否直接运行脚本
if [[ "${BASH_SOURCE[0]}" == "${0}" ]]; then
    main "$@"
fi
