#!/bin/bash

# Leee 项目文档生成脚本
# 用于生成doxygen文档

set -e  # 遇到错误立即退出

# 颜色定义
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

# 日志函数
log_info() {
    echo -e "${GREEN}[INFO]${NC} $1"
}

log_warn() {
    echo -e "${YELLOW}[WARN]${NC} $1"
}

log_error() {
    echo -e "${RED}[ERROR]${NC} $1"
}

log_step() {
    echo -e "${BLUE}[STEP]${NC} $1"
}

# 检查命令是否存在
check_command() {
    if ! command -v "$1" &> /dev/null; then
        log_error "命令 '$1' 未找到，请先安装"
        return 1
    fi
    return 0
}

# 主函数
main() {
    log_info "开始生成Leee项目文档..."

    # 检查必要的工具
    log_step "检查必要的工具..."
    check_command doxygen || {
        log_error "请安装doxygen: sudo apt install doxygen"
        exit 1
    }

    check_command dot || {
        log_warn "Graphviz dot 未找到，类图和调用图将不会生成"
        log_warn "安装方法: sudo apt install graphviz"
    }

    # 检查Doxyfile是否存在
    if [ ! -f "Doxyfile" ]; then
        log_error "Doxyfile 文件不存在，请确保在项目根目录运行此脚本"
        exit 1
    fi

    # 检查输入目录是否存在
    if [ ! -d "include" ]; then
        log_error "include 目录不存在"
        exit 1
    fi

    if [ ! -d "doc" ]; then
        log_error "doc 目录不存在"
        exit 1
    fi

    # 创建输出目录
    log_step "创建输出目录..."
    mkdir -p docs

    # 备份旧的文档（如果存在）
    if [ -d "docs" ] && [ "$(ls -A docs)" ]; then
        log_step "备份旧文档..."
        backup_dir="docs_backup_$(date +%Y%m%d_%H%M%S)"
        mv docs "$backup_dir"
        log_info "旧文档已备份到: $backup_dir"
        mkdir -p docs
    fi

    # 生成文档
    log_step "生成doxygen文档..."
    if doxygen Doxyfile; then
        log_info "文档生成成功!"
    else
        log_error "文档生成失败"
        exit 1
    fi

    # 检查生成结果
    log_step "检查生成结果..."
    if [ -f "docs/index.html" ]; then
        log_info "HTML文档生成成功: docs/index.html"
    else
        log_warn "未找到index.html文件"
    fi

    if [ -f "docs/refman.pdf" ]; then
        log_info "PDF文档生成成功: docs/refman.pdf"
    else
        log_warn "未找到PDF文档（需要额外配置）"
    fi

    # 显示统计信息
    log_step "生成统计信息..."
    if [ -d "docs" ]; then
        html_count=$(find docs -name "*.html" | wc -l)
        png_count=$(find docs -name "*.png" | wc -l)
        css_count=$(find docs -name "*.css" | wc -l)

        log_info "生成的文件统计:"
        log_info "  HTML文件: $html_count 个"
        log_info "  图片文件: $png_count 个"
        log_info "  样式文件: $css_count 个"
    fi

    # 显示使用说明
    log_info ""
    log_info "文档生成完成!"
    log_info "打开浏览器查看: docs/index.html"
    log_info ""

    # 可选：启动本地服务器
    if check_command python3; then
        log_info "启动本地文档服务器..."
        log_info "访问地址: http://localhost:8000"
        log_info "按Ctrl+C停止服务器"
        log_info ""
        cd docs && python3 -m http.server 8000
    else
        log_info "安装Python3可以启动本地文档服务器"
    fi
}

# 显示帮助信息
show_help() {
    echo "Leee 项目文档生成脚本"
    echo ""
    echo "用法:"
    echo "  $0              # 生成文档"
    echo "  $0 --help       # 显示此帮助信息"
    echo "  $0 --clean      # 清理生成的文档"
    echo ""
    echo "要求:"
    echo "  - doxygen       # 文档生成工具"
    echo "  - graphviz      # 可选，用于生成图表"
    echo ""
    echo "输出:"
    echo "  - docs/         # 生成的HTML文档目录"
    echo ""
}

# 清理函数
clean_docs() {
    log_step "清理生成的文档..."

    if [ -d "docs" ]; then
        rm -rf docs
        log_info "已删除 docs 目录"
    else
        log_warn "docs 目录不存在，无需清理"
    fi

    # 清理其他可能的输出目录
    for dir in html latex rtf xml; do
        if [ -d "$dir" ]; then
            rm -rf "$dir"
            log_info "已删除 $dir 目录"
        fi
    done

    # 清理doxygen警告文件
    if [ -f "doxygen_warnings.txt" ]; then
        rm -f doxygen_warnings.txt
        log_info "已删除 doxygen_warnings.txt"
    fi

    log_info "清理完成"
}

# 参数处理
case "${1:-}" in
    --help|-h)
        show_help
        exit 0
        ;;
    --clean)
        clean_docs
        exit 0
        ;;
    "")
        main
        ;;
    *)
        log_error "无效参数: $1"
        log_error "使用 --help 查看帮助信息"
        exit 1
        ;;
esac
