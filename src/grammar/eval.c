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

/* eval.c */

#include <stdio.h>

#include "port.h"
#include "util.h"
#include "symbol.h"
#include "ptree.h"
#include "check.h"
#include "error.h"
#include "carray.h"
#include "eval.h"
#include "globals.h"

#include "y.tab.h"

/* 
 * Expression evaluation and error check functions
 */

/* eval identifier (for constants) */
node_t *eval_ident(symbol_t *ident)
{
    /* NOTE: The following code has been modified to reduce globals only, 
     *       because the lengthof() implementation breaks. If you have
     *            const int(5) i=2;
     *            int j=lengthof(i);
     *       the eval will result in lengthof(2) and an error message.
     */
    node_t *np;

    /* we can reduce here only non-array constants (arrays are reduced elsewhere) */
    if (!(ident->modifiers & M_CONST) || ident->dims > 0) return leafs(VARIABLE, ident);

    /* if constant, there must be an initial value */
    if (ident->init == NULL) fatal("Internal error EV%d: Constant without initial value", __LINE__);

    /* we only reduce if init is a literal */
    if (!is_literal(ident->init->op)) return leafs(VARIABLE, ident);

    /* all clear; reduce, and save original variable for a possible lengthof() down the road */
    np = node_dup(ident->init);

    /* convert the literal to have the type of the const variable */
    np->ptype = ident->ptype;
    np->type = ident->ptype->ident;
    np->eval = leafs(VARIABLE, ident);

    return np;
}

/* eval a++ and a-- expressions */
node_t *eval_incdec(int op, node_t *expr)
{
    node_t *n;
    
    if (expr == NULL) {
        yyerror("Internal error E%d: No expression", __LINE__);
        return NULL;
    }
    
    /* we cannot have a class variable on the left */
    if (expr->ptype->ident == CLASS_TYPE) {
        yyerror("Invalid operator for class: %s", s_op2str(op));
        /* ignore operator */
        return expr;
    }
    /* there must be a valid lvalue in the expression */
    else if (!check_unary(expr)) {
        yyerror("Operator '%s' can only be applied on a non-constant, non-parsable variable", s_op2str(op));
        /* ignore operator */
        return expr;
    }
    n = node(op, expr, NULL);
    n->type = expr->type;
    n->ptype = expr->ptype;
    return n;
}

/* eval scoped variable: a '.' b */
node_t *eval_scoped(node_t *left, symbol_t *right)
{
    node_t *n;

    /* check if left side is not a class */
    if (left->ptype == NULL || left->ptype->ident != CLASS_TYPE) {
        yyerror("Left side of scope operator '.' is not a class");
       
        /* if right side is undeclared variable, set as 'int'; note that it may be declared, as symbol lookup may have a hit on a higher-level context */
        if (right->ptype == NULL) {
            right->ident = INT;
            right->ptype = s_lookup("int");
        }
    }
    else {
        /* check if right side is a declared variable */
        if (right->ptype == NULL) {
            yyerror("Class '%s' has no member variable '%s'", left->ptype->name, right->name);
            /* set to 'int' */
            right->ident = INT;
            right->ptype = s_lookup("int");
        }
        else {
            /* check that owner is identical (means symbol lookup found a proper entry, rather than create a dummy 'int' */
            if (right->owner != left->ptype) {
                yyerror("Class '%s' has no member variable '%s'", left->ptype->name, right->name);
                /* set it as 'int' so we can continue */
                right->ident = INT;
                right->ptype = s_lookup("int");
            }
        }
    }
    n = node('.', left, leafs(VARIABLE, right));     
    n->type = right->ptype->ident; 
    n->ptype = right->ptype;
    
    return n;
}

/* eval unary operator expression (e.g., -expr) */
node_t *eval_unary(int op, node_t *expr)
{
    node_t *n;
    
    /* our operators are INC, DEC, +, -, ~, ! */
    if (expr->type == CLASS_TYPE) {
        yyerror("Operator '%s' cannot be applied to classes", s_op2str(op));
        goto eval_unary_end;
    }
    if (op == INC || op == DEC) {
        /* there must be a valid lvalue in the expression */
        if (!check_unary(expr))
            yyerror("Operator '%s' can only be applied on a non-constant, non-parsable variable", s_op2str(op));
        goto eval_unary_end;
    }
    /* ~, ! cannot be applied to anything higher than int */
    if (expr->type > INT && !(op == '+' || op == '-') ) {
        yyerror("Operator '%s' cannot be applied to type '%s'", s_op2str(op), expr->ptype->name);
        goto eval_unary_end;
    }

    /* the remaining operators can be applied to both vars and literals */
    
    /* no reduction possible on variables, unless they are const */
    if (expr->op == VARIABLE) {
        if (expr->left.sp->modifiers & M_CONST) {
            /* sanity check */
            if (expr->left.sp->init == NULL)
                fatal("Internal error EV%d: Constant without initial value", __LINE__);
            else
                return eval_unary(op, node_dup(expr->left.sp->init));
        }
        else goto eval_unary_end;
    }
    /* if literal, we can reduce */
    if (is_literal(expr->op)) {
        /* if bitstring, convert to int */
        if (expr->op == BIT_LITERAL) expr = n_bit2int(node_dup(expr));
        
        switch (op) {
        case '+':
            /* nothing to do */
            return expr;
            
        case '-':
            if (expr->type>INT) expr->left.dval = -expr->left.dval;
            else expr->left.ival = -expr->left.ival;
            return expr;
            
        case '~':
            expr->left.ival = ~expr->left.ival;
            return expr;
            
        case '!':
            expr->left.ival = !expr->left.ival;
            return expr;
        }
    }
eval_unary_end:
    n = node(op, NULL, expr);
    n->type = expr->type;
    n->ptype = expr->ptype;
    return n;
}

/* eval multiplicative operator expression (*, /, %) */
node_t *eval_mult(int op, node_t *left, node_t *right)
{
    node_t *n;
    
    /* not allowed on classes */
    if (left->type == CLASS_TYPE || right->type == CLASS_TYPE) {
        yyerror("Operator '%s' cannot be applied to classes", s_op2str(op));
        goto eval_mult_end;
    }
    /* '%' not allowed on doubles */
    if (op == '%' && (left->type > INT || right->type > INT)) {
        yyerror("Operator '%s' cannot be applied to floats or doubles", s_op2str(op));
        goto eval_mult_end;
    }
    /* no reduction possible on variables, unless they are const */
    if (left->op == VARIABLE) {
        if (left->left.sp->modifiers & M_CONST) {
            /* sanity check */
            if (left->left.sp->init == NULL)
                fatal("Internal error EV%d: Constant without initial value", __LINE__);
            else
                return eval_mult(op, node_dup(left->left.sp->init), right);
        }
        else goto eval_mult_end;
    }
    /* same for right side */
    if (right->op == VARIABLE) {
        if (right->left.sp->modifiers & M_CONST) {
            /* sanity check */
            if (right->left.sp->init == NULL)
                fatal("Internal error EV%d: Constant without initial value", __LINE__);
            else
                return eval_mult(op, left, node_dup(right->left.sp->init));
        }
        else goto eval_mult_end;
    }
    /* if literal, we can reduce */
    if (is_literal(left->op) && is_literal(right->op)) {
        /* if bitstrings, convert to int */
        if (left->op == BIT_LITERAL) left = n_bit2int(node_dup(left));
        if (right->op == BIT_LITERAL) right = n_bit2int(node_dup(right));
        
        switch (op) {
            
        case '*':
            /* int/int */
            if (left->type <= INT && right->type <= INT) {
                left->left.ival *= right->left.ival;
                return left;
            }
            /* int/double */
            else if (left->type <= INT && right->type > INT) {
                right->left.dval *= left->left.ival;
                return right;
            }
            /* double/int */
            else if (left->type > INT && right->type <= INT) {
                left->left.dval *= right->left.ival;
                return left;
            }
            /* double/double */
            else {
                left->left.dval *= right->left.dval;
                return left;
            }
            
        case '/':
            /* int/int */
            if (left->type <= INT && right->type <= INT) {
                if (right->left.ival == 0) {
                    yyerror("Division by zero");
                    goto eval_mult_end;
                }
                left->left.ival /= right->left.ival;
                return left;
            }
            /* int/double */
            else if (left->type <= INT && right->type > INT) {
                if (right->left.ival == 0) {
                    yyerror("Division by zero");
                    n = node(op, left, right);
                    n->type = right->type;
                    n->ptype = right->ptype;
                    return n;
                }
                left->left.dval /= right->left.ival;
                return right;
            }
            /* double/int */
            else if (left->type > INT && right->type <= INT) {
                if (right->left.ival == 0) {
                    yyerror("Division by zero");
                    goto eval_mult_end;
                }
                left->left.dval /= right->left.ival;
                return left;
            }
            /* double/double */
            else {
                if (right->left.dval == 0) {
                    yyerror("Division by zero");
                    goto eval_mult_end;
                }
                left->left.dval /= right->left.dval;
                return left;
            }
            fatal("Inernal error U%: unreachable", __LINE__);
            
        case '%':
            /* both are guaranteed to be ints */
            left->left.ival %= right->left.ival;
            return left;
            
        default:
            fatal("Internal error U%: Unreachable", __LINE__);
        }
    }
eval_mult_end:
    n = node(op, left, right);
    n->type = left->type;
    n->ptype = left->ptype;
    return n;
}

/* eval additive operator expression (+, -) */
node_t *eval_add(int op, node_t *left, node_t *right)
{
    node_t *n;
    
    /* not allowed on classes */
    if (left->type == CLASS_TYPE || right->type == CLASS_TYPE) {
        yyerror("Operator '%s' cannot be applied to classes", s_op2str(op));
        goto eval_add_end;
    }
    /* no reduction possible on variables, unless they are const */
    if (left->op == VARIABLE) {
        if (left->left.sp->modifiers & M_CONST) {
            /* sanity check */
            if (left->left.sp->init == NULL)
                fatal("Internal error EV%d: Constant without initial value", __LINE__);
            else
                return eval_add(op, node_dup(left->left.sp->init), right);
        }
        else goto eval_add_end;
    }
    /* same for right side */
    if (right->op == VARIABLE) {
        if (right->left.sp->modifiers & M_CONST) {
            /* sanity check */
            if (right->left.sp->init == NULL)
                fatal("Internal error EV%d: Constant without initial value", __LINE__);
            else
                return eval_add(op, left, node_dup(right->left.sp->init));
        }
        else goto eval_add_end;
    }
    /* if literal, we can reduce */
    if (is_literal(left->op) && is_literal(right->op)) {
        /* if bitstrings, convert to int */
        if (left->op == BIT_LITERAL)
            left = n_bit2int(node_dup(left));
        if (right->op == BIT_LITERAL)
            right = n_bit2int(node_dup(right));
        
        switch (op) {
            
        case '+':
            /* int/int */
            if (left->type <= INT && right->type <= INT) {
                left->left.ival += right->left.ival;
                return left;
            }
            /* int/double */
            else if (left->type <= INT && right->type > INT) {
                right->left.dval += left->left.ival;
                return right;
            }
            /* double/int */
            else if (left->type > INT && right->type <= INT) {
                left->left.dval += right->left.ival;
                return left;
            }
            /* double/double */
            else {
                left->left.dval += right->left.dval;
                return left;
            }
            
        case '-':
            /* int/int */
            if (left->type <= INT && right->type <= INT) {
                left->left.ival -= right->left.ival;
                return left;
            }
            /* int/double */
            else if (left->type <= INT && right->type > INT) {
                right->left.dval -= left->left.ival;
                return right;
            }
            /* double/int */
            else if (left->type > INT && right->type <= INT) {
                left->left.dval -= right->left.ival;
                return left;
            }
            /* double/double */
            else {
                left->left.dval -= right->left.dval;
                return left;
            }
            
        default:
            fatal("Internal error U%: Unreachable", __LINE__);
        }
    }
eval_add_end:
    n = node(op, left, right);
    n->type = left->type;
    n->ptype = left->ptype;
    return n;
}

/* 05/08/2002 - three additional bitwise operations are added (&, ^, |) */

/* eval shift operator expression (<<, >>) */
node_t *eval_shift(int op, node_t *left, node_t *right)
{
    node_t *n;
    
    /* not allowed on classes */
    if (left->type == CLASS_TYPE || right->type == CLASS_TYPE) {
        yyerror("Operator '%s' cannot be applied to classes", s_op2str(op));
        goto eval_shift_end;
    }

    /* no reduction possible on variables, unless they are const */
    if (left->op == VARIABLE) {
        if (left->left.sp->modifiers & M_CONST) {
            /* sanity check */
            if (left->left.sp->init == NULL)
                fatal("Internal error EV%d: Constant without initial value", __LINE__);
            else
                return eval_shift(op, node_dup(left->left.sp->init), right);
        }
        else goto eval_shift_end;
    }

    /* same for right side */
    if (right->op == VARIABLE) {
        if (right->left.sp->modifiers & M_CONST) {
            /* sanity check */
            if (right->left.sp->init == NULL)
                fatal("Internal error EV%d: Constant without initial value", __LINE__);
            else
                return eval_shift(op, left, node_dup(right->left.sp->init));
        }
        else goto eval_shift_end;
    }

    /* shift operators not allowed on double values */
    if (left->type > INT || right->type > INT) {
        yyerror("Operator '%s' cannot be applied to floats or doubles", s_op2str(op));
        goto eval_shift_end;
    }
    
    /* if literal, we can reduce */
    if (is_literal(left->op) && is_literal(right->op)) {
        /* if bitstrings, convert to int */
        if (left->op == BIT_LITERAL) left = n_bit2int(node_dup(left));
        if (right->op == BIT_LITERAL) right = n_bit2int(node_dup(right));
        
        /* right-hand side should not be negative */
        if (right->left.ival < 0)
            yyerror("Warning: Negative value for shift operator '%s'", s_op2str(op));

        switch (op) {
            
        case LS:
            left->left.ival <<= right->left.ival;
            return left;
            
        case RS:
            left->left.ival >>= right->left.ival;
            return left;

        /* 05/08/2002 - three additional bitwise operations are added */
        case '&':
            left->left.ival &= right->left.ival;
            return left;
        case '^':
            left->left.ival ^= right->left.ival;
            return left;
        case '|':
            left->left.ival |= right->left.ival;
            return left;
            
        default:
            fatal("Internal error U%: Unreachable", __LINE__);
        }
    }

eval_shift_end:
    n = node(op, left, right);
    n->type = left->type;
    n->ptype = left->ptype;
    return n;
}

/* eval relational operator expression (<, >, LE, GE. EQ, NE) */
node_t *eval_rel(int op, node_t *left, node_t *right)
{
    node_t *n;
    
    /* not allowed on classes */
    if (left->type == CLASS_TYPE || right->type == CLASS_TYPE) {
        yyerror("Operator '%s' cannot be applied to classes", s_op2str(op));
        goto eval_rel_end;
    }
    /* no reduction possible on variables, unless they are const */
    if (left->op == VARIABLE) {
        if (left->left.sp->modifiers & M_CONST) {
            /* sanity check */
            if (left->left.sp->init == NULL)
                fatal("Internal error EV%d: Constant without initial value", __LINE__);
            else
                return eval_rel(op, node_dup(left->left.sp->init), right);
        }
        else goto eval_rel_end;
    }
    /* same for right side */
    if (right->op == VARIABLE) {
        if (right->left.sp->modifiers & M_CONST) {
            /* sanity check */
            if (right->left.sp->init==NULL)
                fatal("Internal error EV%d: Constant without initial value", __LINE__);
            else
                return eval_rel(op, left, node_dup(right->left.sp->init));
        }
        else goto eval_rel_end;
    }

    /* if literal, we can reduce */
    if (is_literal(left->op) && is_literal(right->op)) {
        /* if bitstrings, convert to int */
        if (left->op == BIT_LITERAL) left = n_bit2int(node_dup(left));
        if (right->op == BIT_LITERAL) right = n_bit2int(node_dup(right));
        
        /* int/int */
        if (left->type <= INT && right->type <= INT) {
            switch (op) {
            case '<':
                left->left.ival = (left->left.ival < right->left.ival);
                break;
            case '>':
                left->left.ival = (left->left.ival > right->left.ival);
                break;
            case LE:
                left->left.ival = (left->left.ival <= right->left.ival);
                break;
            case GE:
                left->left.ival = (left->left.ival <= right->left.ival);
                break;
            case EQ:
                left->left.ival = (left->left.ival == right->left.ival);
                break;
            case NE:
                left->left.ival = (left->left.ival != right->left.ival);
                break;
            }
            return left;
        }
        /* int/double */
        else if (left->type <= INT && right->type > INT) {
            switch (op) {
            case '<':
                left->left.ival = (left->left.ival < right->left.dval);
                break;
            case '>':
                left->left.ival = (left->left.ival > right->left.dval);
                break;
            case LE:
                left->left.ival = (left->left.ival <= right->left.dval);
                break;
            case GE:
                left->left.ival = (left->left.ival <= right->left.dval);
                break;
            case EQ:
                left->left.ival = (left->left.ival == right->left.dval);
                break;
            case NE:
                left->left.ival = (left->left.ival != right->left.dval);
                break;
            }
            return left;
        }
        /* double/int */
        else if (left->type > INT && right->type <= INT) {
            switch (op) {
            case '<':
                right->left.ival = (left->left.dval < right->left.ival);
                break;
            case '>':
                right->left.ival = (left->left.dval > right->left.ival);
                break;
            case LE:
                right->left.ival = (left->left.dval <= right->left.ival);
                break;
            case GE:
                right->left.ival = (left->left.dval <= right->left.ival);
                break;
            case EQ:
                right->left.ival = (left->left.dval == right->left.ival);
                break;
            case NE:
                right->left.ival = (left->left.dval != right->left.ival);
                break;
            }
            return right;
        }
        /* double/double */
        else {
            switch (op) {
            case '<':
                left->left.ival = (left->left.dval < right->left.dval);
                break;
            case '>':
                left->left.ival = (left->left.dval > right->left.dval);
                break;
            case LE:
                left->left.ival = (left->left.dval <= right->left.dval);
                break;
            case GE:
                left->left.ival = (left->left.dval <= right->left.dval);
                break;
            case EQ:
                left->left.ival = (left->left.dval == right->left.dval);
                break;
            case NE:
                left->left.ival = (left->left.dval != right->left.dval);
                break;
            }
            left->type = INT;
            left->ptype = s_lookup("int");
            return left;
        }
    }
    
eval_rel_end:
    n = node(op, left, right);
    n->type = left->type;
    n->ptype = left->ptype;
    return n;
}

/* eval logical operator expression (&, ^, |, &&, ||) */
node_t *eval_logic(int op, node_t *left, node_t *right)
{
    node_t *n;
    
    /* not allowed on classes */
    if (left->type == CLASS_TYPE || right->type == CLASS_TYPE) {
        yyerror("Operator '%s' cannot be applied to classes", s_op2str(op));
        goto eval_logic_end;
    }

    /* no reduction possible on variables, unless they are const */
    if (left->op == VARIABLE) {
        if (left->left.sp->modifiers & M_CONST) {
            /* sanity check */
            if (left->left.sp->init == NULL)
                fatal("Internal error EV%d: Constant without initial value", __LINE__);
            else
                return eval_logic(op, node_dup(left->left.sp->init), right);
        }
        else goto eval_logic_end;
    }

    /* same for right side */
    if (right->op == VARIABLE) {
        if (right->left.sp->modifiers & M_CONST) {
            /* sanity check */
            if (right->left.sp->init == NULL)
                fatal("Internal error EV%d: Constant without initial value", __LINE__);
            else
                return eval_logic(op, left, node_dup(right->left.sp->init));
        }
        else goto eval_logic_end;
    }

    /* logical operators not allowed on double values */
    if ((left->type > INT || right->type > INT) && !(op == AND || op == OR))  {
        yyerror("Operator '%s' cannot be applied to floats or doubles", s_op2str(op));
        goto eval_logic_end;
    }
    
    /* if literal, we can reduce */
    if (is_literal(left->op) && is_literal(right->op)) {
        /* if bitstrings, convert to int */
        if (left->op == BIT_LITERAL) left = n_bit2int(node_dup(left));
        if (right->op == BIT_LITERAL) right = n_bit2int(node_dup(right));
        
        switch (op) {
        case '&':
            left->left.ival = (left->left.ival & right->left.ival);
            break;
        case '^':
            left->left.ival = (left->left.ival ^ right->left.ival);
            break;
        case '|':
            left->left.ival = (left->left.ival | right->left.ival);
            break;
        case AND:
            left->left.ival = (left->left.ival && right->left.ival);
            break;
        case OR:
            left->left.ival = (left->left.ival || right->left.ival);
            break;
        }
        return left;
    }
    
eval_logic_end:
    n = node(op, left, right);
    n->type = left->type;
    n->ptype = left->ptype;
    return n;
}

/* eval conditional operator expression (?:) */
node_t *eval_cond(int op, node_t *cond, node_t *left, node_t *right)
{
    node_t *n;
    int v;
    
    /* not allowed on classes */
    if (cond->type == CLASS_TYPE || left->type == CLASS_TYPE || right->type == CLASS_TYPE) {
        yyerror("Operator '%s' cannot be applied to classes", s_op2str(op));
        goto eval_cond_end;
    }

    /* no reduction possible on variables, unless they are const */
    if (cond->op == VARIABLE) {
        if (cond->left.sp->modifiers & M_CONST) {
            /* sanity check */
            if (cond->left.sp->init == NULL)
                fatal("Internal error EV%d: Constant without initial value", __LINE__);
            else
                return eval_cond(op, node_dup(cond->left.sp->init), left, right);
        }
    }

    /* same for left side */
    if (left->op == VARIABLE) {
        if (left->left.sp->modifiers & M_CONST) {
            /* sanity check */
            if (left->left.sp->init == NULL)
                fatal("Internal error EV%d: Constant without initial value", __LINE__);
            else
                return eval_cond(op, cond, node_dup(left->left.sp->init), right);
        }
    }

    /* same for right side */
    if (right->op == VARIABLE) {
        if (right->left.sp->modifiers & M_CONST) {
            /* sanity check */
            if (right->left.sp->init == NULL)
                fatal("Internal error EV%d: Constant without initial value", __LINE__);
            else
                return eval_cond(op, cond, left, node_dup(right->left.sp->init));
        }
    }
    
    /* if cond is a literal, we can reduce */
    if (is_literal(cond->op)) {
        switch (cond->op) {
            
        case BIT_LITERAL:
            v = cond->left.bit->val;
            break;
            
        case INT_LITERAL:
            v = cond->left.ival;
            break;
            
        case DOUBLE_LITERAL:
            v = (cond->left.dval != 0);
            
        default:
            fatal("Internal error EV%d: Expected literal", __LINE__);
        }

        if (v) return left;
        else return right;
    }

eval_cond_end:
    n = node3(op, cond, left, right);
    n->type = left->type;
    n->ptype = left->ptype;
    return n;
}

/* eval array operator */
node_t *eval_subarray(node_t *left, node_t *right)
{
    node_t *n;
    
    if (left == NULL || right == NULL) fatal("Internal error EV%d: Invalid argument", __LINE__);
    
    if (right->type > INT) {
        if (right->ptype == NULL)
            fatal("Internal error EV%d: Expression without type", __LINE__);
        else {
            yyerror("Invalid array index type ('%s')", right->ptype->name);
            goto eval_subarray_end;
        }
    }
    /* see if we can reduce */
    if (right->op == VARIABLE) {
        if (right->left.sp->modifiers & M_CONST) {
            /* sanity check */
            if (right->left.sp->init == NULL)
                fatal("Internal error EV%d: Constant without initial value", __LINE__);
            else
                return eval_subarray(left, node_dup(right->left.sp->init));
        }
    }
    /* more comprehensive tests */
    check_array_index(right);
    
eval_subarray_end:
    n = node('[', left, right);
    n->type = left->type;
    n->ptype = left->ptype;
    return n;
}

/* eval a unary expression for completed arrays */
node_t *eval_array(node_t *expr)
{
    int c, literal, *dim_idx=NULL, *dim_values=NULL, i;
    symbol_t *sp;
    node_t *n;
    node_t *ret;

	/* NOTE:
	 * We don't evaluate arrays, even if constant, because side-effects from 
	 * array index expressions would be lost! The only exception is const arrays
     * with all indices being literals, and init value being a literal.
     *
     * Also note that this function is called on various expressions, not just
     * arrays, so beware (this is because completed arrays can only be found
     * in higher-level expressions).
     */
    c = check_array(expr, &sp, &literal);
    /* if not ok, don't do eval */
    if (!c) return expr;

    /* if global scope, eval */
    if (sp == NULL) fatal("Internal error E%d: Invalid argument", __LINE__);

    /* if the variable doesn't have dims, then there was no array to begin with */
    if (sp->dims == 0) return expr;

    /* sanity check */
    if (sp->dim == NULL) return expr;

    /* constant with all literal indices, with init value (may not have init due to user error) */
    if (!literal || !(sp->modifiers & M_CONST) || sp->init == NULL) return expr;

    /* our default return value */
    ret = expr;

    /* if not array initializer, great; just return the single initial value but only if it is a literal */
    if (sp->init->op != ARRAY) if (is_literal(sp->init->op)) ret = sp->init;
    else {
        /* now we need to find the initializing element */

        /* first extract indices */
        dim_idx = (int *)getmem(sp->dims*sizeof(int));
        /* look for '[' */
        n = expr;
        i = 0;
        /* descent to the base of the array, counting levels */
        while (n->op == '[') {
            if (n->right.np == NULL)
                fatal("Internal error E%d: Unexpected end of array expression", __LINE__);
            if (n->right.np->op != INT_LITERAL)
                fatal("Internal error E%d: Expected integer literal array index", __LINE__);
            dim_idx[i] = n->right.np->left.ival;
            n = n->left.np;
            i++;
        }
        /* now construct dim values array */
        dim_values = (int *)getmem(sp->dims*sizeof(int));
        for (i=0; i<sp->dims; i++) {
            n = sp->dim[i];
            if (n == NULL)
                fatal("Internal error E%d: Unexpected end of dimensioning array", __LINE__);
            if (n->left.np == NULL)
                fatal("Internal error E%d: Array expression without value", __LINE__);
            if (n->left.np->op != INT_LITERAL) goto eval_array_end;
            dim_values[i]=n->left.np->left.ival;
        }
        /* get the initializing element (we are done with 'n', so it is reused here) */
        n = c_array_get_init(sp, dim_values, dim_idx, sp->init);
        if (n == NULL) fatal("Internal error E%d: Could not find array element", __LINE__);
        if (is_literal(n->op)) {
            ret = node_dup(n);
            ret->eval = expr;
        }
        /* else fall below */
    }

eval_array_end:
    if (dim_idx != NULL) free(dim_idx);
    if (dim_values != NULL) free(dim_values);
    return ret;
}

/* get lvalue node (helper for built-in operators) -- similar to get_lvalue in check.c */
node_t *eval_get_lvalue(node_t *exp)
{
    /*
     * The only caveat here is arrays and rescoped vars; to find the actual symbol,
     * you have to go all the way to the left when you hit a '[', or to the immediate
     * right when you hit a '.'; this is because of the way parsing is performed.
     * If you hit a unary operator, you just move over it
     */
    if (exp->op==VARIABLE)
        return exp;
    if (exp->op=='.')
        return exp->right.np;
    if (exp->op=='[')
        /* recursively look for base of array */
        return eval_get_lvalue(exp->left.np);
    if (is_literal(exp->op))
        return NULL;
    /* we hit a unary operator - go either left or right (only one should be valid) */
    if (exp->left.np!=NULL && exp->right.np==NULL)
        return eval_get_lvalue(exp->left.np);
    else if (exp->left.np==NULL && exp->right.np!=NULL)
        return eval_get_lvalue(exp->right.np);
    else if (exp->left.np==NULL && exp->right.np==NULL)
        /* lvalue not found */
        return NULL;
    else
        /* fatal("Internal error CH%d: Lvalue contained binary operator", __LINE__); */
		return NULL;
}

/* eval lengthof operator */
node_t *eval_lengthof(node_t *args)
{
	node_t *np, *ret, *pa;
    symbol_t *sp;

    /* try original variable expr, if present (in case it was reduced by evaluator) */
    if (args->eval != NULL) pa = args->eval;
    else pa = args;

    /* check that we have a valid argument */
    if (!check_lengthof(pa)) {
        /* create a 0 value */
        ret = leafi(INT_LITERAL, 0);
        return ret;
    }
    /* get node with lvalue */
    np = eval_get_lvalue(pa);
    if (np == NULL)
        fatal("Internal error EV%d: Could not find lvalue", __LINE__);
    /* get the symbol */
    sp = np->left.sp;
    /* add a 'lengthof' variable */
    set_lengthof(sp);
    
    /* rebuild expression tree */
    if (sp->ptype->ident == CLASS_TYPE) {
        /* class; just add .lengthof in the expression */
        ret = node('.', pa, leafs(VARIABLE, sp->plengthof));     
        ret->type = sp->plengthof->ptype->ident; 
        ret->ptype = sp->plengthof->ptype;
    }
    else {
        /* regular variable; replace symbol entry */
        np->left.sp = np->left.sp->plengthof;
        /* also change the type of the expression */
        pa->type = sp->plengthof->ptype->ident;
        pa->ptype = sp->plengthof->ptype;
        ret = pa;
    }
    return ret;
}

/* eval isidof operator */
node_t *eval_isidof(symbol_t *sp, node_t *args)
{
    node_t *ret;

    if (sp->ident != CLASS_TYPE) fatal("Internal error EV%d: Expected class type", __LINE__);
 
    /* check if we have a valid argument */
    if (!check_isidof(sp, args)) {
        /* create a 0 value */
        ret = leafi(INT_LITERAL, 0);
        return ret;
    }
    /* mark for isidof() method generation */
    sp->isidof = 1;

    /* create expression */
    ret = node(ISIDOF, leafs(CLASS_TYPE, sp), args);
    ret->type = INT;
    ret->ptype = s_lookup("int");
    return ret;
}

/* eval skipbits operator */
node_t *eval_skipbits(node_t *args)
{
    node_t *ret;

    /* check if we have a valid argument */
    if (args->type == CLASS_TYPE) {
        yyerror("The skipbits function cannot take a class as an argument");
        goto eval_skipbits_end;
    }

    /* no reduction possible on variables, unless they are const */
    if (args->op == VARIABLE) {
        if (args->left.sp->modifiers & M_CONST) {
            /* sanity check */
            if (args->left.sp->init == NULL)
                fatal("Internal error EV%d: Constant without initial value", __LINE__);
            else return eval_skipbits(node_dup(args->left.sp->init));
        }
        else goto eval_skipbits_end;
    }

    /* skipbits function cannot take a double value */
    if (args->type > INT) {
        yyerror("The skipbits function cannot take a float or double");
        goto eval_skipbits_end;
    }
    
    /* if a bitstring, convert to int */
    if (args->op == BIT_LITERAL) args = n_bit2int(node_dup(args));
        
eval_skipbits_end:
    ret = node(SKIPBITS, args, NULL);
    ret->type = INT;
    ret->ptype = s_lookup("int");

    return ret;
}

/* eval nextbits operator */
node_t *eval_nextbits(node_t *n, unsigned int mod_val, node_t* align_expr)
{
    int type;
    node_t *ret;

    /* check if we have a valid argument */
    if (n->type == CLASS_TYPE) {
        yyerror("The nextbits function cannot take a class as an argument");
        goto eval_nextbits_end;
    }

    /* no reduction possible on variables, unless they are const */
    if (n->op == VARIABLE) {
        if (n->left.sp->modifiers & M_CONST) {
            /* sanity check */
            if (n->left.sp->init == NULL)
                fatal("Internal error EV%d: Constant without initial value", __LINE__);
            else return eval_nextbits(node_dup(n->left.sp->init), mod_val, align_expr);
        }
        else goto eval_nextbits_end;
    }

    /* nextbits function cannot take a double value */
    if (n->type > INT) {
        yyerror("The nextbits function cannot take a float or double");
        goto eval_nextbits_end;
    }
    
    /* if bitstrings, convert to int */
    if (n->op == BIT_LITERAL) n = n_bit2int(node_dup(n));
        
eval_nextbits_end:
    if (mod_val & M_LITTLE) {
        if (mod_val & M_SIGNED) type = LITTLE_SNEXTBITS;
        else type = LITTLE_NEXTBITS;
    }
    else {
        if (mod_val & M_SIGNED) type = SNEXTBITS;
        else type = NEXTBITS;
    }
    ret = node(type, n, align_expr);
    ret->type = INT;
    ret->ptype = s_lookup("int");

    return ret;
}

/* expand a string into an array */
node_t* eval_array_string(char* str)
{
    char* p;
    node_t* np = NULL;

    p = str;
    while (*p) {
        if (np == NULL) np = leafi(INT_LITERAL, *p);
        else np = node(',', np, leafi(INT_LITERAL, *p));
        p++;
    }
    /* reached trailing null character */
    if (null_strings) {
        if (np == NULL) np = leafi(INT_LITERAL, *p);
        else np = node(',', np, leafi(INT_LITERAL, *p));
    }
    return np;
}
