#include <iostream>
#include <thread>
#include <mutex>
#include <chrono>

std::mutex mtx;
int client_count = 0;
const int max_clients = 10;


void client_function() {
    for (int i = 0; i < max_clients; ++i) {
        std::this_thread::sleep_for(std::chrono::seconds(1));
        std::lock_guard<std::mutex> lock(mtx);
        client_count++;
        std::cout << "Клиент пришёл. Клиентов: " << client_count << std::endl;
    }
}


void operator_function() {
    while (true) {
        std::this_thread::sleep_for(std::chrono::seconds(2));
        {
            std::lock_guard<std::mutex> lock(mtx);
            if (client_count > 0) {
                client_count--;
                std::cout << "Клиент ушёл. Осталось: " << client_count << std::endl;
            }
            else {
                std::cout << "Нету клиентов." << std::endl;
                break;
            }
        }
    }
}

int main() {
    setlocale(LC_ALL, "Russian");

    std::thread client_thread(client_function);
    std::thread operator_thread(operator_function);

    client_thread.join();
    operator_thread.join();

    std::cout << "Конец." << std::endl;
    return 0;
}