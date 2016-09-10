#ifndef EASYCHANGE_H
#define EASYCHANGE_H


class EasyChange
{
public:
    EasyChange();

    void setThreshold(float threshold);
    void setSpeed(float speed);
    void setChange(float change);

    void reset();

    void update(float depth);
    void setDepthHard(float depth);

    float getEasyDepth();

private:
    void draw();

    float getEasyVal(float x);

private:
    float threshold;
    float speed;
    float change;

    float startDepth;
    bool needRecalculateTime;

    bool isChanging;
    float easyDepth;
    bool needIncrease;

    float changeTime;

    long long startTime;
};

#endif // EASYCHANGE_H
