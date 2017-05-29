/*******************************************************************************
 *
 * (c) Copyright IBM Corp. 2017, 2017
 *
 *  This program and the accompanying materials are made available
 *  under the terms of the Eclipse Public License v1.0 and
 *  Apache License v2.0 which accompanies this distribution.
 *
 *      The Eclipse Public License is available at
 *      http://www.eclipse.org/legal/epl-v10.html
 *
 *      The Apache License v2.0 is available at
 *      http://www.opensource.org/licenses/apache2.0.php
 *
 * Contributors:
 *    Multiple authors (IBM Corp.) - initial implementation and documentation
 ******************************************************************************/


#ifndef OMR_ILVALUE_INCL
#define OMR_ILVALUE_INCL

#ifndef TR_ILVALUE_DEFINED
#define TR_ILVALUE_DEFINED
#define PUT_OMR_ILVALUE_INTO_TR
#endif // !defined(TR_ILVALUE_DEFINED)

#include "il/DataTypes.hpp" // for TR::DataType

namespace TR { class Node; }
namespace TR { class TreeTop; }
namespace TR { class Block; }
namespace TR { class Symbol; }
namespace TR { class SymbolReference; }
namespace TR { class MethodBuilder; }
namespace TR { class IlValue; }

namespace OMR
{

class IlValue
   {
public:
   TR_ALLOC(TR_Memory::IlGenerator)

   /**
    * @brief initial state assumes value will only be used locally
    */
   IlValue(TR::Node *node, TR::TreeTop *treeTop, TR::Block *block, TR::MethodBuilder *methodBuilder);

   /**
    * @brief return a TR::Node that computes this value, either directly if in same block or by loading a local variable if not
    * @param block TR::Block for the load point so it can be compared to the TR::Block where value is computed
    */
   TR::Node *load(TR::Block *block);

   /**
    * @brief ensures that the provided value replaces the current value, but only affects subsequent loads
    * @param value the new value that should be returned whenever the current value is loaded
    * @param block TR::Block for the replacement point, so that either the node pointer is changed or what's stored in the symbol reference
    */
   void storeOver(TR::IlValue *value, TR::Block *block);

   /**
    * @brief return the data type of this value
    */
   TR::DataType getDataType();

   /**
    * @brief returns the TR::SymbolReference holding the value, or NULL if willBeUsedInAnotherBlock() has not yet been called
    * Caller should ensure the current block is different than the block that computes the value; if the current block is the
    * same then it is better to use the value as a node pointer via GetNodePointerIfLegal().
    */
   TR::SymbolReference *getSymbolReference()
      {
      return _symRefThatCanBeUsedInOtherBlocks;
      }

   /**
    * @brief returns a unique identifier for this IlValue object within its MethodBuilder
    * Note that IlValues from different MethodBuilders can have the same ID.
    */
   int32_t getID()
      {
      return _id;
      }

protected:
   /**
    * @brief ensures this value is accessible via an auto symbol, but only generates store if hasn't already been stored
    */
   void storeToAuto();

private:

   /**
    * @brief identifying number for values guaranteed to be unique per MethodBuilder
    */
   int32_t               _id;

   /**
    * @brief TR::Node pointer that computes the actual value
    */
   TR::Node            * _nodeThatComputesValue;

   /**
    * @brief TR::TreeTop that anchors this node (though not necessarily directly)
    */
   TR::TreeTop         * _treeTopThatAnchorsValue;

   /**
    * @brief TR::Block where the value is computed
    */
   TR::Block           * _blockThatComputesValue;

   /**
    * @brief Method builder object where value is computed
    */
   TR::MethodBuilder   * _methodBuilder;

   /**
    * @brief TR::SymbolReference for temp holding value if it needs to be used outside basic block that computes it
    */
   TR::SymbolReference * _symRefThatCanBeUsedInOtherBlocks;
   };

} // namespace OMR



#if defined(PUT_OMR_ILVALUE_INTO_TR)

namespace TR
{
   class IlValue : public OMR::IlValue
      {
      public:
         IlValue(TR::Node *node, TR::TreeTop *treeTop, TR::Block *block, TR::MethodBuilder *methodBuilder)
            : OMR::IlValue(node, treeTop, block, methodBuilder)
            { }
      };

} // namespace TR

#endif // defined(PUT_OMR_ILVALUE_INTO_TR)

#endif // !defined(OMR_ILVALUE_INCL)