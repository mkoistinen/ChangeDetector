# ChangeDetector

Simple Arduino-compatible library for smoothing a sequence of input values and alerting if they stray from a "normal" range. This is useful for taming sensor input where the sensor may vary a lot due to noise, ambient conditions or just slowly changing conditions.

Create an instance like this:

```c++
...
#include "ChangeDetector.h"
...

// Constructor: ChangeDetector(num_values, threshold, magnitude)
ChangeDetector cp = ChangeDetector(1000, 10, 3.5);

void someLoop() {
   ...
   value = digitalRead(WATER_LEVEL_SENSOR);
   int state = cp.run(value);
   if (state == ChangeDetector::CHANGE_UP) {
      // The sensor picked up an abnormally large change UP in value...
      ...
      // Water level has sharply risen! Did someone fall in the tub?
   }
   else if (state == ChangeDetector::CHANGE_DOWN) {
      // The sensor picked up an abnormally large change DOWN in value...
      // Water level has sharply dropped! Did we spring a leak?
      ...
   }
   else {
      // Nothing unusual going on here...
      int average cp.get_smooth();
      // use smoothed value for something...
      ...
   }
}
```

This example will maintain a moving average of up to 1000 data points and internal compute the standard deviation of these values. If the next data point is closer to the moving average than `threshold` and `magnitude` standard deviations, then the `run` method will return a constant `NO_CHANGE`. If the input value differs from the moving average by at least `threshold` and `magnitude` standard deviations, then it will return either `CHANGE_UP` or `CHANGE_DOWN`, which ever is appropriate.

Folks familiar with the financial markets will quickly identify similarities to Bollinger Bands.

This example of measuring water level is a good one, because we could imagine that the water level will probably undulate up and down as it sits in a tub. We're not really all that interested in these movements, but rather the moving average of these movements. However, we may be interested if these values sharply rise or fall, as the narrative in the comments above suggest.
