#ifndef __GPIO_H_
#define __GPIO_H_

class Gpio {
    public:
    void Init();

    void SetQuarterPulseLed(bool active);
    bool StartStopButtonPressed();

};

#endif // __GPIO_H_

