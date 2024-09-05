/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#pragma once

#include "scpKeynodes.hpp"

#include "sc-memory/sc_module.hpp"

#define SCP_PREFIX "[scp-machine] "
#define SCP_LOG_INFO(...) SC_LOG_INFO(SCP_PREFIX << __VA_ARGS__)
#define SCP_LOG_DEBUG(...) SC_LOG_DEBUG(SCP_PREFIX __VA_ARGS__)
#define SCP_LOG_WARNING(...) SC_LOG_WARNING(SCP_PREFIX __VA_ARGS__)
#define SCP_LOG_ERROR(...) SC_LOG_ERROR(SCP_PREFIX __VA_ARGS__)

class scpModule : public ScModule
{
public:
  static ScMemoryContext s_default_ctx;

  void Initialize(ScMemoryContext * context) override;

  void Shutdown(ScMemoryContext * context) override;
};
