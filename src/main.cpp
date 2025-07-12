#include "crow.h"
#include <iostream>

int main() {
    crow::SimpleApp app;

    CROW_ROUTE(app, "/")([] {
        return "Hello from Crow full repo!";
    });

    app.port(3013).multithreaded().run();
}