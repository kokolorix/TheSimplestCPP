#pragma once
#include <utils.hpp>
/**
 * @brief Enables the debug heap to detect memory leaks
 * 
 */
#ifdef _DEBUG
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
#define new DBG_NEW
#endif

#include <assert.h>
