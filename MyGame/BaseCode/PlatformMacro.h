#pragma once
#include <assert.h>


//----------------------------------------------------------
#ifndef MY_ASSERT
#ifdef _DEBUG
#define	MY_ASSERT(x) assert(x)
#else
#define	MY_ASSERT(x)	{if(!(x)) ::LogMyMsg("ASSERT(" #x ") in %d line of %s [%s]", __LINE__, __FILE__, _TYPENAME(x)); }
#endif
#endif

//----------------------------------------------------------
#ifndef IF_SUC
#ifdef _DEBUG
#define	IF_SUC(x)  if( ((x)) ? true : ( assert(!("IF_SUC: " #x)), false ) )
#else
#define	IF_SUC(x)  if( ((x)) ? true : ( assert(!("IF_SUC: " #x)), false ) )
#endif
#endif


//----------------------------------------------------------
#ifndef IF_NOT
#ifdef _DEBUG
#define	IF_NOT(x)  if( (!(x)) ? ( assert(!("IF_NOT: " #x)),1 ) : 0 )
#else
#define	IF_NOT(x)  if( (!(x)) ? 1 : 0 )
#endif
#endif