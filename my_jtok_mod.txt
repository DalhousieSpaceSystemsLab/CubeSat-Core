/**
 * @file jtok.c
 * @author Carl Mattatall (cmattatall2@gmail.com)
 * @brief Statically allocated JTOK parser for embedded systems
 * @version 2.3
 * @date 2020-11-13
 *
 * @copyright Copyright (c) 2020 Carl Mattatall
 *
 */

#include <assert.h>
#include <limits.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>

#include "jtok.h"
#include "jtok_object.h"
#include "jtok_array.h"
#include "jtok_primitive.h"
#include "jtok_string.h"
#include "jtok_shared.h"


static jtok_parser_t jtok_new_parser(const char *json_str, jtok_tkn_t *tokens,
                                     unsigned int poolsize);
static bool          jtok_is_type_aggregate(const jtok_tkn_t *const tkn);


static bool (*const tokcmp_funcs[])(const jtok_tkn_t *const,
                                    const jtok_tkn_t *const) = {
    [JTOK_PRIMITIVE] = jtok_toktokcmp_primitive,
    [JTOK_OBJECT]    = jtok_toktokcmp_object,
    [JTOK_ARRAY]     = jtok_toktokcmp_array,
    [JTOK_STRING]    = jtok_toktokcmp_string,
};


static const char *jtokerr_messages[] = {
    [JTOK_PARSE_STATUS_OK]            = "JTOK_PARSE_STATUS_OK",
    [JTOK_PARSE_STATUS_UNKNOWN_ERROR] = "JTOK_PARSE_STATUS_UNKNOWN_ERROR",
    [JTOK_PARSE_STATUS_NOMEM]         = "JTOK_PARSE_STATUS_NOMEM",
    [JTOK_PARSE_STATUS_INVAL]         = "JTOK_PARSE_STATUS_INVAL",
    [JTOK_PARSE_STATUS_PARTIAL_TOKEN] = "JTOK_PARSE_STATUS_PARTIAL_TOKEN",
    [JTOK_PARSE_STATUS_KEY_NO_VAL]    = "JTOK_PARSE_STATUS_KEY_NO_VAL",
    [JTOK_PARSE_STATUS_COMMA_NO_KEY]  = "JTOK_PARSE_STATUS_COMMA_NO_KEY",
    [JTOK_PARSE_STATUS_OBJECT_INVALID_PARENT] =
        "JTOK_PARSE_STATUS_OBJECT_INVALID_PARENT",
    [JTOK_PARSE_STATUS_INVALID_PRIMITIVE] =
        "JTOK_PARSE_STATUS_INVALID_PRIMITIVE",
    [JTOK_PARSE_STATUS_NON_OBJECT]       = "JTOK_PARSE_STATUS_NON_OBJECT",
    [JTOK_PARSE_STATUS_INVALID_START]    = "JTOK_PARSE_STATUS_INVALID_START",
    [JTOK_PARSE_STATUS_INVALID_END]      = "JTOK_PARSE_STATUS_INVALID_END",
    [JTOK_PARSE_STATUS_OBJ_NOKEY]        = "JTOK_PARSE_STATUS_OBJ_NOKEY",
    [JTOK_STATUS_MIXED_ARRAY]            = "JTOK_STATUS_MIXED_ARRAY",
    [JTOK_PARSE_STATUS_ARRAY_SEPARATOR]  = "JTOK_PARSE_STATUS_ARRAY_SEPARATOR",
    [JTOK_PARSE_STATUS_STRAY_COMMA]      = "JTOK_PARSE_STATUS_STRAY_COMMA",
    [JTOK_PARSE_STATUS_VAL_NO_COLON]     = "JTOK_PARSE_STATUS_VAL_NO_COLON",
    [JTOK_PARSE_STATUS_KEY_MULTIPLE_VAL] = "JTOK_PARSE_STATUS_KEY_MULTIPLE_VAL",
    [JTOK_PARSE_STATUS_INVALID_PARENT]   = "JTOK_PARSE_STATUS_INVALID_PARENT",
    [JTOK_PARSE_STATUS_VAL_NO_COMMA]     = "JTOK_PARSE_STATUS_VAL_NO_COMMA",
    [JTOK_PARSE_STATUS_NON_ARRAY]        = "JTOK_PARSE_STATUS_NON_ARRAY",
    [JTOK_PARSE_STATUS_EMPTY_KEY]        = "JTOK_PARSE_STATUS_EMPTY_KEY",
    [JTOK_PARSE_STATUS_BAD_STRING]       = "JTOK_PARSE_STATUS_BAD_STRING",
};


char *jtok_toktypename(JTOK_TYPE_t type)
{
    static const char *jtok_tkn_type_names[] = {
        [JTOK_PRIMITIVE]        = "JTOK_PRIMITIVE",
        [JTOK_OBJECT]           = "JTOK_OBJECT",
        [JTOK_ARRAY]            = "JTOK_ARRAY",
        [JTOK_STRING]           = "JTOK_STRING",
        [JTOK_UNASSIGNED_TOKEN] = "JTOK_UNASSIGNED_TOKEN"};
    char *retval = NULL;
    switch (type)
    {
        case JTOK_PRIMITIVE:
        case JTOK_OBJECT:
        case JTOK_ARRAY:
        case JTOK_STRING:
        {
            retval = (char *)jtok_tkn_type_names[type];
        }
        break;
        default:
        {
        }
        break;
    }
    return retval;
}


char *jtok_jtokerr_messages(JTOK_PARSE_STATUS_t err)
{
    char *retval;
    switch (err)
    {
        case JTOK_PARSE_STATUS_OK:
        case JTOK_PARSE_STATUS_UNKNOWN_ERROR:
        case JTOK_PARSE_STATUS_NOMEM:
        case JTOK_PARSE_STATUS_INVAL:
        case JTOK_PARSE_STATUS_PARTIAL_TOKEN:
        case JTOK_PARSE_STATUS_KEY_NO_VAL:
        case JTOK_PARSE_STATUS_COMMA_NO_KEY:
        case JTOK_PARSE_STATUS_OBJECT_INVALID_PARENT:
        case JTOK_PARSE_STATUS_INVALID_PRIMITIVE:
        case JTOK_PARSE_STATUS_NON_OBJECT:
        case JTOK_PARSE_STATUS_INVALID_START:
        case JTOK_PARSE_STATUS_INVALID_END:
        case JTOK_PARSE_STATUS_OBJ_NOKEY:
        case JTOK_STATUS_MIXED_ARRAY:
        case JTOK_PARSE_STATUS_ARRAY_SEPARATOR:
        case JTOK_PARSE_STATUS_STRAY_COMMA:
        case JTOK_PARSE_STATUS_VAL_NO_COLON:
        case JTOK_PARSE_STATUS_KEY_MULTIPLE_VAL:
        case JTOK_PARSE_STATUS_INVALID_PARENT:
        case JTOK_PARSE_STATUS_VAL_NO_COMMA:
        case JTOK_PARSE_STATUS_NON_ARRAY:
        case JTOK_PARSE_STATUS_EMPTY_KEY:
        case JTOK_PARSE_STATUS_BAD_STRING:
        {
            retval = (char *)jtokerr_messages[err];
        }
        break;
        default:
        {
            assert(0);
            retval = NULL;
        }
        break;
    }
    return retval;
}


uint_least16_t jtok_toklen(const jtok_tkn_t *tok)
{
    uint_least16_t len = 0;
    if (tok != NULL)
    {
        uint_least64_t tokstart = tok->start;
        uint_least64_t tokend   = tok->end;
        if (tokend - tokstart < UINT16_MAX)
        {
            len = tokend - tokstart;
        }
    }
    return len;
}


bool jtok_tokcmp(const char *str, const jtok_tkn_t *tok)
{
    bool result = false;
    if (str == NULL)
    {
        if (tok != NULL && tok->json == NULL)
        {
            result = true;
        }
    }
    else if (tok != NULL && tok->json == NULL)
    {
        if (str == NULL)
        {
            result = true;
        }
    }
    else
    {
        uint_least16_t least_size = jtok_toklen(tok);
        uint_least16_t slen       = strlen(str);
        if (least_size < slen)
        {
            least_size = slen;
        }

        /* actually compare them */
        if (strncmp((const char *)str, (char *)&tok->json[tok->start],
                    least_size) == 0)
        {
            result = true;
        }
        else
        {
            result = false;
        }
    }
    return result;
}


bool jtok_tokncmp(const char *str, const jtok_tkn_t *tok, uint_least16_t n)
{
    bool result = false;
    if (str != NULL && tok != NULL && tok->json != NULL)
    {
        uint_least16_t least_size = jtok_toklen(tok);
        uint_least16_t slen       = strlen(str);
        if (least_size < slen)
        {
            least_size = slen;
        }

        if (least_size < n)
        {
            least_size = n;
        }

        /* actually compare them */
        if (strncmp((const char *)str, (char *)&tok->json[tok->start],
                    least_size) == 0)
        {
            result = true;
        }
        else
        {
            result = false;
        }
    }
    return result;
}


char *jtok_tokcpy(char *dst, uint_least16_t bufsize, const jtok_tkn_t *tkn)
{
    char *result = NULL;
    if (dst != NULL && tkn != NULL && tkn->json != NULL)
    {
        uint_least16_t copy_count = jtok_toklen(tkn);
        if (copy_count > bufsize)
        {
            copy_count = bufsize;
        }
        result = strncpy(dst, (char *)&tkn->json[tkn->start], copy_count);
    }
    return result;
}


char *jtok_tokncpy(char *dst, uint_least16_t bufsize, const jtok_tkn_t *tkn,
                   uint_least16_t n)
{
    char *         result = NULL;
    uint_least16_t count  = bufsize;
    if (bufsize > n)
    {
        count = n;
    }
    result = jtok_tokcpy(dst, count, tkn);
    return result;
}


JTOK_PARSE_STATUS_t jtok_parse(const char *json, jtok_tkn_t *tkns, size_t size)
{
    jtok_parser_t       parser;
    JTOK_PARSE_STATUS_t status;
    if (NULL == json)
    {
        status = JTOK_PARSE_STATUS_NULL_PARAM;
    }
    else if (tkns == NULL)
    {
        status = JTOK_PARSE_STATUS_NULL_PARAM;
    }
    else if (size < 1)
    {
        status = JTOK_PARSE_STATUS_NOMEM;
    }
    else
    {
        parser = jtok_new_parser(json, tkns, size);

        /* Skip leading whitespace */
        while (isspace((int)json[parser.pos]))
        {
            parser.pos++;
        }
        status = jtok_parse_object(&parser, 0);
    }

  // Populates remaining unused tokens with JTOK_UNASSIGNED_TOKEN
  // - Alex
  for (int x = parser.toknext; x < size; x++) {
    tkns[x].type = JTOK_UNASSIGNED_TOKEN;
  }

    return status;
}


bool jtok_tokenIsKey(jtok_tkn_t token)
{
    if (token.type == JTOK_STRING)
    {
        if (token.size == 1)
        {
            return true;
        }
    }
    return false;
}


int jtok_token_tostr(char *buf, unsigned int size, const char *json,
                     jtok_tkn_t token)
{
    if (buf != NULL)
    {
        unsigned int blen = 0;
        int          k;
        blen += snprintf(buf + blen, size - blen, "token : ");
        for (k = token.start; k < token.end; k++)
        {
            blen += snprintf(buf + blen, size - blen, "%c", json[k]);
        }
        blen += snprintf(buf + blen, size - blen, "\n");
        blen += snprintf(buf + blen, size - blen, "type: %s\n",
                         jtok_toktypename(token.type));

#ifdef DEBUG
        blen += snprintf(buf + blen, size - blen, "start : %d\n", token.start);
        blen += snprintf(buf + blen, size - blen, "end : %d\n", token.end);
#endif /* ifdef DEBUG */

        return blen;
    }
    else
    {
        return -1;
    }
}


bool jtok_toktokcmp(const jtok_tkn_t *tkn1, const jtok_tkn_t *tkn2)
{
    bool is_equal = false;
    if (tkn1->type == tkn2->type)
    {
        return tokcmp_funcs[tkn1->type];
    }
    return is_equal;
}


jtok_tkn_t *jtok_obj_has_key(const jtok_tkn_t *obj, const char *key_str)
{
    jtok_tkn_t *key = NULL;
    if (obj->type == JTOK_OBJECT)
    {
        size_t      i;
        jtok_tkn_t *tkns = obj->pool;
        jtok_tkn_t *cur_key_tkn;
        if (obj->size > 0)
        {
            cur_key_tkn = (jtok_tkn_t *)(obj + 1);
            for (i = 0; i < (size_t)obj->size; i++)
            {
                /* If size is nonzero, first key of object will be RIGHT AFTER
                 */
                if (jtok_tokcmp(key_str, cur_key_tkn))
                {
                    key = cur_key_tkn;
                    break;
                }
                else
                {
                    if (cur_key_tkn->sibling != JTOK_NO_SIBLING_IDX)
                    {
                        cur_key_tkn = &tkns[cur_key_tkn->sibling];
                    }
                    else
                    {
                        break;
                    }
                }
            }
        }
    }
    return key;
}


jtok_tkn_t *jtok_get_child(const jtok_tkn_t *obj)
{
    if (obj == NULL || obj->size == -1)
    {
        return NULL;
    }
    else
    {
        return (jtok_tkn_t *)(obj + 1);
    }
}


jtok_tkn_t *jtok_get_next_sibling(const jtok_tkn_t *child)
{
    if (child == NULL || child->parent == JTOK_NO_PARENT_IDX)
    {
        return NULL;
    }
    else
    {
        if (child->sibling == JTOK_NO_SIBLING_IDX)
        {
            return NULL;
        }
        else
        {
            jtok_tkn_t *token_pool = child->pool;
            return &token_pool[child->sibling];
        }
    }
}


static jtok_parser_t jtok_new_parser(const char *json_str, jtok_tkn_t *tokens,
                                     unsigned int poolsize)
{
    jtok_parser_t parser;
    parser.pos        = 0;
    parser.toknext    = 0;
    parser.toksuper   = JTOK_NO_PARENT_IDX;
    parser.json       = (char *)json_str;
    parser.json_len   = strlen(json_str);
    parser.last_child = JTOK_NO_CHILD_IDX;
    parser.tkn_pool   = tokens;
    parser.pool_size  = poolsize;
    return parser;
}


static bool jtok_is_type_aggregate(const jtok_tkn_t *const tkn)
{
    assert(NULL != tkn);
    return tkn->type == JTOK_OBJECT || tkn->type == JTOK_ARRAY;
}
