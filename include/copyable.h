#pragma once
namespace mysqlpool
{
/// @brief 使用public继承
class copyable
{
protected:
    copyable() = default;
    ~copyable() = default;
};
} // namespace mysqlpool