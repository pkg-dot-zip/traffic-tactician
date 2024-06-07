struct dnnmathsettings_0_t {
  std::uint64_t blurKSize;
  std::uint64_t blurSigmaX;
  std::uint64_t blurSigmaY;
  double confTh;
  double confidenceMapThreshold;
  std::uint64_t nInterpSamples;
  double pafScoreTh;
  std::uint64_t spatialSizeFactor;
};

constexpr dnnmathsettings_0_t dnnmathsettings = {
  .blurKSize = 3, 
  .blurSigmaX = 0, 
  .blurSigmaY = 0, 
  .confTh = 0.700000, 
  .confidenceMapThreshold = 0.100000, 
  .nInterpSamples = 10, 
  .pafScoreTh = 0.100000, 
  .spatialSizeFactor = 100, 
};
