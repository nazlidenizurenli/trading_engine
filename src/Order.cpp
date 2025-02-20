#include "Order.hpp"
#include <iostream>
#include <string>

Order::Order(uint64_t id, OrderType order_type, double price, uint32_t quantity)
    : order_id(id), order_type(order_type), price(price), quantity(quantity),
      timestamp(std::chrono::system_clock::now()) {}

uint64_t Order::get_order_id() const{
    return this->order_id;
}

OrderType Order::get_order_type() const{
    return this->order_type;
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

std::string side_to_string(OrderType order_type) {
    return (order_type == OrderType::BUY) ? "BUY" : "SELL";
}

void Order::print() const {
    std::cout << "Order ID: " << this->order_id 
          << " | Price: " << this->price 
          << " | Quantity: " << this->quantity 
          << " | Side: " << side_to_string(this->order_type) 
          << std::endl;
}

void Order::set_quantity(uint32_t new_quantity) {
    this->quantity = (new_quantity >= 0) ? new_quantity : 0;
}


