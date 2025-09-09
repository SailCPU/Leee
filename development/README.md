# Leee 开发过程管理

这个板块用于管理 Leee 项目的开发过程，包括开发路线图、待办事项、迭代周期和回顾会议记录。

## 目录结构

```
development/
├── README.md                 # 本文件
├── roadmap/                  # 开发路线图
│   ├── README.md            # 路线图说明
│   ├── v1.0-roadmap.md      # 1.0版本路线图
│   └── future-visions.md    # 未来愿景
├── backlog/                  # 待办事项
│   ├── README.md            # 待办事项说明
│   ├── product-backlog.md   # 产品待办列表
│   ├── sprint-backlog.md    # 迭代待办列表
│   └── technical-debt.md    # 技术债务清单
├── sprints/                  # 迭代周期
│   ├── README.md            # 迭代说明
│   ├── sprint-001/          # 第一个迭代
│   │   ├── goals.md         # 目标
│   │   ├── tasks.md         # 任务列表
│   │   └── burndown.md      # 燃尽图
│   └── templates/           # 迭代模板
├── retrospectives/           # 回顾会议记录
│   ├── README.md            # 回顾说明
│   ├── sprint-001-retro.md  # 第一个迭代回顾
│   └── templates/           # 回顾模板
└── docs/                    # 开发过程文档
    ├── development-process.md   # 开发流程说明
    ├── coding-standards.md     # 编码规范
    ├── testing-strategy.md     # 测试策略
    └── release-process.md      # 发布流程
```

## 开发流程

1. **规划阶段**：使用 roadmap/ 制定长期规划
2. **迭代规划**：从 backlog/ 选择任务进入 sprints/
3. **执行阶段**：在迭代中完成任务
4. **回顾阶段**：通过 retrospectives/ 总结经验教训
5. **发布阶段**：按照 docs/release-process.md 发布新版本

## 工具和约定

- 使用 Markdown 格式记录所有文档
- 任务状态：TODO, IN_PROGRESS, DONE, BLOCKED
- 优先级：HIGH, MEDIUM, LOW
- 任务估计：使用故事点或时间估计

## 参与贡献

欢迎所有开发者参与开发过程管理：

1. Fork 本项目
2. 在相应目录创建或更新文档
3. 提交 Pull Request
4. 参与代码审查和讨论

## 联系方式

如有问题或建议，请通过以下方式联系：
- 项目维护者：Sail <sailcpu@gmail.com>
- 问题跟踪：GitHub Issues
