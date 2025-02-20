#include <cassert>
#include "OrderBook.hpp"
#include "Order.hpp"
#include <iostream>

#include <cassert>
#include "OrderBook.hpp"
#include <iostream>

void test_match_orders() {
    OrderBook order_book;

    order_book.add_order(Order(1, OrderType::BUY, 105.0, 10));  
    order_book.add_order(Order(2, OrderType::BUY, 103.0, 5));  
    order_book.add_order(Order(3, OrderType::BUY, 102.0, 8));  

    order_book.add_order(Order(4, OrderType::SELL, 101.0, 6));  
    order_book.add_order(Order(5, OrderType::SELL, 103.0, 7)); 
    order_book.add_order(Order(6, OrderType::SELL, 104.0, 5)); 

    std::cout << "\nExecuting Trades...\n";
    order_book.match_orders();

    const auto& buy_orders = order_book.get_buy_orders();
    const auto& sell_orders = order_book.get_sell_orders();

    // ✅ BUY @ 105 was fully matched and should be removed
    assert(buy_orders.count(105.0) == 0);  

    // ✅ BUY @ 103 should have 2 units remaining
    assert(buy_orders.count(103.0) == 1 && buy_orders.at(103.0).front().get_quantity() == 2); 

    // ✅ BUY @ 102 should be untouched
    assert(buy_orders.count(102.0) == 1 && buy_orders.at(102.0).front().get_quantity() == 8); 

    // ✅ SELL @ 104 should still be in book with 5 units
    assert(sell_orders.count(104.0) == 1 && sell_orders.at(104.0).front().get_quantity() == 5); 

    // ✅ SELL @ 101 and @ 103 should be fully matched and removed
    assert(sell_orders.count(101.0) == 0);
    assert(sell_orders.count(103.0) == 0);


    std::cout << "\n✅ test_match_orders() PASSED\n";
}

void test_match_orders_with_multiple_orders_at_same_price() {
    OrderBook order_book;

    // ✅ Add BUY orders at the same price level
    order_book.add_order(Order(1, OrderType::BUY, 105.0, 5));
    order_book.add_order(Order(2, OrderType::BUY, 105.0, 7));
    order_book.add_order(Order(3, OrderType::BUY, 103.0, 10));
    order_book.add_order(Order(4, OrderType::BUY, 102.0, 6));

    // ✅ Add SELL orders at the same price level
    order_book.add_order(Order(5, OrderType::SELL, 101.0, 4));
    order_book.add_order(Order(6, OrderType::SELL, 103.0, 6));
    order_book.add_order(Order(7, OrderType::SELL, 103.0, 5));
    order_book.add_order(Order(8, OrderType::SELL, 104.0, 8));

    // ✅ Execute trades
    std::cout << "\nExecuting Trades...\n";
    order_book.match_orders();

    // ✅ Check expected BUY book
    const auto& buy_orders = order_book.get_buy_orders();
    assert(buy_orders.count(105.0) == 0);  // Fully executed
    assert(buy_orders.count(103.0) == 1 && buy_orders.at(103.0).front().get_quantity() == 7);
    assert(buy_orders.count(102.0) == 1 && buy_orders.at(102.0).front().get_quantity() == 6);

    // ✅ Check expected SELL book
    const auto& sell_orders = order_book.get_sell_orders();
    assert(sell_orders.count(101.0) == 0);  // Fully executed
    assert(sell_orders.count(103.0) == 0);  // Fully executed
    assert(sell_orders.count(104.0) == 1 && sell_orders.at(104.0).front().get_quantity() == 8);

    std::cout << "\n✅ test_match_orders_with_multiple_orders_at_same_price() PASSED\n";
}


void test_add_order_book() {
    OrderBook order_book;

    for (int i = 0; i < 10; ++i) {
        Order order(i, OrderType::SELL, 3*i, 2*i);
        order_book.add_order(order);
        
        const auto& sell_orders = order_book.get_sell_orders();
        auto it = sell_orders.find(3*i);
        assert(it != sell_orders.end());
        assert(it->second.front().get_order_id() == i);
    }

    for (int i = 0; i < 10; ++i) {
        Order order(i, OrderType::BUY, 3*i, 2*i);
        order_book.add_order(order);

        const auto& buy_orders = order_book.get_buy_orders();
        auto it = buy_orders.find(3*i);
        assert(it != buy_orders.end());
        assert(it->second.front().get_order_id() == i);
    }

    std::cout << "✅ test_add_order_book() PASSED\n";
}

void test_cancel_order() {
    OrderBook order_book;

    order_book.add_order(Order(1, OrderType::BUY, 105.0, 5));
    order_book.add_order(Order(2, OrderType::BUY, 105.0, 7));
    order_book.add_order(Order(3, OrderType::BUY, 103.0, 10));
    order_book.add_order(Order(4, OrderType::BUY, 102.0, 6));

    order_book.add_order(Order(5, OrderType::SELL, 101.0, 4));
    order_book.add_order(Order(6, OrderType::SELL, 103.0, 6));
    order_book.add_order(Order(7, OrderType::SELL, 103.0, 5));
    order_book.add_order(Order(8, OrderType::SELL, 104.0, 8));

    std::cout << "\nCanceling BUY Order 2...\n";
    order_book.cancel_order(2);

    std::cout << "\nCanceling SELL Order 7...\n";
    order_book.cancel_order(7);

    std::cout << "\nCanceling Non-Existent Order 999...\n";
    order_book.cancel_order(999);

    const auto& buy_orders = order_book.get_buy_orders();
    assert(buy_orders.count(105.0) == 1 && buy_orders.at(105.0).front().get_order_id() == 1);
    assert(buy_orders.count(103.0) == 1 && buy_orders.at(103.0).front().get_quantity() == 10);
    assert(buy_orders.count(102.0) == 1 && buy_orders.at(102.0).front().get_quantity() == 6);

    const auto& sell_orders = order_book.get_sell_orders();
    assert(sell_orders.count(103.0) == 1 && sell_orders.at(103.0).front().get_order_id() == 6);
    assert(sell_orders.count(101.0) == 1 && sell_orders.at(101.0).front().get_quantity() == 4);
    assert(sell_orders.count(104.0) == 1 && sell_orders.at(104.0).front().get_quantity() == 8);

    std::cout << "\n✅ test_cancel_order() PASSED\n";
}

int main() {
    test_add_order_book();
    test_match_orders();
    test_match_orders_with_multiple_orders_at_same_price();
    test_cancel_order();
    return 0;
}
