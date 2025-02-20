#include "Order.hpp"
#include <cassert>
#include <iostream>

void test_order_creation() {
    std::cout << "Running test_order_creation()..." << std::endl;

    // Create order
    Order order(1, Side::SELL, 34.6, 5);

    // Assertions to verify correctness
    assert(order.get_order_id() == 1);
    assert(order.get_side() == Side::SELL);
    assert(order.get_price() == 34.6);
    assert(order.get_quantity() == 5);

    order.print();

    std::cout << "✅ test_order_creation() passed!" << std::endl;
}

int main() {
    test_order_creation();
    std::cout << "All tests passed successfully!" << std::endl;
    return 0;
}