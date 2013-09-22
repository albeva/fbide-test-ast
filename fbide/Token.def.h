//
//  Token.def.h
//  fbide
//
//  Created by Albert on 21/09/2013.
//  Copyright (c) 2013 Albert Varaksin. All rights reserved.
//

//     ID               STR                 STYLE
#define TOKEN_GENERIC(_)                                  \
    _( Identifier,      "Identifier",       Identifier  ) \
    _( Invalid,         "Invalid-Token",    Default     ) \
    _( EndOfLine,       "End-Of-Line",      Default     ) \
    _( EndOfFile,       "End-Of-File",      Default     ) \
    _( StringLiteral,   "String-Literal",   String      ) \
    _( NumberLiteral,   "Number-Literal",   Number      ) \
    _( Comment,         "Comment",          Comment     )

//     ID               STR                 STYLE
#define TOKEN_OPERATORS(_)                                \
    _( Colon,           ":",                Operator    ) \
    _( Assign,          "=",                Operator    ) \
    _( Comma,           ",",                Operator    ) \
    _( ParenOpen,       "(",                Operator    ) \
    _( ParenClose,      ")",                Operator    )

//     ID               STR                 STYLE
#define TOKEN_KEYWORDS(_)                                 \
    _( DIM,             "DIM",              Keyword     ) \
    _( AS,              "AS",               Keyword     ) \
    _( STRING,          "STRING",           Keyword     ) \
    _( INTEGER,         "INTEGER",          Keyword     ) \
    _( FUNCTION,        "FUNCTION",         Keyword     ) \
    _( DECLARE,         "DECLARE",          Keyword     ) \
    _( END,             "END",              Keyword     )

// all tokens combined
#define TOKEN_ALL(_)                                      \
    TOKEN_GENERIC(_)                                      \
    TOKEN_OPERATORS(_)                                    \
    TOKEN_KEYWORDS(_)
