struct graphsettings_0_t {
  double fogExponentialDensity;
  double fogLinearFar;
  double fogLinearNear;
  std::string_view fogType;
  bool mxaaEnabled;
  bool useFog;
};

constexpr graphsettings_0_t graphsettings = {
  .fogExponentialDensity = 0.020000, 
  .fogLinearFar = 0.000000, 
  .fogLinearNear = 0.000000, 
  .fogType = R"##(exp2)##", 
  .mxaaEnabled = true, 
  .useFog = true, 
};
