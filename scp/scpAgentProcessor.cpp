/*
* This source file is part of an OSTIS project. For the latest info, see http://ostis.net
* Distributed under the MIT License
* (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
*/

#include "scpAgentProcessor.hpp"
#include "sc-memory/sc_memory.hpp"

namespace scp {

ScAddr ASCPAgentActivator::msAgentKeynode;
ScAddr ASCPAgentDeactivator::msAgentKeynode;

SC_AGENT_IMPLEMENTATION(ASCPAgentActivator)
{
    ScAddr agent =m_memoryCtx.GetEdgeTarget(edgeAddr);

    SCPAgentEvent::register_scp_agent(m_memoryCtx, agent);

    return SC_RESULT_OK;
}

SC_AGENT_IMPLEMENTATION(ASCPAgentDeactivator)
{
    ScAddr agent = otherAddr;
    SCPAgentEvent::unregister_scp_agent(m_memoryCtx, agent);

    return SC_RESULT_OK;
}

}
