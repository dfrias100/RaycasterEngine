#include "Raycast.h"

int main(int argc, char** argv) {
    Raycast* pRayApp = new Raycast();
    int nRetVal = pRayApp->RunGame();
    delete pRayApp;
    return nRetVal;
}