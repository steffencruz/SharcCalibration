SharcInput{
	:
		:
		run data : 		“analysis123_000”
		run data : 		“analysis123_001”
		:
}

OR ——————————

TSharcInput{
	:
		:
		run data : 		“analysis123*”
		:
}

OR —————————— 

TSharcInput{
	:
		:
		run data : 		“analysis*”
		:
}

OR —————————— 

TSharcInput{
	:
		:
		run data : 		“analysis*”
		:
}


TCalibrateFunctions::AddRunData(const char *rundata){
		chain->Add(run data);
}
