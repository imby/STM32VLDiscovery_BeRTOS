/**
 * \file
 * <!--
 * This file is part of BeRTOS.
 *
 * Bertos is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 * As a special exception, you may use this file as part of a free software
 * library without restriction.  Specifically, if other files instantiate
 * templates or use macros or inline functions from this file, or you compile
 * this file and link it with other files to produce an executable, this
 * file does not by itself cause the resulting executable to be covered by
 * the GNU General Public License.  This exception does not however
 * invalidate any other reasons why the executable file might be covered by
 * the GNU General Public License.
 *
 * Copyright 2003, 2006 Develer S.r.l. (http://www.develer.com/)
 * All Rights Reserved.
 * -->
 *
 * \defgroup parser Simple RPC machinery
 * \ingroup mware
 * \{
 *
 * \brief Channel protocol parser and commands.
 *
 * This module provides a simple text based RPC implementation.
 * Often there is the need to give a command to the device and receive results
 * back. Each command may have a variable number of input and output
 * parameters, with variable type, and a return code which indicates if the
 * command was successfully executed or not; this module provides the machinery
 * to facilitate the above RPC scenario.
 * You will need to write the RPC input and reply code as well as
 * the definition of the commands.
 *
 * Commands are defined using a CmdTemplate struct containing:
 * - command name: the string that will be matched by the parser;
 * - command arguments: a string representing type and number of input
 *   arguments;
 * - command output: a string representing type and number of output arguments;
 * - function callback: function implementing the command.
 *
 * Once you have declared the commands, you need to register them in the
 * parser with the function parser_register_cmd().
 * You are strongly encouraged to use MAKE_CMD() (or alternatively
 * MAKE_TEMPLATE()) and REGISTER_CMD() to declare and register commands.
 *
 * A command line can be parsed with the following steps:
 * - find the corresponding command template with parser_get_cmd_template()
 * - extract command arguments with parser_get_cmd_arguments()
 * - execute the command with parser_execute_cmd()
 *
 * You can also provide interactive command line completion using
 * parser_rl_match().
 *
 * Example:
 * \code
 * // Declare a buzzer command
 * MAKE_CMD(beep, "d", "",
 * ({
 * 	buz_beep(args[1].l);
 * 	RC_OK;
 * }), 0)
 *
 * // initialize the parser
 * parser_init();
 * REGISTER_CMD(beep);
 *
 * // parse an input line
 * char buf[80];
 * // read line from somewhere
 * rpc_get(buf);
 * // now parse the line
 * const struct CmdTemplate *templ;
 * templ = parser_get_cmd_template(buf);
 *
 * // Take arguments (optionally check errors)
 * parms args[PARSER_MAX_ARGS];
 * parser_get_cmd_arguments(buf, templ, args);
 * //Execute command
 * if(!parser_execute_cmd(templ, args))
 * {
 * 	// error
 * }
 * // Now args contain the outputs of the function, you can send it
 * // back to the caller
 * rpc_reply(args)
 *
 * \endcode
 *
 * \author Bernie Innocenti <bernie@codewiz.org>
 * \author Stefano Fedrigo <aleph@develer.com>
 * \author Giovanni Bajo <rasky@develer.com>
 *
 * $WIZ$ module_name = "parser"
 * $WIZ$ module_configuration = "bertos/cfg/cfg_parser.h"
 * $WIZ$ module_depends = "kfile", "hashtable"
 */


#ifndef MWARE_PARSER_H
#define MWARE_PARSER_H

#include "cfg/cfg_parser.h"

#include <cpu/types.h>

/**
 * Error generated by the commands through the return code.
 */
typedef enum
{
	RC_ERROR  = -1, ///< Reply with error.
	RC_OK     = 0,  ///< No reply (ignore reply arguments).
	RC_REPLY  = 1,  ///< Reply command arguments.
	RC_SKIP   = 2   ///< Skip following commands
} ResultCode;

/** union that contains parameters passed to and from commands */
typedef union { long l; const char *s; } parms;
/** pointer to commands */
typedef ResultCode (*CmdFuncPtr)(parms args_results[]);

/**
 * Define a command that can be tokenized by the parser.
 *
 * The format strings are sequences of characters, one for each
 * parameter/result. Valid characters are:
 *
 *  d - a long integer, in decimal format
 *  s - a var string (in RAM)
 *
 * \note To create and fill an instance for this function, it is strongly
 * advised to use \c DECLARE_CMD_HUNK (cmd_hunk.h).
 */
struct CmdTemplate
{
	const char *name;          ///< Name of command
	const char *arg_fmt;       ///< Format string for the input
	const char *result_fmt;    ///< Format string for the output
	CmdFuncPtr func;           ///< Pointer to the handler function
	uint16_t   flags;          ///< Currently unused.
};

#define REGISTER_FUNCTION parser_register_cmd

/**
 * Utility function to register a command.
 *
 * \param NAME Command name to register
 */
#define REGISTER_CMD(NAME) REGISTER_FUNCTION(&cmd_ ## NAME ## _template)

/**
 * Utility macro to create a command template.
 *
 * It requires that a callback function with name \a cmd_NAME
 * is already defined.
 * \param NAME Command name
 * \param ARGS Input arguments
 * \param RES Output arguments
 * \param FLAGS Command flags
 */
#define MAKE_TEMPLATE(NAME, ARGS, RES, FLAGS)          \
const struct CmdTemplate cmd_ ## NAME ## _template =   \
{                                                      \
	#NAME, ARGS, RES, cmd_ ## NAME, FLAGS          \
};

/**
 * Utility macro to create command templates and callback functions.
 *
 * Example for a version command:
 * \code
 * MAKE_CMD(ver, "", "ddd",
 * ({
 * 	args[1].l = VERS_MAJOR;
 * 	args[2].l = VERS_MINOR;
 * 	args[3].l = VERS_REV;
 * 	RC_OK;
 * }), 0);
 * \endcode
 *
 * Remember that input and output parameters start from index 1, since
 * args[0] is the command itself.
 * The last line is the return value of the function.
 *
 * \param NAME Command name matched by the parser
 * \param ARGS Input arguments to the command
 * \param RES Output arguments of the command
 * \param BODY Command body, expressed with C 'statement expression'
 * \param FLAGS Command flags
 */
#define MAKE_CMD(NAME, ARGS, RES, BODY, FLAGS)  \
static ResultCode cmd_ ## NAME (parms *args)    \
{                                               \
	return (ResultCode)BODY;                \
}                                               \
MAKE_TEMPLATE(NAME, ARGS, RES, FLAGS)

/**
 * Initialize the parser module
 *
 * \note This function must be called before any other function in this module
 */
void parser_init(void);

void parser_register_cmd(const struct CmdTemplate* cmd);


/**
 * Hook for readline to provide completion support for the commands
 * registered in the parser.
 *
 * \note This is meant to be used with mware/readline.c. See the
 * documentation there for a description of this hook.
 */
const char* parser_rl_match(void* dummy, const char* word, int word_len);

bool parser_process_line(const char* line);

/**
 * Execute a command with its arguments, and fetch its results.
 *
 * The \a args paramenter is value-result: it provides input arguments to
 * the callback function and it stores output values on return.
 *
 * \param templ Template of the command to be executed
 * \param args Arguments for the command, and will contain the results
 *
 * \return False if the command returned an error, true otherwise
 */
INLINE bool parser_execute_cmd(const struct CmdTemplate* templ, parms args[CONFIG_PARSER_MAX_ARGS])
{
	return (templ->func(args) == 0);
}

const struct CmdTemplate* parser_get_cmd_template(const char* line);

bool parser_get_cmd_arguments(const char* line, const struct CmdTemplate* templ, parms args[CONFIG_PARSER_MAX_ARGS]);

#if CONFIG_ENABLE_COMPAT_BEHAVIOUR
/**
 * Extract the ID from the command text line.
 *
 * \param line Text line to be processed (ASCIIZ)
 * \param ID Will contain the ID extracted.
 *
 * \return True if everything ok, false if there is no ID
 *
 */
bool parser_get_cmd_id(const char* line, unsigned long* ID);
#endif


/** \} */ // defgroup parser
#endif /* MWARE_PARSER_H */

