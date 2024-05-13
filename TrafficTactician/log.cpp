#include "easylogging++.h"

const std::string file_name = "logs/%datetime{%Y-%M-%d-%H-%m}.log";

void initLogsFolder()
{
	_mkdir("logs");
}

void setupLogger()
{
	initLogsFolder();

	el::Configurations defaultConf;
	defaultConf.setToDefault();

	defaultConf.setGlobally(el::ConfigurationType::Filename, file_name);
	// Values are always std::string
	defaultConf.set(el::Level::Info,
	                el::ConfigurationType::Format, "%datetime %level %msg");
	// default logger uses default configurations
	el::Loggers::reconfigureLogger("default", defaultConf);
	LOG(INFO) << "Starting logging";
	// To set GLOBAL configurations you may use
	defaultConf.setGlobally(
		el::ConfigurationType::Format, "%date %msg");
}
