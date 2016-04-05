#include "application.h"

#ifndef ChangeDetector_h
#define ChangeDetector_h

class ChangeDetector {
    public:
        ChangeDetector(uint16_t, uint16_t, float);
        
        static const uint8_t NO_CHANGE;
        static const uint8_t CHANGE_UP;
        static const uint8_t CHANGE_DOWN;

        uint16_t count;
        uint16_t magnitude;
        float threshold;

        // Holds the most recently read value from the pin
        uint16_t raw;
        // Holds the most recently computed smoothed value
        float smooth;
        
        uint8_t run(uint16_t);
        uint16_t add_value(uint16_t);
        uint16_t get_raw(void);
        float get_smooth(void);
        float get_std_dev(void);
        
    private:
        bool primed;
        uint8_t runs_remaining;
        // For maintaining the rolling average
        uint16_t values_list[2000];
        uint16_t values_idx;
        uint32_t avg_sum;
        float std_dev;
};

#endif
