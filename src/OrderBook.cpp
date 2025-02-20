#include "OrderBook.hpp"
#include <map>
#include <deque>
#include <iostream>

void OrderBook::add_order(const Order& order) {
    double order_price = order.get_price();
    OrderType type = order.get_order_type();
    if (type == OrderType::SELL) {
        sell_orders[order_price].push_back(order);
    } else {
        buy_orders[order_price].push_back(order);
    }
}

void OrderBook::match_orders() {
    // Continue processing as long as there are orders on both sides
    while (!buy_orders.empty() && !sell_orders.empty()) {
        // Get the highest BUY order
        auto buy_it = buy_orders.begin();
        double current_buy_price = buy_it->first;
        Order& highest_buy_order = buy_it->second.front();

        // Get the lowest SELL order
        auto sell_it = sell_orders.begin();
        double current_sell_price = sell_it->first;
        Order& lowest_sell_order = sell_it->second.front();

        // If no match is possible, break the loop (instead of return)
        if (current_buy_price < current_sell_price) {
            break;
        }

        // Determine the maximum quantity that can be traded
        uint32_t buy_amount = highest_buy_order.get_quantity();
        uint32_t sell_amount = lowest_sell_order.get_quantity();
        uint32_t trade_amount = std::min(buy_amount, sell_amount);

        // ✅ Print trade execution details
        std::cout << "💰 Trade Executed: " << trade_amount
                  << " units @ $" << current_sell_price
                  << " (BUY ID: " << highest_buy_order.get_order_id()
                  << ", SELL ID: " << lowest_sell_order.get_order_id() << ")\n";

        // ✅ Update order quantities and remove fully filled orders
        if (buy_amount > sell_amount) {
            highest_buy_order.set_quantity(buy_amount - trade_amount);
            sell_it->second.pop_front();
            if (sell_it->second.empty()) {
                sell_orders.erase(sell_it);
            }
        } else if (buy_amount < sell_amount) {
            lowest_sell_order.set_quantity(sell_amount - trade_amount);
            buy_it->second.pop_front();
            if (buy_it->second.empty()) {
                buy_orders.erase(buy_it);
            }
        } else {
            buy_it->second.pop_front();
            sell_it->second.pop_front();

            if (buy_it->second.empty()) {
                buy_orders.erase(buy_it);
            }
            if (sell_it->second.empty()) {
                sell_orders.erase(sell_it);
            }
        }
    }
}

bool OrderBook::cancel_order(uint64_t order_id) {
    for (auto buy_it = buy_orders.begin(); buy_it != buy_orders.end(); ++buy_it) {
        std::deque<Order>& order_queue = buy_it->second;
        auto it = std::remove_if(order_queue.begin(), order_queue.end(),
                                 [&](const Order& order) { return order.get_order_id() == order_id; });

        if (it != order_queue.end()) {
            order_queue.erase(it, order_queue.end());
            if (order_queue.empty()) {
                buy_orders.erase(buy_it);
            }
            std::cout << "✅ Order " << order_id << " successfully canceled from BUY orders.\n";
            return true;
        }
    }

    for (auto sell_it = sell_orders.begin(); sell_it != sell_orders.end(); ++sell_it) {
        std::deque<Order>& order_queue = sell_it->second;
        auto it = std::remove_if(order_queue.begin(), order_queue.end(),
                                 [&](const Order& order) { return order.get_order_id() == order_id; });

        if (it != order_queue.end()) {
            order_queue.erase(it, order_queue.end());
            if (order_queue.empty()) {
                sell_orders.erase(sell_it);
            }
            std::cout << "✅ Order " << order_id << " successfully canceled from SELL orders.\n";
            return true;
        }
    }

    std::cout << "❌ Order " << order_id << " not found.\n";
    return false;
}


// Getter for SELL orders
const std::map<double, std::deque<Order>>& OrderBook::get_sell_orders() const {
    return sell_orders;
}

// Getter for BUY orders
const std::map<double, std::deque<Order>, std::greater<>>& OrderBook::get_buy_orders() const {
    return buy_orders;
}


void OrderBook::print_order_book() const {
    std::cout << "================ ORDER BOOK ================\n";

    // Print SELL orders (lowest price first)
    std::cout << "SELL ORDERS:\n";
    for (const auto& [price, orders] : sell_orders) {
        std::cout << "Price: $" << price << " → ";
        for (const auto& order : orders) {
            std::cout << "[ID: " << order.get_order_id() << ", Qty: " << order.get_quantity() << "] ";
        }
        std::cout << "\n";
    }

    std::cout << "--------------------------------------------\n";

    // Print BUY orders (highest price first)
    std::cout << "BUY ORDERS:\n";
    for (const auto& [price, orders] : buy_orders) {
        std::cout << "Price: $" << price << " → ";
        for (const auto& order : orders) {
            std::cout << "[ID: " << order.get_order_id() << ", Qty: " << order.get_quantity() << "] ";
        }
        std::cout << "\n";
    }

    std::cout << "============================================\n";
}