#include "application.h"
#include "ChangeDetector.h"
#include <math.h>

float fabs(float num) {
    if (num < 0.0) {
        return -1.0 * num;
    }
    return num;
}

const uint8_t ChangeDetector::NO_CHANGE = 0x00;
const uint8_t ChangeDetector::CHANGE_UP = 0x01;
const uint8_t ChangeDetector::CHANGE_DOWN = 0x02;

ChangeDetector::ChangeDetector(uint16_t count_p, uint16_t threshold_p, float magnitude_p) {
    count = constrain(count_p, 1, 2000);
    threshold = abs(threshold_p);
    magnitude = fabs(magnitude_p);

    std_dev = 0.0;
    primed = false;
    runs_remaining = count;
    
    for (int i=0; i<2000; i++) {
        values_list[i] = 0;
    }
}

uint16_t ChangeDetector::get_raw() {
    return raw;
}

float ChangeDetector::get_smooth() {
    return smooth;
}

float ChangeDetector::get_std_dev() {
    return std_dev;
}

uint8_t ChangeDetector::run(uint16_t value) {

    uint8_t state = ChangeDetector::NO_CHANGE;
    float fraw, delta, delta_sum;
    uint16_t idx;

    raw = value;

    // It can't be a peak if we're not primed...
    if (primed) {
        fraw = (float) raw;
        if (fabs(fraw - smooth) > max(threshold, magnitude * std_dev)) {
            if (fraw > smooth) {
                state = ChangeDetector::CHANGE_UP;
            }
            else {
                state = ChangeDetector::CHANGE_DOWN;
            }
        }
    }
    else {
        if (--runs_remaining == 0) {
            primed = true;
        }
    }

    values_idx = (values_idx + 1) % count;
    
    // Instead of looping through all the values to take the sum
    // We simply subtract the old value and add the new value.
    avg_sum -= values_list[values_idx];
    avg_sum += raw;
    values_list[values_idx] = raw;

    // Compute the standard deviation for use next time
    if (count > 0) {
        smooth = (float) avg_sum / (float) count;
        delta_sum = 0.0;
        for (idx=0; idx<count; idx++) {
            delta = (float) values_list[idx] - smooth;
            delta_sum += delta * delta;
        }
        std_dev = sqrt(delta_sum / (float) count);
    }

    return state;
}
