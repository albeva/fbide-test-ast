//
//  SourceContext.cpp
//  fbide
//
//  Created by Albert on 15/09/2013.
//  Copyright (c) 2013 Albert Varaksin. All rights reserved.
//
#include "SourceContext.h"
#include "Ast.h"


/**
 * Create
 */
SourceContext::SourceContext() : m_buffer(nullptr)
{}


/**
 * Run the internal analyses machinery and return
 * the first suitable ast node for that line
 * or null. Ast node return must not be
 * deleted or modified.
 */
const Ast * SourceContext::analyze(int line, unsigned int offset, int length)
{
    return nullptr;
}
