struct dnnsettings_0_t {
  double anglesForDirectionCalculationTolerance;
  std::string_view caffemodel;
  std::uint64_t cameraToUse;
  std::uint64_t downscaleTargetHeight;
  std::uint64_t downscaleTargetWidth;
  double heightDifferenceBetweenShoulderAndWristTolerance;
  double personOrientationShoulderTolerance;
  std::string_view preferredDevice;
  std::string_view prototxt;
  double toleranceWristNearShoulderDistanceLeftAndDown;
  double toleranceWristNearShoulderDistanceX;
  double toleranceWristNearShoulderDistanceY;
  std::uint64_t upscaleTargetHeight;
  std::uint64_t upscaleTargetWidth;
  bool useColorsForPose;
  bool useRealTimePriority;
};

constexpr dnnsettings_0_t dnnsettings = {
  .anglesForDirectionCalculationTolerance = 20.000000, 
  .caffemodel = R"##(./pose/coco/pose_iter_440000.caffemodel)##", 
  .cameraToUse = 0, 
  .downscaleTargetHeight = 192, 
  .downscaleTargetWidth = 256, 
  .heightDifferenceBetweenShoulderAndWristTolerance = 40.000000, 
  .personOrientationShoulderTolerance = 20.000000, 
  .preferredDevice = R"##(cpu)##", 
  .prototxt = R"##(./pose/coco/pose_deploy_linevec.prototxt)##", 
  .toleranceWristNearShoulderDistanceLeftAndDown = 5.000000, 
  .toleranceWristNearShoulderDistanceX = 30.000000, 
  .toleranceWristNearShoulderDistanceY = 60.000000, 
  .upscaleTargetHeight = 768, 
  .upscaleTargetWidth = 1024, 
  .useColorsForPose = true, 
  .useRealTimePriority = true, 
};
