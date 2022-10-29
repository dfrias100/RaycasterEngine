#include "GameApp.h"

class TestApp : public GameApp {
    bool OnUpdate(float fFrameTime) override {
	return true;
    };

    bool OnInitialize() override {
	return true;
    };
};

int main(int argc, char** argv) {
    TestApp* pApp = new TestApp();
    int nRetVal = pApp->RunGame();
    delete pApp;
    return nRetVal;
}