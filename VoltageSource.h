
#include "Simulator.h"
#include "Battery.h"


class VoltageSource : public Device
{
    public:

    // Constructor (Va, f, and phi are optional arguments):

    //VoltageSource(int nodei, int nodej, double Vdc, double Va=0.0, double f=0.0, double phi=0.0);
    VoltageSource(int nodei, int nodej, double SOC, double, double k = -35.0, double a0 = 3.685, double a1 = 0.2156,
        double a2 = -0.1178, double a3 = 0.3201);
    // Device interface:
    
    void Init();
    void DC();
    void Step(double t, double dt);

    // Viewable functions:
    
    double GetVoltage();
    double GetCurrent();
    double GetPower();

    // Member Variables:
    
    int nodei;
    int nodej;
    int nodep;
    /*double Vdc;
    double Va;
    double f;
    double phi;*/
    double SOC;
    double A;
    double k;
    double a0;
    double a1;
    double a2;
    double a3;

};


VoltageSource::VoltageSource(int nodei, int nodej,double SOC, double A, double k, double a0, double a1, double a2, double a3)
{
    this->nodei = nodei;
    this->nodej = nodej;
    /*this->Vdc = Vdc;
    this->Va = Va;
    this->f = f;
    this->phi = phi;*/
    this->SOC = SOC;
    this->A = A;
    this->k = k;
    this->a0 = a0;
    this->a1 = a1;
    this->a2 = a2;
    this->a3 = a3;
}

void VoltageSource::Init()
{
    // this is where and how to setup internal nodes:
    this->nodep = GetNextNode();
}

void VoltageSource::Step(double t, double dt)
{
    //double v = Vdc + Va * sin(2.0 * M_PI * f * t + phi);
    double v = -1.031 *  * exp(-35.0 * GetSOC()) + a0 + a1 * GetSOC() + a2 * (GetSOC())^2) + a3 * (GetSOC())^3;
    
    AddJacobian(nodei, nodep, 1.0);
    AddJacobian(nodej, nodep, -1.0);
    AddJacobian(nodep, nodei, 1.0);
    AddJacobian(nodep, nodej, -1.0);
    AddBEquivalent(nodep, v);
}

void VoltageSource::DC()
{
    AddJacobian(nodei, nodep, -1.0);
    AddJacobian(nodej, nodep, 1.0);
    AddJacobian(nodep, nodei, -1.0);
    AddJacobian(nodep, nodej, 1.0);
    AddBEquivalent(nodep, Vdc);
}

double VoltageSource::GetVoltage()
{
    return GetStateDifference(nodei, nodej);
}

double VoltageSource::GetCurrent()
{
    return -GetState(nodep);
}

double VoltageSource::GetPower()
{
    return GetVoltage() * GetCurrent();
}
