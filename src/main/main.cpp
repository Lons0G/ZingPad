#include "ui/AppUI.hpp"

int main(int argc, char *argv[]) {
    AppUI app;

    // SI EXISTE UN ERROR SE CIERRA LA APLICACION
    if (!app.openCamera()) {
        return -1;
    }
    app.ShowVideoLoop();

    return 0;
}
