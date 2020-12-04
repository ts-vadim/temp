
#ifndef __LINE_FOLLOW_H__
#define __LINE_FOLLOW_H__

#include <TroykaLedMatrix.h>
#include <PID_dragster.h>
#include <Octoliner.h>
#include <Dragster.h>

class LineFollowProgram
{
public:
	LineFollowProgram() {}

	void OnStart() {}
	void OnUpdate() {}
	void OnPrintStatus() {}
	void OnDrawMatrix() {}

};

#endif