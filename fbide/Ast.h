//
//  Ast.h
//  fbide
//
//  Created by Albert on 15/09/2013.
//  Copyright (c) 2013 Albert Varaksin. All rights reserved.
//
#pragma once
#include <vector>
#include <memory>

struct Ast
{
    // line number
    int m_line;
    
    // column
    int m_pos;
    
    // the length of the *current* not and NOT child nodes
    int m_length;
    
    // node style constant
    int m_style;
    
    // child nodes
    std::vector<std::unique_ptr<Ast>> m_nodes;
};