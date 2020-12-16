
#include <model/CHSI.hpp>
#include <ctrl/track.hpp>

class LekkasFossen : public TrackingCtrl {

  CubicHermineSplineInterpolation interpolation;

public:
  LekkasFossen(const MotionModel& model, const double& ts,
	       const char* input_file_name,
	       const iSeeML::rob::OrPtConfig& init_config)
    : TrackingCtrl(model, ts, input_file_name, init_config) {}
};
