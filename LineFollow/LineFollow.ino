#include <LineFollowProgram.h>


void log(String str = "", char end = '\n');
enum ProgramState { PAUSED, WAITING_FOR_START, FOLLOWING_LINE, EXIT };

LineFollowProgram lineFollow;
ProgramState programState = ProgramState::WAITING_FOR_START;


void setup()
{
	Serial.begin(9600);
	log("Starting...");

	lineFollow.OnStart();
	lineFollow.OnPrintStatus();


	while (programState != ProgramState::EXIT)
	{
		switch (programState)
		{
			case ProgramState::PAUSED:
				log("paused");
				break;

			case ProgramState::WAITING_FOR_START:
				log("waiting");
				break;

			case ProgramState::FOLLOWING_LINE:
				log("Status:");
				lineFollow.OnPrintStatus();
				lineFollow.OnDrawMatrix();
				log();
				lineFollow.OnUpdate();
				break;
		}
	}

	log("Exit");
}

void loop()
{
}





void log(String str, char end)
{
	Serial.print(str + end);
}
