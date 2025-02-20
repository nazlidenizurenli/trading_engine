#include "Order.hpp"
#include <iostream>
#include <string>

Order::Order(uint64_t id, Side side, double price, uint32_t quantity)
    : order_id(id), side(side), price(price), quantity(quantity),
      timestamp(std::chrono::system_clock::now()) {}

uint64_t Order::get_order_id() const{
    return this->order_id;
}

Side Order::get_side() const{
    return this->side;
}

double Order::get_price() const{
    return this->price;
}

uint32_t Order::get_quantity() const{
    return this->quantity;
}

const std::chrono::time_point<std::chrono::system_clock>& Order::get_time() const {
    return this->timestamp;
}

std::string side_to_string(Side side) {
    return (side == Side::BUY) ? "BUY" : "SELL";
}

void Order::print() const {
    std::cout << "Order ID: " << this->order_id 
          << " | Price: " << this->price 
          << " | Quantity: " << this->quantity 
          << " | Side: " << side_to_string(this->side) 
          << std::endl;
}

