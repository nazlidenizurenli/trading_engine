/*

Order Class that represents a single BUY or SELL

*/
#ifndef ORDER_HPP
#define ORDER_HPP

#include <chrono>

enum class OrderType {BUY, SELL};

class Order {
    private:
        uint64_t order_id;
        OrderType order_type;
        double price;
        uint32_t quantity;
        std::chrono::time_point<std::chrono::system_clock> timestamp;

    public:
        Order(uint64_t id, OrderType order_type, double price, uint32_t quantity);
        uint64_t get_order_id() const;
        OrderType get_order_type() const;
        double get_price() const;
        uint32_t get_quantity() const;
        void set_quantity(uint32_t new_quantity);
        const std::chrono::time_point<std::chrono::system_clock>& get_time() const;
        void print() const;
};

#endif