/*

Order Class that represents a single BUY or SELL

*/
#ifndef ORDER_HPP
#define ORDER_HPP

#include <chrono>

enum class Side {BUY, SELL};

class Order {
    private:
        uint64_t order_id;
        Side side;
        double price;
        uint32_t quantity;
        std::chrono::time_point<std::chrono::system_clock> timestamp;

    public:
        Order(uint64_t id, Side side, double price, uint32_t quantity);
        uint64_t get_order_id() const;
        Side get_side() const;
        double get_price() const;
        uint32_t get_quantity() const;
        const std::chrono::time_point<std::chrono::system_clock>& get_time() const;
        void print() const;
};

#endif