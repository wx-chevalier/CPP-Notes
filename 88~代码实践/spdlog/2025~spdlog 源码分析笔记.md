# spdlog 源码分析笔记

## 1. 整体架构

spdlog 采用了典型的前端-后端架构设计：

```cpp
// 核心组件关系
Logger (前端) -> Sinks (后端) -> 实际的输出目标

// 基本类图
class logger {
    std::vector<sink_ptr> sinks_; // 支持多个sink
    std::string name_;
    level_t level_{level::info};
    // ...
};

class sink {
    virtual void log(const details::log_msg& msg) = 0;
    virtual void flush() = 0;
    // ...
};
```

## 2. 关键模块分析

### 2.1 Logger 实现

```cpp
// include/spdlog/logger.h
class SPDLOG_API logger
{
public:
    // 主要的日志接口
    template<typename... Args>
    void log(source_loc loc, level::level_enum lvl, fmt::format_string<Args...> fmt, Args &&...args)
    {
        if (!should_log(lvl)) {
            return;
        }

        SPDLOG_TRY
        {
            // 创建日志消息
            memory_buf_t buf;
            fmt::format_to(std::back_inserter(buf), fmt, std::forward<Args>(args)...);
            details::log_msg log_msg(loc, name_, lvl, string_view_t(buf.data(), buf.size()));
            sink_it_(log_msg);
        }
        SPDLOG_LOGGER_CATCH(loc)
    }

private:
    void sink_it_(const details::log_msg& msg) {
        for (auto& sink : sinks_) {
            sink->log(msg);
        }
    }
};
```

### 2.2 Sink 体系

```cpp
// include/spdlog/sinks/base_sink.h
template<typename Mutex>
class base_sink : public sink
{
protected:
    Mutex mutex_;

public:
    void log(const details::log_msg& msg) final
    {
        std::lock_guard<Mutex> lock(mutex_);
        sink_it_(msg);
    }

protected:
    virtual void sink_it_(const details::log_msg& msg) = 0;
};

// 文件 Sink 实现
template<typename Mutex>
class basic_file_sink final : public base_sink<Mutex>
{
    std::FILE* fp_{nullptr};

protected:
    void sink_it_(const details::log_msg& msg) override
    {
        fmt::memory_buffer formatted;
        base_sink<Mutex>::formatter_->format(msg, formatted);
        std::fwrite(formatted.data(), sizeof(char), formatted.size(), fp_);
    }
};
```

### 2.3 异步日志实现

```cpp
// include/spdlog/async.h
class SPDLOG_API async_logger : public std::enable_shared_from_this<async_logger>
{
private:
    std::shared_ptr<details::thread_pool> thread_pool_;

    void async_sink_it_(const details::log_msg& msg)
    {
        if (thread_pool_->overrun_counter() == 0)
        {
            thread_pool_->post_log(shared_from_this(), msg, overflow_policy_);
        }
        else if (overflow_policy_ == overflow_policy::block)
        {
            thread_pool_->post_log(shared_from_this(), msg, overflow_policy_);
        }
    }
};

// 线程池实现
class SPDLOG_API thread_pool
{
    using item_type = async_msg;
    using q_type = details::mpmc_blocking_queue<item_type>;

    q_type q_;
    std::vector<std::thread> threads_;

    void worker_loop_()
    {
        while (process_next_msg_()) {}
    }

    bool process_next_msg_()
    {
        async_msg incoming_async_msg;
        bool dequeued = q_.dequeue_for(incoming_async_msg, std::chrono::seconds(10));
        if (!dequeued)
        {
            return true;
        }

        incoming_async_msg.worker_ptr->log(incoming_async_msg.msg);
        return true;
    }
};
```

### 2.4 格式化系统

```cpp
// include/spdlog/formatter.h
class SPDLOG_API formatter
{
public:
    virtual ~formatter() = default;
    virtual void format(const details::log_msg& msg, memory_buf_t& dest) = 0;
    virtual std::unique_ptr<formatter> clone() const = 0;
};

// 默认格式化器实现
class SPDLOG_API pattern_formatter final : public formatter
{
    std::vector<std::unique_ptr<pattern_flag>> formatters_;
    std::string pattern_;

public:
    void format(const details::log_msg& msg, memory_buf_t& dest) override
    {
        for (auto& f : formatters_)
        {
            f->format(msg, dest);
        }
    }
};
```

## 3. 关键设计特点

### 3.1 内存管理

```cpp
// 使用内存池优化小字符串分配
template<typename T>
class memory_pool
{
    std::vector<T*> chunks_;
    std::vector<T*> free_list_;

public:
    T* allocate()
    {
        if (free_list_.empty())
        {
            // 分配新的chunk
            chunks_.push_back(new T[ChunkSize]);
            // 填充free list
            for(size_t i = 0; i < ChunkSize; ++i)
            {
                free_list_.push_back(&chunks_.back()[i]);
            }
        }

        T* item = free_list_.back();
        free_list_.pop_back();
        return item;
    }
};
```

### 3.2 线程安全设计

```cpp
// 使用模板参数控制线程安全
template<typename Mutex>
class SPDLOG_API base_logger
{
protected:
    Mutex mutex_;

    template<typename... Args>
    void log_(level::level_enum lvl, fmt::format_string<Args...> fmt, Args &&...args)
    {
        std::lock_guard<Mutex> lock(mutex_);
        // 日志处理逻辑
    }
};

// 支持不同的互斥类型
using logger_mt = base_logger<std::mutex>;                // 线程安全版本
using logger_st = base_logger<details::null_mutex>;       // 非线程安全版本
```

### 3.3 性能优化

```cpp
// 1. 日志级别快速过滤
if (!should_log(level))
    return;

// 2. 使用fmt库进行快速格式化
template<typename... Args>
void info(fmt::format_string<Args...> fmt, Args &&...args)
{
    log(level::info, fmt, std::forward<Args>(args)...);
}

// 3. 使用内存缓冲区减少内存分配
memory_buf_t buf;
fmt::format_to(std::back_inserter(buf), fmt, args...);
```

## 4. 核心流程

### 4.1 日志写入流程

```cpp
// 1. 日志调用入口
logger->info("message");

// 2. 格式化消息
memory_buf_t buf;
fmt::format_to(buf, ...);

// 3. 创建日志消息对象
log_msg msg(level, buf);

// 4. 投递到 sinks
for (auto& sink : sinks_) {
    sink->log(msg);
}

// 5. sink 处理消息
void sink_it_(const log_msg& msg) {
    fmt::memory_buffer formatted;
    formatter_->format(msg, formatted);
    write_to_target(formatted);
}
```

这些是 spdlog 的核心实现细节。它的设计特点包括：

1. **模块化设计**

   - Logger/Sink 分离
   - 可扩展的格式化系统

2. **性能优化**

   - 内存池
   - 异步日志
   - 快速的格式化库

3. **灵活性**

   - 支持多种输出目标
   - 可配置的线程安全性
   - 可自定义的格式化

4. **可靠性**
   - 异常处理
   - 优雅的错误处理
   - 线程安全保证

理解这些实现细节对于：

1. 更好地使用 spdlog
2. 学习 C++ 项目设计
3. 实现自己的日志系统
