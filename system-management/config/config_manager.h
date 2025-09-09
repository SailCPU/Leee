#pragma once

/**
 * @file config_manager.h
 * @brief 配置管理器
 */

#include <string>
#include <unordered_map>
#include <variant>
#include <memory>
#include <mutex>
#include <nlohmann/json.hpp>

namespace leee {

/**
 * @brief 配置值类型
 */
using ConfigValue = std::variant<int, double, bool, std::string>;

/**
 * @brief 配置管理器
 */
class ConfigManager {
private:
    std::unordered_map<std::string, ConfigValue> config_map_;
    std::string config_file_path_;
    std::mutex mutex_;
    bool auto_save_;

public:
    /**
     * @brief 构造函数
     * @param config_file_path 配置文件路径
     * @param auto_save 是否自动保存
     */
    explicit ConfigManager(const std::string& config_file_path = "",
                          bool auto_save = true);

    /**
     * @brief 从文件加载配置
     * @param file_path 配置文件路径
     * @return 是否加载成功
     */
    bool loadFromFile(const std::string& file_path);

    /**
     * @brief 保存配置到文件
     * @param file_path 保存路径（可选，默认使用构造函数中的路径）
     * @return 是否保存成功
     */
    bool saveToFile(const std::string& file_path = "");

    /**
     * @brief 设置配置值
     * @param key 配置键
     * @param value 配置值
     */
    void setValue(const std::string& key, const ConfigValue& value);

    /**
     * @brief 获取配置值
     * @param key 配置键
     * @param default_value 默认值
     * @return 配置值
     */
    ConfigValue getValue(const std::string& key, const ConfigValue& default_value = 0);

    /**
     * @brief 获取整型配置值
     * @param key 配置键
     * @param default_value 默认值
     * @return 整型值
     */
    int getInt(const std::string& key, int default_value = 0);

    /**
     * @brief 获取浮点型配置值
     * @param key 配置键
     * @param default_value 默认值
     * @return 浮点值
     */
    double getDouble(const std::string& key, double default_value = 0.0);

    /**
     * @brief 获取布尔型配置值
     * @param key 配置键
     * @param default_value 默认值
     * @return 布尔值
     */
    bool getBool(const std::string& key, bool default_value = false);

    /**
     * @brief 获取字符串配置值
     * @param key 配置键
     * @param default_value 默认值
     * @return 字符串值
     */
    std::string getString(const std::string& key, const std::string& default_value = "");

    /**
     * @brief 检查配置键是否存在
     * @param key 配置键
     * @return 是否存在
     */
    bool hasKey(const std::string& key) const;

    /**
     * @brief 删除配置项
     * @param key 配置键
     */
    void removeKey(const std::string& key);

    /**
     * @brief 获取所有配置键
     * @return 配置键列表
     */
    std::vector<std::string> getAllKeys() const;

    /**
     * @brief 清空所有配置
     */
    void clear();

    /**
     * @brief 设置自动保存
     * @param auto_save 是否自动保存
     */
    void setAutoSave(bool auto_save);

    /**
     * @brief 获取配置文件的JSON表示
     * @return JSON对象
     */
    nlohmann::json toJson() const;

    /**
     * @brief 从JSON加载配置
     * @param json JSON对象
     */
    void fromJson(const nlohmann::json& json);
};

/**
 * @brief 参数服务器
 */
class ParameterServer {
private:
    std::unordered_map<std::string, ConfigManager> namespaces_;
    std::string base_path_;
    std::mutex mutex_;

public:
    /**
     * @brief 构造函数
     * @param base_path 基础路径
     */
    explicit ParameterServer(const std::string& base_path = "");

    /**
     * @brief 获取或创建命名空间
     * @param namespace_name 命名空间名称
     * @return 配置管理器引用
     */
    ConfigManager& getNamespace(const std::string& namespace_name);

    /**
     * @brief 删除命名空间
     * @param namespace_name 命名空间名称
     */
    void removeNamespace(const std::string& namespace_name);

    /**
     * @brief 获取所有命名空间
     * @return 命名空间列表
     */
    std::vector<std::string> getAllNamespaces() const;

    /**
     * @brief 保存所有命名空间
     */
    void saveAll();

    /**
     * @brief 加载所有命名空间
     */
    void loadAll();
};

} // namespace leee
