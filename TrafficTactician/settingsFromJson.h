struct settings_0_t {
  std::string_view caffemodel;
  std::uint64_t cameraToUse;
  double confTh;
  double downscaleFactor;
  std::uint64_t nInterpSamples;
  double pafScoreTh;
  std::string_view preferredDevice;
  std::string_view prototxt;
  std::string_view testKey;
  double tolerance;
  double upscaleFactor;
  bool useRealTimePriority;
};

constexpr settings_0_t settings = {
  .caffemodel = R"##(./pose/coco/pose_iter_440000.caffemodel)##", 
  .cameraToUse = 0, 
  .confTh = 0.700000, 
  .downscaleFactor = 0.400000, 
  .nInterpSamples = 10, 
  .pafScoreTh = 0.100000, 
  .preferredDevice = R"##(cpu)##", 
  .prototxt = R"##(./pose/coco/pose_deploy_linevec.prototxt)##", 
  .testKey = R"##(TestValue)##", 
  .tolerance = 20.000000, 
  .upscaleFactor = 4.000000, 
  .useRealTimePriority = false, 
};
