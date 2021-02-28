#include "Description.h"
/**
 * @interface Description Description.h
 * @brief Interface for objects which can describe themselves
 * @details
 * The methods serve for the output of a short and a 
 * detailed description, of the object.
 * 
 */

/**
 * @name Descriptions
 * Methods to describe objects
 */
/** @{ */

/**
 * @fn virtual string Description::summary()
 * @brief Summary of the object
 * A short (one line) summary of the obejct
 * 
 */

/**
 * @fn virtual string Description::detailed()
 * @brief Detailed description of the object
 * A longer text, that describes the obect in all its facets 
 */

/** @} */ // end Descriptions

/** @defgroup DescriptionOptions Options to Description interface
 * @brief Some options to the Description interface
 * @details
 * Here is a detailed description of the description :-)
 *  @{
 */

/**
 * @page behavior Section behavior
 * @brief The behavior of the description
 */

/**
 * @page meaning Section meaning
 * @brief The meaning of the description
 */

/** @} */ // end of DescriptionOptions

/**
 * @ingroup DescriptionOptions
 * @page options Section options
 * @brief The options of the description
 * - length, the maximal length of the @ref Description::summary
 *   + cut, the text will be shorten to the length
 *   + ellipsis, the text will be shorten as ellipsis
 */

