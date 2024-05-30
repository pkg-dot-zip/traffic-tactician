struct settings_0_t {
  std::uint64_t blurKSize;
  std::uint64_t blurSigmaX;
  std::uint64_t blurSigmaY;
  std::string_view caffemodel;
  std::uint64_t cameraToUse;
  double confTh;
  double confidenceMapThreshold;
  std::uint64_t downscaleTargetHeight;
  std::uint64_t downscaleTargetWidth;
  double heightElbowTolerance;
  std::uint64_t nInterpSamples;
  double pafScoreTh;
  std::string_view preferredDevice;
  std::string_view prototxt;
  std::uint64_t spatialSizeFactor;
  double tolerance;
  std::uint64_t upscaleTargetHeight;
  std::uint64_t upscaleTargetWidth;
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
  .downscaleTargetHeight = 192, 
  .downscaleTargetWidth = 256, 
  .heightElbowTolerance = 40.000000, 
  .nInterpSamples = 10, 
  .pafScoreTh = 0.100000, 
  .preferredDevice = R"##(cpu)##", 
  .prototxt = R"##(./pose/coco/pose_deploy_linevec.prototxt)##", 
  .spatialSizeFactor = 100, 
  .tolerance = 20.000000, 
  .upscaleTargetHeight = 768, 
  .upscaleTargetWidth = 1024, 
  .useColorsForPose = true, 
  .useRealTimePriority = true, 
};
