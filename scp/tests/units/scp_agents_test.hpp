/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#pragma once

#include <sc-memory/test/sc_test.hpp>

#include <memory>
#include <algorithm>
#include <unordered_set>
#include <filesystem>

#include <sc-memory/sc_memory.hpp>
#include <sc-memory/sc_agent_context.hpp>

class SCPAgentsTest : public testing::Test
{
public:
  static inline std::string const & SCP_MACHINE_KB = "../../../kb/agent_test/search_all_outgoing_arcs_with_relations/";
  static inline std::string const & SCP_AGENTS_KB = "../test-structures/agent-test/";
  static inline std::string const & SCP_OPERATORS_KB = "../test-structures/gen-operators/";
  static inline std::string const & SCP_AGENTS_KB_BIN = "../kb.bin";

protected:
  virtual void SetUp()
  {
    SCPAgentsTest::Initialize();
    m_ctx = std::make_unique<ScAgentContext>();
  }

  virtual void TearDown()
  {
    if (m_ctx)
      m_ctx->Destroy();

    SCPAgentsTest::Shutdown();

    std::filesystem::remove_all(SCP_AGENTS_KB_BIN);
  }

  static void Initialize()
  {
    sc_memory_params params;
    sc_memory_params_clear(&params);

    params.dump_memory = SC_FALSE;
    params.dump_memory_statistics = SC_FALSE;

    params.clear = SC_TRUE;
    params.storage = SCP_AGENTS_KB_BIN.c_str();

    ScMemory::LogMute();
    ScMemory::Initialize(params);
    ScMemory::LogUnmute();
  }

  static void Shutdown()
  {
    ScMemory::LogMute();
    ScMemory::Shutdown(SC_FALSE);
    ScMemory::LogUnmute();
  }

protected:
  std::unique_ptr<ScAgentContext> m_ctx;
};
