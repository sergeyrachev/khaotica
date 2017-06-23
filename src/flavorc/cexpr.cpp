/*
 * Copyright (c) 1997-2004 Alexandros Eleftheriadis, Danny Hong and 
 * Yuntai Kyong.
 *
 * This file is part of Flavor, developed at Columbia University
 * (http://flavor.sourceforge.net).
 *
 * Flavor is free software; you can redistribute it and/or modify
 * it under the terms of the Flavor Artistic License as described in
 * the file COPYING.txt. 
 *
 */

/*
 * Authors:
 * Alexandros Eleftheriadis <eleft@ee.columbia.edu>
 * Danny Hong <danny@ee.columbia.edu>
 * Yuntai Kyong <yuntaikyong@ieee.org>
 *
 */

/* cexpr.c */


#include "cdef.h"


/* Danny 9/1/2003  
    - A parenthesis will only be output if the user has used it in the Flavor description.
      This is determined from _node_t.paren.  Automatic parenthesis generation is disabled.
    - Java only allows boolean expressions in "if(...)," "while(...)," and "for(;...;)".
      If a non-boolean expression is used in such statements, we have to convert it into an 
      equivalent boolean expression.
*/


/* generate expression */
void c_expression(node_t *exp, short bool_needed)
{
    int op;
    
    if (exp == NULL) return;
    if (exp->paren == 1) c_out("(");

    op = exp->op;
    
    switch (op) {
        
    case flavor::Parser::token::INT_LITERAL:
        c_out("%d", exp->left.ival);
        if (java() && bool_needed) c_out(">0");
        break;
        
    case flavor::Parser::token::DOUBLE_LITERAL:
        c_out("%g", exp->left.dval);
        if (java() && bool_needed) c_out(">0");
        break;
        
    case flavor::Parser::token::BIT_LITERAL:
        c_out("0x%x", exp->left.bit->val);
        if (java() && bool_needed) c_out(">0");
        break;
        
    case flavor::Parser::token::VARIABLE:
        if (java() && exp->left.sp->owner == NULL)
            c_out("%s.", global_class);
        /* Prefix is used here to insert the prefix in 'lengthof' and other translator-generated 
         * variables. In other variables it will be simply ignored (no '%s' in the name).
         */
        c_out(exp->left.sp->name, prefix);
        if (java() && bool_needed) c_out(">0");
        break;

    case flavor::Parser::token::ISIDOF:
        c_out("%s", exp->left.np->left.sp->name);
        c_out("%s%sisidof(", c_static_scope(), prefix);
        c_expression(exp->right.np, 0);
        c_out(")");
        if (java() && bool_needed) c_out(">0");
        break;

    case flavor::Parser::token::SKIPBITS:
        c_out("%sbs.skipbits(", prefix);
        c_expression(exp->left.np, 0);
        c_out(")");
        break;

    case flavor::Parser::token::NEXTBITS:
    case flavor::Parser::token::SNEXTBITS:
    case flavor::Parser::token::LITTLE_NEXTBITS:
    case flavor::Parser::token::LITTLE_SNEXTBITS:
        c_out("%sbs.next(", prefix);
        c_expression(exp->left.np, 0);
        c_out(", ");

        if (op == flavor::Parser::token::NEXTBITS)
            c_out("1, 0, ");
        else if (op == flavor::Parser::token::SNEXTBITS)
            c_out("1, 1, ");
        else if (op == flavor::Parser::token::LITTLE_NEXTBITS)
            c_out("0, 0, ");
        else 
            c_out("0, 1, ");
      
        /* if alignment expression is given, take care of it */
        if (exp->right.np != NULL) 
            c_expression(exp->right.np, 0);
        else 
            c_out("0");

        c_out(")");
        if (java() && bool_needed) c_out(">0");
        break;

    case flavor::Parser::token::NEXTCODE:
        c_out("%sbs.nextcode(0x%x, %d, ", prefix, exp->left.np->left.bit->val, exp->left.np->left.bit->len);
        if (exp->right.np != NULL) 
            c_expression(exp->right.np, 0);
        else  
            c_out("0");

        c_out(")");
        if (java() && bool_needed) c_out(">0");
        break;

    case flavor::Parser::token::NUMBITS:
        c_out("%sbs.getpos()", prefix);
        if (java() && bool_needed) c_out(">0");
        break;

    case flavor::Parser::token::ARRAY:
        c_out("{ ");
        c_expression(exp->left.np, 0);
        c_out(" }");
        break;

    default:
        /* it is an operator */
        if (op=='?') {
            /* ternary */
            c_out("("); 
            c_expression(exp->left.np, 1);
            c_out(" ? ");
            c_expression(exp->right.np, 0);
            c_out(" : ");
            c_expression(exp->third.np, 0);
            c_out(")");
            if (java() && bool_needed) c_out(">0");
        }
        else if (exp->left.np == NULL && exp->right.np != NULL) {
            /* unary prefix */
            
            /* Java doesn't like the third expression encolsed with parenthesis in 'for' statement:
             * ( for(;;()) ).
             */
            c_out("%s", s_op2str(op));
            if (java() && bool_needed && op == 33) { /* 33 = '!' */
                c_out("(");
                c_expression(exp->right.np, 1);
                c_out(")");
            }
            else 
                c_expression(exp->right.np, 0);

            if (java() && bool_needed && op != 33) c_out(">0");
        }
        else if (exp->left.np != NULL && exp->right.np == NULL) {
            /* unary postfix */
            c_expression(exp->left.np, 0);
            c_out("%s", s_op2str(op));
            if (java() && bool_needed) c_out(">0");
        }
        else {
            /* binary */
            
            if (java() && bool_needed && (op == flavor::Parser::token::AND || op == flavor::Parser::token::OR))
                c_expression(exp->left.np, 1);
            else
                c_expression(exp->left.np, 0);

            /* if '.', C++ uses a pointer */
            if (op == '.' && exp->right.np->op == flavor::Parser::token::VARIABLE)
                c_out(c_scope());
            else {
                c_out("%s", s_op2str(op));
                /* leave a blank after a comma */
                if (op == ',') c_out(" ");
            }

            if (java() && bool_needed && (op == flavor::Parser::token::AND || op == flavor::Parser::token::OR))
                c_expression(exp->right.np, 1);
            else 
                c_expression(exp->right.np, 0);

            if (op == '[') c_out("]");

            if (java() && bool_needed 
                && (op != '<' && op != '>' && op != flavor::Parser::token::LE && op != flavor::Parser::token::GE
                    && op != flavor::Parser::token::EQ && op != flavor::Parser::token::NE && op != flavor::Parser::token::AND && op != flavor::Parser::token::OR))
                c_out(">0");
        }
    }
    if (exp->paren == 1) c_out(")");
}
