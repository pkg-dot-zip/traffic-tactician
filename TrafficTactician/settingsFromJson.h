struct settings_0_t {
  double anglesForDirectionCalculationTolerance;
  std::uint64_t blurKSize;
  std::uint64_t blurSigmaX;
  std::uint64_t blurSigmaY;
  std::string_view caffemodel;
  std::uint64_t cameraToUse;
  double confTh;
  double confidenceMapThreshold;
  std::uint64_t downscaleTargetHeight;
  std::uint64_t downscaleTargetWidth;
  double fogExponentialDensity;
  double fogLinearFar;
  double fogLinearNear;
  std::string_view fogType;
  double heightDifferenceBetweenShoulderAndWristTolerance;
  bool mxaaEnabled;
  std::uint64_t nInterpSamples;
  double pafScoreTh;
  double personOrientationShoulderTolerance;
  std::string_view preferredDevice;
  std::string_view prototxt;
  std::uint64_t spatialSizeFactor;
  double toleranceWristNearShoulderDistanceLeftAndDown;
  double toleranceWristNearShoulderDistanceX;
  double toleranceWristNearShoulderDistanceY;
  std::uint64_t upscaleTargetHeight;
  std::uint64_t upscaleTargetWidth;
  bool useColorsForPose;
  bool useFog;
  bool useRealTimePriority;
};

constexpr settings_0_t settings = {
  .anglesForDirectionCalculationTolerance = 20.000000, 
  .blurKSize = 3, 
  .blurSigmaX = 0, 
  .blurSigmaY = 0, 
  .caffemodel = R"##(./pose/coco/pose_iter_440000.caffemodel)##", 
  .cameraToUse = 0, 
  .confTh = 0.700000, 
  .confidenceMapThreshold = 0.100000, 
  .downscaleTargetHeight = 192, 
  .downscaleTargetWidth = 256, 
  .fogExponentialDensity = 0.020000, 
  .fogLinearFar = 0.000000, 
  .fogLinearNear = 0.000000, 
  .fogType = R"##(exp2)##", 
  .heightDifferenceBetweenShoulderAndWristTolerance = 40.000000, 
  .mxaaEnabled = true, 
  .nInterpSamples = 10, 
  .pafScoreTh = 0.100000, 
  .personOrientationShoulderTolerance = 20.000000, 
  .preferredDevice = R"##(cpu)##", 
  .prototxt = R"##(./pose/coco/pose_deploy_linevec.prototxt)##", 
  .spatialSizeFactor = 100, 
  .toleranceWristNearShoulderDistanceLeftAndDown = 5.000000, 
  .toleranceWristNearShoulderDistanceX = 30.000000, 
  .toleranceWristNearShoulderDistanceY = 60.000000, 
  .upscaleTargetHeight = 768, 
  .upscaleTargetWidth = 1024, 
  .useColorsForPose = true, 
  .useFog = true, 
  .useRealTimePriority = true, 
};
