#include <TroykaLedMatrix.h>
#include <PID_dragster.h>
#include <Octoliner.h>
#include <Dragster.h>

#define Matrix TroykaLedMatrix


//  ############  Externs / Forwards  ############  
extern void log(String str = "", char end = '\n');
extern String SerialRead(bool wait_for_input = false);


//  ############  Declarations  ############  
struct DragsterControlVars
{
	Matrix& matrix;
	Dragster& dragster;
	Octoliner& octoliner;

	DragsterControlVars(Matrix& _matrix, Dragster& _dragster, Octoliner& _octoliner)
		: matrix(_matrix), dragster(_dragster), octoliner(_octoliner) {}
};


class ProgramState
{
  public:
    enum State { WAITING_FOR_START, FOLLOWING_LINE, STOPPED, NONE };

    static void SetState(State state)
    {
      switch (state)
      {
        case State::WAITING_FOR_START:
          m_State = state;
          log("State: WAITING_FOR_START");
          break;

        case State::FOLLOWING_LINE:
          m_State = state;
          log("State: FOLLOWING_LINE");
          break;

        case State::STOPPED:
          m_State = state;
          log("State: STOPPED");
          break;

        case State::NONE:
          m_State = state;
          log("State: NONE");
          break;

        default:
          log("State: invalid state value \'" + String((int)state) + "\'");
          break;
      }
    }
    
    static State GetState() { return m_State; }

  private:
    static State m_State;
};
static ProgramState::State ProgramState::m_State = State::STOPPED;

struct UserCommand
{
  const char* text;
  ProgramState::State triggerState;
  void (*function)(DragsterControlVars& vars);

  UserCommand(const char* _text, ProgramState::State state, void (*_function)(DragsterControlVars& vars))
    : text(_text), triggerState(state), function(_function) {}
};


//  ############  Main Line Follow Program  ############  
class LineFollowProgram
{
private:
	DragsterControlVars& vars;

public:
	LineFollowProgram(DragsterControlVars& _vars)
		: vars(_vars) {}

	void OnStart() {}
	void OnUpdate() { log("OnUpdate()"); }
	void OnPrintStatus() {}
	void OnDrawMatrix() {}
};


//  ############  Externs / Forwards  ############  
void UpdateUserCommand(DragsterControlVars& vars);
void StateWaiting(DragsterControlVars& vars);
void StateFollowing(LineFollowProgram& lineFollow);
void StateStopped(DragsterControlVars& vars);

// WAITING_FOR_START, FOLLOWING_LINE, STOPPED, NONE
const UserCommand userCommands[] = {
  UserCommand("wait", ProgramState::State::WAITING_FOR_START, StateWaiting),
  UserCommand("help", -1, [](){ log("HELP:\n  wait\n  follow\n  stop"); }),
  UserCommand((const char*)0, ProgramState::State::NONE, (void*)0)
};

void UpdateUserCommands(String cmd, DragsterControlVars& vars, UserCommand commands[]);



//  ############  Arduino Mains  ############  
void setup()
{
	ProgramState programState;
	Matrix matrix;
	Dragster dragster;
	Octoliner octoliner;
	DragsterControlVars vars(matrix, dragster, octoliner);
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
    String cmd = SerialRead();
    if (cmd.length() == 0)
      continue;
      
		UpdateState(cmd);

		vars.matrix.customClear();
    UpdateUserCommands(cmd, vars, userCommands);
		vars.matrix.customUpdate();
	}
	log("Mainloop exited");
}

void loop()
{
}





//  ############  Implementation ############

//  Update State / Main Functions

void UpdateUserCommands(String cmd, DragsterControlVars& vars, UserCommand commands[])
{
  ProgramState::State current_state = ProgramState::GetState();
  for (int i = 0; commands[i].function != 0; i++)
  {
    if (cmd.equals(commands[i].text) || current_state == commands[i].triggerState)
    {
      log("CALLING: \'" + String(commands[i].text) + "\'");
      commands->function(vars);
    }
  } 
}

void UpdateState(String cmd)
{
  if (cmd.equals("wait"))
    ProgramState::SetState(ProgramState::State::WAITING_FOR_START);
   
  else if (cmd.equals("follow"))
    ProgramState::SetState(ProgramState::State::FOLLOWING_LINE);
   
  else if (cmd.equals("stop"))
    ProgramState::SetState(ProgramState::State::STOPPED);
   
  else
    log("Unkown command \'" + cmd + "\'");
}

void StateWaiting(DragsterControlVars& vars)
{
  static const float radius = 3;
  static const float rotation_speed = 10;// rps
  static const float coord_pad = 4;
  static int prev_time = 0;
  static float angle = 0;

  int current_time = millis();

  float x = cos(angle) * radius;
  float y = sin(angle) * radius;
  
  vars.matrix.pixel(-x + coord_pad, -y + coord_pad, true);
  vars.matrix.pixel(x + coord_pad, y + coord_pad, true);

  angle += rotation_speed * (current_time - prev_time) / 1000.0;
  prev_time = current_time;
}

void StateFollowing(LineFollowProgram& lineFollow)
{
  lineFollow.OnUpdate();
  lineFollow.OnPrintStatus();
  lineFollow.OnDrawMatrix();
}

void StateStoped(DragsterControlVars& vars)
{
  static ProgramState::State prev_program_state = ProgramState::GetState();
  ProgramState::State current = ProgramState::GetState();

  if (prev_program_state != current)
    log("Now Stopped");
  prev_program_state = current;
}








//
