#!/bin/bash

# Leee 项目构建脚本 - 功能导向的构建系统
# 支持按需构建功能模块和CI/CD集成

set -e  # 遇到错误立即退出

# 默认构建参数 - 功能导向的配置选项
BUILD_TYPE=${BUILD_TYPE:-Release}
BUILD_DIR=${BUILD_DIR:-build}
CLEAN_BUILD=${CLEAN_BUILD:-false}
RUN_TESTS=${RUN_TESTS:-true}
GENERATE_DOCS=${GENERATE_DOCS:-false}

# 功能模块构建选项 - 基于业务需求的模块选择
BUILD_MOTION_CONTROL=${BUILD_MOTION_CONTROL:-true}
BUILD_PERCEPTION=${BUILD_PERCEPTION:-true}
BUILD_TASK_PLANNING=${BUILD_TASK_PLANNING:-true}
BUILD_HUMAN_INTERFACE=${BUILD_HUMAN_INTERFACE:-true}
BUILD_SYSTEM_MANAGEMENT=${BUILD_SYSTEM_MANAGEMENT:-true}
BUILD_INDUSTRIAL_INTEGRATION=${BUILD_INDUSTRIAL_INTEGRATION:-true}
BUILD_DEVELOPMENT_TOOLS=${BUILD_DEVELOPMENT_TOOLS:-true}

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
    echo "Leee 项目构建脚本 - 功能导向的构建系统"
    echo ""
    echo "用法: $0 [选项]"
    echo ""
    echo "基础选项:"
    echo "  -t, --type TYPE      构建类型 (Debug/Release) [默认: Release]"
    echo "  -d, --dir DIR        构建目录 [默认: build]"
    echo "  -c, --clean          清理构建目录"
    echo "  --no-tests           跳过测试"
    echo "  --docs               生成文档"
    echo ""
    echo "功能模块选项 (可按需选择):"
    echo "  --motion-control     构建运动控制模块 [默认: true]"
    echo "  --perception         构建感知模块 [默认: true]"
    echo "  --task-planning      构建任务规划模块 [默认: true]"
    echo "  --human-interface    构建人机界面模块 [默认: true]"
    echo "  --system-management  构建系统管理模块 [默认: true]"
    echo "  --industrial-integration 构建工业集成模块 [默认: true]"
    echo "  --development-tools  构建开发工具 [默认: true]"
    echo ""
    echo "环境变量:"
    echo "  BUILD_TYPE           构建类型"
    echo "  BUILD_DIR            构建目录"
    echo "  CLEAN_BUILD          是否清理构建 (true/false)"
    echo "  RUN_TESTS            是否运行测试 (true/false)"
    echo "  BUILD_MOTION_CONTROL 构建运动控制模块 (true/false)"
    echo "  BUILD_PERCEPTION     构建感知模块 (true/false)"
    echo "  ...以此类推其他功能模块"
}

# 解析命令行参数
parse_args() {
    while [[ $# -gt 0 ]]; do
        case $1 in
            -t|--type)
                BUILD_TYPE="$2"
                shift 2
                ;;
            -d|--dir)
                BUILD_DIR="$2"
                shift 2
                ;;
            -c|--clean)
                CLEAN_BUILD=true
                shift
                ;;
            --no-tests)
                RUN_TESTS=false
                shift
                ;;
            --docs)
                GENERATE_DOCS=true
                shift
                ;;
            --no-python)
                ENABLE_PYTHON=false
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

# 检查依赖
check_dependencies() {
    log_info "检查构建依赖..."

    # 检查CMake
    if ! command -v cmake &> /dev/null; then
        log_error "CMake 未找到，请安装 CMake 3.16+"
        exit 1
    fi

    CMAKE_VERSION=$(cmake --version | head -n1 | cut -d' ' -f3)
    log_info "CMake 版本: $CMAKE_VERSION"

    # 检查编译器
    if command -v g++ &> /dev/null; then
        GCC_VERSION=$(g++ --version | head -n1)
        log_info "GCC 版本: $GCC_VERSION"
    elif command -v clang++ &> /dev/null; then
        CLANG_VERSION=$(clang++ --version | head -n1)
        log_info "Clang 版本: $CLANG_VERSION"
    else
        log_error "未找到 C++ 编译器 (GCC 或 Clang)"
        exit 1
    fi

    # 检查Python（如果启用）
    if [ "$ENABLE_PYTHON" = true ]; then
        if ! command -v python3 &> /dev/null; then
            log_warning "Python3 未找到，禁用Python模块"
            ENABLE_PYTHON=false
        else
            PYTHON_VERSION=$(python3 --version)
            log_info "$PYTHON_VERSION"
        fi
    fi

    log_success "依赖检查完成"
}

# 清理构建目录
clean_build() {
    if [ "$CLEAN_BUILD" = true ] && [ -d "$BUILD_DIR" ]; then
        log_info "清理构建目录: $BUILD_DIR"
        rm -rf "$BUILD_DIR"
    fi
}

# 配置项目
configure_project() {
    log_info "配置项目..."

    mkdir -p "$BUILD_DIR"
    cd "$BUILD_DIR"

    CMAKE_ARGS=(
        -DCMAKE_BUILD_TYPE="$BUILD_TYPE"
        -DBUILD_TESTING="$RUN_TESTS"
        -DBUILD_EXAMPLES="$BUILD_EXAMPLES"
        -DGENERATE_DOCS="$GENERATE_DOCS"
        -DBUILD_MOTION_CONTROL="$BUILD_MOTION_CONTROL"
        -DBUILD_PERCEPTION="$BUILD_PERCEPTION"
        -DBUILD_TASK_PLANNING="$BUILD_TASK_PLANNING"
        -DBUILD_HUMAN_INTERFACE="$BUILD_HUMAN_INTERFACE"
        -DBUILD_SYSTEM_MANAGEMENT="$BUILD_SYSTEM_MANAGEMENT"
        -DBUILD_INDUSTRIAL_INTEGRATION="$BUILD_INDUSTRIAL_INTEGRATION"
        -DBUILD_DEVELOPMENT_TOOLS="$BUILD_DEVELOPMENT_TOOLS"
    )

    # 添加平台特定的配置
    case "$(uname -s)" in
        Linux)
            CMAKE_ARGS+=(-DCMAKE_CXX_COMPILER=g++ -DCMAKE_C_COMPILER=gcc)
            ;;
        Darwin)
            CMAKE_ARGS+=(-DCMAKE_CXX_COMPILER=clang++ -DCMAKE_C_COMPILER=clang)
            ;;
        MINGW*|MSYS*)
            CMAKE_ARGS+=(-G "MinGW Makefiles")
            ;;
    esac

    log_info "CMake 参数: ${CMAKE_ARGS[*]}"
    cmake .. "${CMAKE_ARGS[@]}"

    if [ $? -ne 0 ]; then
        log_error "CMake 配置失败"
        exit 1
    fi

    log_success "项目配置完成"
}

# 构建项目
build_project() {
    log_info "构建项目..."

    cd "$BUILD_DIR"

    # 获取CPU核心数
    if command -v nproc &> /dev/null; then
        CPU_CORES=$(nproc)
    elif command -v sysctl &> /dev/null; then
        CPU_CORES=$(sysctl -n hw.ncpu)
    else
        CPU_CORES=4
    fi

    log_info "使用 $CPU_CORES 个CPU核心进行构建"

    make -j"$CPU_CORES"

    if [ $? -ne 0 ]; then
        log_error "构建失败"
        exit 1
    fi

    log_success "项目构建完成"
}

# 运行测试
run_tests() {
    if [ "$RUN_TESTS" = false ]; then
        log_info "跳过测试"
        return
    fi

    log_info "运行测试..."

    cd "$BUILD_DIR"

    if [ -f "CTestTestfile.cmake" ]; then
        ctest --output-on-failure
        if [ $? -ne 0 ]; then
            log_error "测试失败"
            exit 1
        fi
    else
        log_warning "未找到测试文件，跳过测试"
    fi

    log_success "测试完成"
}

# 生成文档
generate_docs() {
    if [ "$GENERATE_DOCS" = false ]; then
        return
    fi

    log_info "生成文档..."

    cd "$BUILD_DIR"

    if [ -f "docs" ]; then
        make docs
        if [ $? -ne 0 ]; then
            log_warning "文档生成失败"
        else
            log_success "文档生成完成"
        fi
    else
        log_warning "未找到文档目标，跳过文档生成"
    fi
}

# 显示构建信息
show_build_info() {
    log_success "构建完成!"
    echo ""
    echo "构建信息:"
    echo "  构建类型: $BUILD_TYPE"
    echo "  构建目录: $BUILD_DIR"
    echo "  Python支持: $ENABLE_PYTHON"
    echo ""
    echo "可用的目标:"
    echo "  主程序: $BUILD_DIR/bin/Leee_main"
    echo "  示例程序: $BUILD_DIR/examples/"
    echo "  测试程序: $BUILD_DIR/test/"

    if [ "$GENERATE_DOCS" = true ]; then
        echo "  文档: $BUILD_DIR/docs/"
    fi
}

# 主函数
main() {
    log_info "开始构建 Leee 项目"
    log_info "构建类型: $BUILD_TYPE"
    log_info "构建目录: $BUILD_DIR"

    parse_args "$@"
    check_dependencies
    clean_build
    configure_project
    build_project
    run_tests
    generate_docs
    show_build_info

    log_success "所有构建步骤完成!"
}

# 执行主函数
main "$@"
