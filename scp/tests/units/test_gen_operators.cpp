#include <tests/sc-memory/_test/sc_test.hpp>
#include <scs_loader.hpp>

#include "scpAgentEvent.hpp"
#include "scpAgentProcessor.hpp"
#include "scpGenOperatorInterpreter.hpp"
#include "scpProcessInterpreter.hpp"
#include "scpSearchOperatorInterpreter.hpp"
#include "scpProcessDestroyer.hpp"
#include "scpProcessControlOperatorInterpreter.hpp"
#include "scpProgramExecutionSyncronizer.hpp"

using namespace scp;

namespace scpGenOperatorsTest
{
ScsLoader loader;
std::string const TEST_FILES_DIR_PATH = SCP_MACHINE_TEST_SRC_PATH "/testStructures/genOperators/";
std::string const TEST_OPERATOR = "test_operator";
size_t const WAIT_TIME = 1000;

static ScKeynode action_gen_el("action_gen_el", ScType::ConstNodeClass);
static ScKeynode concept_set("concept_set", ScType::ConstNodeClass);

using SCPGenOperatorsTest = ScMemoryTest;

void SubscribeAgents(ScAgentContext & context)
{
  context.SubscribeAgent<ASCPAgentActivator>();
  context.SubscribeAgent<ASCPGenOperatorInterpreter>();
  context.SubscribeAgent<ASCPProcessInterpreter>();
  context.SubscribeAgent<ASCPProcessControlOperatorInterpreter>();
  context.SubscribeAgent<ASCPProcessDestroyer>();
  context.SubscribeAgent<ASCPProgramExecutionSyncronizer>();
  context.SubscribeAgent<ASCPSearchOperatorInterpreter>();
}

void UnsubscribeAgents(ScAgentContext & context)
{
  context.UnsubscribeAgent<ASCPAgentActivator>();
  context.UnsubscribeAgent<ASCPGenOperatorInterpreter>();
  context.UnsubscribeAgent<ASCPProcessInterpreter>();
  context.UnsubscribeAgent<ASCPProcessControlOperatorInterpreter>();
  context.UnsubscribeAgent<ASCPProcessDestroyer>();
  context.UnsubscribeAgent<ASCPProgramExecutionSyncronizer>();
  context.UnsubscribeAgent<ASCPSearchOperatorInterpreter>();
}

bool ApplyOperator(ScAgentContext & context, ScAddr const & operatorAddr, size_t waitTime)
{
  return context
      .CreateConditionWaiter<ScEventAfterGenerateIncomingArc<ScType::ConstPermPosArc>>(
          operatorAddr,
          [&context, &operatorAddr]()
          {
            context.GenerateConnector(ScType::ConstPermPosArc, Keynodes::active_action, operatorAddr);
          },
          [](ScEventAfterGenerateIncomingArc<ScType::ConstPermPosArc> const & event)
          {
            return event.GetOtherElement() == Keynodes::action_finished_successfully;
          })
      ->Wait(waitTime);
}

TEST_F(SCPGenOperatorsTest, TestGenEl)
{
  ScAgentContext & context = *m_ctx;
  loader.loadScsFile(context, TEST_FILES_DIR_PATH + "gen_el_test.scs");
  SubscribeAgents(context);
  ScAddr const & testOperator = context.SearchElementBySystemIdentifier(TEST_OPERATOR);
  EXPECT_TRUE(testOperator.IsValid());
  EXPECT_TRUE(ApplyOperator(context, testOperator, WAIT_TIME));
  auto const & operandIterator = context.CreateIterator5(
      testOperator, ScType::ConstPermPosArc, ScType::ConstNode, ScType::ConstPermPosArc, Keynodes::rrel_1);
  EXPECT_TRUE(operandIterator->Next());
  ScAddr const & operand = operandIterator->Get(2);
  auto const & operandValueIterator = context.CreateIterator3(operand, ScType::ConstTempPosArc, ScType::ConstNode);
  EXPECT_TRUE(operandValueIterator->Next());
  UnsubscribeAgents(context);
}

TEST_F(SCPGenOperatorsTest, ComplexAgentsChain)
{
  ScAgentContext & context = *m_ctx;
  SubscribeAgents(context);
  context.BeginEventsBlocking();
  loader.loadScsFile(context, TEST_FILES_DIR_PATH + "agent_gen_el.scs");
  context.EndEventsBlocking();
  ScAddr const & agent = context.SearchElementBySystemIdentifier("agent_gen_el_active");
  EXPECT_TRUE(agent.IsValid());
  SCPAgentEvent::HandleActiveAgent(context, SCPAgentEvent::RegisterSCPAgent, agent);
  ScAction action = context.GenerateAction(action_gen_el);
  ScAddr const & setAddr = context.GenerateNode(ScType::ConstNode);
  context.GenerateConnector(ScType::ConstPermPosArc, concept_set, setAddr);
  action.SetArguments(setAddr);
  auto const & iteratorBefore = context.CreateIterator3(setAddr, ScType::ConstPermPosArc, ScType::ConstNode);
  EXPECT_FALSE(iteratorBefore->Next());
  EXPECT_TRUE(action.InitiateAndWait(WAIT_TIME));
  EXPECT_TRUE(action.IsFinishedSuccessfully());
  auto const & iteratorAfter = context.CreateIterator3(setAddr, ScType::ConstPermPosArc, ScType::ConstNode);
  EXPECT_TRUE(iteratorAfter->Next());
  EXPECT_FALSE(iteratorAfter->Next());

  SCPAgentEvent::HandleActiveAgent(context, SCPAgentEvent::UnregisterSCPAgent, agent);
  UnsubscribeAgents(context);
}

}  // namespace scpGenOperatorsTest
