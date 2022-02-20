// ===========================================================================
// Program.cppm
// ===========================================================================

import Module;
import Module2;

void useModule()
{
    anotherMyFunc();
}

void useModuleAgain()
{
    Point p{ 1, 2 };
    Rectangle r{ p, 10, 20 };

    NamespaceExample::connect();
    NamespaceExample::disconnect();

    AnotherNamespaceExample::doConnect();
    AnotherNamespaceExample::doDisconnect();
    // AnotherNamespaceExample::doOutput("bla");  // error
}

int main()
{
    myFunc();
    useModule();
    useModuleAgain();
    return 0;
}

// ===========================================================================
// End-of-File
// ===========================================================================
