/*
* This source file is part of an OSTIS project. For the latest info, see http://ostis.net
* Distributed under the MIT License
* (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
*/

#pragma once

#include "sc-memory/sc_memory.hpp"
#include "sc-memory/sc-memory/sc_module.hpp"

#include "scp.generated.hpp"

#define SCP_PREFIX "[scp-machine] "
#define SCP_LOG_INFO(...) SC_LOG_INFO(SCP_PREFIX << __VA_ARGS__)
#define SCP_LOG_DEBUG(...) SC_LOG_DEBUG(SCP_PREFIX __VA_ARGS__)
#define SCP_LOG_WARNING(...) SC_LOG_WARNING(SCP_PREFIX __VA_ARGS__)
#define SCP_LOG_ERROR(...) SC_LOG_ERROR(SCP_PREFIX __VA_ARGS__)

class scpModule : public ScModule
{
    SC_CLASS(LoadOrder(50))
    SC_GENERATED_BODY()

    virtual sc_result InitializeImpl() override;
    virtual sc_result ShutdownImpl() override;

public:
    static std::unique_ptr<ScMemoryContext> s_default_ctx;

};
