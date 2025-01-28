#include "scp_agents_test.hpp"

#include <sc-builder/scs_loader.hpp>

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

using namespace scp;

namespace kbAgentsTest
{
ScsLoader loader;
size_t const WAIT_TIME = 1000;

static ScKeynode action_search_all_outgoing_arcs_with_relations{
    "action_search_all_outgoing_arcs_with_relations",
    ScType::ConstNodeClass};

void SubscribeAgents(ScAgentContext & context)
{
  context.SubscribeAgent<ASCPProcessInterpreter>();
  context.SubscribeAgent<ASCPProcessDestroyer>();
  context.SubscribeAgent<ASCPGenOperatorInterpreter>();
  context.SubscribeAgent<ASCPEraseOperatorInterpreter>();
  context.SubscribeAgent<ASCPSearchOperatorInterpreter>();
  context.SubscribeAgent<ASCPIfOperatorInterpreter>();
  context.SubscribeAgent<ASCPVarValueOperatorInterpreter>();
  context.SubscribeAgent<ASCPPrintOperatorInterpreter>();
  context.SubscribeAgent<ASCPProgramExecutionSyncronizer>();
  context.SubscribeAgent<ASCPProcessControlOperatorInterpreter>();
  context.SubscribeAgent<ASCPAgentActivator>();
  context.SubscribeAgent<ASCPAgentDeactivator>();
  context.SubscribeAgent<ASCPMathOperatorInterpreter>();
  context.SubscribeAgent<ASCPStringOperatorInterpreter>();
  context.SubscribeAgent<ASCPFinishedInterpretationActionProcessor>();
}

void UnsubscribeAgents(ScAgentContext & context)
{
  context.UnsubscribeAgent<ASCPProcessInterpreter>();
  context.UnsubscribeAgent<ASCPProcessDestroyer>();
  context.UnsubscribeAgent<ASCPGenOperatorInterpreter>();
  context.UnsubscribeAgent<ASCPEraseOperatorInterpreter>();
  context.UnsubscribeAgent<ASCPSearchOperatorInterpreter>();
  context.UnsubscribeAgent<ASCPIfOperatorInterpreter>();
  context.UnsubscribeAgent<ASCPVarValueOperatorInterpreter>();
  context.UnsubscribeAgent<ASCPPrintOperatorInterpreter>();
  context.UnsubscribeAgent<ASCPProgramExecutionSyncronizer>();
  context.UnsubscribeAgent<ASCPProcessControlOperatorInterpreter>();
  context.UnsubscribeAgent<ASCPAgentActivator>();
  context.UnsubscribeAgent<ASCPAgentDeactivator>();
  context.UnsubscribeAgent<ASCPMathOperatorInterpreter>();
  context.UnsubscribeAgent<ASCPStringOperatorInterpreter>();
  context.UnsubscribeAgent<ASCPFinishedInterpretationActionProcessor>();
}

TEST_F(SCPAgentsTest, Test_sc_agent1_scp)
{
  ScAgentContext & context = *m_ctx;
  context.BeginEventsBlocking();
  loader.loadScsFile(context, SCP_MACHINE_KB + "agent_program_search_all_outgoing_arcs_with_relations.scs");
  loader.loadScsFile(context, SCP_MACHINE_KB + "program_search_all_role_relations_for_given_membership_arc.scs");
  loader.loadScsFile(context, SCP_MACHINE_KB + "sc_agent_search_all_outgoing_arcs_with_relations.scs");

  loader.loadScsFile(context, SCP_AGENTS_KB + "incoming_arcs.scs");
  loader.loadScsFile(context, SCP_AGENTS_KB + "operators.scs");
  context.EndEventsBlocking();
  SubscribeAgents(context);
  ScAddr const & agent =
      context.SearchElementBySystemIdentifier("sc_agent_search_all_outgoing_arcs_with_relations_scp");
  EXPECT_TRUE(agent.IsValid());
  SCPAgentEvent::HandleActiveAgent(context, SCPAgentEvent::RegisterSCPAgent, agent);

  ScAddr const & actionArgument = context.SearchElementBySystemIdentifier("test_node");
  EXPECT_TRUE(actionArgument.IsValid());
  ScAddr const & actionExpectedResult = context.SearchElementBySystemIdentifier("test_result");
  EXPECT_TRUE(actionExpectedResult.IsValid());
  ScAction action = context.GenerateAction(action_search_all_outgoing_arcs_with_relations).SetArguments(actionArgument);
  EXPECT_TRUE(action.InitiateAndWait(WAIT_TIME));
  EXPECT_TRUE(action.IsFinished());
  EXPECT_TRUE(action.IsFinishedSuccessfully());
  ScAddr const & actionResult = action.GetResult();
  EXPECT_TRUE(context.IsElement(actionResult));
  auto const & actionExpectedResultIterator =
      context.CreateIterator3(actionExpectedResult, ScType::ConstPermPosArc, ScType::Unknown);
  while (actionExpectedResultIterator->Next())
  {
    ScAddr const & actionResultElement = actionExpectedResultIterator->Get(2);
    EXPECT_TRUE(context.CheckConnector(actionResult, actionResultElement, ScType::ConstPermPosArc))
        << "Action result does not have element with hash " << actionResultElement.Hash() << " and system identifier "
        << context.GetElementSystemIdentifier(actionResultElement);
  }

  SCPAgentEvent::HandleActiveAgent(context, SCPAgentEvent::UnregisterSCPAgent, agent);
  UnsubscribeAgents(context);
}

}  // namespace kbAgentsTest
