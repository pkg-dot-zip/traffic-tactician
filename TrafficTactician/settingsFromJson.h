struct settings_0_t {
  std::uint64_t blurKSize;
  std::uint64_t blurSigmaX;
  std::uint64_t blurSigmaY;
  std::string_view caffemodel;
  std::uint64_t cameraToUse;
  double confTh;
  double confidenceMapThreshold;
  double downscaleFactor;
  double heightElbowTolerance;
  std::uint64_t nInterpSamples;
  double pafScoreTh;
  std::string_view preferredDevice;
  std::string_view prototxt;
  std::uint64_t spatialSizeFactor;
  double tolerance;
  double upscaleFactor;
  bool useColorsForPose;
  bool useRealTimePriority;
};

constexpr settings_0_t settings = {
  .blurKSize = 3, 
  .blurSigmaX = 0, 
  .blurSigmaY = 0, 
  .caffemodel = R"##(./pose/coco/pose_iter_440000.caffemodel)##", 
  .cameraToUse = 0, 
  .confTh = 0.700000, 
  .confidenceMapThreshold = 0.100000, 
  .downscaleFactor = 0.400000, 
  .heightElbowTolerance = 40.000000, 
  .nInterpSamples = 10, 
  .pafScoreTh = 0.100000, 
  .preferredDevice = R"##(cpu)##", 
  .prototxt = R"##(./pose/coco/pose_deploy_linevec.prototxt)##", 
  .spatialSizeFactor = 100, 
  .tolerance = 20.000000, 
  .upscaleFactor = 4.000000, 
  .useColorsForPose = true, 
  .useRealTimePriority = false, 
};
