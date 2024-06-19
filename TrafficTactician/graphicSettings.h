struct graphsettings_0_t {
  double fogExponentialDensity;
  double fogLinearFar;
  double fogLinearNear;
  std::string_view fogType;
  bool mxaaEnabled;
  std::uint64_t screenHeight;
  std::uint64_t screenWidth;
  bool useFog;
};

constexpr graphsettings_0_t graphsettings = {
  .fogExponentialDensity = 0.020000, 
  .fogLinearFar = 0.000000, 
  .fogLinearNear = 0.000000, 
  .fogType = R"##(exp2)##", 
  .mxaaEnabled = true, 
  .screenHeight = 900, 
  .screenWidth = 1600, 
  .useFog = true, 
};
