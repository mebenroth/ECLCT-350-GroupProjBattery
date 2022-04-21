
#include "Simulator.h"

//Trouble in GetSOC function when mulitplying by step h

class Battery : public Device
{
    public:

    // Constructor (Va, f, and phi are optional arguments):

    Battery(int positive, int negative, double wh, double soc0);

    // Device interface:
    
    void Init();
    //void DC();
    void Step(double t, double dt);

    // Viewable functions:
    
    //double GetVoltage();
    //double GetCurrent();
    //double GetPower();
    double GetSOC();
    double GetTerminalVoltage();
    double GetTerminalCurrent();

    // Member Variables:
    
    int positive;
    int negative;
    int nodep;
    double wh;
    double soc0;
    //double f;
    //double phi;

};


Battery::Battery(int positive, int negative, double wh, double soc0)
{
    this->positive = positive;
    this->negative = negative;
    this->nodep = nodep;
    this->wh = wh;
    this->soc0 = soc0;
    //this->f = f;
    //this->phi = phi;
}

void Battery::Init()
{
    // this is where and how to setup internal nodes:
    this->nodep = GetNextNode();
}

void Battery::Step(double t, double dt)
{
    //double v = Vdc + Va * sin(2.0 * M_PI * f * t + phi);
    double v = GetTerminalVoltage();
    
    AddJacobian(positive, nodep, 1.0);
    AddJacobian(negative, nodep, -1.0);
    AddJacobian(nodep, positive, 1.0);
    AddJacobian(nodep, negative, -1.0);
    AddBEquivalent(nodep, v);
}

//void VoltageSource::DC()
//{
//    AddJacobian(positive, nodep, -1.0);
//    AddJacobian(negative, nodep, 1.0);
//    AddJacobian(nodep, positive, -1.0);
//    AddJacobian(nodep, negative, 1.0);
//    AddBEquivalent(nodep, Vdc);
//}

double Battery::GetTerminalVoltage()
{
    return GetStateDifference(positive, negative);
}

double Battery::GetTerminalCurrent()
{
    return -GetState(nodep);
}

double Battery::GetSOC()
{
    return GetSOC() + ((GetTerminalVoltage() * GetTerminalCurrent() * dt) / (wh * 3600));
}

//double Battery::GetTerminalVoltage()
//{
//    return GetVoltage() * GetCurrent();
//}

//double Battery::GetTerminalCurrent()
//{
//    return;
//}