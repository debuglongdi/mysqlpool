#pragma once
namespace mysqlpool
{
/// @brief 使用默认private继承
class noncopyable
{
public:
    noncopyable(const noncopyable&) = delete;
    void operator=(const noncopyable&) = delete;
protected:
    noncopyable() = default;
    ~noncopyable() = default;
};
} // namespace mysqlpool
