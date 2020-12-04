void log(String str, char end = '\n');
enum ProgramState { PAUSED, WAITING_FOR_START, FOLLOWING_LINE };

LineFollowProgram lineFollow;
ProgramState programState = ProgramState::WAITING_FOR_START;


void setup()
{
	Serial.begin(9600);
	log("Starting...");

	lineFollow.OnStart();
	lineFollow.OnPrintStatus();
}

void loop()
{

}





void log(String str, char end);
{
	Serial.print(str + end);
}