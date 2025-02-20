/*

Class for storing and managing Orders

*/
#ifndef ORDER_BOOK_HPP
#define ORDER_BOOK_HPP
#include <map>
#include <deque>
#include "Order.hpp"

class OrderBook {
    private:
        std::map<double, std::deque<Order>, std::greater<>> buy_orders;
        std::map<double, std::deque<Order>> sell_orders;
    public:
        // Constructor
        // OrderBook();

        // Add an order (handles both BUY and SELL)
        void add_order(const Order& order);

        // Match buy and sell orders
        void match_orders();

        // Cancel an order by order ID
        bool cancel_order(uint64_t order_id);

        // Print the order book
        void print_order_book() const;

        const std::map<double, std::deque<Order>>& get_sell_orders() const;

        const std::map<double, std::deque<Order>, std::greater<>>& get_buy_orders() const;
};

#endif