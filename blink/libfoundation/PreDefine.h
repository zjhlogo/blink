/*!
 * \file PreDefine.h
 *
 * \author zjhlogo
 * \date 2019/11/15
 *
 * 
 */
#pragma once

// define the namespace for current project
#define NS blink

#define NS_BEGIN namespace NS {
#define NS_END }

#define SAFE_DELETE(x) if (x) {delete x; x=nullptr;}
#define SAFE_DELETE_ARRAY(x) if (x) {delete[] x; x=nullptr;}
#define SAFE_DELETE_AND_TERMINATE(x) if (x) {x->terminate(); delete x; x=nullptr;}
