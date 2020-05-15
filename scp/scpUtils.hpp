/*
* This source file is part of an OSTIS project. For the latest info, see http://ostis.net
* Distributed under the MIT License
* (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
*/

#pragma once

#include "sc-memory/cpp/sc_memory.hpp"
#include "sc-memory/cpp/sc_stream.hpp"
#include "scpOperand.hpp"

#include <string>

namespace scp {
namespace Utils {

class ScStreamMemory : public ScStream
{
public:
  _SC_EXTERN explicit ScStreamMemory(MemoryBufferPtr const & buff);
  _SC_EXTERN virtual ~ScStreamMemory();

private:
  MemoryBufferPtr m_buffer;
};

/*! Adds element (elAddr) into set (setAddr).
 * @returns If element already exists in set, then do nothing and returns false. Otherwise
 * add element into set and returns true.
 */
bool addToSet(const std::unique_ptr<ScMemoryContext>& ctx, ScAddr const& setAddr, ScAddr const& elAddr);

/*! Remove element (elAddr) from set (setAddr)
 * @returns If element doesn't exist in set, then do nothing and returns false. Otherwise
 * remove element from set and returns true.
 */
bool removeFromSet(const std::unique_ptr<ScMemoryContext>& ctx, ScAddr const& setAddr, ScAddr const& elAddr);

/*! Resolves order role relation (1'-10') (relationAddr) of given arc (arcAddr)
 * @returns true, if relation resolved
 */
bool resolveOrderRoleRelation(const std::unique_ptr<ScMemoryContext>& ctx, ScAddr const& arcAddr, ScAddr& relationAddr);

/*! Resolves order role relation (1'-10') (relationAddr) by given number (order)
 * @returns true, if relation resolved
 */
bool resolveOrderRoleRelation(const std::unique_ptr<ScMemoryContext>& ctx, uint8_t const order, ScAddr& relationAddr);

/*! Resolves atomic operator type (operatorType) of given operator (operatorAddr)
 * @returns true, if relation resolved
 */
bool resolveOperatorType(const std::unique_ptr<ScMemoryContext>& ctx, ScAddr const& operatorAddr, ScAddr& operatorType);

/*! Prints system identifier or address of sc-element (elemAddr)
 */
void printSystemIdentifier(const std::unique_ptr<ScMemoryContext>& ctx, ScAddr const& elemAddr);

/*! Prints semantic neighborhood of sc-element (elemAddr)
 */
void printInfo(const std::unique_ptr<ScMemoryContext>& ctx, ScAddr const& elemAddr);

bool scLinkContentIsInt(const std::unique_ptr<ScMemoryContext>& ctx, ScAddr const& elemAddr);

int scLinkGetContentInt(const std::unique_ptr<ScMemoryContext>& ctx, ScAddr const& elemAddr);

bool scLinkContentIsUint(const std::unique_ptr<ScMemoryContext>& ctx, ScAddr const& elemAddr);

int scLinkGetContentUint(const std::unique_ptr<ScMemoryContext>& ctx, ScAddr const& elemAddr);

bool scLinkContentIsFloat(const std::unique_ptr<ScMemoryContext>& ctx, ScAddr const& elemAddr);

float scLinkGetContentFloat(const std::unique_ptr<ScMemoryContext>& ctx, ScAddr const& elemAddr);

bool scLinkContentIsDouble(const std::unique_ptr<ScMemoryContext>& ctx, ScAddr const& elemAddr);

double scLinkGetContentDouble(const std::unique_ptr<ScMemoryContext>& ctx, ScAddr const& elemAddr);

ScStreamPtr StreamFromString(std::string const & str);

std::string scLinkGetString(const std::unique_ptr<ScMemoryContext>& ctx, ScAddr const& elemAddr);

std::string scLinkPlainNumbers(const std::unique_ptr<ScMemoryContext>& ctx, ScAddr const& elemAddr);

void printOperatorAnswer(const std::unique_ptr<ScMemoryContext>& ctx, SCPOperand* nodeAddr , ScAddr const& linkAddr);

std::string getIntegerString(const std::unique_ptr<ScMemoryContext>& ctx, ScAddr const& elemAddr);

#ifdef SCP_DEBUG
    /*! Logs error about unknown type of given operator (addr)
    */
    void logUnknownOperatorTypeError(const std::unique_ptr<ScMemoryContext>& ctx, ScAddr& addr);

    /*! Logs error with text (text) and element addr (addr)
    */
    void logSCPError(const std::unique_ptr<ScMemoryContext>& ctx, std::string text, ScAddr& addr);
#endif

}
}
