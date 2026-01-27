#include "HSLAPixel.h"
#include <cmath>
#include <iostream>

using namespace std;

namespace cs225 {

  HSLAPixel::HSLAPixel() : h(0), s(0), l(1), a(1.0) {}

  HSLAPixel::HSLAPixel(double hueValue, double saturationValue, double luminanceValue)
      : h(hueValue), s(saturationValue), l(luminanceValue), a(1.0) {}

  HSLAPixel::HSLAPixel(double hueValue, double saturationValue, double luminanceValue, double alphaValue)
      : h(hueValue), s(saturationValue), l(luminanceValue), a(alphaValue) {}

}