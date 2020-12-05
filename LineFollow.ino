#include <TroykaLedMatrix.h>
#include <PID_dragster.h>
#include <Octoliner.h>
#include <Dragster.h>

#define Matrix TroykaLedMatrix


//	Globals
enum ProgramState { WAITING_FOR_START, FOLLOWING_LINE, STOPPED };
struct EnvVarsStruct
{
	Matrix& matrix;
	Dragster& dragster;
	Octoliner& octoliner;
	ProgramState& programState;

	EnvVarsStruct(ProgramState& _programState, Matrix& _matrix, Dragster& _dragster, Octoliner& _octoliner)
		: programState(_programState), matrix(_matrix), dragster(_dragster), octoliner(_octoliner) {}
};


//	Functions Forward Declarations
void log(String str = "", char end = '\n');
String SerialRead(bool wait_for_input = false);
void SetState(ProgramState state);


//	Main Line Follow Program
class LineFollowProgram
{
private:
	EnvVarsStruct& vars;

public:
	LineFollowProgram(EnvVarsStruct& _vars)
		: vars(_vars) {}

	void OnStart() {}
	void OnUpdate() {}
	void OnPrintStatus() {}
	void OnDrawMatrix() {}
};


//	Update State / Main Functions
void UpdateUserCommand(EnvVarsStruct& vars)
{
	String cmd = SerialRead();
	if (cmd.length() == 0)
		return;

	if (cmd.equals("wait"))
	{
		vars.programState = ProgramState::WAITING_FOR_START;
		log("Set Wait State");
	}
	else if (cmd.equals("follow"))
	{
		vars.programState = ProgramState::FOLLOWING_LINE;
		log("Set Follow State");
	}
	else if (cmd.equals("stop"))
	{
		vars.programState = ProgramState::STOPPED;
		log("Set Stop State");
	}
	else
	{
		log("Unkown command \'" + cmd + "\'");
	}
}

void StateWaiting(EnvVarsStruct& vars)
{
	static const float radius = 3;
	static const float rotation_speed = 10;// rps
	static const float coord_pad = 4;
	static int prev_time = 0;
	static float angle = 0;

	int current_time = millis();

	float x = cos(angle) * radius;
	float y = sin(angle) * radius;

	log("Current: " + String(current_time) + " XY: " + String(x) + " " + String(y));

	vars.matrix.pixel(-x + coord_pad, -y + coord_pad, true);
	vars.matrix.pixel(x + coord_pad, y + coord_pad, true);

	angle += rotation_speed * (current_time - prev_time) / 1000.0;
	prev_time = current_time;
}

void StateFollowing(LineFollowProgram& lineFollow)
{
	/*
	lineFollow.OnUpdate();
	log("Line Follow Status:");
	lineFollow.OnPrintStatus();
	log();
	lineFollow.OnDrawMatrix();
	*/
}

void StateStoped(EnvVarsStruct& vars)
{
	/*
	static ProgramState prev_program_state = vars.programState;
	ProgramState current_program_state = vars.programState;

	if (prev_program_state != current_program_state)
		log("Now Stopped");
	prev_program_state = current_program_state;

*/
}



//	Arduino Mains
void setup()
{
	ProgramState programState = ProgramState::STOPPED;
	Matrix matrix;
	Dragster dragster;
	Octoliner octoliner;
	EnvVarsStruct vars(programState, matrix, dragster, octoliner);
	LineFollowProgram lineFollow(vars);

	Serial.begin(9600);

	log("Starting...");
	dragster.begin();
	matrix.begin();
	octoliner.begin(200);
	lineFollow.OnStart();

	log("Mainloop");
	while (true)
	{
		UpdateUserCommand(vars);

		vars.matrix.customClear();
		switch (vars.programState)
		{
			case ProgramState::WAITING_FOR_START:
				StateWaiting(vars);
				break;

			case ProgramState::FOLLOWING_LINE:
				StateFollowing(lineFollow);
				break;

			case ProgramState::STOPPED:
				StateStoped(vars);
				break;
		}
		vars.matrix.customUpdate();
	}
	log("Mainloop exited");
}

void loop()
{
}


//	Implementations
void log(String str = "", char end = '\n')
{
	if (Serial)
		Serial.print(str + end);
}

String SerialRead(bool wait_for_input = false)
{
	String output;
	if (wait_for_input)
		while (Serial.available() <= 0);
	while (Serial.available() > 0)
	{
		char c = Serial.read();
		if (c == '\n')
			break;
		output += c;
	}
	return output;
}

void SetState(EnvVarsStruct& vars, ProgramState state)
{
	switch (state)
	{
		case ProgramState::WAITING_FOR_START:
			vars.programState = state;
			log("State: WAITING_FOR_START");
			break;
	}
}












//
