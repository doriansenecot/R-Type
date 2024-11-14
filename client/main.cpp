/*
** EPITECH PROJECT, 2024
** Tek3
** File description:
** main.cpp
*/

#include "client.hpp"

int main(void) {
    try {
        Client client;
        client.run();
    } catch (const std::exception &e) {
        std::cerr << "Unhandled exception: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
