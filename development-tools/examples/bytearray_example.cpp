#include <capability-foundations/leee/logger.h>
#include <iostream>

int main() {
    // 初始化日志
    leee::init_logger();

    LOG_INFO << "Bytearray example started";
    return 0;
}
