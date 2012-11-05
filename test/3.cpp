#include <iostream>

using namespace std;

class App
{
public:
   App() {};
   ~App() {};
   virtual void init() { cout << "Inside App::init" << endl; };
   virtual void main() { cout << "Inside App::main" << endl; init(); };
};

class MyApp: public App
{
public:
   MyApp() {};
   ~MyApp() {};
   virtual void init() { cout << "Inside MyApp::init" << endl; };
};

class MyActor: public MyApp
{
public:
   MyActor() {};
   ~MyActor() {};
   virtual void init() { cout << "Inside MyActor::init" << endl; };
};

int main(int argc, char **argv)
{
    MyActor().main();
}
