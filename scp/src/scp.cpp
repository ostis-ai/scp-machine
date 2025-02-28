
#include "scp.hpp"

#include "scpKeynodes.hpp"

#include "scpAgentEvent.hpp"
#include "scpAgentProcessor.hpp"
#include "scpEraseOperatorInterpreter.hpp"
#include "scpFinishedInterpretationActionProcessor.hpp"
#include "scpGenOperatorInterpreter.hpp"
#include "scpIfOperatorInterpreter.hpp"
#include "scpMathOperatorInterpreter.hpp"
#include "scpPrintOperatorInterpreter.hpp"
#include "scpProcessControlOperatorInterpreter.hpp"
#include "scpProcessInterpreter.hpp"
#include "scpProcessDestroyer.hpp"
#include "scpProgramExecutionSyncronizer.hpp"
#include "scpSearchOperatorInterpreter.hpp"
#include "scpStringOperatorInterpreter.hpp"
#include "scpVarValueOperatorInterpreter.hpp"
#include "scpWaitEvent.hpp"

#include <iostream>

using namespace scp;

SC_MODULE_REGISTER(scpModule)
    ->Agent<ASCPProcessInterpreter>()
    ->Agent<ASCPProcessDestroyer>()
    ->Agent<ASCPGenOperatorInterpreter>()
    ->Agent<ASCPEraseOperatorInterpreter>()
    ->Agent<ASCPSearchOperatorInterpreter>()
    ->Agent<ASCPIfOperatorInterpreter>()
    ->Agent<ASCPVarValueOperatorInterpreter>()
    ->Agent<ASCPPrintOperatorInterpreter>()
    ->Agent<ASCPProgramExecutionSyncronizer>()
    ->Agent<ASCPProcessControlOperatorInterpreter>()
    ->Agent<ASCPAgentActivator>()
    ->Agent<ASCPAgentDeactivator>()
    ->Agent<ASCPMathOperatorInterpreter>()
    ->Agent<ASCPStringOperatorInterpreter>()
    ->Agent<ASCPFinishedInterpretationActionProcessor>();

void scpModule::Initialize(ScMemoryContext * context)
{
  std::cout << "SCP START" << std::endl;
  ScAgentContext agentContext;
  SCPAgentEvent::SubscribeAllSCPAgents(agentContext);
}

void scpModule::Shutdown(ScMemoryContext * context)
{
  std::cout << "SCP END" << std::endl;

  ScAgentContext agentContext;
  SCPAgentEvent::UnsubscribeAllSCPAgents(agentContext);
  SCPWaitEvent::DeleteAllSysWaiters();
}
