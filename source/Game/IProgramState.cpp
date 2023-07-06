#include <Game/Program.h>
#include <Game/IProgramState.h>

IProgramState::IProgramState(Program* pProgram) : mpProgram(pProgram) { }
IProgramState::~IProgramState() { }