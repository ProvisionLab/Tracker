#ifndef FPSCOUNTER_H
#define FPSCOUNTER_H


class FpsCounter
{
public:
    FpsCounter();

    int updateFps();

private:
    long long lastTime_;
    double prevFps_;
};

#endif // FPSCOUNTER_H
