// ===========================================================================
// Module2.ixx
// ===========================================================================

export module Module2;

import <iostream>;
import <string>;

// -------------------------------------------------------
// Interfaces

// public
export void anotherMyFunc();

export struct Point
{
    int m_x;
    int m_y;
};

export class Rectangle
{
private:
    Point m_upperLeft;
    int m_width;
    int m_height;

public:
    Rectangle(Point p, int width, int height) 
        : m_upperLeft{ p }, m_width{ width }, m_height{ height } {}
};

export namespace NamespaceExample {
    void connect() {
        std::cout << "connect" << std::endl;
    }
    void disconnect() {
        std::cout << "disconnect" << std::endl;
    }
}

namespace AnotherNamespaceExample {
    void doOutput(const std::string& s) {
        std::cout << s << std::endl;
    }
    export void doConnect() {
        doOutput("doConnect");
    }
    export void doDisconnect() {
        doOutput("doDisconnect");
    }
}

// private
void anotherMyFuncImpl();

// -------------------------------------------------------
// Implementation

void anotherMyFuncImpl()
{
    std::cout << "Another Hello Modules again:)" << std::endl;
}

void anotherMyFunc()
{
    anotherMyFuncImpl();
}

// ===========================================================================
// End-of-File
// ===========================================================================
