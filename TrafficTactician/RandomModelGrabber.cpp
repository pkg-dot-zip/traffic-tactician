#include <string>
#include <array>
#include <string_view>

namespace randomModelGrabber
{
	std::array randomCarModels = {
		"models/car_kit/ambulance.obj",
		"models/car_kit/delivery.obj",
		"models/car_kit/firetruck.obj",
		"models/car_kit/garbage-truck.obj",
		"models/car_kit/hatchback-sports.obj",
		"models/car_kit/police.obj",
		"models/car_kit/race.obj",
		"models/car_kit/sedan.obj",
		"models/car_kit/sedan-sports.obj",
		"models/car_kit/suv.obj",
		"models/car_kit/suv-luxury.obj",
		"models/car_kit/taxi.obj",
		"models/car_kit/tractor.obj",
		"models/car_kit/tractor-police.obj",
		"models/car_kit/truck.obj",
		"models/car_kit/truck-flat.obj",
		"models/car_kit/van.obj"
	};

	std::string getRandomCarModelPathAsString()
	{
		return randomCarModels[rand() % randomCarModels.size()];
	}
}
