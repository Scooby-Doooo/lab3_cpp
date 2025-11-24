#include <iostream>
#include <thread>
#include <vector>
#include <latch>
#include <chrono>
#include <mutex>
#include <sstream>
#include "utils.h"

// Synchronization primitives
std::latch latch_a(7);
std::latch latch_b(6);
std::latch latch_c(5);
std::latch latch_d(6);
std::latch latch_e(4);
std::latch latch_f(7);
std::latch latch_g(5);
std::latch latch_h(5);

// Function to simulate action execution
void f(char name, int index) {
    osyncstream(std::cout) << "From set " << name << " action " << index << " executed." << std::endl;
}

void thread1_func() {
    // --- Stage A ---
    f('a', 1);
    latch_a.count_down();
    f('a', 6);
    latch_a.count_down();
    
    latch_a.wait(); // Wait for all A to finish

    // --- Stage B ---
    f('b', 1);
    latch_b.count_down();
    f('b', 2);
    latch_b.count_down();
    f('b', 3);
    latch_b.count_down();

    // --- Stage E ---
    latch_b.wait();
    
    f('e', 1);
    latch_e.count_down();
    f('e', 2);
    latch_e.count_down();
    f('e', 3);
    latch_e.count_down();
    f('e', 4);
    latch_e.count_down();

    // --- Stage I ---
    latch_e.wait();
    latch_f.wait();

    f('i', 1);
    f('i', 2);
    f('i', 3);
}

void thread2_func() {
    // --- Stage A ---
    f('a', 2);
    latch_a.count_down();
    f('a', 7);
    latch_a.count_down();

    latch_a.wait();

    // --- Stage B ---
    f('b', 4);
    latch_b.count_down();
    f('b', 5);
    latch_b.count_down();
    f('b', 6);
    latch_b.count_down();

    // --- Stage F ---
    latch_c.wait();

    f('f', 1);
    latch_f.count_down();
    f('f', 2);
    latch_f.count_down();
    f('f', 3);
    latch_f.count_down();
    f('f', 4);
    latch_f.count_down();

    // --- Stage I ---
    latch_e.wait();
    latch_f.wait();

    f('i', 4);
    f('i', 5);
    f('i', 6);
}

void thread3_func() {
    // --- Stage A ---
    f('a', 3);
    latch_a.count_down();

    latch_a.wait();

    // --- Stage C ---
    f('c', 1);
    latch_c.count_down();
    f('c', 2);
    latch_c.count_down();
    f('c', 3);
    latch_c.count_down();
    f('c', 4);
    latch_c.count_down();

    // --- Stage F & G ---
    latch_c.wait();

    // F part
    f('f', 5);
    latch_f.count_down();
    f('f', 6);
    latch_f.count_down();
    f('f', 7);
    latch_f.count_down();

    // G part
    f('g', 1);
    latch_g.count_down();

    // --- Stage I ---
    latch_e.wait();
    latch_f.wait();

    f('i', 7);
    f('i', 8);

    // --- Stage J ---
    latch_g.wait();
    latch_h.wait();

    f('j', 1);
}

void thread4_func() {
    // --- Stage A ---
    f('a', 4);
    latch_a.count_down();

    latch_a.wait();

    // --- Stage C & D ---
    f('c', 5);
    latch_c.count_down();

    f('d', 1);
    latch_d.count_down();
    f('d', 2);
    latch_d.count_down();
    f('d', 3);
    latch_d.count_down();

    // --- Stage G ---
    latch_c.wait();

    f('g', 2);
    latch_g.count_down();
    f('g', 3);
    latch_g.count_down();
    f('g', 4);
    latch_g.count_down();
    f('g', 5);
    latch_g.count_down();

    // --- Stage J ---
    latch_g.wait();
    latch_h.wait();

    f('j', 2);
    f('j', 3);
}

void thread5_func() {
    // --- Stage A ---
    f('a', 5);
    latch_a.count_down();

    latch_a.wait();

    // --- Stage D ---
    f('d', 4);
    latch_d.count_down();
    f('d', 5);
    latch_d.count_down();
    f('d', 6);
    latch_d.count_down();

    // --- Stage H ---
    latch_d.wait();

    f('h', 1);
    latch_h.count_down();
    f('h', 2);
    latch_h.count_down();
    f('h', 3);
    latch_h.count_down();
    f('h', 4);
    latch_h.count_down();
    f('h', 5);
    latch_h.count_down();

    // --- Stage J ---
    latch_g.wait();
    latch_h.wait();

    f('j', 4);
    f('j', 5);
}

int main() {
    std::cout << "Calculation started." << std::endl;

    std::jthread t1(thread1_func);
    std::jthread t2(thread2_func);
    std::jthread t3(thread3_func);
    std::jthread t4(thread4_func);
    std::jthread t5(thread5_func);

    t1.join();
    t2.join();
    t3.join();
    t4.join();
    t5.join();

    std::cout << "Calculation finished." << std::endl;

    return 0;
}
