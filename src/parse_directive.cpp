/** parse Directives
 * parse_directive.cpp
 *
 */

#include <cstdio>
#include <cstdlib>
#include <string>
#include "common.h"
#include "buffer.h"
#include "error.h"
#include "parser.h"

/** parse_execute_directive      Opens an external script module
 *                      for parsing.
 *
 *      #include <identifier>
 *
 * @param p_function_id : ptr to the routine owning this directive call.
 */
void cx_parser::parse_execute_directive (cx_symtab_node *p_function_id) {

    extern cx_symtab_node *p_program_ptr_id;

    switch (token) {
        case tc_INCLUDE:
        {
            std::string lib_path;
            char *env_path = getenv(__CX_STDLIB__);

            if (env_path != nullptr) {
                lib_path = env_path;
                lib_path += "/";
            }

            get_token();

            lib_path += p_token->string__();
            p_program_ptr_id->found_global_end = true;

            // save current line number
            int old_line_num = current_line_number;
            current_line_number = 0;

            /* true : stdlib module
             * returns nullptr */
            cx_parser *parser = new cx_parser
                    (new cx_source_buffer(lib_path.c_str()), true);

            parser->parse();

            current_line_number = old_line_num;

            delete parser;

            icode.reset();
            icode.put(tc_left_bracket);
            p_program_ptr_id->found_global_end = false;
            get_token_append();
        }
            break;
		case tc_WARN:
			get_token();
			
			if (token != tc_string){
				cx_error(err_unexpected_token);
			}

			{
				std::string msg = p_token->string__();
				msg[0] = ' '; msg[msg.length() - 1] = '\0';
				std::cerr << "warning:" << msg << std::endl;
			}

			get_token_append();
			break;
        default:
            break;
    }
}
