#include <capability-foundations/leee/logger.h>
#include <iostream>

int main() {
    // 初始化日志
    leee::init_logger();

    LOG_INFO << "Logger example started";
    return 0;
}
