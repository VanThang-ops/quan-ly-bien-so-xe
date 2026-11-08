#include "ui/Menu.h"

int main() {
    LicensePlateService service;
    Menu menu(service);
    menu.run();
    return 0;
}
